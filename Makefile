all:
	@echo 'start compile...'
	gcc -o inject inject.c -g
clean:
	@rm inject
