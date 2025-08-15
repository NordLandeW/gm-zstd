

var _testSize = 100 * 1024 * 1024;
var _testNums = _testSize / 4;
var _testBuffer = buffer_create(_testSize, buffer_fixed, 1);

for(var i=0; i<_testNums; i++)
    buffer_write(_testBuffer, buffer_u32, i);

// ZSTD Test
var _cBuff = zstd_buffer_compress(_testBuffer, buffer_get_size(_testBuffer), 11);
var _dBuff = zstd_buffer_decompress(_cBuff);

buffer_seek(_dBuff, buffer_seek_start, 0);
for(var i=0; i<10; i++) {
    if(buffer_read(_dBuff, buffer_u32) != i) {
        show_debug_message("ZSTD failed.");
        return;
    }
}
buffer_delete(_cBuff);
buffer_delete(_dBuff);

show_debug_message("ZSTD Ok.");

// LZ4 Test
var _cBuffLZ4 = lz4_buffer_compress(_testBuffer, buffer_get_size(_testBuffer));
var _dBuffLZ4 = lz4_buffer_decompress(_cBuffLZ4);

buffer_seek(_dBuffLZ4, buffer_seek_start, 0);
for(var i=0; i<_testNums; i++) {
    if(buffer_read(_dBuffLZ4, buffer_u32) != i) {
        show_debug_message("LZ4 failed.");
        return;
    }
}
buffer_delete(_cBuffLZ4);
buffer_delete(_dBuffLZ4);

show_debug_message("LZ4 Ok.");

// LZ4-fast Test
var _cBuffLZ4Fast = lz4_buffer_compress_fast(_testBuffer, buffer_get_size(_testBuffer), 9);
var _dBuffLZ4Fast = lz4_buffer_decompress(_cBuffLZ4Fast);

buffer_seek(_dBuffLZ4Fast, buffer_seek_start, 0);
for(var i=0; i<_testNums; i++) {
    if(buffer_read(_dBuffLZ4Fast, buffer_u32) != i) {
        show_debug_message("LZ4-fast failed.");
        return;
    }
}
buffer_delete(_cBuffLZ4Fast);
buffer_delete(_dBuffLZ4Fast);

show_debug_message("LZ4-fast Ok.");

// LZ4-HC Test
var _cBuffLZ4HC = lz4_buffer_compress_hc(_testBuffer, buffer_get_size(_testBuffer), 9);
var _dBuffLZ4HC = lz4_buffer_decompress(_cBuffLZ4HC);

buffer_seek(_dBuffLZ4HC, buffer_seek_start, 0);
for(var i=0; i<_testNums; i++) {
    if(buffer_read(_dBuffLZ4HC, buffer_u32) != i) {
        show_debug_message("LZ4-fast failed.");
        return;
    }
}
buffer_delete(_cBuffLZ4HC);
buffer_delete(_dBuffLZ4HC);

show_debug_message("LZ4-HC Ok.");

buffer_delete(_testBuffer);