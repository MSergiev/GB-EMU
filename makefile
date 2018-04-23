run: *.cpp *.h 
	g++ -Wall -pedantic -ggdb -O3 -o run *.cpp -lncursesw
