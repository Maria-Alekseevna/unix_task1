CC=gcc
CFLAGS=-Wall -Wextra -O2

all: myprogram

myprogram: myprogram.c
	$(CC) $(CFLAGS) myprogram.c -o myprogram

clean:
	rm -f myprogram *.o A B C D *.gz result.txt