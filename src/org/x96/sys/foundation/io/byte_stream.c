#include "org/x96/sys/foundation/io/byte_stream.h"
#include <string.h>

static uint8_t *add_sentinels(const uint8_t *input, size_t len, size_t *out_len) {
    *out_len = len + 2;
    uint8_t *result = malloc(*out_len);
    if (!result)
        return NULL;

    result[0] = 0x02;
    memcpy(&result[1], input, len);
    result[*out_len - 1] = 0x03;
    return result;
}

static InputError byte_stream_init(ByteStream *stream, uint8_t *bytes, size_t len) {
    if (!bytes)
        return INPUT_ALLOC_FAIL;
    stream->bytes = bytes;
    stream->length = len;
    return INPUT_OK;
}

InputError byte_stream_raw(ByteStream *stream, const uint8_t *input, size_t len) {
    uint8_t *copy = malloc(len);
    if (!copy)
        return INPUT_ALLOC_FAIL;

    memcpy(copy, input, len);
    return byte_stream_init(stream, copy, len);
}

InputError byte_stream_wrapped(ByteStream *stream, const uint8_t *input, size_t len) {
    size_t total_len;
    uint8_t *wrapped = add_sentinels(input, len, &total_len);
    return byte_stream_init(stream, wrapped, total_len);
}

InputError byte_stream_at(ByteStream *stream, int index, uint8_t *out) {
    if (index < 0)
        return INPUT_UNDERFLOW;
    if ((size_t)index >= stream->length)
        return INPUT_OVERFLOW;
    *out = stream->bytes[index];
    return INPUT_OK;
}

size_t byte_stream_length(ByteStream *stream) { return stream->length; }

void byte_stream_free(ByteStream *stream) {
    if (stream->bytes) {
        free(stream->bytes);
        stream->bytes = NULL;
        stream->length = 0;
    }
}
