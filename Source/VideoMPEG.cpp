/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2024 Open Fodder
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "stdafx.hpp"
#include "VideoMPEG.hpp"

#ifdef OPENFODDER_ENABLE_FFMPEG

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/version.h>
#include <libavutil/channel_layout.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

struct sVideoMPEG_FFmpeg {
	AVFormatContext*	format;
	AVCodecContext*		video_ctx;
	AVCodecContext*		audio_ctx;
	AVStream*			video_stream;
	AVStream*			audio_stream;
	SwsContext*			sws;
	SwrContext*			swr;
	AVFrame*			video_frame;
	AVFrame*			video_frame_bgra;
	AVFrame*			audio_frame;
	AVPacket*			packet;
	uint8_t*			video_buffer;
	int					video_buffer_size;
	int					video_stream_index;
	int					audio_stream_index;
	AVRational			video_time_base;
	double				video_frame_duration;
	int					audio_out_channels;
	int					audio_out_sample_rate;
	std::vector<uint8_t> audio_buffer;

	sVideoMPEG_FFmpeg()
		: format(nullptr)
		, video_ctx(nullptr)
		, audio_ctx(nullptr)
		, video_stream(nullptr)
		, audio_stream(nullptr)
		, sws(nullptr)
		, swr(nullptr)
		, video_frame(nullptr)
		, video_frame_bgra(nullptr)
		, audio_frame(nullptr)
		, packet(nullptr)
		, video_buffer(nullptr)
		, video_buffer_size(0)
		, video_stream_index(-1)
		, audio_stream_index(-1)
		, video_frame_duration(0.0)
		, audio_out_channels(0)
		, audio_out_sample_rate(0) {
	}
};

namespace {
	void free_ffmpeg(sVideoMPEG_FFmpeg* ff) {
		if (!ff)
			return;
		if (ff->video_ctx)
			avcodec_free_context(&ff->video_ctx);
		if (ff->audio_ctx)
			avcodec_free_context(&ff->audio_ctx);
		if (ff->format)
			avformat_close_input(&ff->format);
		if (ff->video_frame)
			av_frame_free(&ff->video_frame);
		if (ff->video_frame_bgra)
			av_frame_free(&ff->video_frame_bgra);
		if (ff->audio_frame)
			av_frame_free(&ff->audio_frame);
		if (ff->packet)
			av_packet_free(&ff->packet);
		if (ff->sws)
			sws_freeContext(ff->sws);
		if (ff->swr)
			swr_free(&ff->swr);
		if (ff->video_buffer)
			av_free(ff->video_buffer);
		delete ff;
	}

	bool is_audio_enabled() {
		return !(g_Fodder && g_Fodder->mParams && g_Fodder->mParams->mDisableSound);
	}

	void disable_audio(sVideoMPEG_FFmpeg* ff) {
		if (!ff)
			return;
		if (ff->swr)
			swr_free(&ff->swr);
		if (ff->audio_ctx)
			avcodec_free_context(&ff->audio_ctx);
		ff->audio_ctx = nullptr;
		ff->audio_stream_index = -1;
	}

	bool open_input(sVideoMPEG_FFmpeg* ff, const std::string& path) {
		if (avformat_open_input(&ff->format, path.c_str(), nullptr, nullptr) < 0) {
			g_Debugger->Error("Failed to open MPEG file.");
			return false;
		}
		if (avformat_find_stream_info(ff->format, nullptr) < 0) {
			g_Debugger->Error("Failed to read MPEG stream info.");
			return false;
		}
		return true;
	}

	bool init_video_decoder(sVideoMPEG_FFmpeg* ff, int& out_width, int& out_height) {
		const AVCodec* video_codec = nullptr;
		ff->video_stream_index = av_find_best_stream(ff->format, AVMEDIA_TYPE_VIDEO, -1, -1, &video_codec, 0);
		if (ff->video_stream_index < 0) {
			g_Debugger->Error("No video stream found.");
			return false;
		}
		ff->video_stream = ff->format->streams[ff->video_stream_index];
		ff->video_ctx = avcodec_alloc_context3(video_codec);
		avcodec_parameters_to_context(ff->video_ctx, ff->video_stream->codecpar);
		if (avcodec_open2(ff->video_ctx, video_codec, nullptr) < 0) {
			g_Debugger->Error("Failed to open video decoder.");
			return false;
		}
		out_width = ff->video_ctx->width;
		out_height = ff->video_ctx->height;
		return true;
	}

