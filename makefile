.DEFAULT_GOAL := compile_run

assign7:
	@gcc -Wall -o assign7 assign7.c

compile_run:
	@gcc -Wall -o assign7 assign7.c
	@./assign7