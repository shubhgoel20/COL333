CC="g++"
CFLAGS="-Wall -std=c++17 -O3 -funroll-loops -DASIO_STANDALONE"

INCLUDES="-Iinclude"

SRC="learn.cpp board.cpp butils.cpp bdata.cpp engine_old.cpp"

$CC $CFLAGS $INCLUDES $SRC -lpthread -o learner.out

