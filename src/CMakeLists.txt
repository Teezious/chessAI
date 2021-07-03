
add_executable(chessExec
main.cpp)

target_include_directories(chessExec PUBLIC
../lib/chessAI )

target_link_libraries(chessExec chessAI)