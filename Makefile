CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude

SRC = src/main.c src/runtime.c src/process.c src/fs.c src/mount.c src/error.c src/util.c
OBJ = $(SRC:.c=.o)

TARGET = mini-docker

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)
