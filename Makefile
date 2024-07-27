all: clean run

SOURCES += main.cpp
SOURCES += parser.cpp
SOURCES += configurator.cpp
SOURCES += req_checker.cpp
SOURCES += builder.cpp
SOURCES += expression.cpp

run: mpp
	./mpp --in mpp.mpp

mpp:
	g++ $(SOURCES) -o mpp -g3 -ggdb

clean:
	rm -rf mpp mpp_ *.o

clean_all: clean
	rm -rf .example.mpp .mpp.mpp
