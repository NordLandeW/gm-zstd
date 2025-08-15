
#include "main.h"

#include <lz4.h>
#include <lz4frame.h>
#include <lz4hc.h>

#include <cstring>
#include <iostream>
#include <string>

using std::string;

static const LZ4F_preferences_t kPrefs = {
    {LZ4F_max256KB, LZ4F_blockLinked, LZ4F_contentChecksumEnabled, LZ4F_frame,
     1, 0, LZ4F_noBlockChecksum},
    0,
    0,
    0,
    {0, 0, 0},
};

void output_log(string str) {
    std::cout << "[gm-zstd] " << str << std::endl;
}

GM_API double gm_zstd_compress_buffer(const char* sourceBuffer,
                                      double sourceSize, char* targetBuffer,
                                      double compressionLevel) {
    size_t fSize = (size_t)sourceSize;
    size_t cBuffSize = ZSTD_compressBound(fSize);

    char* const fBuff = new char[fSize];
    char* const cBuff = new char[cBuffSize];

    memcpy(fBuff, sourceBuffer, fSize);

    size_t const cSize =
        ZSTD_compress(cBuff, cBuffSize, fBuff, fSize, (int)compressionLevel);

    try {
        CHECK_ZSTD(cSize);
    } catch (...) {
        delete[] cBuff;
        delete[] fBuff;
        return -1;
    };

    // Success
    memcpy(targetBuffer, cBuff, cSize);

    delete[] cBuff;
    delete[] fBuff;

    return (double)cSize;
}

GM_API double gm_lz4_compress_buffer_fast(const char* sourceBuffer,
                                          double sourceSize, char* targetBuffer,
                                          double acceleration) {
    size_t fSize = (size_t)sourceSize;

    LZ4F_preferences_t prefs = kPrefs;
    prefs.frameInfo.contentSize = fSize;

    size_t headerSize = sizeof(gmlz4_header_t);
    size_t cBuffSize = LZ4_compressBound(fSize) + headerSize;

    char* const fBuff = new char[fSize];
    char* const cBuff = new char[cBuffSize];
    char* const cBuffS = cBuff + headerSize;

    gmlz4_header_t header = {GMLZ4_MAGIC, fSize};
    memcpy(cBuff, &header, headerSize);
    memcpy(fBuff, sourceBuffer, fSize);

    size_t cSize = LZ4_compress_fast(fBuff, cBuffS, fSize,
                                     cBuffSize - headerSize, (int)acceleration);

    if (cSize <= 0) {
        output_log("!!! LZ4 compression failed.");
        delete[] cBuff;
        delete[] fBuff;
        return -1;
    }

    cSize += headerSize;

    // Success
    memcpy(targetBuffer, cBuff, cSize);

    delete[] cBuff;
    delete[] fBuff;

    return (double)cSize;
}

GM_API double gm_lz4_compress_buffer_hc(const char* sourceBuffer,
                                        double sourceSize, char* targetBuffer,
                                        double compressionLevel) {
    size_t fSize = (size_t)sourceSize;

    LZ4F_preferences_t prefs = kPrefs;
    prefs.frameInfo.contentSize = fSize;

    size_t headerSize = sizeof(gmlz4_header_t);
    size_t cBuffSize = LZ4_compressBound(fSize) + headerSize;

    char* const fBuff = new char[fSize];
    char* const cBuff = new char[cBuffSize];
    char* const cBuffS = cBuff + headerSize;

    gmlz4_header_t header = {GMLZ4_MAGIC, fSize};
    memcpy(cBuff, &header, headerSize);
    memcpy(fBuff, sourceBuffer, fSize);

    size_t cSize = LZ4_compress_HC(fBuff, cBuffS, fSize, cBuffSize - headerSize,
                                   (int)compressionLevel);

    if (cSize <= 0) {
        output_log("!!! LZ4 compression failed.");
        delete[] cBuff;
        delete[] fBuff;
        return -1;
    }

    cSize += headerSize;

    // Success
    memcpy(targetBuffer, cBuff, cSize);

    delete[] cBuff;
    delete[] fBuff;

    return (double)cSize;
}

bool zstd_check_compressed(const char* str, double _sSize) {
    size_t sSize = (size_t)_sSize;
    unsigned long long const rSize = ZSTD_getFrameContentSize(str, sSize);
    return rSize != ZSTD_CONTENTSIZE_ERROR && rSize != ZSTD_CONTENTSIZE_UNKNOWN;
}

