CXX=c++
ifndef CXXFLAGS
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11
endif

EXE := critters
LIB := libcritters.so
LIBS := -lncurses -lm -lc

COMMON :=   Color.o \
			Critter.o \
			Direction.o \
			Point.o \
			Simulator.o \
			Species.o \
			ViewCurses.o \
			solutions/Bear.o \
			solutions/Lion.o \
			solutions/Tiger.o \
			solutions/Wombat.o

CRITTERS     := main.o $(COMMON)

LIB_OBJS    := $(COMMON)

all: $(EXE)

$(LIB): $(LIB_OBJS)
	$(CXX) -shared -o $(LIB) $(LIB_OBJS) $(LIBS)

$(EXE): $(LIB) $(CRITTERS)
	$(CXX) $(CXXFLAGS) $(CRITTERS) $< -L. $(LIB) $(LIBS) -o $@ 

# import existing depenency info
-include $(OBJS:.o=.d)

%.o: %.cpp
	$(CXX) -fPIC -c $(CXXFLAGS) -I. -I.. $*.cpp -o $*.o
	$(CXX) -MM $(CXXFLAGS) -I. -I.. $*.cpp > $*.d

.PHONY: clean
clean: 
	rm -f $(EXE) *.o *.d $(LIB)
	rm -f solutions/*.o solutions/*.d


