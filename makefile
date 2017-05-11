CC = g++
MKLIB = ar rcs
LIBNM = libDreng.a
CMPARG = -c
DEBUG = -g
OBJS = board.o game.o jump.o move.o
DEPS = engine/board.cpp engine/game.cpp engine/jump.cpp engine/move.cpp
TEST = test.cpp $(DEPS) tests/engine/board.spec.cpp tests/engine/game.spec.cpp tests/engine/move.spec.cpp

dreng: $(DEPS)
	$(CC) $(CMPARG) $(DEPS)
	$(MKLIB) $(LIBNM) $(OBJS)
	rm $(OBJS)
	$(CC) -o example main.cpp -L. -lDreng

unittest:
	$(CC) $(DEBUG) $(TEST)

clean:
	rm *.out
	rm *.a