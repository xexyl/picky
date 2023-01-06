# A simple Unix Makefile
CFLAGS=-O
PROG=picky
$(PROG): $(PROG).c
	$(CC) $(CFLAGS) -o $(PROG) $(PROG).c
	strip $(PROG)
clean:
	rm -f $(PROG)
test: $(PROG)
	./$(PROG) -s Makefile -t $(PROG).1 $(PROG).c
