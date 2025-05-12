COMPILE_OPTION=-Wall -g

cpu_scheduler: process.o queue.o processor.o simulator.o main.o
	gcc -o cpu_scheduler process.o queue.o main.o simulator.o processor.o

processor.o: processor.c processor.h queue.h process.h
	gcc $(COMPILE_OPTION) -c -o processor.o processor.c

simulator.o: simulator.c simulator.h processor.h 
	gcc $(COMPILE_OPTION) -c -o simulator.o simulator.c

process.o: process.c process.h 
	gcc $(COMPILE_OPTION) -c -o process.o process.c

queue.o: queue.c process.h queue.h 
	gcc $(COMPILE_OPTION) -c -o queue.o queue.c

main.o: main.c queue.h process.h simulator.h processor.h
	gcc $(COMPILE_OPTION) -c -o main.o main.c

clean:
	rm *.o cpu_scheduler