	void init_video_timing(sVideoMPEG_FFmpeg* ff) {
		ff->video_time_base = ff->video_stream->time_base;
		AVRational rate = av_guess_frame_rate(ff->format, ff->video_stream, nullptr);
		double fps = (rate.num > 0 && rate.den > 0) ? av_q2d(rate) : 25.0;
		ff->video_frame_duration = (fps > 0.0) ? (1.0 / fps) : (1.0 / 25.0);
	}

	void init_video_buffers(sVideoMPEG_FFmpeg* ff, int width, int height) {
		ff->video_frame = av_frame_alloc();
		ff->video_frame_bgra = av_frame_alloc();
		ff->audio_frame = av_frame_alloc();
		ff->packet = av_packet_alloc();

		ff->video_buffer_size = av_image_get_buffer_size(AV_PIX_FMT_BGRA, width, height, 1);
		ff->video_buffer = (uint8_t*)av_malloc(ff->video_buffer_size);
		av_image_fill_arrays(ff->video_frame_bgra->data, ff->video_frame_bgra->linesize,
			ff->video_buffer, AV_PIX_FMT_BGRA, width, height, 1);

		ff->sws = sws_getContext(
			width, height, ff->video_ctx->pix_fmt,
			width, height, AV_PIX_FMT_BGRA,
			SWS_BILINEAR, nullptr, nullptr, nullptr);

		if (!ff->sws)
			return;

		const int* coeffs = sws_getCoefficients(SWS_CS_ITU601);
		AVColorSpace colorspace = AVCOL_SPC_UNSPECIFIED;
		AVColorRange color_range = AVCOL_RANGE_UNSPECIFIED;
		if (ff->video_stream && ff->video_stream->codecpar) {
			colorspace = ff->video_stream->codecpar->color_space;
			color_range = ff->video_stream->codecpar->color_range;
		}
		if (colorspace == AVCOL_SPC_UNSPECIFIED)
			colorspace = ff->video_ctx->colorspace;
		if (color_range == AVCOL_RANGE_UNSPECIFIED)
			color_range = ff->video_ctx->color_range;

		switch (colorspace) {
		case AVCOL_SPC_BT709:
			coeffs = sws_getCoefficients(SWS_CS_ITU709);
			break;
		case AVCOL_SPC_SMPTE170M:
		case AVCOL_SPC_BT470BG:
			coeffs = sws_getCoefficients(SWS_CS_ITU601);
			break;
		case AVCOL_SPC_SMPTE240M:
			coeffs = sws_getCoefficients(SWS_CS_SMPTE240M);
			break;
		default:
			break;
		}

		const int src_range = (color_range == AVCOL_RANGE_JPEG) ? 1 : 0;
		const int dst_range = 0;
		sws_setColorspaceDetails(ff->sws, coeffs, src_range, coeffs, dst_range, 0, 1 << 16, 1 << 16);
	}

	bool init_audio_decoder(sVideoMPEG_FFmpeg* ff) {
		const AVCodec* audio_codec = nullptr;
		ff->audio_stream_index = av_find_best_stream(ff->format, AVMEDIA_TYPE_AUDIO, -1, -1, &audio_codec, 0);
		if (ff->audio_stream_index < 0)
			return false;

		ff->audio_stream = ff->format->streams[ff->audio_stream_index];
		ff->audio_ctx = avcodec_alloc_context3(audio_codec);
		avcodec_parameters_to_context(ff->audio_ctx, ff->audio_stream->codecpar);
		if (avcodec_open2(ff->audio_ctx, audio_codec, nullptr) < 0) {
			g_Debugger->Notice("Failed to open audio decoder; continuing without audio.");
			disable_audio(ff);
			return false;
		}
		return true;
	}

	SDL_AudioStream* init_audio_output(sVideoMPEG_FFmpeg* ff) {
		if (!ff || !ff->audio_ctx || !is_audio_enabled())
			return nullptr;

		ff->audio_out_sample_rate = ff->audio_ctx->sample_rate;
#if LIBAVUTIL_VERSION_MAJOR >= 57
		ff->audio_out_channels = ff->audio_ctx->ch_layout.nb_channels;
		AVChannelLayout out_layout;
		av_channel_layout_default(&out_layout, ff->audio_out_channels);
		swr_alloc_set_opts2(&ff->swr,
			&out_layout, AV_SAMPLE_FMT_S16, ff->audio_out_sample_rate,
			&ff->audio_ctx->ch_layout, ff->audio_ctx->sample_fmt, ff->audio_ctx->sample_rate,
			0, nullptr);
		av_channel_layout_uninit(&out_layout);
#else
		ff->audio_out_channels = ff->audio_ctx->channels;
		int64_t in_layout = ff->audio_ctx->channel_layout
			? ff->audio_ctx->channel_layout
			: av_get_default_channel_layout(ff->audio_ctx->channels);
		ff->swr = swr_alloc_set_opts(nullptr,
			in_layout, AV_SAMPLE_FMT_S16, ff->audio_out_sample_rate,
			in_layout, ff->audio_ctx->sample_fmt, ff->audio_ctx->sample_rate,
			0, nullptr);
#endif
		if (!ff->swr || swr_init(ff->swr) < 0) {
			g_Debugger->Notice("Failed to initialize audio resampler; continuing without audio.");
			disable_audio(ff);
			return nullptr;
		}

		SDL_AudioSpec spec = {};
		spec.freq = ff->audio_out_sample_rate;
		spec.format = SDL_AUDIO_S16;
		spec.channels = (uint8)ff->audio_out_channels;
		SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
		if (!stream) {
			g_Debugger->Notice("Failed to open SDL audio stream; continuing without audio.");
			disable_audio(ff);
			return nullptr;
		}

		SDL_ResumeAudioStreamDevice(stream);
		return stream;
	}
}

