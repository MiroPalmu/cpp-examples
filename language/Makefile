default_cxx_flags := -std=c++26 -g -O0 -fsanitize=address -fconcepts-diagnostics-depth=10

.PHONY: clean

one_file_sources := $(wildcard *.cpp)
examples := $(one_file_sources:.cpp=)

# Clean by default when calling make.
clean:
	@echo Cleaning...
	rm -f $(examples)

%: %.cpp
	$(CXX) $(default_cxx_flags) $< -o $@


