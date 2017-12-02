CXX=c++
ifndef CXXFLAGS
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11
endif

EXE := critters
LIB := libcritters.so
STATICS := -lncurses -lm -lc
INCLUDES := -I./include

critters: clean *.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) *.cpp -L. $(LIB) $(STATICS) -o $@

%.o: %.cpp
	$(CXX) -fPIC -c $(CXXFLAGS) $(INCLUDES) $*.cpp -o $*.o

.PHONY: clean
clean:
	rm -f critters


