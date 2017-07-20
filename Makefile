
MAKE?=make

NPROC:=$(shell nproc || echo 2)

ifeq ($(OS),Windows_NT)
 TARGET_EXE := release
 TARGET_NAME := rtide.exe
else
 TARGET_EXE := all
 TARGET_NAME := rtide
endif

all: bin/$(TARGET_NAME)

build/Makefile:
	@test -d build/ || mkdir -p build/
	cd build/ && qmake src/rtide.pro

bin/$(TARGET_NAME): build/Makefile FORCE
	cd build/ && $(MAKE) $(TARGET_EXE) -j$(NPROC)

FORCE:
