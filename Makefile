LLVMFLAGS=$(shell llvm-config --cxxflags --ldflags --system-libs --libs)
CXX=g++
ANTLR=java -jar /usr/local/lib/antlr-4.7.2-complete.jar
ANTLRFLAGS=-Dlanguage=Cpp -visitor -no-listener
RM=rm -rf

all: ir

ir: parser
	$(CXX) $(LLVMFLAGS) src/parser/*.cpp src/ir/*.cpp -o irgen

parser:
	$(ANTLR) $(ANTLRFLAGS) src/Z.g4 -o parser
	mv parser/src/* parser && rm -rf parser/src
	mv parser/ src/

clean:
	$(RM) src/parser irgen