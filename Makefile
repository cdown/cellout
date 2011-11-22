# vim: noexpandtab :

PREFIX ?= .
CFLAGS := -ansi -pedantic -Wall -Wextra -Werror \
          -Wno-missing-braces -Wno-missing-field-initializers -Wformat=2 \
          -Wswitch-default -Wswitch-enum -Wcast-align -Wpointer-arith \
          -Wbad-function-cast -Wstrict-overflow=5 -Wstrict-prototypes -Winline \
          -Wundef -Wnested-externs -Wcast-qual -Wshadow -Wunreachable-code \
          -Wlogical-op -Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls \
          -Wold-style-definition \
          ${CFLAGS}

cellout: cellout.c
	${CC} ${CFLAGS} -o cellout cellout.c

install:
	install -m755 cellout ${PREFIX}
