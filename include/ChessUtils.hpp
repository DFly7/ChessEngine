#ifndef CHESS_UTILS_HPP
#define CHESS_UTILS_HPP

#include <string>
#include <unordered_map>
#include <array>

namespace ChessUtils
{
    // Maps internal coordinates (0-7) to chess notation
    extern const std::array<char, 8> FILES; // a-h
    extern const std::array<char, 8> RANKS; // 1-8

    // Convert internal coordinates to chess notation
    std::string coordsToSquare(int row, int col);

    // Convert chess notation to internal coordinates
    std::pair<int, int> squareToCoords(const std::string &square);

    // Convert single coordinate
    char fileToChar(int col);
    char rankToChar(int row);
    int charToFile(char file);
    int charToRank(char rank);
}

#endif // CHESS_UTILS_HPP