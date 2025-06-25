
#include "main.h"

#include <cstring>
#include <iostream>
#include <string>

using std::string;

void output_log(string str) {
    std::cout << "[gm-zstd] " << str << std::endl;
}

GM_API double gm_zstd_compress_buffer(const char* sourceBuffer, double sourceSize, char* targetBuffer,
                                      double compressionLevel) {
    size_t fSize = (size_t)sourceSize;
    size_t cBuffSize = ZSTD_compressBound(fSize);

    char* const fBuff = new char[fSize];
    char* const cBuff = new char[cBuffSize];

    memcpy(fBuff, sourceBuffer, fSize);

    output_log("Start compressing...");

    size_t const cSize =
        ZSTD_compress(cBuff, cBuffSize, fBuff, fSize, (int)compressionLevel);

    output_log("Finish compressing, checking...");

    try {
        CHECK_ZSTD(cSize);
    } catch (...) {
        delete[] cBuff;
        delete[] fBuff;
        return -1;
    };

    output_log("No error found. Success. " + std::to_string(fSize) + "->" + std::to_string(cSize));

    // Success
    memcpy(targetBuffer, cBuff, cSize);

    delete[] cBuff;
    delete[] fBuff;

    return (double)cSize;
}

bool check_compressed(const char* str, double _sSize) {
    size_t sSize = (size_t)_sSize;
    unsigned long long const rSize = ZSTD_getFrameContentSize(str, sSize);
    return rSize != ZSTD_CONTENTSIZE_ERROR && rSize != ZSTD_CONTENTSIZE_UNKNOWN;
}

GM_API double gm_zstd_is_compressed(const char* str, double _sSize) {
    size_t sSize = (size_t)_sSize;
    return check_compressed(str, sSize) ? 0.0 : -1.0;
}

GM_API double gm_zstd_compress_bound(double sourceSize) {
    size_t fSize = (size_t)sourceSize;
    return (double)ZSTD_compressBound(fSize);
}

GM_API double gm_zstd_decompress_bound(const char* sourceBuffer, double sourceSize) {
    size_t sSize = (size_t)sourceSize;
    if (!check_compressed(sourceBuffer, sSize)) {
        output_log("Not a zstd file!");
        return -1.0;
    }

    unsigned long long const rSize = ZSTD_getFrameContentSize(sourceBuffer, sSize);
    if (rSize == ZSTD_CONTENTSIZE_ERROR || rSize == ZSTD_CONTENTSIZE_UNKNOWN) {
        output_log("Error getting decompressed size.");
        return -1.0;
    }
    return (double)rSize;
}

GM_API double gm_zstd_decompress_buffer(const char* sourceBuffer, double sourceSize, char* targetBuffer) {
    size_t sSize = (size_t)sourceSize;
    if (!check_compressed(sourceBuffer, sSize)) {
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

    output_log("Decompression done. No error found.");

    delete[] rBuff;

    return (double)dSize;
}

GM_API double gm_zstd_buffer_copy(void* dst, void* src, double size) {
    memcpy(dst, src, (size_t)size);
    return 0;
}