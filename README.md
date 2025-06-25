# gm-zstd

ZSTD compression extension for GameMaker 2024.13.

## Usage
```gml
// Replace buffer_compress()
compressedBuffer = zstd_buffer_compress(buffer, size, compressionLevel);

// Replace buffer_decompress()
decompressedBuffer = zstd_buffer_decompress(buffer);
```
