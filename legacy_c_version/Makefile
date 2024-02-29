CC=cc
CFLAGS=-Wall -Wextra --std=c99 -pedantic
LDFLAGS=-lm


SRCDIR=src
SRC=$(addprefix $(SRCDIR)/, main.c ipv4_subnet.c)
OBJ=$(SRC:.c=.o)

TARGET=subnetters

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm $(OBJ) $(TARGET)
