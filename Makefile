CC = $(shell xcrun --sdk iphoneos --find clang)
SDK = $(shell xcrun --sdk iphoneos --show-sdk-path)
SRC = $(wildcard ./src/*.c)
LIBS = $(SDK)/usr/lib

CFLAGS = -isysroot $(SDK) -miphoneos-version-min=6.0 -arch armv7 -I./include
LDFLAGS = -L$(LIBS) -lc -lSystem -framework CoreFoundation -framework IOKit 

all: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o bad_queue $^

clean:
	@rm -rf bad_queue
	@rm -rf *.o
