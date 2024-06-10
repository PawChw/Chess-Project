#include <iostream>
#include<stdexcept>

#include "Board.h"
#include "Generator.h"

int main()
{
    try {
        Board board("3q1rk1/1b1r1p1p/4P1p1/pB6/Pb2n3/1QN1P3/1P3P1P/R1B1K1R1 w Q - 0 1");
        auto moves = Generator(board).GenerateLegalMoves();
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    std::cin.get();
}
