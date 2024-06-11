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
	int val = 0, mg=0, eg=0, mg_progress = 0;
	int attacks, index, eg_progress;
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
                mg_progress += mgIncrement[p];
                mg+=mg_table[p][index];
                eg+=eg_table[p][index];
				attacks += BitboardHelpers::GetNumOfBitsSet(bd.getPieceAttacks(bd.getPieceOnSquare(index), index) & kingSafety);
			}
		}
		val += static_cast<int>(std::pow(attacks, 2));
		val = -val;
        mg = -mg;
        eg = -eg;
	}
    if(mg_progress>24) mg_progress = 24;
    eg_progress = 24 - mg_progress;
	return val + (mg*mg_progress + eg*eg_progress)/24;
}

ComputerPlayer::ComputerPlayer(uint8_t max_depth) : max_depth(max_depth)
{
	transposition = new std::array<MoveEval, mask + 1>();
	transposition->fill(MoveEval{ 0,NullMove,0,0,0 });
    int mg_pawn_table[64] = {
          0,   0,   0,   0,   0,   0,  0,   0,
        -35,  -1, -20, -23, -15,  24, 38, -22,
        -26,  -4,  -4, -10,   3,   3, 33, -12,
        -27,  -2,  -5,  12,  17,   6, 10, -25,
        -14,  13,   6,  21,  23,  12, 17, -23,
         -6,   7,  26,  31,  65,  56, 25, -20,
         98, 134,  61,  95,  68, 126, 34, -11,
          0,   0,   0,   0,   0,   0,  0,   0,
    };

    int eg_pawn_table[64] = {
          0,   0,   0,   0,   0,   0,   0,   0,
         13,   8,   8,  10,  13,   0,   2,  -7,
          4,   7,  -6,   1,   0,  -5,  -1,  -8,
         13,   9,  -3,  -7,  -7,  -8,   3,  -1,
         32,  24,  13,   5,  -2,   4,  17,  17,
         94, 100,  85,  67,  56,  53,  82,  84,
        178, 173, 158, 134, 147, 132, 165, 187,
          0,   0,   0,   0,   0,   0,   0,   0,
    };

    int mg_knight_table[64] = {
        -105, -21, -58, -33, -17, -28, -19,  -23,
         -29, -53, -12,  -3,  -1,  18, -14,  -19,
         -23,  -9,  12,  10,  19,  17,  25,  -16,
         -13,   4,  16,  13,  28,  19,  21,   -8,
          -9,  17,  19,  53,  37,  69,  18,   22,
         -47,  60,  37,  65,  84, 129,  73,   44,
         -73, -41,  72,  36,  23,  62,   7,  -17,
        -167, -89, -34, -49,  61, -97, -15, -107,
    };

    int eg_knight_table[64] = {
        -29, -51, -23, -15, -22, -18, -50, -64,
        -42, -20, -10,  -5,  -2, -20, -23, -44,
        -23,  -3,  -1,  15,  10,  -3, -20, -22,
        -18,  -6,  16,  25,  16,  17,   4, -18,
        -17,   3,  22,  22,  22,  11,   8, -18,
        -24, -20,  10,   9,  -1,  -9, -19, -41,
        -25,  -8, -25,  -2,  -9, -25, -24, -52,
        -58, -38, -13, -28, -31, -27, -63, -99,
    };

    int mg_bishop_table[64] = {
        -33,  -3, -14, -21, -13, -12, -39, -21,
          4,  15,  16,   0,   7,  21,  33,   1,
          0,  15,  15,  15,  14,  27,  18,  10,
         -6,  13,  13,  26,  34,  12,  10,   4,
         -4,   5,  19,  50,  37,  37,   7,  -2,
        -16,  37,  43,  40,  35,  50,  37,  -2,
        -26,  16, -18, -13,  30,  59,  18, -47,
        -29,   4, -82, -37, -25, -42,   7,  -8,
    };

    int eg_bishop_table[64] = {
        -23,  -9, -23,  -5, -9, -16,  -5, -17,
        -14, -18,  -7,  -1,  4,  -9, -15, -27,
        -12,  -3,   8,  10, 13,   3,  -7, -15,
         -6,   3,  13,  19,  7,  10,  -3,  -9,
         -3,   9,  12,   9, 14,  10,   3,   2,
          2,  -8,   0,  -1, -2,   6,   0,   4,
         -8,  -4,   7, -12, -3, -13,  -4, -14,
        -14, -21, -11,  -8, -7,  -9, -17, -24,
    };

    int mg_rook_table[64] = {
        -19, -13,   1,  17, 16,  7, -37, -26,
        -44, -16, -20,  -9, -1, 11,  -6, -71,
        -45, -25, -16, -17,  3,  0,  -5, -33,
        -36, -26, -12,  -1,  9, -7,   6, -23,
        -24, -11,   7,  26, 24, 35,  -8, -20,
         -5,  19,  26,  36, 17, 45,  61,  16,
         27,  32,  58,  62, 80, 67,  26,  44,
         32,  42,  32,  51, 63,  9,  31,  43,
    };

    int eg_rook_table[64] = {
        -9,  2,  3, -1, -5, -13,   4, -20,
        -6, -6,  0,  2, -9,  -9, -11,  -3,
        -4,  0, -5, -1, -7, -12,  -8, -16,
         3,  5,  8,  4, -5,  -6,  -8, -11,
         4,  3, 13,  1,  2,   1,  -1,   2,
         7,  7,  7,  5,  4,  -3,  -5,  -3,
        11, 13, 13, 11, -3,   3,   8,   3,
        13, 10, 18, 15, 12,  12,   8,   5,
    };

    int mg_queen_table[64] = {
         -1, -18,  -9,  10, -15, -25, -31, -50,
        -35,  -8,  11,   2,   8,  15,  -3,   1,
        -14,   2, -11,  -2,  -5,   2,  14,   5,
         -9, -26,  -9, -10,  -2,  -4,   3,  -3,
        -27, -27, -16, -16,  -1,  17,  -2,   1,
        -13, -17,   7,   8,  29,  56,  47,  57,
        -24, -39,  -5,   1, -16,  57,  28,  54,
        -28,   0,  29,  12,  59,  44,  43,  45,
    };

    int eg_queen_table[64] = {
        -33, -28, -22, -43,  -5, -32, -20, -41,
        -22, -23, -30, -16, -16, -23, -36, -32,
        -16, -27,  15,   6,   9,  17,  10,   5,
        -18,  28,  19,  47,  31,  34,  39,  23,
          3,  22,  24,  45,  57,  40,  57,  36,
        -20,   6,   9,  49,  47,  35,  19,   9,
        -17,  20,  32,  41,  58,  25,  30,   0,
         -9,  22,  22,  27,  27,  19,  10,  20,
    };

    int mg_king_table[64] = {
        -15,  36,  12, -54,   8, -28,  24,  14,
          1,   7,  -8, -64, -43, -16,   9,   8,
        -14, -14, -22, -46, -44, -30, -15, -27,
        -49,  -1, -27, -39, -46, -44, -33, -51,
        -17, -20, -12, -27, -30, -25, -14, -36,
         -9,  24,   2, -16, -20,   6,  22, -22,
         29,  -1, -20,  -7,  -8,  -4, -38, -29,
        -65,  23,  16, -15, -56, -34,   2,  13,
    };

    int eg_king_table[64] = {
        -53, -34, -21, -11, -28, -14, -24, -43,
        -27, -11,   4,  13,  14,   4,  -5, -17,
        -19,  -3,  11,  21,  23,  16,   7,  -9,
        -18,  -4,  21,  24,  27,  23,   9, -11,
         -8,  22,  24,  27,  26,  33,  26,   3,
         10,  17,  23,  15,  20,  45,  44,  13,
        -12,  17,  14,  17,  17,  38,  23,  11,
        -74, -35, -18, -18, -11,  15,   4, -17
    };

    int* mg_pesto_table[7] = {
        nullptr,
        mg_pawn_table,
        mg_knight_table,
        mg_bishop_table,
        mg_rook_table,
        mg_queen_table,
        mg_king_table
    };

    int* eg_pesto_table[7] = {
        nullptr,
        eg_pawn_table,
        eg_knight_table,
        eg_bishop_table,
        eg_rook_table,
        eg_queen_table,
        eg_king_table
    };

	for (int p = PieceUtils::Pawn; p <= PieceUtils::King; p++) {
		for (int sq = 0; sq < 64; sq++) {
			mg_table[p | PieceUtils::White][sq] = mg_pesto_table[p][sq];
			eg_table[p | PieceUtils::White][sq] = eg_pesto_table[p][sq];
			mg_table[p][sq] = mg_pesto_table[p][sq] = mg_pesto_table[p][sq^56];
			eg_table[p][sq] = eg_pesto_table[p][sq] = eg_pesto_table[p][sq^56];
		}
	}
}

