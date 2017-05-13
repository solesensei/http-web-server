tokenizing.o:tokenizing.cpp interpretlib.h
	g++ -Wall -g -c tokenizing.cpp -o tokenizing.o
token:tmain.cpp tokenizing.o interpretlib.h
	g++ -Wall -g tokenizing.o tmain.cpp -o token
syntax.o:syntax.cpp interpretlib.h
	g++ -Wall -g -c syntax.cpp -o syntax.o
semantic.o:semantic.cpp interpretlib.h
	g++ -Wall -g -c semantic.cpp -o semantic.o
execute.o:execute.cpp interpretlib.h
	g++ -Wall -g -c execute.cpp -o execute.o
main: polizmain.cpp tokenizing.o syntax.o semantic.o execute.o interpretlib.h
	g++ -Wall -g polizmain.cpp tokenizing.o syntax.o semantic.o execute.o -o js
