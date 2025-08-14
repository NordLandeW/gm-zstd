
/// @description Compresses a buffer using Zstandard compression
/// @param {Id.Buffer} buffer - The buffer to compress
/// @param {Real} size - The size of the buffer
/// @param {Real} compressionLevel - The compression level to use (0~22)
/// @returns {Id.Buffer} - The compressed buffer, or undefined if compression fails
function zstd_buffer_compress(buffer, size, compressionLevel) {
    var _compBound = __gm_zstd_compress_bound(size);
    var _compBuffer = buffer_create(_compBound, buffer_fixed, 1);
    var _result = __gm_zstd_compress_buffer(buffer_get_address(buffer), size, buffer_get_address(_compBuffer), compressionLevel);

    if(_result >= 0) {
        buffer_resize(_compBuffer, _result);
        buffer_set_used_size(_compBuffer, _result);
        return _compBuffer;
    }
    buffer_delete(_compBuffer);
    return undefined;
}

/// @description Decompresses a buffer using Zstandard decompression
/// @param {Id.Buffer} buffer - The buffer to decompress
/// @returns {Id.Buffer} - The decompressed buffer, or undefined if decompression fails
function zstd_buffer_decompress(buffer) {
    var size = buffer_get_size(buffer);
    var _decompBound = __gm_zstd_decompress_bound(buffer_get_address(buffer), size);
    if(_decompBound < 0) {
        return undefined;
    }

    var _decompBuffer = buffer_create(_decompBound, buffer_fixed, 1);
    var _result = __gm_zstd_decompress_buffer(buffer_get_address(buffer), size, buffer_get_address(_decompBuffer));

    if(_result >= 0) {
        buffer_set_used_size(_decompBuffer, _result);
        return _decompBuffer;
    }
    buffer_delete(_decompBuffer);
    return undefined;
}


/// @description Compresses a buffer using LZ4 compression with 'acceleration' factor
/// @param {Id.Buffer} buffer - The buffer to compress
/// @param {Real} size - The size of the buffer
/// @param {Real} acceleration - Acceleration value for compression (1(default)~65537)
/// @returns {Id.Buffer} - The compressed buffer, or undefined if compression fails
function lz4_buffer_compress_fast(buffer, size, acceleration) {
    var _compBound = __gm_lz4_compress_bound(size);
    var _compBuffer = buffer_create(_compBound, buffer_fixed, 1);

    var _result = __gm_lz4_compress_buffer_fast(buffer_get_address(buffer), size, buffer_get_address(_compBuffer), acceleration);

    if(_result >= 0) {
        buffer_resize(_compBuffer, _result);
        buffer_set_used_size(_compBuffer, _result);
        return _compBuffer;
    }
    buffer_delete(_compBuffer);
    return undefined;
}

/// @description Compresses a buffer using LZ4-HC compression
/// @param {Id.Buffer} buffer - The buffer to compress
/// @param {Real} size - The size of the buffer
/// @param {Real} compressionLevel - The compression level to use (1~12)
/// @returns {Id.Buffer} - The compressed buffer, or undefined if compression fails
function lz4_buffer_compress_hc(buffer, size, compressionLevel) {
    var _compBound = __gm_lz4_compress_bound(size);
    var _compBuffer = buffer_create(_compBound, buffer_fixed, 1);

    var _result = __gm_lz4_compress_buffer_hc(buffer_get_address(buffer), size, buffer_get_address(_compBuffer), compressionLevel);

    if(_result >= 0) {
        buffer_resize(_compBuffer, _result);
        buffer_set_used_size(_compBuffer, _result);
        return _compBuffer;
    }
    buffer_delete(_compBuffer);
    return undefined;
}

/// @description Compresses a buffer using LZ4 compression
/// @param {Id.Buffer} buffer - The buffer to compress
/// @param {Real} size - The size of the buffer
/// @returns {Id.Buffer} - The compressed buffer, or undefined if compression fails
function lz4_buffer_compress(buffer, size) {
    return lz4_buffer_compress_fast(buffer, size, 1);
}

/// @description Decompresses a buffer using LZ4 decompression
/// @param {Id.Buffer} buffer - The buffer to decompress
/// @returns {Id.Buffer} - The decompressed buffer, or undefined if decompression fails
function lz4_buffer_decompress(buffer) {
    var size = buffer_get_size(buffer);
    var _decompBound = __gm_lz4_decompress_bound(buffer_get_address(buffer), size);
    if(_decompBound < 0) {
        return undefined;
    }

    var _decompBuffer = buffer_create(_decompBound, buffer_fixed, 1);
    var _result = __gm_lz4_decompress_buffer(buffer_get_address(buffer), size, buffer_get_address(_decompBuffer));

    if(_result >= 0) {
        buffer_set_used_size(_decompBuffer, _result);
        return _decompBuffer;
    }
    buffer_delete(_decompBuffer);
    return undefined;
}