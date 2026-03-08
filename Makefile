

compile: clean
	gcc -I include src/*.c -o shell

clean:
	rm -rf shell

test: compile
	./shell comandos
