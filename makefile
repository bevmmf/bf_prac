# var
CC := gcc
CFLAGS := -Wall -O2 -g
LDLIBS := -lcheck -lsubunit -lpthread -lm -lrt

# file list
SRC := bloom.c test_bloom.c xxhash.c
# header files
HEADERS := bloom.h xxhash.h
# object files
OBJ := $(SRC:.c=.o)
TARGET := bloom

# rule
%.o: %.c bloom.h xxhash.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: all check clean
all: $(TARGET)

check: $(TARGET)
	./$(TARGET)
clean:
	$(RM) $(OBJ) $(TARGET)
