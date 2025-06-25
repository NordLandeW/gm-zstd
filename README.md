# gm-zstd

ZSTD compression extension for GameMaker 2024.13.

Around 10x faster then original buffer compress/decompress functions.

Only Windows x64 version is precompiled. For other systems, you need to compile the extension yourself.

## Usage
```gml
// Replace buffer_compress()
compressedBuffer = zstd_buffer_compress(buffer, size, compressionLevel);

// Replace buffer_decompress()
decompressedBuffer = zstd_buffer_decompress(buffer);
```
