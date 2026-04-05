# Common ReXamples lesson build rules
#
# Expected environment:
#   vc available in PATH
#   AMIGA_NDK_HEADERS_INCLUDE exported by env.sh

SRC ?= main.c
OBJ ?= main.o

ifndef VBCCENV
$(error VBCCENV is not set. Please source your env.sh before building.)
endif

ifndef AMIGA_NDK_HEADERS_INCLUDE
$(error AMIGA_NDK_HEADERS_INCLUDE is not set. Please source your env.sh before building.)
endif

export VBCC
export VBCCENV

CFLAGS ?= \
	-c \
	-O2 \
	-D__AMIGAOS3__ \
	-I"$(AMIGA_NDK_HEADERS_INCLUDE)"

LDFLAGS ?= \
	-o $(PROGRAM)

.PHONY: all clean envcheck

all: envcheck $(PROGRAM)

envcheck:
	@printf "VBCC=%s\n" "$(VBCC)"
	@printf "VBCCENV=%s\n" "$(VBCCENV)"
	@printf "AMIGA_NDK_HEADERS_INCLUDE=%s\n" "$(AMIGA_NDK_HEADERS_INCLUDE)"
	@which vc >/dev/null

$(PROGRAM): $(OBJ)
	vc $(LDFLAGS) $(OBJ) -lamiga

main.o: $(SRC)
	vc $(CFLAGS) $(SRC)

clean:
	rm -f $(OBJ) $(PROGRAM)
