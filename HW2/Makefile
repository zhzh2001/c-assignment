objects = main.o list.o sort.o

sort: $(objects)
	gcc -o sort $(objects)

main.o: main.c general.h list.h sort.h
	gcc -c main.c
list.o: list.c general.h list.h
	gcc -c list.c
sort.o: sort.c general.h sort.h list.h
	gcc -c sort.c
clean: 
	rm sort $(objects)
