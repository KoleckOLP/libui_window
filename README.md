# libui Example Application

This is a simple example project demonstrating usage of libui-ng with a basic GUI on Windows.

### clone recursively!

```
git clone --recursive https://github.com/KoleckOLP/libui_window.git
```

## Requirements

- Windows OS (currently only tested on Windows)
- GCC (MSYS / MingGW) with windres
- libui-ng clone with the repo, built with make.
- Standard Windows libraries (comctl32, ole32, uuid, d2d1, dwrite, dgi32) they come with Windows.

## Building

Run make to build the executable myapp.exe.

`make`

This builds libui-ng, controls.c main.c as well as Windows resource file and links them all together with Windows libraries. Production build by default.

## other make commands

`make clean debug run`

Builds the debug build and runs it, this one has Console output.

`make clean prod run`

Builds the prod build and runs it, optimized and not Console ouput.

changing between proc and debug needs a `make clean`.

## Cleaning

Remove build artifacts with:

`make clean`

## TODO

- Test on other platforms like, Linux and macOS.
- macOS linker line: `gcc -Llibui-ng/builddir/meson-out -lui -framework CoreText -framework CoreGraphics -framework Foundation -framework AppKit controls.o main.o -o myapp`
