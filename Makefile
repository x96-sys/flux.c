CC = gcc
CFLAGS = -O3 -Wall -Wextra -Iinclude
SRC = $(shell find src -name "*.c")
OBJ = $(SRC:.c=.o)
LIB = libflux.a

all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: CFLAGS += -Iinclude
test_src = $(shell find tests -name "*.c")
test:
	$(CC) $(CFLAGS) $(SRC) $(test_src) -o test_flux

clean:
	rm -f $(OBJ) $(LIB) test_flux
