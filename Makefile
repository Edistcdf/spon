CC = gcc
CFLAGS = -Wall -Werror -g
TARGET = main
TEST_TARGET = tests

all: $(TARGET)

$(TARGET): main.o add.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o add.o

main.o: main.c add.h
	$(CC) $(CFLAGS) -c main.c

test: test.o add.o
	$(CC) $(CFLAGS) -o $(TEST_TARGET) test.o add.o
	./$(TEST_TARGET)

test.o: test/test.c add.h
	$(CC) $(CFLAGS) -c test/test.c

add.o: add.c add.h
	$(CC) $(CFLAGS) -c add.c

clean:
	rm -f *.o $(TARGET) $(TEST_TARGET)

