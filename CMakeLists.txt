
cmake_minimum_required(VERSION 3.22)
project(SEM)

set(CMAKE_CXX_STANDARD 23)


add_executable(SEM  main.cpp huffmanCode.cpp huffmanCode.h Writter.cpp Writter.h ReaderInput.cpp ReaderInput.h Compressor.cpp Compressor.h StringWritter.cpp StringWritter.h parrallel_content.h)
