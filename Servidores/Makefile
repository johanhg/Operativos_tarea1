
all: libancillary.a main.o main2.o SystemFunctions.o AttendRequest.o ParseRequest.o Resource.o RespondRequest.o CGIExecution.o  WebServerFork.o WebServerThread.o
	gcc -Wall -pthread -std=c99 main.o SystemFunctions.o AttendRequest.o ParseRequest.o  Resource.o RespondRequest.o CGIExecution.o  WebServerFork.o libancillary.a -o WebServerFork
	gcc -Wall -pthread -std=c99 main2.o SystemFunctions.o AttendRequest.o ParseRequest.o  Resource.o RespondRequest.o CGIExecution.o WebServerThread.o libancillary.a -o WebServerThread

main2.o: main2.c WebServerThread.h
	gcc -c -D _SVID_SOURCE -Wall -std=c99 main2.c -o $@

WebServerThread.o: WebServerThread.c WebServerThread.h SystemFunctions.h AttendRequest.h
	gcc -c -D _XOPEN_SOURCE -Wall -std=c99 WebServerThread.c -o $@

libancillary.a: fd_send.o fd_recv.o
	ar cr $@ fd_send.o fd_recv.o
	ranlib $@

fd_send.o: lib/ancillary.h
	gcc -c -Wall -std=c99 lib/fd_send.c -o $@

fd_recv.o: lib/ancillary.h
	gcc -c -Wall -std=c99 lib/fd_recv.c -o $@

main.o: main.c WebServerFork.h
	gcc -c -D _SVID_SOURCE -Wall -std=c99 main.c -o $@

SystemFunctions.o: SystemFunctions.c SystemFunctions.h
	gcc -c -Wall -std=c99 SystemFunctions.c -o $@

AttendRequest.o: AttendRequest.c AttendRequest.h ParseRequest.h SystemFunctions.h Resource.h RespondRequest.h CGIExecution.h
	gcc -c -Wall -std=c99 AttendRequest.c -o $@

ParseRequest.o: ParseRequest.c ParseRequest.h SystemFunctions.h
	gcc -c -Wall -std=c99 ParseRequest.c -o $@

Resource.o: Resource.c Resource.h SystemFunctions.h
	gcc -c -Wall -std=c99 Resource.c -o $@	

RespondRequest.o: RespondRequest.c RespondRequest.h SystemFunctions.h
	gcc -c -Wall -std=c99 RespondRequest.c -o $@

CGIExecution.o: CGIExecution.h CGIExecution.c
	gcc -c -Wall -std=c99 CGIExecution.c -o $@

WebServerFork.o: WebServerFork.c WebServerFork.h SystemFunctions.h AttendRequest.h
	gcc -c -D _XOPEN_SOURCE -Wall -std=c99 WebServerFork.c -o $@