cVideoMPEG::cVideoMPEG()
	: mFF(nullptr)
	, mLoaded(false)
	, mPlaying(false)
	, mLoop(false)
	, mPlaybackClock(0.0)
	, mNextVideoTime(0.0)
	, mTexture(nullptr)
	, mAudioStream(nullptr)
	, mVideoWidth(0)
	, mVideoHeight(0) {
}

cVideoMPEG::~cVideoMPEG() {
	Stop();
}

std::string cVideoMPEG::FindIntroPath() const {
	if (!g_ResourceMan)
		return "";

	static const char* kIntroNames[] = {
		"Intro.mpg",
		"INTRO.MPG",
		"intro.mpg",
		"Track02.mpg",
		"TRACK02.MPG",
		"track02.mpg",
	};

	for (const auto& base : g_ResourceMan->getValidPaths()) {
		for (const char* name : kIntroNames) {
			std::string candidate = base + g_ResourceMan->PathGenerate(name, eData);
			if (g_ResourceMan->FileExists(candidate))
				return candidate;
		}
	}

	return "";
}

void cVideoMPEG::ResetTiming() {
	mPlaybackClock = 0.0;
	mNextVideoTime = 0.0;

	if (mAudioStream) {
		SDL_ClearAudioStream(mAudioStream);
		SDL_ResumeAudioStreamDevice(mAudioStream);
	}
}

bool cVideoMPEG::LoadIntroFromData() {
	if (!g_ResourceMan)
		return false;

	g_ResourceMan->refresh();
	const std::string path = FindIntroPath();
	if (path.empty()) {
		g_Debugger->Notice("Intro.mpg not found in data paths.");
		return false;
	}

	return LoadFromFile(path);
}

void cVideoMPEG::CleanupFFmpeg() {
	free_ffmpeg(mFF);
	mFF = nullptr;
}

bool cVideoMPEG::LoadFromFile(const std::string& pPath) {
	Stop();

	std::unique_ptr<sVideoMPEG_FFmpeg, void(*)(sVideoMPEG_FFmpeg*)> ff_guard(new sVideoMPEG_FFmpeg(), free_ffmpeg);
	sVideoMPEG_FFmpeg* ff = ff_guard.get();

	if (!open_input(ff, pPath) || !init_video_decoder(ff, mVideoWidth, mVideoHeight))
		return false;

	init_audio_decoder(ff);
	init_video_timing(ff);
	init_video_buffers(ff, mVideoWidth, mVideoHeight);

	SDL_Renderer* renderer = g_Window ? g_Window->GetRenderer() : nullptr;
	if (!renderer) {
		g_Debugger->Error("No SDL renderer available for video playback.");
		return false;
	}

	SDL_Texture* tempTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		mVideoWidth, mVideoHeight);
	if (!tempTexture) {
		g_Debugger->Error("Failed to create SDL texture for video.");
		return false;
	}

	SDL_AudioStream* tempAudio = init_audio_output(ff);

	mFF = ff_guard.release();
	mTexture = tempTexture;
	mAudioStream = tempAudio;

	mLoaded = true;
	mPlaying = false;
	ResetTiming();

	return true;
}

void cVideoMPEG::SetLoop(const bool pLoop) {
	mLoop = pLoop;
}

bool cVideoMPEG::Play() {
	if (!mLoaded)
		return false;

	mPlaying = true;
	ResetTiming();
	return true;
}

void cVideoMPEG::Stop() {
	mPlaying = false;
	mLoaded = false;

	if (mTexture) {
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
	}

	if (mAudioStream) {
		SDL_DestroyAudioStream(mAudioStream);
		mAudioStream = nullptr;
	}

	CleanupFFmpeg();
}

