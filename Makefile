all: clean parser

parser:
	g++ parser.cpp -o parser -DTEST
	./parser --in example.mpp

clean:
	rm -rf parser
