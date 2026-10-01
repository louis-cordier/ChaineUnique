cmake_minimum_required(VERSION 3.15)
project(ChaineUnique VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings and optimizations
if (MSVC)
    add_compile_options(/W4 /O2)
else()
    add_compile_options(-Wall -Wextra -O3)
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/order_book_parser.cpp
    src/generator/order_generator.cpp
    src/visualizer/ascii_timeline.cpp
)

# Headers
set(HEADERS
    src/order.hpp
    src/order_book_parser.hpp
    src/generator/prng.hpp
    src/generator/order_generator.hpp
    src/visualizer/ascii_timeline.hpp
)

add_executable(chaine_unique ${SOURCES} ${HEADERS})
target_include_directories(chaine_unique PRIVATE src)
