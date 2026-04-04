# Common ReXamples lesson build rules
#
# Expected environment:
#   vc available in PATH
#   AMIGA_NDK_HEADERS_INCLUDE exported by env.sh

SRC ?= main.c
OBJ ?= main.o

CFLAGS ?= \
	-c \
	-O2 \
	+amigaos \
	-D__AMIGAOS3__ \
	-I"$(AMIGA_NDK_HEADERS_INCLUDE)"

LDFLAGS ?= \
	+amigaos \
	-o $(PROGRAM)

.PHONY: all clean

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	vc $(LDFLAGS) $(OBJ)

main.o: $(SRC)
	vc $(CFLAGS) $(SRC)

clean:
	rm -f $(OBJ) $(PROGRAM)
