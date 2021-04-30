cmake_minimum_required(VERSION 3.0)
project(chessAI)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(chessAI
        src/ChessGame.cpp
        src/ChessGame.h
        src/main.cpp
        src/HumanChessPlayer.cpp
        src/ChessAI.cpp src/Evaluation.cpp src/Evaluation.h)

add_executable(ScenarioTesting
        src/ScenarioTesting/main.cpp
        src/ScenarioTesting/Scenario.cpp
        src/ChessAI.cpp src/Evaluation.cpp)

add_subdirectory("${PROJECT_SOURCE_DIR}/lib/thc" EXCLUDE_FROM_ALL)
include_directories(${THC_CHESS_INCLUDE})
target_link_libraries(chessAI thc_chess_static)
target_link_libraries(ScenarioTesting thc_chess_static)

