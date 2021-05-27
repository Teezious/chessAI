cmake_minimum_required(VERSION 3.0)
project(chessAI)

set(CMAKE_CXX_STANDARD 17)
find_package(Threads)
include_directories(src)

add_executable(chessAI
        src/ChessGame.cpp
        src/ChessGame.h
        src/main.cpp
        src/HumanChessPlayer.cpp
        src/ChessAI.cpp
        src/Evaluation.cpp
        src/Evaluation.h
        src/thread_pool.cc src/MoveSorter.cpp src/MoveSorter.h src/Opening.cpp src/Opening.h)

add_executable(PerformanceTest
        src/PerformanceMeasurement/PerformanceTester.cpp
        src/PerformanceMeasurement/PerformanceTester.h
        src/PerformanceMeasurement/main.cpp
        src/ChessAI.cpp
        src/ChessAI.h
        src/MoveSorter.cpp
        src/MoveSorter.h
        src/Evaluation.cpp
        src/Evaluation.h
        src/thread_pool.cc)

add_subdirectory("${PROJECT_SOURCE_DIR}/lib/thc" EXCLUDE_FROM_ALL)
include_directories(${THC_CHESS_INCLUDE})
target_link_libraries(chessAI thc_chess_static)
target_link_libraries(PerformanceTest thc_chess_static)
target_link_libraries(chessAI ${CMAKE_THREAD_LIBS_INIT})
