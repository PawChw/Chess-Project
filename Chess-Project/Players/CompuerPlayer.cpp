#include "CompuerPlayer.h"


int ComputerPlayer::NegaMax(Board& bd, int alpha, int beta, int depth)
{
	if (bd.isCheckMate()) return -Checkmate + bd.ply_count;
	int staticEval = Eval(bd) * (bd.isWhiteToMove ? 1 : -1);
	if (bd.isDraw()) return (staticEval < -2000 ? 1000 : -30);
	auto zobrist = bd.getZobristKey();
	auto& tt = transposition->at(mask & zobrist);
	auto moves = bd.GetLegalMoves();
	if (tt.hash == zobrist && tt.depth >= depth && isMoveInVector(moves, tt.move)) {
		if (tt.flag == 1) return tt.eval;
		if (tt.flag == 2 && tt.eval >= beta) return tt.eval;
		if (tt.flag == 3 && tt.eval <= alpha) return tt.eval;
	}
	if (depth == 0) return staticEval;
	std::qsort(moves.data(), moves.size(), sizeof(Move),
		[](void const* x, void const* y)->int {
			Move mx = *static_cast<const Move*>(x),
			my = *static_cast<const Move*>(y);
			int xval = getPieceType(mx.capturedPiece) | getPieceType(mx.promotedToPieceType);
			int yval = getPieceType(my.capturedPiece) | getPieceType(my.promotedToPieceType);
			return yval - xval;
		}
	);
	int bestVal = alpha;
	Move bestMove = moves.front();
	for (auto m : moves) {
		bd.ForceMakeMove(m);
		int val = -NegaMax(bd, beta, alpha, depth - 1);
		bd.UndoMove();
		if (val >= beta) {
			bestVal = val;
			break;
		}
		if (val >= bestVal) {
			bestVal = val;
			bestMove = m;
		}
	}
	tt.eval = bestVal;
	tt.hash = zobrist;
	tt.move = bestMove;
	if (bestVal < alpha) {
		tt.flag = 3;
	}
	else if (bestVal >= beta) {
		tt.flag = 2;
	}
	else {
		tt.flag = 3;
	}
	return bestVal;
}

int ComputerPlayer::Eval(Board& bd) const
{
	int val = 0;
	int attacks, index;
	Bitboard kingSafety, tmp;
	for (Bitboard side : {bd.getWhiteBitboard(), bd.getBlackBitboard()}) {
		kingSafety = bd.getBitboard(King) & side;
		kingSafety |= kingSafety << 8 | kingSafety << 16 | kingSafety >> 16 | kingSafety >> 8;
		kingSafety |= kingSafety << 1 | kingSafety >> 1;
		attacks = 0;
		for (PieceType p = Pawn; p < King; p++) {
			tmp = bd.getBitboard(p) & side;
			while (tmp) {
				index = BitboardHelpers::getAndClearIndexOfLSB(tmp);
				val += PieceVals[p];
				attacks += BitboardHelpers::GetNumOfBitsSet(bd.getPieceAttacks(bd.getPieceOnSquare(index), index) & kingSafety);
			}
		}
		val += static_cast<int>(std::pow(attacks, 2));
		val = -val;
	}
	return val;
}

ComputerPlayer::ComputerPlayer(uint8_t max_depth) : max_depth(max_depth)
{
	transposition = new std::array<MoveEval, mask + 1>();
	transposition->fill(MoveEval{ 0,NullMove,0,0,0 });
}

Square ComputerPlayer::ThinkBlocker(Board bd)
{
	auto& tt = transposition->at(bd.getZobristKey() & mask);
	Bitboard av = ~bd.getAllPiecesBitboard();
	if(isValidSquare(bd.blockerSquare))
		BitboardHelpers::clearBit(av, bd.blockerSquare);
	if(tt.hash == bd.getZobristKey() && BitboardHelpers::getBit(av, tt.move.to))
		return tt.move.to;
	return BitboardHelpers::getIndexOfLSB(av);
}

Move ComputerPlayer::Think(Board bd)
{
	bd.GetLegalMoves();
	bd.blockerSquare = -1;
	NegaMax(bd, Checkmate, -Checkmate, max_depth);
	auto tt = transposition->at(bd.getZobristKey() & mask);
	return tt.move;
}

ComputerPlayer::~ComputerPlayer()
{
	delete transposition;
}
