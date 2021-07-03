//
// Created by Florian on 04.05.2021.
//

#include "PerformanceTester.h"
#include "ChessAI.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <thc.h>
#include <vector>

using namespace std;
using namespace chrono;

void PerformanceTester::runPerformanceTest()
{
    vector<string> FENs;
    ifstream inFile;
    string line;
    thc::ChessRules board;
    ChessAI ai(true);

    // read Scenarios
    int i = 1;
    while(true)
    {
        inFile.open(scenarioPath + to_string(i) + ".txt");
        if(!inFile.is_open())
            break;
        getline(inFile, line);
        FENs.push_back(line);
        inFile.close();
        i++;
    }

    // measure performance
    int scenario = 1;
    for(string FEN : FENs)
    {
        if(!board.Forsyth(FEN.c_str()))
        {
            cout << "Error while setting board state!" << endl;
            continue;
        }
        ai.setIsWhite(board.WhiteToPlay());
        cout << "TESTING SCENARIO " << scenario << endl << board.ToDebugStr() << endl << endl;
        double sumTimeSeconds = 0;
        unsigned long long sumNodes = 0;
        for(int i = 0; i < NUMBER_TESTS; i++)
        {
            cout << "-----TEST NUMBER " << i + 1 << " -----" << endl;
            auto start = high_resolution_clock::now();
            std::atomic<unsigned int> nodesSearched = 1;
            int eval;
            string move = ai.multiThreadedSearch(board, nodesSearched, eval);
            auto stop = high_resolution_clock::now();
            duration<double, milli> ms = stop - start;

            cout << "Minmax finished in " << ms.count() / 1000 << " seconds" << endl;
            cout << "eval: " << eval << " move: " << move << endl;
            cout << "Nodes searched: " << nodesSearched << endl << endl;

            sumTimeSeconds += ms.count() / 1000;
            sumNodes += nodesSearched;
        }
        cout << "STATISTICS:" << endl;
        cout << "\tTime average: " << sumTimeSeconds / NUMBER_TESTS << " seconds" << endl;
        cout << "\tNodes searched average: " << sumNodes / NUMBER_TESTS << endl << endl;
        cout << "******************************************" << endl << endl;
        ++scenario;
    }

    // write results to Logfile?
}
