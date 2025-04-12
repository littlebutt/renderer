CC = gcc
CFLAGS = -I include -g -Wall -O0
OS := $(shell uname)

ifeq ($(OS),Linux)
	LDFLAGS =
	TARGET = renderer
else ifeq ($(OS),Darwin)
	LDFLAGS =
	TARGET = renderer
else
	LDFLAGS = -lgdi32
	TARGET = renderer
	OBJS = \
	$(patsubst ./src/%.c, ./build/%.o, $(wildcard ./src/*.c)) \
	$(patsubst ./win/%.c, ./build/%.o, $(wildcard ./win/*.c))
endif

all: build $(TARGET)

build:
	mkdir -p build

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

./build/%.o: ./src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

./build/%.o: ./win/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)