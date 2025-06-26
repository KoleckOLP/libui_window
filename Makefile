# Detect platform
UNAME_S := $(shell uname -s 2>/dev/null || echo Unknown)
OS_VAR := $(OS)

ifeq ($(OS_VAR),Windows_NT)
  # Could be native Windows or MinGW (Git Bash etc.)
  ifneq (,$(findstring MINGW,$(UNAME_S)))
    PLATFORM := MinGW
  else
    PLATFORM := Windows
  endif
else
  ifeq ($(UNAME_S),Darwin)
    PLATFORM := macOS
  else ifeq ($(UNAME_S),Linux)
    PLATFORM := Linux
  else
    PLATFORM := Unknown
  endif
endif

# Variables
CC ?= gcc
WINDRES = windres
# libui-ng is C99 so we follow
CFLAGS_PROD = -Ilibui-ng -Wall -Wextra -O2 -std=c99 -static
CFLAGS_DEBUG = -Ilibui-ng -Wall -Wextra -g -O0 -std=c99 -DDEBUG
SRC = controls.c main.c 
OBJ = $(SRC:.c=.o)
RC = myapp.rc
RES = myapp.res
MANIFEST = myapp.manifest
OUT = myapp
APPBUNDLE = myapp.app/

ifeq ($(PLATFORM),MinGW)
LDFLAGS_PROD = -Llibui-ng/builddir/meson-out -lui -lstdc++ -lcomctl32 -lole32 -luuid -ld2d1 -ldwrite -mwindows -static
LDFLAGS_DEBUG = -Llibui-ng/builddir/meson-out -lui -lstdc++ -lcomctl32 -lole32 -luuid -ld2d1 -ldwrite -lgdi32
else ifeq ($(PLATFORM),macOS)
LDFLAGS_PROD := -Llibui-ng/builddir/meson-out -lui -framework CoreText -framework CoreGraphics -framework Foundation -framework AppKit
# there no linker change between prod and debug cause macOS libraries doesn't support static linking
LDFLAGS_DEBUG := -Llibui-ng/builddir/meson-out -lui -framework CoreText -framework CoreGraphics -framework Foundation -framework AppKit
endif

ifeq ($(PLATFORM),MinGW)
all: prod
else ifeq ($(PLATFORM),macOS)
all: prod $(APPBUNDLE)
endif

# macOS only
$(APPBUNDLE): $(OUT)
	mkdir $(APPBUNDLE) && mkdir $(APPBUNDLE)Contents && mkdir $(APPBUNDLE)Contents/MacOS
	cp Info.plist $(APPBUNDLE)/Contents
	cp $(OUT) $(APPBUNDLE)/Contents/MacOS/

prod: CFLAGS := $(CFLAGS_PROD)
prod: LDFLAGS := $(LDFLAGS_PROD)
prod: $(OUT)

debug: CFLAGS := $(CFLAGS_DEBUG)
debug: LDFLAGS := $(LDFLAGS_DEBUG)
debug: $(OUT)

libui-ng/builddir/meson-out/libui.a:
	cd libui-ng && meson setup builddir --buildtype=release --default-library=static
	cd libui-ng && ninja -C builddir

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Windows only
$(RES): $(RC) $(MANIFEST)
	$(WINDRES) $< -O coff -o $@

ifeq ($(PLATFORM),MinGW)
$(OUT): libui-ng/builddir/meson-out/libui.a $(OBJ) $(RES)
	$(CC) $(OBJ) $(RES) $(LDFLAGS) -o $@
else ifeq ($(PLATFORM),macOS)
$(OUT): libui-ng/builddir/meson-out/libui.a $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@
endif

run: prod $(OUT)
	./$(OUT)

# macOS only
runapp: prod $(APPBUNDLE)
	open $(APPBUNDLE)

clean:
	rm -rf $(OBJ) $(RES) $(OUT) $(OUT).exe $(APPBUNDLE)

mrproper: clean
	rm -rf libui-ng/builddir/

platform:
	@echo "Detected platform: $(PLATFORM)"
