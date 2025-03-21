# Cross Compile Example

To Cross Compile, you need to install [`zig`](https://ziglang.org/download/).

``` 
zig build
```

You can find the artifacts in the `zig-cache/bin` directory.

<!-- ```
cmake -S . -DCMAKE_C_FLAGS="-fuse-ld=lld" -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake -B build
cmake --build build
``` -->

Or you can just compile for your host machine using CMake.

```
cmake -S . -B build
cmake --build build
```