app:main.o ./src/add.o ./src/div.o ./src/mul.o ./src/sub.o
	gcc main.o ./src/add.o ./src/div.o ./src/mul.o ./src/sub.o -o app -I ./include

main.o:main.c
	gcc -c main.c -o main.o -I ./include
add.o:add.c
	gcc -c ./src/add.c -o ./src/add.o -I ./include
div.o:div.c
	gcc -c ./src/div.c -o ./src/div.o -I ./include
mul.o:mul.c
	gcc -c ./src/mul.c -o ./src/mul.o -I ./include
sub.o:sub.c
	gcc -c ./src/sub.c -o ./src/sub.o -I ./include
