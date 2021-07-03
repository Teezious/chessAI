cmake_minimum_required(VERSION 3.0)
project(chessAI)

set(CMAKE_CXX_STANDARD 17)
find_package(Threads)
include_directories(${THC_CHESS_INCLUDE})
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/thc" EXCLUDE_FROM_ALL)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/chessAI")
add_subdirectory("${PROJECT_SOURCE_DIR}/src")

#if(CMAKE_TESTING_ENABLED)
add_subdirectory(googletest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(test)

#endif()