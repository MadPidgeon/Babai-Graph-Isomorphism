CXX = g++-5
CXXFLAGS = -Wall -Wextra -std=c++1y -Wfatal-errors -I misc -L misc -DDEBUG -pthread
LIB = bin/ext.o bin/unionfind.o bin/permutation.o bin/fhl.o bin/group.o bin/coset.o bin/luks.o bin/action.o bin/datastructures.o
EXAMPLES = examples/groups_and_permutations.exe examples/luks_algorithm.exe examples/babai_algorithm.exe examples/cosets_and_pullbacks.exe examples/configurations.exe

.PHONY: clean all

all: lib examples

clean:
	rm bin/*.o
	rm examples/*.exe

lib: $(LIB)

examples: $(EXAMPLES)

bin/%.o: misc/%.cc
	$(CXX) -c $(CXXFLAGS) -o $@ $<

examples/%.o: examples/%.cc
	$(CXX) -c $(CXXFLAGS) -o $@ $<

examples/%.exe: examples/%.o $(LIB)
	$(CXX) $(CXXFLAGS) -o $@ $(LIB) $<

bin/%.o: %.cc
	$(CXX) -c $(CXXFLAGS) -o $@ $<

