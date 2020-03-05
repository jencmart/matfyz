.PHONY: test
test: hash_experiment
	@rm -rf out && mkdir out
	@for exp in grow usage; do \
		for impl in ms-low ms-high poly-1 poly-2 tab ; do \
			echo "t-$$exp-$$impl" ; \
			./hash_experiment $$exp-$$impl 18  >out/t-$$exp-$$impl ; \
		done ; \
	done


INCLUDE ?= .
CXXFLAGS=-std=c++11 -O2 -Wall -Wextra -g -Wno-sign-compare -I$(INCLUDE)

hash_experiment: hash_experiment.cpp $(INCLUDE)/random.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) hash_experiment.cpp -o $@

.PHONY: clean
clean:
	rm -f hash_experiment
