CC = clang
CXX = clang++
CXXFLAGS = -DDEBUG

# select all
DEBUG_TEST_SRC = $(wildcard src/*.cpp)
# pattern substitute (all .c to .o from 3rd parameter)
DEBUG_TEST_OBJECTS = $(patsubst %.cpp,%.o,$(DEBUG_TEST_SRC))
TARGET = debug-learn

# $@ all targets, $^ all dependencies, $< 1st dependency
$(TARGET): $(DEBUG_TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(DEBUG_TEST_OBJECTS): $(DEBUG_TEST_SRC)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

# not building the file called `clean`
# don't need any target
# commands heading with `-` means do not stop if failed
.PHONY: clean
clean:
	-rm -f $(TARGET) $(DEBUG_TEST_OBJECTS)
	@echo "Clean finished..."