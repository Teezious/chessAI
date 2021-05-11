cmake_minimum_required(VERSION 3.0)
project(chessAI)

set(CMAKE_CXX_STANDARD 17)
find_package (Threads)
include_directories(src)

add_executable(chessAI
        src/ChessGame.cpp
        src/ChessGame.h
        src/main.cpp
        src/HumanChessPlayer.cpp
        src/ChessAI.cpp
        src/Evaluation.cpp
        src/Evaluation.h
        src/thread_pool.cc src/MoveSorter.cpp src/MoveSorter.h test/Unittest.cpp test/TestHeader.h src/PerformanceTester.cpp src/PerformanceTester.h)

add_subdirectory("${PROJECT_SOURCE_DIR}/lib/thc" EXCLUDE_FROM_ALL)
include_directories(${THC_CHESS_INCLUDE})

#googletest
add_subdirectory(googletest-master)
include_directories(googletest-master/googletest/include)
include_directories(googletest-master/googlemock/include)

target_link_libraries(chessAI thc_chess_static)
target_link_libraries (chessAI ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(chessAI gtest )