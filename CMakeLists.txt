cmake_minimum_required(VERSION 3.12)
project(confi)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# List of source files
set(SOURCES
    src/main.cpp
)

# List of header files
set(HEADERS
    include/Lexer.hpp
    include/Parser.hpp
)

# Add library target for lexer and parser
add_library(confi_lib ${HEADERS})

# Add executable target
add_executable(confi ${SOURCES})

# Link the executable target with the library
target_link_libraries(confi confi_lib)

