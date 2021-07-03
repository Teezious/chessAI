

add_executable(PerformanceTest
        PerformanceTester.cpp
        PerformanceTester.h
        main.cpp)

target_include_directories(PerformanceTest PUBLIC
        ../../lib/chessAI)

target_link_libraries(PerformanceTest
        chessAI
        thc_chess_static
        ${CMAKE_THREAD_LIBS_INIT})