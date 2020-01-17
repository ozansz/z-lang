LLVMFLAGS=$(shell llvm-config --cxxflags --ldflags --system-libs --libs)
CXX=g++
ANTLR=java -jar /usr/local/lib/antlr-4.7.2-complete.jar
ANTLRFLAGS=-Dlanguage=Cpp -visitor -no-listener
RM=rm -rf

all: ir

ir: parser
	$(CXX) $(LLVMFLAGS) src/parser/*.cpp src/ir/*.cpp -o irgen

parser:
	$(ANTLR) $(ANTLRFLAGS) src/Z.g4 -o visitor
	mv visitor/src/* src/antlr4-runtime/
	rm -rf visitor/src/
	rm -rf visitor/

clean:
	$(RM) src/antlr4-runtime/Z*.interp src/antlr4-runtime/Z*.tokens src/antlr4-runtime/Z*.cpp src/antlr4-runtime/Z*.h irgen