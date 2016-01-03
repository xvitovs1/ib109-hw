all: compile run

compile: 
	gcc -std=c99 -pthread -O0 -o falseSharing falseSharing.c
	gcc -std=c99 -pthread -O0 -o threadsAndProcesses threadsAndProcesses.c

run: 
	@./falseSharing
	@./threadsAndProcesses

