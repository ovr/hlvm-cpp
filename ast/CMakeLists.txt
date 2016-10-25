
# I am not interested to write this manualy, lets hack :)
PROJECT(ast C CXX)

file(GLOB AST_SOURCES *.cpp Statement/*.cpp)
add_library(ast ${AST_SOURCES})
