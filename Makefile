.PHONY: all clean

CFLAGS = -O2 -Wall
CPPFLAGS = -O2 -std=c++11 -Wall
LDFLAGS+=-lm     # link to math library

TARGET=test train document.pdf

all: $(TARGET)

%: %.cpp
	g++ $(CPPFLAGS) $< -o $@

document.pdf:
	pdflatex document.tex
clean:
	rm $(TARGET) *.aux *.log  # type make clean to remove the compiled file
