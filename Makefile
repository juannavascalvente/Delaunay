# Set compiler and directives.
CC=g++
CFLAGS=
RM= rm -f
EXEC=Delaunay
LIBRARY=-lglut -lgomp -lGLU -lGL
SRCEXT=cpp

# Folders paths.
INC_DIR=-I./test -I./include -I./performance -I./statistics
OBJ_DIR=./obj
SRC_DIR=./src
STAT_DIR=./statistics
TEST_DIR=./test
PERF_DIR=./performance
SRC:= $(shell find . -name '*.$(SRCEXT)')

# Files list.
FILES=$(subst ./test/, ,$(subst ./src/, ,$(subst ./performance/, ,$(subst ./statistics/, ,$(SRC)))))
OBJS=$(patsubst	%.cpp,$(OBJ_DIR)/%.o,$(subst ./test/, ,$(FILES)))

$(EXEC): $(OBJS)
	$(CC) $(INC_DIR) $(CFLAGS) $(OBJS) -o $(EXEC) $(LIBRARY)

$(OBJ_DIR)/%.o: $(STAT_DIR)/%.cpp
	$(CC) $(INC_DIR) -c $< $(CFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(INC_DIR) -c $< $(CFLAGS) -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CC) $(INC_DIR) -c $< $(CFLAGS) -o $@

$(OBJ_DIR)/%.o: $(PERF_DIR)/%.cpp
	$(CC) $(INC_DIR) -c $< $(CFLAGS) -o $@

clean:
	$(RM) $(OBJS) $(EXEC)