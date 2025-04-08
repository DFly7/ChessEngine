//
//  Opening.cpp
//  CHESSGUIBB
//
//  Created by Darragh Flynn on 05/10/2024.
//

#include "Opening.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;


Move getOpeningMove(int moveNum, Move lastMove){
    Move book;
    
//    std::string filename = "twic1560.pgn";
    const char* filename = "twic1560.pgn";

    std::ifstream inputFile;
    
    inputFile.open(filename);

    // Check if the file is successfully opened
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        // Check for specific error conditions
          if (inputFile.bad()) {
              std::cerr << "Fatal error: badbit is set." << endl;
          }

          if (inputFile.fail()) {
              // Print a more detailed error message using
              // strerror
              cerr << "Error details: " << strerror(errno)
                   << endl;
          }
        return book;
    }

//        Game currentGame;
    std::string line;
    int i = 0;
    while (std::getline(inputFile, line) and i < 10) {
        // Handle metadata lines
        if (line.starts_with("[") && line.ends_with("]")) {
            continue;

            // ... other metadata tags
        } else {
            // Handle move lines
            std::cout << "line: " << line << std::endl;

        }
        i++;
    }
    
    inputFile.close();
    return book;
}

