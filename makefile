ZUCTest: ZUCTest.o ZUC.o EIA3.o
	g++ -g ZUCTest.o ZUC.o EIA3.o -o ZUCTest
ZUCTest.o: ZUCTest.cpp ZUC.h
	g++ -g -c ZUCTest.cpp
ZUC.o: ZUC.cpp ZUC.h
	g++ -g -c ZUC.cpp
EIA3.o: EIA3.cpp EIA3.h
	g++ -g -c EIA3.cpp

.PHONY: clean
clean:
	-rm -f ZUC.o ZUCTest.o EIA3.o ZUCTest
