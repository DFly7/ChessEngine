#include "../include/UCIInterface.hpp"
#include <iostream>
#include <string>
#include <sstream>

UCIInterface::UCIInterface()
{
    board = Board();
    search = Search();
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

    std::cout << command_full << std::endl;

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
        board = Board();
    }
    else if (first_word == "position")
    {
        iss >> first_word;

        std::string fen;

        std::getline(iss >> std::ws, fen);

        std::cout << "fen " << fen << std::endl;

        board.setBitBoard(fen);
    }
    else if (first_word == "go")
    {
        search.search(board);
    }
}