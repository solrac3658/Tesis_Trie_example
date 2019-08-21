TARGET = trie_exe
TEST_TARGET = test_exe
BASE_OBJECTS = Host.o hash.o version.o allocators.o trieengine.o trie.o
TEST_OBJECTS = test.o $(BASE_OBJECTS)
OBJECTS = main.o $(BASE_OBJECTS)
STATIC_LIBS = lib/libevmc-instructions.a lib/libevmc-loader.a
CXXFLAGS = -O0 -Og -g -std=c++11 -I . -I ./include
LDLIBS = -lboost_system -lboost_thread -lssl -lcrypto -lleveldb -lsqlite3
TEST_LDLIBS = $(LDLIBS) -lboost_unit_test_framework

.PHONY: all
all:  $(TARGET)

.PHONY: test
test: $(TEST_TARGET)

$(TARGET): $(OBJECTS)
	g++ -o $(TARGET) $(OBJECTS) $(STATIC_LIBS) $(CXXFLAGS) $(LDLIBS)

$(TEST_TARGET): $(TEST_OBJECTS)
	g++ -o $(TEST_TARGET) $(TEST_OBJECTS) $(STATIC_LIBS) $(CXXFLAGS) $(TEST_LDLIBS) 

allocators.o: allocators.cpp allocators.h

hash.o: hash.cpp hash.h serialize.h uint256.h version.h

Host.o: Host.hpp Host.cpp allocators.h hash.h serialize.h uint256.h version.h 

trie.o: trie.cpp trie.h hash.h uint256.h

trieengine.o: trieengine.cpp trie.h hash.h uint256.h

version.o: version.cpp version.h clientversion.h

main.o: main.cpp uint256.h hash.h trie.h

test.o: test.cpp Host.hpp trie.h hash.h uint256.h

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(TEST_OBJECTS)
