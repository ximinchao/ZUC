ZUCTest: ZUCTest.o ZUC.o
	g++ -g ZUCTest.o ZUC.o -o ZUCTest
ZUCTest.o: ZUCTest.cpp ZUC.h
	g++ -g -c ZUCTest.cpp
ZUC.o: ZUC.cpp ZUC.h
	g++ -g -c ZUC.cpp

.PHONY: clean
clean:
	-rm -f ZUC.o ZUCTest.o ZUCTest
