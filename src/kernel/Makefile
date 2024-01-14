CC := x86_64-linux-gnu-gcc


SRC_DIR := src


CFLAGS :=          \
	-ffreestanding \
	-mcmodel=large \
	-mno-red-zone  \
	-mno-mmx       \
	-mno-sse       \
	-mno-sse2

LDFLAGS :=    \
	-nostdlib \
	-z max-page-size=0x1000

LIBS := -lgcc

C_SRCS := ${SRC_DIR}/main.c

OBJECTS := ${C_SOURCES:.c=.o}
BUILD_DIR  := build
BINARY     := ${BUILD_DIR}/kernel.elf

.PHONY: all clean

all: ${BINARY}

${BINARY}: ${BUILD_DIR} ${OBJECTS}
	${CC} -T ${SRC_DIR}/kernel.ld -g3 -o ${BINARY} ${LDFLAGS} ${OBJECTS} ${LIBS}

%.o: %.c
	${CC} ${INCLUDE_FLAG} -g -c $< -o $@ ${CFLAGS}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

clean:
	rm -f ${OBJECTS}
	rm -f ${BINARY}