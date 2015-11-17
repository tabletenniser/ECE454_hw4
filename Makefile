
CC = g++-4.9
CFLAGS = -O3
CFLAGS_PTHREAD = -O3 -lpthread

all: randtrack_global_lock

randtrack: list.h hash.h defs.h randtrack.cc
	$(CC) $(CFLAGS) randtrack.cc -o randtrack
randtrack_global_lock: list.h hash.h defs.h randtrack_global_lock.cc
	$(CC) $(CFLAGS_PTHREAD) randtrack_global_lock.cc -o randtrack_global_lock


clean:
	rm -f *.o randtrack randtrack_global_lock randtrack_tm randtrack_list_lock
