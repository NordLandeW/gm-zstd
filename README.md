# gm-zstd

Simplified zstd/lz4 wrapper for GameMaker.

Around 10x/20x (zstd/lz4) faster then original buffer compress/decompress functions.

Only Windows x64 version is precompiled. For other systems, you need to compile the extension yourself.

## Usage
```gml
// Replace buffer_compress()
compressedBuffer = zstd_buffer_compress(buffer, size, compressionLevel);
compressedBuffer = lz4_buffer_compress(buffer, size);

// Replace buffer_decompress()
decompressedBuffer = zstd_buffer_decompress(buffer);
decompressedBuffer = lz4_buffer_decompress(buffer);
```
