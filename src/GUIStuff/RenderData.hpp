//
//  RenderData.hpp
//  CHESSGUIBB
//
//  Created by Darragh Flynn on 03/10/2024.
//

#ifndef RenderData_hpp
#define RenderData_hpp

#include <stdio.h>
#include <vector>

struct RenderData{
    std::vector<int> pieceXPositions; // x-coordinates of each piece
    std::vector<int> pieceYPositions; // y-coordinates of each piece
    std::vector<char> pieceTypes;     // Types of each piece (e.g., 'P', 'N', 'B', 'R', 'Q', 'K')
};


#endif /* RenderData_hpp */
