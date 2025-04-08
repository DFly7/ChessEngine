//
//  main.cpp
//  BitBoardStuff
//
//  Created by Darragh Flynn on 13/09/2024.
//
//
#include <iostream>

#include <string>
#include "BitBoards.hpp"
#include "View.hpp"
#include <utility>
#include "../opening/Opening.hpp"


void printLegalMoves(const std::vector<Move> legalMoves) {
    std::cout << "Legal Moves:\n";
    for (const Move& move : legalMoves) {
        std::cout << move.pieceName << " from (" << move.startR << ", " << move.startC << ") to (" << move.endR << ", " << move.endC << ")";
        if (move.capture) {
            std::cout << " (capture)";
        }
        if (move.castling){
            std::cout << " (castle)";
        }
        std::cout << std::endl;
    }
}

void printMove(const Move& move) {
    std::cout << move.pieceName << " from (" << move.startR << ", " << move.startC << ") to (" << move.endR << ", " << move.endC << ")";
    if (move.capture != '\0') {
        std::cout << " (captured " << move.capture << ")";
    }
    if (move.castling != 0){
        std::cout << " (castling " << move.castling << ")";
    }
    std::cout << std::endl;
}

void printRenderData(const RenderData& renderData) {
    std::cout << "Piece Positions and Types:\n";
    for (size_t i = 0; i < renderData.pieceXPositions.size(); ++i) {
        std::cout << renderData.pieceTypes[i] << " (" << renderData.pieceXPositions[i] << ", " << renderData.pieceYPositions[i] << ")\n";
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    std::string start = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    BitBoards b = BitBoards(start);
//    "2rq1rk1/pp2bppp/2n1pn2/2pp4/3P1B2/2PQPN2/PP1N1PPP/R4RK1"
    printf("start Eval = %d\n", b.evalPosition());
//    Move m2 = {'r','\0',0,7,0,4,4};
//
//    getOpeningMove(1, m2);
//
//    Move m = b.AImove();
//    b.printBitBoards();
//    Move m2 = {'P','\0',0,6,1,7,0};
//    b.makeMove(m2);
//    printf("MOVE MADE\n");
//
//    b.printBitBoards();
//    b.undoMove(m2);
//    b.printBitBoards();
//    
//    b.printBitboard(b.WhiteQueen);
//    
//    
    
    RenderData data = b.populateRenderData();

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        return 1;
     }
    
    View view = View();
    if (!view.init()) {
        return 1; // Initialization failed
    }
    
    int player = 1;

    printf("HERE");
    
    bool quit = false;
    int i = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else {
                std::pair<int, int> movePair = view.handleEvents(e);
                view.render(data);

                if(movePair.first >= 0){
//                    printf("START row = %d, COL = %d", movePair.first/8, movePair.first%8);
//                    printf("START row = %d, COL = %d", movePair.second/8, movePair.second%8);
                    Move m;
                    char letter = b.getPieceletter(movePair.first);
                    if(letter != '\0'){
                        if((letter == 'k' or letter == 'K') and (abs(movePair.first%8-movePair.second%8) == 2) and (movePair.first/8 == movePair.second/8)){
                            int way = (movePair.first%8-movePair.second%8 == 2) ? 2 : 1;
                            m = {letter,'\0', way,movePair.first/8, movePair.first%8, movePair.second/8, movePair.second%8};
                            printMove(m);
//                            b.makeMove(m);
//                            data = b.populateRenderData();
//                            player = (player == 1) ? 2 : 1;
//                            view.render(data);
                        }
                        else{
                            m = {letter,'\0', 0,movePair.first/8, movePair.first%8, movePair.second/8, movePair.second%8};
                        }
                        if(b.validatePlayerMove(player, m)){
                            b.makeMove(m);
                            data = b.populateRenderData();
                            player = (player == 1) ? 2 : 1;
                            view.render(data);
                        }
                    }
                }
            }
        }
        
        
        
        if(player == 2){
            Move m = b.AImove();
            b.makeMove(m);
            b.printMove(m);
            data = b.populateRenderData();
            player = (player == 1) ? 2 : 1;
            view.render(data);
        }
        
        i++;
        view.render(data);
        SDL_Delay(100);
    }
    return 0;
}


//Make it make all moves
//std::vector<RenderData> datavec;
//
////    RenderData data = b.populateRenderData();
//
//for(Move m : legalMoves){
//    b.makeMove(m);
//    b.printBitBoards();
//    RenderData data = b.populateRenderData();
//    datavec.push_back(data);
//    std::vector<Move> legalMoves2 = b.GenerateAllMoves(2);
//    for(Move m2 : legalMoves2){
//        b.makeMove(m2);
//        b.printBitBoards();
//        RenderData data2 = b.populateRenderData();
//        datavec.push_back(data2);
//        
//        std::vector<Move> legalMoves3 = b.GenerateAllMoves(1);
//        for(Move m3 : legalMoves3){
//            b.makeMove(m3);
//            b.printBitBoards();
//            RenderData data3 = b.populateRenderData();
//            datavec.push_back(data3);
//            b.undoMove(m3);
//        }
//
//        b.undoMove(m2);
//    }
//    b.undoMove(m);
//}
