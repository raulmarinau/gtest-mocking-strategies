# UT guideline

## Requirements

CMake:
```bash
(sudo) apt install cmake
```
Boost Libraries
```bash
(sudo) apt install libboost-all-dev
```

## Commands

```bash
cmake -S . -B build
```

```bash
cmake --build build
```

```bash
cd build && ctest
```

## Scenarios

### Testing private methods

### Free functions / C-like APIs

- interface
- templates

### Injecting mock in a class

### Timers
