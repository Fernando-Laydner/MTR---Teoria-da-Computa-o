CFLAGS=-Wpedantic -Wall -Wextra -Wstrict-aliasing -fstrict-aliasing
SOURCE=mtr.c
TARGET=mtr

all:$(TARGET)

$(TARGET):$(SOURCE)
	$(CC) $(CFLAGS) -o $@ $(SOURCE)

clean:
	rm -f $(TARGET)

run:
	./$(TARGET) < exemplo-1.txt
