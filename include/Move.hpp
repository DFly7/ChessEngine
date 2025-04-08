//
//  Move.hpp
//  BitBoardStuff
//
//  Created by Darragh Flynn on 01/10/2024.
//

#ifndef Move_hpp
#define Move_hpp

#include <stdio.h>

struct Move{
    char pieceName;
    //'\0' means no
    char capture;
    
    //castling side 1 = king side, 2 == queen side, 0 == no
    int castling;
    
    //char of the letter promoting too
    char promotion = '\0';
    
    int startR;
    int startC;
    int endR;
    int endC;
    
    Move(char pieceName, char capture, int castling, int startR, int startC, int endR, int endC) : pieceName(pieceName), capture(capture), castling(castling),startR(startR), startC(startC), endR(endR), endC(endC) {}
    Move(){}
};

#endif /* Move_hpp */
