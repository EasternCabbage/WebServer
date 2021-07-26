webserver : http.o main.o
	g++ -g http.o main.o -o webserver -lc

clean:
	rm *.o webserver