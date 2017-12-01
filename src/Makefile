CXX=c++
ifndef CXXFLAGS
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11
endif

EXE := critters
LIB := libcritters.a

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
	ar crs $(LIB) $(LIB_OBJS)

$(EXE): $(LIB) $(CRITTERS)
	$(CXX) $(CXXFLAGS) $(CRITTERS) -static $< -L. -lcritters -o $@

# import existing depenency info
-include $(OBJS:.o=.d)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -I. -I.. $*.cpp -o $*.o
	$(CXX) -MM $(CXXFLAGS) -I. -I.. $*.cpp > $*.d

.PHONY: clean test
clean: 
	rm -f $(exe) *.o *.d $(LIB)


