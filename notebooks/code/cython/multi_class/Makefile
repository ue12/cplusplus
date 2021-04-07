

all: libFake.a 

libFake.a: classA.o classB.o 
	ar -r $@ $^

libFake.so: classA.o classB.o 
	g++ -shared -fPIC -o $@ $^

classA.o: src/classA.cpp src/classA.hpp src/classB.hpp 
	g++ -fPIC -o $@ -c $<

classB.o: src/classB.cpp src/classB.hpp 
	g++ -fPIC -o $@ -c $<

