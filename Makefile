all: histo

AVL.o: AVL.c AVL.h
	gcc -c AVL.c -o AVL.o

noeud.o: noeud.c noeud.h
	gcc -c noeud.c -o noeud.o

histo.o: histo.c histo.h AVL.h noeud.h
	gcc -c histo.c -o histo.o

main.o: main.c AVL.h histo.h
	gcc -c main.c -o main.o

histo: AVL.o noeud.o histo.o main.o
	gcc AVL.o noeud.o histo.o main.o -o histo

clean:
	rm -f *.o test histo *.png *.dat *.csv

re: clean all


