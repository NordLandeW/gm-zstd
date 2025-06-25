

var _testBuffer = buffer_create(100, buffer_fixed, 1);

for(var i=0; i<10; i++)
    buffer_write(_testBuffer, buffer_u8, i);

var _cBuff = zstd_buffer_compress(_testBuffer, buffer_get_size(_testBuffer), 11);
var _dBuff = zstd_buffer_decompress(_cBuff);

buffer_seek(_dBuff, buffer_seek_start, 0);
for(var i=0; i<10; i++) {
    show_debug_message(string(buffer_read(_dBuff, buffer_u8)));
}

buffer_delete(_testBuffer);
buffer_delete(_cBuff);
buffer_delete(_dBuff);