CC = gcc
CFLAGS = -Wall -Wextra -pthread -O0
TARGETS = fire fire4 fire_safe

all: $(TARGETS)

fire: fire.c
	$(CC) $(CFLAGS) -o $@ $<

fire4: fire4.c
	$(CC) $(CFLAGS) -o $@ $<

fire_safe: fire_safe.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean
