
CC = g++-4.9
CFLAGS = -O3
CFLAGS_PTHREAD = -O3 -lpthread
CFLAGS_PTHREAD_TM = -O3 -lpthread -fgnu-tm

all: randtrack_list_lock

randtrack: list.h hash.h defs.h randtrack.cc
	$(CC) $(CFLAGS) randtrack.cc -o randtrack
randtrack_global_lock: list.h hash.h defs.h randtrack_global_lock.cc
	$(CC) $(CFLAGS_PTHREAD) randtrack_global_lock.cc -o randtrack_global_lock
randtrack_tm: list.h hash.h defs.h randtrack_tm.cc
	$(CC) $(CFLAGS_PTHREAD_TM) randtrack_tm.cc -o randtrack_tm
randtrack_list_lock: list.h hash_list.h defs.h randtrack_list_lock.cc
	$(CC) $(CFLAGS_PTHREAD) randtrack_list_lock.cc -o randtrack_list_lock

clean:
	rm -f *.o randtrack randtrack_global_lock randtrack_tm randtrack_list_lock