bool lz4_check_compressed(const char* str, double _sSize) {
    size_t sSize = (size_t)_sSize;
    gmlz4_header_t header;
    memcpy(&header, str, sizeof(gmlz4_header_t));
    return header.magic == GMLZ4_MAGIC;
}

GM_API double gm_zstd_is_compressed(const char* str, double _sSize) {
    size_t sSize = (size_t)_sSize;
    return zstd_check_compressed(str, sSize) ? 0.0 : -1.0;
}

GM_API double gm_lz4_is_compressed(const char* str, double _sSize) {
    size_t sSize = (size_t)_sSize;
    return lz4_check_compressed(str, sSize) ? 0.0 : -1.0;
}

GM_API double gm_zstd_compress_bound(double sourceSize) {
    size_t fSize = (size_t)sourceSize;
    return (double)ZSTD_compressBound(fSize);
}

GM_API double gm_zstd_decompress_bound(const char* sourceBuffer,
                                       double sourceSize) {
    size_t sSize = (size_t)sourceSize;
    if (!zstd_check_compressed(sourceBuffer, sSize)) {
        output_log("Not a zstd file!");
        return -1.0;
    }

    unsigned long long const rSize =
        ZSTD_getFrameContentSize(sourceBuffer, sSize);
    if (rSize == ZSTD_CONTENTSIZE_ERROR || rSize == ZSTD_CONTENTSIZE_UNKNOWN) {
        output_log("Error getting decompressed size.");
        return -1.0;
    }
    return (double)rSize;
}

GM_API double gm_lz4_compress_bound(double sourceSize) {
    size_t fSize = (size_t)sourceSize;
    return (double)(LZ4_compressBound(fSize - sizeof(gmlz4_header_t)) +
                    sizeof(gmlz4_header_t));
}
GM_API double gm_lz4_decompress_bound(const char* sourceBuffer,
                                      double sourceSize) {
    size_t sSize = (size_t)sourceSize;
    if (!lz4_check_compressed(sourceBuffer, sSize)) {
        output_log("Not a valid LZ4 frame.");
        return -1.0;
    }

    gmlz4_header_t header;
    memcpy(&header, sourceBuffer, sizeof(gmlz4_header_t));
    return (double)header.original_size;
}

GM_API double gm_zstd_decompress_buffer(const char* sourceBuffer,
                                        double sourceSize, char* targetBuffer) {
    size_t sSize = (size_t)sourceSize;
    if (!zstd_check_compressed(sourceBuffer, sSize)) {
        output_log("Not a zstd file!");
        return -1.0;
    }

    size_t const rSize = ZSTD_getFrameContentSize(sourceBuffer, sSize);
    char* const rBuff = new char[rSize];
    size_t const dSize = ZSTD_decompress(rBuff, rSize, sourceBuffer, sSize);

    if (dSize != rSize)
        return -1.0;

    // Success
    memcpy(targetBuffer, rBuff, rSize);

    delete[] rBuff;

    return (double)dSize;
}

GM_API double gm_lz4_decompress_buffer(const char* sourceBuffer,
                                       double sourceSize, char* targetBuffer) {
    size_t sSize = (size_t)sourceSize;
    if (!lz4_check_compressed(sourceBuffer, sSize)) {
        output_log("Not a valid LZ4 frame.");
        return -1.0;
    }

    gmlz4_header_t header;
    memcpy(&header, sourceBuffer, sizeof(gmlz4_header_t));

    size_t headerSize = sizeof(gmlz4_header_t);
    const char* cBuff = sourceBuffer + headerSize;
    size_t cSize = sSize - headerSize;
    size_t rSize = header.original_size;

    int const dSize = LZ4_decompress_safe(cBuff, targetBuffer, cSize, rSize);

    if (dSize < 0) {
        output_log("!!! LZ4 decompression failed.");
        return -1.0;
    }

    if ((size_t)dSize != rSize) {
        output_log("!!! LZ4 decompression failed: incorrect size.");
        return -1.0;
    }

    return (double)dSize;
}

GM_API double gm_zstd_buffer_copy(void* dst, void* src, double size) {
    memcpy(dst, src, (size_t)size);
    return 0;
}
