

# screenshot info
SOURCE="sst.c"
NAME="sst"
VERSION="00"

# includes and libs
INCS=""
LIBS="-lX11"

# flags
CPPFLAGS="-D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=2 -DVERSION=\"${VERSION}\""
CFLAGS="-g -std=c99 -pedantic -Wall -Wno-deprecated-declarations -O0 -fsanitize=address ${INCS} ${CPPFLAGS}"
LDFLAGS=${LIBS}

# compiler and linker
CC="cc"

# building
${CC} $CFLAGS -o $NAME $SOURCE $LDFLAGS
