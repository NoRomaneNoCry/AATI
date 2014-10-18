CC = clang++
CFLAGS = -c -Wall -pedantic
 
OPENFLAGS = `pkg-config opencv --cflags`
OPENLIB = `pkg-config opencv --libs`

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
EXEC = main
 

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(OPENLIB)

main.o: main.cpp 
	$(CC) $< $(CFLAGS) $(OPENFLAGS)

%.o: %.cpp %.h
	$(CC) $< $(CFLAGS) $(OPENFLAGS)

clean: 
	rm -rf *.o *~ $(EXEC)
