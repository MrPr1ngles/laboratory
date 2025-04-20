CC = gcc
CFLAGS = -Wall -Wextra -g
SRC = main.c polynomial.c integer.c double.c interface.c polynomialErrors.c
OBJ = $(SRC:.c=.o)
TARGET = start

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)