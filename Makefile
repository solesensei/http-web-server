tokenizing.o:tokenizing.cpp interpretlib.h
	g++ -Wall -g -c tokenizing.cpp -o tokenizing.o
token:main.cpp tokenizing.o interpretlib.h
	g++ -Wall -g tokenizing.o main.cpp -o token
syntax.o:syntax.cpp interpretlib.h
	g++ -Wall -g -c syntax.cpp -o syntax.o
smain:smain.cpp tokenizing.o syntax.o interpretlib.h
	g++ -Wall -g smain.cpp tokenizing.o syntax.o -o smain
