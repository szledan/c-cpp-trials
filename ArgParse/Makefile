all: unittest main

unittest: makedir
	g++ -std=c++11 -o ./bin/unittest arg-parse.cpp unittest.cpp

main: makedir
	g++ -std=c++11 -o ./bin/main arg-parse.cpp main.cpp

makedir:
	mkdir -p ./bin

clean:
	rm -rf ./bin
