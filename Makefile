CC = gcc
CFLAGS = -D_GNU_SOURCE -Wall -Wextra -Werror -std=c11 -Iinclude

SRC = src/main.c src/runtime.c src/process.c src/fs.c src/mount.c
OBJ = $(SRC:.c=.o)

TARGET = dockey

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)
