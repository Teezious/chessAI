#include "Scenario.h"
//
// Created by Florian on 29.04.2021.
//
int main(void) {
    Scenario scenario;
    scenario.createNewBoard("r   k  r"
                                      "p ppqpb "
                                      "Bn  pnp "
                                      "   PN   "
                                      " p  P   "
                                      "  N  Q p"
                                      "PPPB PPP"
                                      "R   K  R"
                            );
    scenario.board.white = false;
    scenario.chessAI.setIsWhite(false);
    std::cout << "WITH MOVE ORDERING" << std::endl;
    scenario.aiTest();
    std::cout << "WITHOUT MOVE ORDERING" << std::endl;
    scenario.chessAI.enableOrderMoves = false;
    scenario.aiTest();
}