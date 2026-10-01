CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -Isrc
TARGET = chaine_unique

SRCS = src/main.cpp \
       src/order_book_parser.cpp \
       src/generator/order_generator.cpp \
       src/visualizer/ascii_timeline.cpp \
       src/scheduler/feasibility_judge.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

.PHONY: all clean
