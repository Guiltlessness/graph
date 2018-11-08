CC		:= g++
TARGET		:= main
DBGTARGET	:= test
FLAGS		:= -Wall -Wextra -std=c++17 -msse4 -O3 -ffast-math
DBGFLAGS	:= -Wall -Wextra -std=c++17 -msse4 -ggdb -ffast-math
LIBS		:= -lm -lpthread
SOURCE		:= $(wildcard src/*.cpp)
HEADERS		:= $(wildcard src/*.h src/*.inl)
OBJS		:= $(patsubst %.cpp, %.o, $(SOURCE))
DBGOBJS		:= $(patsubst %.cpp, %_dbg.o, $(SOURCE))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.cpp $(HEADERS)
	$(CC) -o $@ -c $< $(FLAGS)

debug: $(DBGTARGET)

$(DBGTARGET): $(DBGOBJS)
	$(CC) -o $@ $^ $(LIBS)

%_dbg.o: %.cpp $(HEADERS)
	$(CC) -o $@ -c $< $(DBGFLAGS)

clean:
	rm -f $(TARGET) $(DBGTARGET) $(OBJS) $(DBGOBJS)

.PHONY: all clean debug
