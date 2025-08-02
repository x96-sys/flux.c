#ifndef BYTE_STREAM_H
#define BYTE_STREAM_H

#include <stdint.h>
#include <stdlib.h>

typedef enum { INPUT_OK = 0, INPUT_OVERFLOW, INPUT_UNDERFLOW, INPUT_ALLOC_FAIL } InputError;

typedef struct {
    uint8_t *bytes;
    size_t length;
} ByteStream;

InputError byte_stream_raw(ByteStream *stream, const uint8_t *input, size_t len);
InputError byte_stream_wrapped(ByteStream *stream, const uint8_t *input, size_t len);

InputError byte_stream_at(ByteStream *stream, int index, uint8_t *out);
size_t byte_stream_length(ByteStream *stream);

void byte_stream_free(ByteStream *stream);

#endif
