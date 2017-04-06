.PHONY: all clean

CFLAGS = -O2 -Wall
CPPFLAGS = -O2 -std=c++11 -Wall
LDFLAGS+=-lm     # link to math library

TARGET=test train 

all: $(TARGET)

%: %.cpp
	g++ $(CPPFLAGS) $< -o $@

clean:
	rm $(TARGET)   # type make clean to remove the compiled file
