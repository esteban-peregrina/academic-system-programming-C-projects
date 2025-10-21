CC=clang
all: block_ctrlc new_handler sigaction


block_ctrlc: block_ctrlc.o
	$(CC) block_ctrlc.o -o block_ctrlc

new_handler: new_handler.o
	$(CC) new_handler.o -o new_handler

sigaction: sigaction.o
	$(CC) sigaction.o -o sigaction

new_handler.o: new_handler.c
	$(CC) -c new_handler.c

block_ctrlc.o: block_ctrlc.c
	$(CC) -c block_ctrlc.c

sigaction.o: sigaction.c
	$(CC) -c sigaction.c

clean:
	rm -f sigaction block_ctrlc new_handler *.o
