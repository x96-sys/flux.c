# Flux

C library for handling stream of bytes

[Implementation of](https://github.com/x96-sys/flux)

## Scripts

### Build

```bash
make
```

### Clean

```bash
make clean
```

### Build Test

```bash
make test
```

### Run Test

```bash
./test_flux
```

### Verbose Test

```bash
./test_flux -v
```

### Watch for Changes and Test

```bash
ls **/*.{c,h} | entr sh -c 'make clean && make test && ./test_flux -v'
```
