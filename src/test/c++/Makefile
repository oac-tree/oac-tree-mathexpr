.PHONY: all cmake_targets

PROGNAME=unit-tests

TARGET=../../../target

BUILD_DIR := ./build

BUILD_TYPE = RelWithDebInfo

CMAKE_FLAGS = -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
CMAKE_FLAGS += -DCMAKE_INSTALL_PREFIX=$(TARGET)

all: cmake_targets

cmake_targets:
	@mkdir -p $(BUILD_DIR)
	cmake3 -B$(BUILD_DIR) $(CMAKE_FLAGS)
	cmake3 --build $(BUILD_DIR)
	make -C $(BUILD_DIR) install