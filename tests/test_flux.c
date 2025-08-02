#include <stdbool.h>
#include <stdio.h>
#include <string.h>

extern void test_byte_stream();

bool VERBOSE = false;

int main(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
            VERBOSE = true;
        }
    }

    printf("🐞");
    test_byte_stream();
    printf("🏁\n");
    return 0;
}