Square ComputerPlayer::ThinkBlocker(Board bd)
{
	auto& tt = transposition->at(bd.getZobristKey() & mask);
	Bitboard av = ~bd.getAllPiecesBitboard();
	if(isValidSquare(bd.blockerSquare))
		BitboardHelpers::clearBit(av, bd.blockerSquare);
	if (tt.hash != bd.getZobristKey())
        NegaMax(bd, Checkmate/10, -Checkmate/10, 2);
	Square to = tt.move.from;
	auto fileDiff = GetFile(tt.move.to) - GetFile(tt.move.from),
		rankDiff = GetRank(tt.move.to) - GetRank(tt.move.from);
	if (fileDiff > 1) to += 1;
	else if(fileDiff < -1) to -= 1;
	if(rankDiff > 1) to += 8;
	else if(rankDiff < -1) to -= 8;
	if(BitboardHelpers::getBit(av, to))
		return to;
	return BitboardHelpers::getIndexOfLSB(av);
}

Move ComputerPlayer::Think(Board bd)
{
	bd.GetLegalMoves();
	bd.blockerSquare = -1;
	NegaMax(bd, Checkmate, -Checkmate, max_depth);
	auto tt = transposition->at(bd.getZobristKey() & mask);
    std::cout << "Computer thinks: " << tt.eval << std::endl;
	return tt.move;
}

ComputerPlayer::~ComputerPlayer()
{
	delete transposition;
}
