CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC = example.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = example

.PHONY: all clean c

all a: $(EXECUTABLE)
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean c:
	rm -f $(OBJ) $(EXECUTABLE)
