CC		= gcc
CFLAGS	= -Wall -Wextra -std=c99
OBJFILES = amaze.o cell.o cda.o da.o stack.o queue.o maze.o processOptions.o 
BSTOFILES = bst.o queue.o tnode.o cda.o integer.o
RBTOFILES = rbt.o gst.o $(BSTOFILES)
TREEFILES = gst.o rbt.o string.o options.o scanner.o $(BSTOFILES)

all: amaze trees

trees: trees.o $(TREEFILES)
	$(CC) $(CFLAGS) -o trees trees.o $(TREEFILES)

rbt-0-9: rbt-0-9.o $(RBTOFILES)
	$(CC) $(CFLAGS) -o rbt-0-9 rbt-0-9.o $(RBTOFILES)

rbt-0-0: rbt-0-0.o $(RBTOFILES)
	$(CC) $(CFLAGS) -o rbt-0-0 rbt-0-0.o $(RBTOFILES)

gst-0-0: gst-0-0.o gst.o $(BSTOFILES)
	$(CC) $(CFLAGS) -o gst-0-0 gst-0-0.o gst.o $(BSTOFILES)

gst-0-4: gst-0-4.o gst.o $(BSTOFILES)
	$(CC) $(CFLAGS) -o gst-0-4 gst-0-4.o gst.o $(BSTOFILES)

bst-0-9: bst-0-9.o $(BSTOFILES)
	$(CC) $(CFLAGS) -o bst-0-9 bst-0-9.o $(BSTOFILES)

bst-0-4: bst-0-4.o $(BSTOFILES)
	$(CC) $(CFLAGS) -o bst-0-4 bst-0-4.o $(BSTOFILES)

bst-0-0: bst-0-0.o $(BSTOFILES)
	$(CC) $(CFLAGS) -o bst-0-0 bst-0-0.o $(BSTOFILES)

rbt-0-4: rbt-0-4.o $(RBTOFILES)
	$(CC) $(CFLAGS) -o rbt-0-4 rbt-0-4.o $(RBTOFILES)

rbt-0-5: rbt-0-5.o $(RBTOFILES)
	$(CC) $(CFLAGS) -o rbt-0-5 rbt-0-5.o $(RBTOFILES)

test: $(OBJFILES) amaze
	./amaze -c 10 10 maze -s maze solved -d solved 

valgrind: $(OBJFILES) amaze
	valgrind ./amaze -c 10 10 maze -s maze solved -d solved

amaze: $(OBJFILES)
	$(CC) $(CFLAGS) -o amaze $(OBJFILES)

clean:
	rm -f *.o amaze bst-0-0 rbt-0-9 bst-0-4 bst-0-9 gst-0-4 rbt-0-4 rbt-0-0 gst-0-0 rbt-0-5 trees