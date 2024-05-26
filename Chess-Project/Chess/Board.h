#pragma once
#include "../BoardView.h"
#include "Pieces/Piece.h"

class Board :
    public BoardView
{
private:
    std::array<Piece, 16> whitePieces = {
        Piece(0, PieceType::Rook),
        Piece(1, PieceType::Knight),
        Piece(2, PieceType::Bishop),
        Piece(3, PieceType::King),
        Piece(4, PieceType::Queen),
        Piece(5, PieceType::Bishop),
        Piece(6, PieceType::Knight),
        Piece(7, PieceType::Rook),
        Piece(8),
        Piece(9),
        Piece(10),
        Piece(11),
        Piece(12),
        Piece(13),
        Piece(14),
        Piece(15),
    };
    std::array<Piece, 16> blackPieces = {
        Piece(63, PieceType::Rook, false),
        Piece(62, PieceType::Knight, false),
        Piece(61, PieceType::Bishop, false),
        Piece(60, PieceType::Queen, false),
        Piece(59, PieceType::King, false),
        Piece(58, PieceType::Bishop, false),
        Piece(57, PieceType::Knight, false),
        Piece(56, PieceType::Rook, false),
        Piece(55, false),
        Piece(54, false),
        Piece(53, false),
        Piece(52, false),
        Piece(51, false),
        Piece(50, false),
        Piece(49, false),
        Piece(48, false),
    };
public:
    Board();
    std::array<PieceData, 64> getBoardState();
};

