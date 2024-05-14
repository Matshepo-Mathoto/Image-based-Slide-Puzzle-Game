puzzle: ImagePuzzle.o Driver.o
	g++ ImagePuzzle.o Driver.o -o puzzle -std=c++20

ImagePuzzle.o: ImagePuzzle.cpp
	g++ -c ImagePuzzle.cpp -o ImagePuzzle.o -std=c++20
	
Driver.o: Driver.cpp
	g++ -c Driver.cpp -o Driver.o -std=c++20

clean:
	rm *.o
	rm puzzle