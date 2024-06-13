#include "Generator.h"

void Generator::seedBitboards()
{
	for (bool side : {true, false}) {
		for (PieceType i = Pawn; i <= King; i++) {
			bitboards[static_cast<int>(side)][static_cast<int>(i)] = board.getBitboard(i, side ? White : Black);
			bitboards[static_cast<int>(side)][0] |= bitboards[static_cast<int>(side)][static_cast<int>(i)];
		}
	}
	if(isValidSquare(board.blockerSquare))
		BitboardHelpers::setBit(bitboards[static_cast<int>(movesFor)][0], board.blockerSquare);
	allPiecesBitboard = bitboards[0][0] | bitboards[1][0];
}

//Square interesetedIn = whoIs ? whiteKing : blackKing;
//return isSquareAttacked(interesetedIn, !whoIs);
// 
//Square opposite = byWho ? whiteKing : blackKing;
//Bitboard allPieces = getAllPiecesBitboard(), currCheckBtboard;
//for (int i = 1; i < 7; i++) {
//	currCheckBtboard = getBitboard(static_cast<PieceType>(i), byWho ? White : Black);
//	Bitboard bb = GetPieceMoves(static_cast<PieceType>(i), square, opposite, allPieces, !byWho, epSquare, true);
//	if (static_cast<bool>(bb & currCheckBtboard)) return true;
//}
//return false;

bool Generator::checkMove(Move& move) const
{
	Square interesetedIn, capturedSquare = move.to;
	PieceType movedPieceType = getPieceType(move.movedPiece),
		capturedPiece = getPieceType(move.capturedPiece);
	if (movedPieceType == King) {
		interesetedIn = move.to;
	}
	else {
		interesetedIn = movesFor ? board.whiteKing : board.blackKing;
	}
	Bitboard blockers = allPiecesBitboard, attacksFromKingSquare, enemyPiecesOfSameType;
	BitboardHelpers::clearBit(blockers, move.from);
	if (capturedPiece && movedPieceType == Pawn && move.to == board.epSquare) {
		capturedSquare = getSquare(GetFile(move.to), GetRank(move.from));
		BitboardHelpers::clearBit(blockers, capturedSquare);
	}
	for (PieceType i = Pawn; i < King; i++) {
		enemyPiecesOfSameType = bitboards[static_cast<int>(!movesFor)][static_cast<int>(i)];
		if (i == capturedPiece) BitboardHelpers::clearBit(enemyPiecesOfSameType, capturedPiece);
		attacksFromKingSquare = GetPieceMoves(i, interesetedIn, 0, blockers, movesFor, 0, true);
		if (static_cast<bool>(attacksFromKingSquare & enemyPiecesOfSameType)) return false;
	}
	return true;
}

Generator::Generator(const Board& board): board(board), movesFor(board.isWhiteToMove)
{
	seedBitboards();
}

Generator::Generator(const Board& board, bool movesFor) : board(board), movesFor(movesFor)
{
	seedBitboards();
}

