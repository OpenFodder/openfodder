# Keep the convenience Make target on the supported CMake build path.
# CMake owns dependency selection and requires SDL3/SDL3_mixer.
CMAKE ?= cmake
BUILD_DIR ?= build
BUILD_TYPE ?= Release

.PHONY: all clean

all:
	$(CMAKE) -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	$(CMAKE) --build $(BUILD_DIR) --config $(BUILD_TYPE)

clean:
	$(CMAKE) --build $(BUILD_DIR) --target clean --config $(BUILD_TYPE)
