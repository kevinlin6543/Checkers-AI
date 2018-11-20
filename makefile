CXX=g++
CFLAGS = -c
DEPS = board.h game.h

%.o: %.cpp $(DEPS)
	$(CXX) $(CFLAGS) -o $@ $< $(CFLAGS)

checkers.exe: ai.o board.o game.o
	$(CXX) -o checkers.exe ai.o board.o game.o

debug:
	$(CXX) $(CFLAGS) -g checkersDebug.exe checkersai.cpp board.cpp game.cpp

clean:
	rm *.exe *.o *.stackdump *~
