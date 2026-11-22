// Offline dependency check: demux MPEG, decode video/audio, and convert the
// output to the same BGRA / signed-16-bit PCM formats used by VideoMPEG.cpp.
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

template<typename T, void (*Release)(T**)>
struct AvDeleter {
    void operator()(T* value) const { Release(&value); }
};
template<typename T, void (*Release)(T**)>
using AvPtr = std::unique_ptr<T, AvDeleter<T, Release>>;

static void require(bool success, const char* message) {
    if (!success) throw std::runtime_error(message);
}

static void check(int result, const char* operation) {
    if (result < 0) {
        char error[AV_ERROR_MAX_STRING_SIZE] = {};
        av_strerror(result, error, sizeof(error));
        throw std::runtime_error(std::string(operation) + ": " + error);
    }
}

#ifdef OPENFODDER_CHECK_MINIMAL_FFMPEG
static void check_components() {
    void* state = nullptr;
    int decoders = 0;
    while (const AVCodec* codec = av_codec_iterate(&state)) {
        require(!av_codec_is_encoder(codec), "Unused encoder was enabled");
        require(std::string(codec->name) == "mpeg1video" || std::string(codec->name) == "mp2",
            "Unused decoder was enabled");
        ++decoders;
    }
    require(decoders == 2, "Expected only MPEG-1 video and MP2 audio decoders");
    state = nullptr;
    int demuxers = 0;
    while (const auto* format = av_demuxer_iterate(&state)) {
        require(std::string(format->name) == "mpeg" || std::string(format->name) == "mpegvideo",
            "Unused demuxer was enabled");
        ++demuxers;
    }
    require(demuxers == 2, "Expected MPEG program-stream and MPEG video probing demuxers");
    state = nullptr;
    require(!av_muxer_iterate(&state), "Unused muxer was enabled");
    state = nullptr;
    const char* protocol = avio_enum_protocols(&state, 0);
    require(protocol && std::string(protocol) == "file" && !avio_enum_protocols(&state, 0),
        "Expected only the local file protocol");
}
#endif

static AvPtr<AVCodecContext, avcodec_free_context> decoder(AVFormatContext* input, int stream) {
    const auto* parameters = input->streams[stream]->codecpar;
    const auto* codec = avcodec_find_decoder(parameters->codec_id);
    require(codec != nullptr, "Required MPEG decoder is missing");
    AvPtr<AVCodecContext, avcodec_free_context> context(avcodec_alloc_context3(codec));
    require(context != nullptr, "Could not allocate decoder");
    check(avcodec_parameters_to_context(context.get(), parameters), "Copy codec parameters");
    check(avcodec_open2(context.get(), codec, nullptr), "Open decoder");
    return context;
}