std::vector<Move> Generator::GenerateLegalMoves()
{
	const static std::array< std::array<Bitboard, 2>, 2> castleChecks = { { {96ull, 14ull}, {6917529027641081856ull, 1008806316530991104ull}} }; // K, Q, k, q
	std::vector<Move> moves;
	Bitboard tmp, movesbb, notOurBB = ~(bitboards[static_cast<int>(movesFor)][0]);
	Square ourKingSquare = movesFor ? board.whiteKing : board.blackKing, enemyKingSquare = movesFor ? board.blackKing : board.whiteKing;
	Color c = movesFor ? White : Black;
	Move move{0};
	move.epSquare = board.epSquare;
	move.isCastle.set({ false, false });
	for (PieceType i = Pawn; i <= King; i++) {
		tmp = bitboards[static_cast<int>(movesFor)][static_cast<int>(i)];
		while (static_cast<bool>(tmp)) {
			move.from = BitboardHelpers::getAndClearIndexOfLSB(tmp);
			movesbb = GetPieceMoves(i, move.from, enemyKingSquare, allPiecesBitboard, movesFor, board.epSquare, false) & notOurBB;
			move.movedPiece = c | i;
			while (static_cast<bool>(movesbb)) {
				move.to = BitboardHelpers::getAndClearIndexOfLSB(movesbb);
				move.capturedPiece = board.getPieceOnSquare(move.to);
				if (board.epSquare == move.to && getPieceType(move.movedPiece) == Pawn) {
					move.capturedPiece = changeColor(move.movedPiece);
				}

				//castle rights
				move.castleRightsLost.set({ false, false, false, false });
				if (i == King) {
					if (board.castleRights[static_cast<int>(movesFor)][0])
						move.castleRightsLost.set(static_cast<int>(movesFor), 0, true);
					if (board.castleRights[static_cast<int>(movesFor)][1])
						move.castleRightsLost.set(static_cast<int>(movesFor), 1, true);
				}
				else if (i == Rook) {
					if (board.castleRights[static_cast<int>(movesFor)][0] && move.from == ourKingSquare + 3)
						move.castleRightsLost.set(static_cast<int>(movesFor), 0, true);
					if (board.castleRights[static_cast<int>(movesFor)][1] && move.from == ourKingSquare - 4)
						move.castleRightsLost.set(static_cast<int>(movesFor), 1, true);
				}
				if (getPieceType(move.capturedPiece) == Rook) {
					if (board.castleRights[static_cast<int>(!movesFor)][0] && move.to == enemyKingSquare + 3)
						move.castleRightsLost.set(static_cast<int>(!movesFor), 0, true);
					if (board.castleRights[static_cast<int>(!movesFor)][1] && move.to == enemyKingSquare - 4)
						move.castleRightsLost.set(static_cast<int>(!movesFor), 1, true);
				}

				//promotion check
				if (i == Pawn && GetRank(move.to) == (movesFor ? 0 : 7)) {
					for (PieceType j = Knight; j < King; j++) {
						move.promotedToPieceType = j;
						if (checkMove(move))
							moves.push_back(move);
					}
				}
				else {
					move.promotedToPieceType = None;
					if (checkMove(move)) {
						moves.push_back(move);
					}
				}
			}
		}
	}
	move.capturedPiece = None;
	move.castleRightsLost.set({ false, false, false, false });
	move.castleRightsLost.set(static_cast<int>(movesFor), { board.castleRights[static_cast<int>(movesFor)][0], board.castleRights[static_cast<int>(movesFor)][1] });
	if (board.castleRights[static_cast<int>(movesFor)][1]
		&& !static_cast<bool>(allPiecesBitboard & castleChecks[static_cast<int>(movesFor)][1])
		&& !board.isSquareAttacked(ourKingSquare, !movesFor)
		&& !board.isSquareAttacked(ourKingSquare - 1, !movesFor)
		&& !board.isSquareAttacked(ourKingSquare - 2, !movesFor)
		&& !board.isSquareAttacked(ourKingSquare - 3, !movesFor)
		&& !board.isSquareAttacked(ourKingSquare - 4, !movesFor)) {
		move.to = ourKingSquare - 2;
		move.isCastle.set({ false, true });
		moves.push_back(move);
	}
	if (board.castleRights[static_cast<int>(movesFor)][0]
		&& !static_cast<bool>(allPiecesBitboard & castleChecks[static_cast<int>(movesFor)][0])
		&& !board.isSquareAttacked(ourKingSquare, !movesFor)
		&& !board.isSquareAttacked(ourKingSquare + 1, !movesFor)
		&& !board.isSquareAttacked(ourKingSquare + 2, !movesFor)
		&& !board.isSquareAttacked(ourKingSquare + 3, !movesFor)) {
		move.to = ourKingSquare + 2;
		move.isCastle.set({ true, false });
		moves.push_back(move);
	}
	return moves;
}
