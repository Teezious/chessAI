add_executable(myTest main.cpp)

target_include_directories(myTest PUBLIC
        ../googletest/googletest/include
        ../lib/chessAI)

target_link_libraries(myTest PRIVATE gtest chessAI)