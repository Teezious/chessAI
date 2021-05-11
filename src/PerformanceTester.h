//
// Created by Florian on 04.05.2021.
//

#ifndef CHESSAI_PERFORMANCETESTER_H
#define CHESSAI_PERFORMANCETESTER_H

#include <string>
#define NUMBER_TESTS 3

class PerformanceTester final
{
  public:
    void runPerformanceTest();
  private:
    std::string logPath = "resources/PerformanceLogs";
    std::string scenarioPath = "resources/Scenarios/scenario";
};

#endif // CHESSAI_PERFORMANCETESTER_H
