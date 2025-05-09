#ifndef UCIINTERFACE_HPP
#define UCIINTERFACE_HPP

#include "Board.hpp"
#include "MoveGenerator.hpp"
#include "Search.hpp"

class UCIInterface
{
private:
    Board board;
    Search search;

public:
    UCIInterface();
    ~UCIInterface();

    void run();
    void handleCommand(const std::string &command_full);

    void printMove(const Move &move);
};

#endif /* UCIINTERFACE_HPP */
