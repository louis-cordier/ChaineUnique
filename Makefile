CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -Isrc
TARGET = chaine_unique

SRCS = src/main.cpp src/order_book_parser.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

.PHONY: all clean
