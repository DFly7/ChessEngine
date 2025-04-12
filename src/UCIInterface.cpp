#include "../include/UCIInterface.hpp"
#include <iostream>
#include <string>
#include <sstream>

UCIInterface::UCIInterface() : board(), search()
{
}

UCIInterface::~UCIInterface()
{
}

void UCIInterface::run()
{
    std::string command_full;
    while (std::getline(std::cin, command_full))
    {
        if (command_full == "quit")
        {
            break;
        }
        handleCommand(command_full);
    }
}

void UCIInterface::handleCommand(const std::string &command_full)
{
    std::istringstream iss(command_full);
    std::string first_word;

    iss >> first_word;

    // std::cout << command_full << std::endl;

    if (first_word == "uci")
    {
        std::cout << "uciok" << std::endl;
    }
    else if (first_word == "isready")
    {
        std::cout << "readyok" << std::endl;
    }
    else if (first_word == "ucinewgame")
    {
        board.clearBoard();
    }
    else if (first_word == "position")
    {
        iss >> first_word;

        std::string fen;

        std::getline(iss >> std::ws, fen);

        // std::cout << "fen " << fen << std::endl;
        board.setBitBoard(fen);
    }
    else if (first_word == "go")
    {
        Move bestMove = search.search(board);
        printMove(bestMove);
    }
}

void UCIInterface::printMove(const Move &move)
{
    char col[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

    std::string moveString = "";
    moveString += col[move.startC];
    if (move.capture != '\0' && (move.pieceName == 'P' || move.pieceName == 'p'))
    {
        moveString += "x";
    }
    else
    {
        moveString += std::to_string(move.startR + 1);
    }

    moveString += col[move.endC];
    moveString += std::to_string(move.endR + 1);

    std::cout << moveString << std::endl;

    std::cout << move.pieceName << " from (" << move.startR << ", " << move.startC << ") to (" << move.endR << ", " << move.endC << ")";
    if (move.capture != '\0')
    {
        std::cout << " (captured " << move.capture << ")";
    }
    if (move.castling != 0)
    {
        std::cout << " (castling " << move.castling << ")";
    }
    std::cout << std::endl;
}