//
// Created by Florian on 04.05.2021.
//

#include "PerformanceTester.h"
#include <ChessAI.h>
#include <fstream>
#include <iostream>
#include <thc.h>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

void PerformanceTester::runPerformanceTest() {
    vector<string> boardStrings;
    ifstream inFile;
    string line;
    char in;
    thc::ChessRules board;
    ChessAI whiteAI(true);
    ChessAI blackAI(false);

    //read Scenarios
    int i = 1;
    while(true) {
        inFile.open(scenarioPath + to_string(i) + ".txt");
        if(!inFile.is_open())
            break;
        getline(inFile, line);
        boardStrings.push_back(line);
        inFile.close();
        i++;
    }
    cout<<"txt read"<<endl;
    //measure performance
    for(string boardString : boardStrings) {
        strcpy(board.squares, boardString.c_str());
        board.white = true;

        auto start = high_resolution_clock::now();
        unsigned int nodesSearched = 1;
        int eval;
        string move = whiteAI.multiThreadedSearch(board, &nodesSearched, &eval);
        auto stop = high_resolution_clock::now();

        duration<double, milli> ms = stop - start;
        cout << "Minmax finished in " << ms.count()/1000 << " seconds" << endl;
        cout << "eval: " << eval << " move: " << move << endl;
        cout << "Nodes searched: " << nodesSearched << endl;
    }

    //write results to Console or Logfile?
}
