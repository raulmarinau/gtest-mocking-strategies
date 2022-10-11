# UT guideline

## Scenarios

### Free functions / C-like APIs

- interface
- templates

### Testing private methods

- public fun -> private fun
- public members
- FRIEND_TEST -> does not work in production

### Injecting mock in a class

### Timers

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
