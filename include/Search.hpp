#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "Board.hpp"

class Search
{
public:
    Search();
    ~Search();
    void search(const Board &board);
};

#endif /* SEARCH_HPP */
