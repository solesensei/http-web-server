tokenizing.o:tokenizing.cpp interpretlib.h
	g++ -Wall -g -c tokenizing.cpp -o tokenizing.o
token:tmain.cpp tokenizing.o interpretlib.h
	g++ -Wall -g tokenizing.o tmain.cpp -o token
syntax.o:syntax.cpp interpretlib.h
	g++ -Wall -g -c syntax.cpp -o syntax.o
semantic.o:semantic.cpp interpretlib.h
	g++ -Wall -g -c semantic.cpp -o semantic.o
smain:main.cpp tokenizing.o syntax.o semantic.o interpretlib.h
	g++ -Wall -g main.cpp tokenizing.o syntax.o semantic.o -o smain
polizmain:polizmain.cpp tokenizing.o syntax.o semantic.o interpretlib.h
	g++ -Wall -g polizmain.cpp tokenizing.o syntax.o semantic.o -o polizmain