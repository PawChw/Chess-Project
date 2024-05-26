#include "Textures.h"

void loadTextures(Textures& textures) {
	if (!textures.whitePawn.loadFromFile("Assets/Pieces/Default/wp.png"))
		throw std::invalid_argument("White Pawn texture missing");
	if (!textures.blackPawn.loadFromFile("Assets/Pieces/Default/bp.png"))
		throw std::invalid_argument("Black Pawn texture missing");

	if (!textures.whiteKnight.loadFromFile("Assets/Pieces/Default/wn.png"))
		throw std::invalid_argument("White Knight texture missing");
	if (!textures.blackKnight.loadFromFile("Assets/Pieces/Default/bn.png"))
		throw std::invalid_argument("Black Knight texture missing");

	if (!textures.whiteBishop.loadFromFile("Assets/Pieces/Default/wb.png"))
		throw std::invalid_argument("White Bishop texture missing");
	if (!textures.blackBishop.loadFromFile("Assets/Pieces/Default/bb.png"))
		throw std::invalid_argument("Black Bishop texture missing");

	if (!textures.whiteRook.loadFromFile("Assets/Pieces/Default/wr.png"))
		throw std::invalid_argument("White Rook texture missing");
	if (!textures.blackRook.loadFromFile("Assets/Pieces/Default/br.png"))
		throw std::invalid_argument("Black Rook texture missing");

	if (!textures.whiteQueen.loadFromFile("Assets/Pieces/Default/wq.png"))
		throw std::invalid_argument("White Queen texture missing");
	if (!textures.blackQueen.loadFromFile("Assets/Pieces/Default/bq.png"))
		throw std::invalid_argument("Black Queen texture missing");

	if (!textures.whiteKing.loadFromFile("Assets/Pieces/Default/wk.png"))
		throw std::invalid_argument("White King texture missing");
	if (!textures.blackKing.loadFromFile("Assets/Pieces/Default/bk.png"))
		throw std::invalid_argument("Black King texture missing");
}