int main(int argc, char** argv) {
    try {
        require(argc == 2 || (argc == 3 && std::string(argv[2]) == "--intro"),
            "Usage: openfodder_video_check fixture.mpg [--intro]");
        const bool intro = argc == 3;
#ifdef OPENFODDER_CHECK_MINIMAL_FFMPEG
        check_components();
#endif
        AVFormatContext* rawInput = nullptr;
        check(avformat_open_input(&rawInput, argv[1], nullptr, nullptr), "Open MPEG fixture");
        AvPtr<AVFormatContext, avformat_close_input> input(rawInput);
        check(avformat_find_stream_info(input.get(), nullptr), "Read MPEG streams");
        const int videoStream = av_find_best_stream(input.get(), AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
        const int audioStream = av_find_best_stream(input.get(), AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
        check(videoStream, "Find video stream");
        check(audioStream, "Find audio stream");
        auto video = decoder(input.get(), videoStream);
        auto audio = decoder(input.get(), audioStream);
        AvPtr<AVPacket, av_packet_free> packet(av_packet_alloc());
        AvPtr<AVFrame, av_frame_free> frame(av_frame_alloc());
        require(packet && frame, "Could not allocate packet/frame");
        std::unique_ptr<SwsContext, decltype(&sws_freeContext)> scaler(nullptr, sws_freeContext);
        AvPtr<SwrContext, swr_free> resampler;
        int videoFrames = 0, audioFrames = 0;
        bool visiblePixels = false, audibleSamples = false;

        auto consume = [&](bool isVideo) {
            if (isVideo) {
                require(frame->width == (intro ? 352 : 32) && frame->height == (intro ? 288 : 32),
                    "Unexpected video dimensions");
                if (!scaler) {
                    scaler.reset(sws_getContext(frame->width, frame->height,
                        static_cast<AVPixelFormat>(frame->format), frame->width, frame->height,
                        AV_PIX_FMT_BGRA, SWS_BILINEAR, nullptr, nullptr, nullptr));
                    require(scaler != nullptr, "Could not create video converter");
                }
                std::vector<uint8_t> pixels(frame->width * frame->height * 4);
                uint8_t* planes[] = {pixels.data(), nullptr, nullptr, nullptr};
                int strides[] = {frame->width * 4, 0, 0, 0};
                require(sws_scale(scaler.get(), frame->data, frame->linesize, 0,
                    frame->height, planes, strides) == frame->height, "Incomplete video conversion");
                for (size_t pixel = 0; pixel < pixels.size(); pixel += 4)
                    visiblePixels |= pixels[pixel] || pixels[pixel + 1] || pixels[pixel + 2];
                ++videoFrames;
            } else {
                if (!resampler) {
                    SwrContext* rawResampler = nullptr;
                    AVChannelLayout stereo = AV_CHANNEL_LAYOUT_STEREO;
                    check(swr_alloc_set_opts2(&rawResampler, &stereo, AV_SAMPLE_FMT_S16, 48000,
                        &frame->ch_layout, static_cast<AVSampleFormat>(frame->format),
                        frame->sample_rate, 0, nullptr), "Create audio converter");
                    resampler.reset(rawResampler);
                    check(swr_init(resampler.get()), "Initialize audio converter");
                }
                const int capacity = swr_get_out_samples(resampler.get(), frame->nb_samples);
                check(capacity, "Get PCM capacity");
                std::vector<int16_t> samples(static_cast<size_t>(capacity) * 2);
                uint8_t* output[] = {reinterpret_cast<uint8_t*>(samples.data())};
                const int count = swr_convert(resampler.get(), output, capacity,
                    const_cast<const uint8_t**>(frame->extended_data), frame->nb_samples);
                check(count, "Convert audio to PCM");
                audibleSamples |= std::any_of(samples.begin(), samples.begin() + count * 2,
                    [](int16_t sample) { return sample != 0; });
                ++audioFrames;
            }
        };

        auto decode = [&](AVCodecContext* context, const AVPacket* current, bool isVideo) {
            check(avcodec_send_packet(context, current), "Send packet to decoder");
            int result;
            while ((result = avcodec_receive_frame(context, frame.get())) >= 0) {
                consume(isVideo);
                av_frame_unref(frame.get());
            }
            if (result != AVERROR(EAGAIN) && result != AVERROR_EOF)
                check(result, "Decode frame");
        };

        int result;
        while ((result = av_read_frame(input.get(), packet.get())) >= 0) {
            if (packet->stream_index == videoStream) decode(video.get(), packet.get(), true);
            else if (packet->stream_index == audioStream) decode(audio.get(), packet.get(), false);
            av_packet_unref(packet.get());
        }
        require(result == AVERROR_EOF, "Could not read entire MPEG fixture");
        decode(video.get(), nullptr, true);
        decode(audio.get(), nullptr, false);
        require((intro ? videoFrames > 0 : videoFrames == 10) && visiblePixels,
            "Video did not produce the expected visible frames");
        require(audioFrames > 0 && audibleSamples, "Audio did not produce non-silent PCM");
        std::cout << "PASS MPEG: " << videoFrames << " BGRA frames, " << audioFrames
                  << " audio frames converted to stereo PCM\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL MPEG: " << error.what() << '\n';
        return 1;
    }
}
