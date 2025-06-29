#include "../include/ChessUtils.hpp"
#include <stdexcept>

namespace ChessUtils
{
    // Static arrays for coordinate mapping
    const std::array<char, 8> FILES = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    const std::array<char, 8> RANKS = {'1', '2', '3', '4', '5', '6', '7', '8'};

    std::string coordsToSquare(int row, int col)
    {
        if (row < 0 || row > 7 || col < 0 || col > 7)
        {
            throw std::out_of_range("Coordinates out of range");
        }
        return std::string(1, FILES[col]) + std::string(1, RANKS[row]);
    }

    std::pair<int, int> squareToCoords(const std::string &square)
    {
        if (square.length() != 2)
        {
            throw std::invalid_argument("Invalid square format");
        }

        char file = square[0];
        char rank = square[1];

        return {charToRank(rank), charToFile(file)};
    }

    char fileToChar(int col)
    {
        if (col < 0 || col > 7)
        {
            throw std::out_of_range("Column out of range");
        }
        return FILES[col];
    }

    char rankToChar(int row)
    {
        if (row < 0 || row > 7)
        {
            throw std::out_of_range("Row out of range");
        }
        return RANKS[row];
    }

    int charToFile(char file)
    {
        for (int i = 0; i < 8; ++i)
        {
            if (FILES[i] == file)
            {
                return i;
            }
        }
        throw std::invalid_argument("Invalid file character");
    }

    int charToRank(char rank)
    {
        for (int i = 0; i < 8; ++i)
        {
            if (RANKS[i] == rank)
            {
                return i;
            }
        }
        throw std::invalid_argument("Invalid rank character");
    }
}