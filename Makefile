CC 				:= gcc
CFLAGS 		:= -Wall -Wextra -Wpedantic -O3
LDLIBS 	:= -lm

SRC 			:= example.c
HEADERS 	:= p6ppm.h

BUILD 		:= build
VIDEO			:= example.mp4

TARGET 		:= $(BUILD)/example

FFMPEG := $(shell command -v ffmpeg)

ifeq ($(FFMPEG),)
$(error ffmpeg not found. please install to run the example)
endif

$(TARGET): $(SRC) $(HEADERS) | $(BUILD)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDLIBS)

$(BUILD):
	mkdir -p $(BUILD)

.PHONY: clean


clean:
	rm -rf 	build
	rm -f 	$(VIDEO)

check-ffmpeg:
	@command -v ffmpeg >/dev/null || \
		{ echo "ffmpeg not found"; exit 1; }

video: check-ffmpeg $(TARGET)
	rm -f $(VIDEO)
	./$(TARGET) | ffmpeg -f image2pipe -framerate 60 -i - -pix_fmt yuv420p $(VIDEO)
