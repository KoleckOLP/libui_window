# Variables
CC ?= gcc
WINDRES = windres
# libui-ng is C99 so we follow
CFLAGS = -Ilibui-ng -Wall -Wextra -O2 -std=c99
LDFLAGS = -Llibui-ng/builddir/meson-out -lui -lstdc++ -lcomctl32 -lole32 -luuid -ld2d1 -ldwrite -lgdi32
#-mwindows
SRC = controls.c main.c 
OBJ = $(SRC:.c=.o)
RC = myapp.rc
RES = myapp.res
MANIFEST = myapp.manifest
OUT = myapp.exe

all: $(OUT)

libui-ng/builddir/meson-out/libui.a:
	cd libui-ng && meson setup builddir --buildtype=release --default-library=static
	cd libui-ng && ninja -C builddir

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(RES): $(RC) $(MANIFEST)
	$(WINDRES) $< -O coff -o $@

$(OUT): libui-ng/builddir/meson-out/libui.a $(OBJ) $(RES)
	$(CC) $(OBJ) $(RES) $(LDFLAGS) -o $@

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OBJ) $(RES) $(OUT)
