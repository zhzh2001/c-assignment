objects = main.o init.o print.o probe.o

maze: $(objects)
	gcc -o maze $(objects)

main.o: main.c maze.h init.h print.h probe.h
	gcc -c main.c -O2
init.o: init.c maze.h init.h
	gcc -c init.c -O2
print.o: print.c maze.h print.h
	gcc -c print.c -O2
probe.o: probe.c maze.h probe.h
	gcc -c probe.c -O2
clean: 
	rm maze $(objects)
