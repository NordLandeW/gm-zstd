
#pragma once
#include <zstd.h>

#if !defined(_MSC_VER)
#define EXPORTED_FN __attribute__((visibility("default")))
#else
#define EXPORTED_FN __declspec(dllexport)
#endif

#define GM_API extern "C" EXPORTED_FN

// ZSTD Stuff

/*! CHECK
 * Check that the condition holds. If it doesn't print a message and die.
 */
#define CHECK(cond, ...)                                                    \
    do {                                                                    \
        if (!(cond)) {                                                      \
            fprintf(stderr, "%s:%d CHECK(%s) failed: ", __FILE__, __LINE__, \
                    #cond);                                                 \
            fprintf(stderr, "" __VA_ARGS__);                                \
            fprintf(stderr, "\n");                                          \
            throw;                                                          \
        }                                                                   \
    } while (0)

/*! CHECK_ZSTD
 * Check the zstd error code and die if an error occurred after printing a
 * message.
 */
#define CHECK_ZSTD(fn)                                           \
    do {                                                         \
        size_t const err = (fn);                                 \
        CHECK(!ZSTD_isError(err), "%s", ZSTD_getErrorName(err)); \
    } while (0)


// Custom LZ4 Header (GMLZ4)

#define GMLZ4_MAGIC (0x7F13420E)
struct gmlz4_header_t{
    unsigned int magic;
    unsigned long long original_size;
};