bool cVideoMPEG::DecodeNextFrame() {
	if (!mFF || !mFF->packet)
		return false;

	while (av_read_frame(mFF->format, mFF->packet) >= 0) {
		if (mFF->packet->stream_index == mFF->video_stream_index) {
			if (avcodec_send_packet(mFF->video_ctx, mFF->packet) == 0) {
				while (avcodec_receive_frame(mFF->video_ctx, mFF->video_frame) == 0) {
					sws_scale(mFF->sws, mFF->video_frame->data, mFF->video_frame->linesize, 0,
						mFF->video_ctx->height, mFF->video_frame_bgra->data, mFF->video_frame_bgra->linesize);

					SDL_UpdateTexture(mTexture, nullptr, mFF->video_frame_bgra->data[0],
						mFF->video_frame_bgra->linesize[0]);

					int64_t pts = mFF->video_frame->best_effort_timestamp;
					if (pts == AV_NOPTS_VALUE)
						pts = 0;
					double pts_seconds = pts * av_q2d(mFF->video_time_base);
					if (pts_seconds < 0.0)
						pts_seconds = mNextVideoTime;
					mNextVideoTime = pts_seconds + mFF->video_frame_duration;

					av_packet_unref(mFF->packet);
					return true;
				}
			}
		} else if (mFF->audio_ctx && mFF->swr && mAudioStream &&
			mFF->packet->stream_index == mFF->audio_stream_index) {
			if (avcodec_send_packet(mFF->audio_ctx, mFF->packet) == 0) {
				while (avcodec_receive_frame(mFF->audio_ctx, mFF->audio_frame) == 0) {
					if (!mAudioStream || !mFF->swr)
						break;

					int out_samples = swr_get_out_samples(mFF->swr, mFF->audio_frame->nb_samples);
					int out_bytes = av_samples_get_buffer_size(
						nullptr, mFF->audio_out_channels, out_samples, AV_SAMPLE_FMT_S16, 1);

					if (out_bytes <= 0)
						continue;

					mFF->audio_buffer.resize(out_bytes);
					uint8_t* out_data[1] = { mFF->audio_buffer.data() };
					int converted = swr_convert(
						mFF->swr, out_data, out_samples,
						(const uint8_t**)mFF->audio_frame->extended_data, mFF->audio_frame->nb_samples);

					if (converted > 0) {
						int converted_bytes = av_samples_get_buffer_size(
							nullptr, mFF->audio_out_channels, converted, AV_SAMPLE_FMT_S16, 1);
						SDL_PutAudioStreamData(mAudioStream, mFF->audio_buffer.data(), converted_bytes);
					}
				}
			}
		}

		av_packet_unref(mFF->packet);
	}

	return false;
}

bool cVideoMPEG::Update(const double pDeltaSeconds) {
	if (!mPlaying)
		return false;

	mPlaybackClock += pDeltaSeconds;

	while (mPlaybackClock >= mNextVideoTime) {
		if (!DecodeNextFrame()) {
			if (mLoop) {
				av_seek_frame(mFF->format, -1, 0, AVSEEK_FLAG_BACKWARD);
				if (mFF->video_ctx)
					avcodec_flush_buffers(mFF->video_ctx);
				if (mFF->audio_ctx)
					avcodec_flush_buffers(mFF->audio_ctx);
				ResetTiming();
			} else {
				mPlaying = false;
				return false;
			}
		} else {
			break;
		}
	}

	return true;
}

void cVideoMPEG::Render() {
	if (!mTexture || !g_Window)
		return;

	SDL_Renderer* renderer = g_Window->GetRenderer();
	if (!renderer)
		return;

	int window_w = 0;
	int window_h = 0;
	if (SDL_GetRenderOutputSize(renderer, &window_w, &window_h) < 0 || window_w <= 0 || window_h <= 0) {
		cDimension window = g_Window->GetWindowSize();
		window_w = (int)window.mWidth;
		window_h = (int)window.mHeight;
	}

	SDL_FRect dest = {};
	float video_aspect = (mVideoHeight > 0) ? ((float)mVideoWidth / (float)mVideoHeight) : 1.0f;
	float window_aspect = (window_h > 0) ? ((float)window_w / (float)window_h) : video_aspect;

	if (window_aspect >= video_aspect) {
		dest.h = (float)window_h;
		dest.w = dest.h * video_aspect;
		dest.x = (float)((window_w - dest.w) * 0.5f);
		dest.y = 0.0f;
	} else {
		dest.w = (float)window_w;
		dest.h = dest.w / video_aspect;
		dest.x = 0.0f;
		dest.y = (float)((window_h - dest.h) * 0.5f);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, mTexture, nullptr, &dest);
	g_Window->FrameEnd();
}

#endif
