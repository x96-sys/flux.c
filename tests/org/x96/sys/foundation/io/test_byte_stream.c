
#include "org/x96/sys/foundation/io/byte_stream.h"
#include <assert.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

extern bool VERBOSE;

void happy_byte_stream() {
    ByteStream is;
    uint8_t out;

    const uint8_t input[] = {0x61, 'b'};
    assert(byte_stream_wrapped(&is, input, sizeof(input)) == INPUT_OK);

    assert(is.length == 4);
    assert(byte_stream_at(&is, 0, &out) == INPUT_OK && out == 0x02);
    assert(byte_stream_at(&is, 1, &out) == INPUT_OK && out == 'a');
    assert(byte_stream_at(&is, 2, &out) == INPUT_OK && out == 0x62);
    assert(byte_stream_at(&is, 3, &out) == INPUT_OK && out == 0x03);

    byte_stream_free(&is);
    printf(".");
}

void happy_byte_stream_overflow() {
    ByteStream is;
    uint8_t out;

    assert(byte_stream_wrapped(&is, NULL, 0) == INPUT_OK);
    assert(is.length == 2);
    assert(byte_stream_at(&is, 0, &out) == INPUT_OK && out == 0x02);
    assert(byte_stream_at(&is, 1, &out) == INPUT_OK && out == 0x03);
    assert(byte_stream_at(&is, 2, &out) == INPUT_OVERFLOW);

    byte_stream_free(&is);
    printf(".");
}

void happy_byte_stream_underflow() {
    ByteStream is;
    uint8_t out;

    assert(byte_stream_wrapped(&is, NULL, 0) == INPUT_OK);
    assert(is.length == 2);
    assert(byte_stream_at(&is, -1, &out) == INPUT_UNDERFLOW);
    byte_stream_free(&is);
    printf(".");
}

uint8_t *generate_payload(size_t target_size) {
    uint8_t *source = malloc(target_size);
    assert(source != NULL);

    for (size_t i = 0; i < target_size; i++) {
        source[i] = (uint8_t)(i % 0x80);
    }

    return source;
}

void benchmark_byte_stream(size_t payload_size) {

    uint8_t *payload = generate_payload(payload_size);

    ByteStream is;
    assert(byte_stream_raw(&is, payload, payload_size) == INPUT_OK);

    uint8_t out;
    size_t checksum = 0;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (size_t i = 0; i < is.length; i++) {
        InputError err = byte_stream_at(&is, i, &out);
        if (err != INPUT_OK)
            break;
        checksum += out;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    uint64_t elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000ULL + (end.tv_nsec - start.tv_nsec);
    double elapsed_sec = elapsed_ns / 1e9;

    double mb = (double)payload_size / (1024.0 * 1024.0);
    double gb = mb / 1024.0;
    double rate = (double)payload_size / elapsed_sec;

    if (VERBOSE) {
        setlocale(LC_NUMERIC, "");
        printf("\n⚡️\n");
        printf("Payload:      %'zu bytes\n", payload_size);
        printf("Tempo:        %.3f s\n", elapsed_sec);
        printf("Velocidade:   %.2f MB/s | %.2f GB/s | %'.0f bytes/s\n", mb / elapsed_sec, gb / elapsed_sec, rate);
        printf("Checksum:     %'zu\n", checksum);
    } else {
        printf("⚡️");
    }

    assert(gb / elapsed_sec > 0.8);
    assert(gb / elapsed_sec < 1.1);

    byte_stream_free(&is);
    free(payload);
}

void slow_benchmark_byte_stream(size_t payload_size) {

    uint8_t *payload = generate_payload(payload_size);

    ByteStream is;
    assert(byte_stream_raw(&is, payload, payload_size) == INPUT_OK);

    uint8_t out;
    size_t checksum = 0;
    size_t c = 0;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (size_t i = 0; i < is.length; i++) {
        InputError err = byte_stream_at(&is, i, &out);
        if (err != INPUT_OK)
            break;
        if (i % 7 == 0)
            c++;
        checksum += out;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    uint64_t elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000ULL + (end.tv_nsec - start.tv_nsec);
    double elapsed_sec = elapsed_ns / 1e9;

    double mb = (double)payload_size / (1024.0 * 1024.0);
    double gb = mb / 1024.0;
    double rate = (double)payload_size / elapsed_sec;

    if (VERBOSE) {
        setlocale(LC_NUMERIC, "");
        printf("🐌\n");
        printf("Payload:      %'zu bytes\n", payload_size);
        printf("Tempo:        %.3f s\n", elapsed_sec);
        printf("Velocidade:   %.2f MB/s | %.2f GB/s | %'.0f bytes/s\n", mb / elapsed_sec, gb / elapsed_sec, rate);
        printf("Checksum:     %'zu\n", checksum + c);
    } else {
        printf("🐌");
    }

    assert(gb / elapsed_sec > 0.6);
    assert(gb / elapsed_sec < 0.8);

    byte_stream_free(&is);
    free(payload);
}

void test_byte_stream() {
    happy_byte_stream();
    happy_byte_stream_overflow();
    happy_byte_stream_underflow();
    benchmark_byte_stream(100 * 1024 * 1024);      // 100 MB
    slow_benchmark_byte_stream(200 * 1024 * 1024); // 200 MB
}
