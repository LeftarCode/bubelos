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

OBJECTS := ${C_SRCS:.c=.o}
BUILD_DIR  := ../../build/kernel
BINARY     := ${BUILD_DIR}/kernel.elf

.PHONY: all clean

all: ${BINARY}

# gcc-13 -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -T src/kernel/kernel.ld build/main.o -o build/kernel.elf -nostdlib -lgcc
${BINARY}: ${BUILD_DIR} ${OBJECTS}
	${CC} -T kernel.ld -o ${BINARY} ${LDFLAGS} ${OBJECTS} ${LIBS}

# gcc-13 -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -c src/kernel/src/main.c -o build/main.o
%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

clean:
	rm -f ${OBJECTS}
	rm -f ${BINARY}