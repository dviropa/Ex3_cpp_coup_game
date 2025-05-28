

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g -Iinclude -Iroles
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC = Game.cpp Action.cpp GameGUI.cpp main.cpp GameController.cpp\
      include/Governor.cpp include/Spy.cpp include/Baron.cpp include/General.cpp \
      include/Judge.cpp include/Merchant.cpp include/Player.cpp 

BIN_DIR = bin
EXECUTABLE = $(BIN_DIR)/coup_game

$(EXECUTABLE): $(SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXECUTABLE) $(LDFLAGS)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)


test: $(BIN_DIR)/game_tests
	./$(BIN_DIR)/game_tests

$(BIN_DIR)/game_tests: game_tests.cpp Game.cpp Action.cpp include/Player.cpp \
                       include/Governor.cpp include/Spy.cpp include/Baron.cpp \
                       include/General.cpp include/Judge.cpp include/Merchant.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@


valgrind: $(BIN_DIR)/game_tests
	valgrind --leak-check=full --show-leak-kinds=all ./$(BIN_DIR)/game_tests


clean:
	rm -rf $(BIN_DIR)





















# # Compiler and flags
# CXX = g++
# CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g

# # Directory structure
# SRC_DIR = .
# PL_DIR = include
# BUILD_DIR = build
# BIN_DIR = bin

# # Sources and object files
# SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(PL_DIR)/*.cpp)
# OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(filter $(SRC_DIR)/%.cpp,$(SOURCES))) \
#            $(patsubst $(PL_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(filter $(PL_DIR)/%.cpp,$(SOURCES)))

# # Executables
# EXECUTABLE = $(BIN_DIR)/coup_game
# TEST_FILE = game_tests.cpp
# TEST_OBJECT = $(BUILD_DIR)/$(TEST_FILE:.cpp=.o)
# TEST_EXECUTABLE = $(BIN_DIR)/game_tests

# # Default target
# all: run

# # Run the main executable
# run: $(EXECUTABLE)
# 	./$(EXECUTABLE)

# # Link the main executable
# $(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
# 	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# # Run tests
# test: $(TEST_EXECUTABLE)
# 	./$(TEST_EXECUTABLE)

# # Build test executable
# $(TEST_EXECUTABLE): $(TEST_OBJECT) $(filter-out $(TEST_OBJECT),$(OBJECTS)) | $(BIN_DIR)
# 	$(CXX) $^ -o $@ $(LDFLAGS)

# # Compile source files from main dir
# $(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Compile source files from roles dir
# $(BUILD_DIR)/%.o: $(PL_DIR)/%.cpp | $(BUILD_DIR)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Compile test file
# $(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Ensure necessary directories exist
# $(BUILD_DIR):
# 	mkdir -p $(BUILD_DIR)

# $(BIN_DIR):
# 	mkdir -p $(BIN_DIR)

# # Clean target
# clean:
# 	rm -rf $(BUILD_DIR) $(BIN_DIR)
