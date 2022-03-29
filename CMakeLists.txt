set(PROJECT underground)
set(TESTS underground_test)

cmake_minimum_required(VERSION 3.17)
project(${PROJECT})

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_executable(${TESTS} metro.h tests.cpp main.cpp)
add_executable(${PROJECT} metro.h main.cpp)

target_link_libraries(${TESTS} gtest_main)


include(GoogleTest)
gtest_discover_tests(${TESTS})

