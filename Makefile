# Variables
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude
SRC = src/*.cpp
OUT_DIR = bin

# Create bin directory if it doesn't exist
$(shell mkdir -p $(OUT_DIR))

.PHONY: all unit integration performance clean

all: unit integration performance

unit:
	$(CXX) $(CXXFLAGS) $(SRC) tests/tests.cpp -o $(OUT_DIR)/run_unit_tests
	./$(OUT_DIR)/run_unit_tests

integration:
	$(CXX) $(CXXFLAGS) $(SRC) tests/integration_tests.cpp -o $(OUT_DIR)/run_integration_tests
	./$(OUT_DIR)/run_integration_tests

performance:
	$(CXX) $(CXXFLAGS) $(SRC) tests/performance_tests.cpp -o $(OUT_DIR)/run_performance_tests
	./$(OUT_DIR)/run_performance_tests

clean:
	rm -rf $(OUT_DIR)