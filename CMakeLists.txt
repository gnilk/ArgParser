cmake_minimum_required(VERSION 3.13)
project(gnklog)


set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "")
set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# this is just a single header library
list(APPEND argparser_src src/ArgParser.h)

# unit testing
list(APPEND utest_src tests/test_argparser.cpp)


# examples
list(APPEND ex1_src example/ex1_app.cpp)
add_executable(ex1 ${ex1_src})
target_include_directories(ex1 PUBLIC src)

add_library(utests SHARED ${utest_src})
target_include_directories(utests PUBLIC src)
