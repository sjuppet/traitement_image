ROOT = ../bcl
ROOT = ../bcl

CPPFLAGS = -I$(ROOT)/include -I.
CFLAGS   = -Wall -Wextra -Werror -std=c99 -g
LDFLAGS  = -L$(ROOT)/lib
LDLIBS   = -lbcl -lm

BIN = \
	move\
	rotate\
	wave\
	flip\
	shear

.PHONY: all
all: $(BIN)

shear: domain.o
rotate: domain.o
wave: domain.o

.PHONY: clean cleanall
clean:
	$(RM) *.o
cleanall: clean
	$(RM) $(BIN)
