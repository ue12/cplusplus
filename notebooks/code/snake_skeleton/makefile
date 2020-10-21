CXX=g++
CXXFLAGS=-Wall -ggdb -Iinclude
AR=/usr/bin/ar -r 

src := $(wildcard src/*.cpp)
obj := $(subst src, build, $(src:.cpp=.o))


.PHONY: directories

all: directories libsnake.a snake 
	@echo $(obj)

snake: main.cpp libsnake.a
	$(CXX) $(CXXFLAGS) $^ -o $@ 

libsnake.a: ${obj}
	${AR} $@ $^

build/%.o: src/%.cpp include/%.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<


directories: build
build:
	mkdir -p $@


clean: 
	rm snake
	rm libsnake.a
	rm -r build
