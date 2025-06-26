# libui Example Application

This is a simple example project demonstrating usage of libui-ng with a basic GUI on Windows.

## Requirements

- Windows OS (for now)
- GCC (MinGW or similar) with windres
- libui-ng built and available in libui-ng directory (recursive module https://github.com/libui-ng/libui-ng)
- Standard Windows libraries (comctl32, ole32, uuid, d2d1, dwrite)

## Building

Run make to build the executable myapp.exe.

`make`

This compiles main.c and links it against libui-ng and required Windows libraries. It also compiles the Windows resource file myapp.rc into myapp.res.

## Running

Run the application with:

`make run`

or

`./myapp.exe`


## Cleaning

Remove build artifacts with:

`make clean`

## TODO

- add a .gitignore (.res .exe .o)
- add libui-ng as a (recursive module https://github.com/libui-ng/libui-ng)
- check if it build on linux / mac OS
