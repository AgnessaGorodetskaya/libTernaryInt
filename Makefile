CXX ?= g++
CXXFLAGS ?= -O2 -g
CXXFLAGS += -std=c++17

CXXFLAGS += -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default \
	-Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wmissing-field-initializers \
	-Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wsign-promo \
	-Weffc++ -Werror=effc++ -Wextra -pedantic-errors -Wsign-conversion -Wconversion \
	-Wunused-variable -Wpedantic

# CXXFLAGS += -fsanitize=undefined -fsanitize=address
# LDFLAGS += -fsanitize=undefined -fsanitize=address

.PHONY: all test clean

all: example1

example1: example1.o libternaryint.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

example1.o:
	$(CXX) $(CXXFLAGS) -c examples/example1.cpp

libternaryint.o:
	$(CXX) $(CXXFLAGS) -c src/libternaryint.cpp

test: all
	./example1

clean:
	rm -rf *.o example1
