# GEC-6818: 智能生产车间环境数据监测系统

Created by [@ajuniezeng](mailto:me@ajunie.com).

Some code is from [@Kenshin2438](https://github.com/Kenshin2438/GEC6818).

## Cross Compile

To Cross Compile to `arm-linux-gnueabi`, you need to install [`zig`](https://ziglang.org/download/). And run:

``` 
zig build
```

You can find the artifacts in the `zig-cache/bin` directory.

## Compile

You can just compile for your host machine using CMake.

```
cmake -S . -B build
cmake --build build
```
