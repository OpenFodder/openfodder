#ifndef SDL3_MIXER_COMPAT_H
#define SDL3_MIXER_COMPAT_H

#ifndef SDL_ALIGNED
#if defined(_MSC_VER)
#define SDL_ALIGNED(x) __declspec(align(x))
#elif defined(__GNUC__)
#define SDL_ALIGNED(x) __attribute__((aligned(x)))
#else
#define SDL_ALIGNED(x)
#endif
#endif

#endif
