# Group 18
#
# NAME: Kartik Kapoor
# NSID: KAK110
# Student Number: 11269913
#
# NAME: Shivansi Prajapati
# NSID: LZU375
# Student Number: 11326745


CFLAGS = -Wall -Wextra -pedantic -std=gnu90 -g
RTT_LIB_PATH = /student/cmpt332/rtt/lib/Linuxx86_64 	
PTHREAD = /student/cmpt332/pthreads/lib/Linuxx86_64
CC = gcc
ARM = arm-linux-gnueabihf-
ARMVER = -10
PPATHARM = /lib/Linuxarmv6l/
PPC = powerpc-linux-gnu-
PPATHPPC = /lib/Linuxppc/

.PHONY: clean all 

all: reader-writer-test x86_64

reader-writer-test: reader-writer-monitor.o reader-writer.o libMonitor.a liblist.a
	${CC} ${CFLAGS} -o my-writer reader-writer-monitor.o reader-writer.o \
	-L${RTT_LIB_PATH} -lRtt -lRttUtils -ltirpc -L. -llist \
	 -lMonitor

reader-writer-monitor.o: reader-writer-monitor.c reader-writer-monitor.h \
	Monitor.h 
	${CC} ${CFLAGS} -o reader-writer-monitor.o -c reader-writer-monitor.c \
	-I.


reader-writer.o: reader-writer.c reader-writer-monitor.h Monitor.h
	${CC} ${CFLAGS} -o reader-writer.o -c reader-writer.c -I. \
	-I/student/cmpt332/rtt/include -I/usr/include/tirpc

libMonitor.a: Monitor.o list_basic.o list_adders.o list_movers.o \
	list_removers.o list_search.o
	ar rcs libMonitor.a Monitor.o list_basic.o list_adders.o \
	list_movers.o list_removers.o list_search.o


Monitor.o: Monitor.c Monitor.h
	${CC} ${CFLAGS} -o Monitor.o -c Monitor.c -I. \
	-I/student/cmpt332/rtt/include -I/usr/include/tirpc

liblist.a: list_basic.o list_adders.o list_movers.o list_removers.o \
	list_search.o
	ar rcs liblist.a list_basic.o list_adders.o list_movers.o \
	list_removers.o list_search.o

list_basic.o: list_basic.c list.h
	$(CC) $(CFLAGS) -c list_basic.c -I .

list_adders.o: list_adders.c list.h
	$(CC) $(CFLAGS) -c list_adders.c -I .

list_movers.o: list_movers.c list.h
	$(CC) $(CFLAGS) -c list_movers.c -I .

list_removers.o: list_removers.c list.h
	$(CC) $(CFLAGS) -c list_removers.c -I .

list_search.o: list_search.c list.h
	$(CC) $(CFLAGS) -c list_search.c -I .



x86_64: s-chat

s-chat: s-chat.o liblist.a
	gcc ${CFLAGS} -o s-chat s-chat.o -L ${PTHREAD} -lpthreads -L . -llist

s-chat.o: s-chat.c
	gcc ${CFLAGS} -c s-chat.c -I/student/cmpt332/pthreads -I.

liblist.a:list_basic.o list_adders.o list_movers.o list_removers.o \
	list_search.o
	ar rcs liblist.a list_basic.o list_adders.o list_movers.o \
        list_removers.o list_search.o

arm: s-chat-arm

s-chat-arm: s-chat.o liblistarm.a
	$(ARM)$(CC)$(ARMVER) $(CFLAGS) -o s-chat-arm s-chat-arm.o -L ${PTHREAD} -lpthreads -L . -llist

s-chat-arm.o: s-chat.c
	$(ARM)$(CC)$(ARMVER) $(CFLAGS) -o s-chat-arm.o -c s-chat.c -I/student/cmpt332/pthreads -I.

liblistarm.a: list_basic.o list_adders.o list_movers.o list_removers.o \
	list_search.o
	$(ARM) ar rcs liblistarm.a list_basic.o list_adders.o list_movers.o \
        list_removers.o list_search.o

ppc: s-chat-ppc

s-chat-ppc: s-chat.o liblistppc.a
	$(PPC)$(CC)$(ARMVER) $(CFLAGS) -o s-chat-ppc s-chat.o -L ${PTHREAD} -lpthreads -L . -llist

s-chat-ppc.o: s-chat.c
	$(PPC)$(CC)$(ARMVER) $(CFLAGS) -o s-chat-ppc.o -c s-chat.c -I/student/cmpt332/pthreads -I.

liblistppc.a: list_basic.o list_adders.o list_movers.o list_removers.o \
	list_search.o
	$(PPC) ar rcs liblistppc.a list_basic.o list_adders.o list_movers.o \
        list_removers.o list_search.o
clean:
	rm -rf *.o *.a my-writer s-chat s-chat-ppc s-chat-arm
