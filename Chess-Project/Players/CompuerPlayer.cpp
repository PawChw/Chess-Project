#include "CompuerPlayer.h"
const int16_t ComputerPlayer::mg_pawn_table[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
        98, 134, 61, 95, 68, 126, 34, -11,
        -6, 7, 26, 31, 65, 56, 25, -20,
        -14, 13, 6, 21, 23, 12, 17, -23,
        -27, -2, -5, 12, 17, 6, 10, -25,
        -26, -4, -4, -10, 3, 3, 33, -12,
        -35, -1, -20, -23, -15, 24, 38, -22,
        0, 0, 0, 0, 0, 0, 0, 0,
};
const int16_t ComputerPlayer::eg_pawn_table[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
        98, 134, 61, 95, 68, 126, 34, -11,
        -6, 7, 26, 31, 65, 56, 25, -20,
        -14, 13, 6, 21, 23, 12, 17, -23,
        -27, -2, -5, 12, 17, 6, 10, -25,
        -26, -4, -4, -10, 3, 3, 33, -12,
        -35, -1, -20, -23, -15, 24, 38, -22,
        0, 0, 0, 0, 0, 0, 0, 0,
};
const int16_t ComputerPlayer::mg_knight_table[64] = {
    -167, -89, -34, -49, 61, -97, -15, -107,
        -73, -41, 72, 36, 23, 62, 7, -17,
        -47, 60, 37, 65, 84, 129, 73, 44,
        -9, 17, 19, 53, 37, 69, 18, 22,
        -13, 4, 16, 13, 28, 19, 21, -8,
        -23, -9, 12, 10, 19, 17, 25, -16,
        -29, -53, -12, -3, -1, 18, -14, -19,
        -105, -21, -58, -33, -17, -28, -19, -23,
};
const int16_t ComputerPlayer::eg_knight_table[64] = {
    -58, -38, -13, -28, -31, -27, -63, -99,
        -25, -8, -25, -2, -9, -25, -24, -52,
        -24, -20, 10, 9, -1, -9, -19, -41,
        -17, 3, 22, 22, 22, 11, 8, -18,
        -18, -6, 16, 25, 16, 17, 4, -18,
        -23, -3, -1, 15, 10, -3, -20, -22,
        -42, -20, -10, -5, -2, -20, -23, -44,
        -29, -51, -23, -15, -22, -18, -50, -64,
};
const int16_t ComputerPlayer::mg_bishop_table[64] = {
    -29, 4, -82, -37, -25, -42, 7, -8,
        -26, 16, -18, -13, 30, 59, 18, -47,
        -16, 37, 43, 40, 35, 50, 37, -2,
        -4, 5, 19, 50, 37, 37, 7, -2,
        -6, 13, 13, 26, 34, 12, 10, 4,
        0, 15, 15, 15, 14, 27, 18, 10,
        4, 15, 16, 0, 7, 21, 33, 1,
        -33, -3, -14, -21, -13, -12, -39, -21,
};
const int16_t ComputerPlayer::eg_bishop_table[64] = {
    -14, -21, -11, -8, -7, -9, -17, -24,
        -8, -4, 7, -12, -3, -13, -4, -14,
        2, -8, 0, -1, -2, 6, 0, 4,
        -3, 9, 12, 9, 14, 10, 3, 2,
        -6, 3, 13, 19, 7, 10, -3, -9,
        -12, -3, 8, 10, 13, 3, -7, -15,
        -14, -18, -7, -1, 4, -9, -15, -27,
        -23, -9, -23, -5, -9, -16, -5, -17,
};
const int16_t ComputerPlayer::mg_rook_table[64] = {
    32, 42, 32, 51, 63, 9, 31, 43,
        27, 32, 58, 62, 80, 67, 26, 44,
        -5, 19, 26, 36, 17, 45, 61, 16,
        -24, -11, 7, 26, 24, 35, -8, -20,
        -36, -26, -12, -1, 9, -7, 6, -23,
        -45, -25, -16, -17, 3, 0, -5, -33,
        -44, -16, -20, -9, -1, 11, -6, -71,
        -19, -13, 1, 17, 16, 7, -37, -26,
};
const int16_t ComputerPlayer::eg_rook_table[64] = {
    13, 10, 18, 15, 12, 12, 8, 5,
        11, 13, 13, 11, -3, 3, 8, 3,
        7, 7, 7, 5, 4, -3, -5, -3,
        4, 3, 13, 1, 2, 1, -1, 2,
        3, 5, 8, 4, -5, -6, -8, -11,
        -4, 0, -5, -1, -7, -12, -8, -16,
        -6, -6, 0, 2, -9, -9, -11, -3,
        -9, 2, 3, -1, -5, -13, 4, -20,
};
const int16_t ComputerPlayer::mg_queen_table[64] = {
    -28, 0, 29, 12, 59, 44, 43, 45,
        -24, -39, -5, 1, -16, 57, 28, 54,
        -13, -17, 7, 8, 29, 56, 47, 57,
        -27, -27, -16, -16, -1, 17, -2, 1,
        -9, -26, -9, -10, -2, -4, 3, -3,
        -14, 2, -11, -2, -5, 2, 14, 5,
        -35, -8, 11, 2, 8, 15, -3, 1,
        -1, -18, -9, 10, -15, -25, -31, -50,
};
const int16_t ComputerPlayer::eg_queen_table[64] = {
    -9, 22, 22, 27, 27, 19, 10, 20,
        -17, 20, 32, 41, 58, 25, 30, 0,
        -20, 6, 9, 49, 47, 35, 19, 9,
        3, 22, 24, 45, 57, 40, 57, 36,
        -18, 28, 19, 47, 31, 34, 39, 23,
        -16, -27, 15, 6, 9, 17, 10, 5,
        -22, -23, -30, -16, -16, -23, -36, -32,
        -33, -28, -22, -43, -5, -32, -20, -41,
};
const int16_t ComputerPlayer::mg_king_table[64] = {
    -65, 23, 16, -15, -56, -34, 2, 13,
        29, -1, -20, -7, -8, -4, -38, -29,
        -9, 24, 2, -16, -20, 6, 22, -22,
        -17, -20, -12, -27, -30, -25, -14, -36,
        -49, -1, -27, -39, -46, -44, -33, -51,
        -14, -14, -22, -46, -44, -30, -15, -27,
        1, 7, -8, -64, -43, -16, 9, 8,
        -15, 36, 12, -54, 8, -28, 74, 14,
};
const int16_t ComputerPlayer::eg_king_table[64] = {
    -74, -35, -18, -18, -11, 15, 4, -17,
        -12, 17, 14, 17, 17, 38, 23, 11,
        10, 17, 23, 15, 20, 45, 44, 13,
        -8, 22, 24, 27, 26, 33, 26, 3,
        -18, -4, 21, 24, 27, 23, 9, -11,
        -19, -3, 11, 21, 23, 16, 7, -9,
        -27, -11, 4, 13, 14, 4, -5, -17,
        -53, -34, -21, -11, -28, -14, -24, -43
};


int ComputerPlayer::NegaMax(Board& bd, int alpha, int beta, int depth)
{
    int bestVal = -Checkmate, val;
    if (depth == 0) return quiesce(bd, alpha, beta);
    if (bd.isCheckMate()) return -Checkmate - bd.ply_count;
    if (bd.isDraw()) {
        val = Eval(bd);
        if (val < -1000) return 100;
        return -1000;
    }
    auto moves = bd.GetLegalMoves();
    Zobrist zobrist = bd.getZobristKey();
    auto& tt = transposition->at(mask & zobrist);
    if (tt.hash == zobrist && tt.depth >= depth) {
        if (tt.flag == transpositionFlag::EXACT) return tt.eval;
        if (tt.flag == transpositionFlag::UPPERBOUND && tt.eval >= beta) return tt.eval;
        if (tt.flag == transpositionFlag::LOWERBOUND && tt.eval <= alpha) return tt.eval;
    }
    std::sort(moves.begin(), moves.end(), [this, zobrist](const Move& mx, const Move& my)->bool {
        int xval = 0, yval = 0;
        Zobrist zx = ZobristKey::GetMove(zobrist, mx.from, mx.to, mx.movedPiece);
        auto tx = transposition->at(mask & zx);
        if (tx.hash == zx) 
            xval += tx.eval;
        Zobrist zy = ZobristKey::GetMove(zobrist, my.from, my.to, my.movedPiece);
        auto ty = transposition->at(mask & zy);
        if (ty.hash == zy) 
            yval += ty.eval;
        xval <<= 3;
        yval <<= 3;
        xval |= getPieceType(mx.capturedPiece) | getPieceType(mx.promotedToPieceType);
        yval |= getPieceType(my.capturedPiece) | getPieceType(my.promotedToPieceType);
        return xval > yval;
    });
    Move bestMove = moves.front();
    transpositionFlag bestFlag = transpositionFlag::EXACT;
    for (auto move : moves) {
        bd.MakeMove(move);
        val = -NegaMax(bd, -beta, -alpha, depth - 1);
        bd.UndoMove();
        if (val >= beta) {
            bestVal = val;
            bestFlag = transpositionFlag::UPPERBOUND;
            break;
        }
        if (val > bestVal) {
            bestVal = val;
            bestMove = move;
            if (val > alpha) {
                bestFlag = transpositionFlag::LOWERBOUND;
                alpha = bestVal;
            }
        }
    }
    tt.flag = bestFlag;
    tt.eval = bestVal;
    tt.hash = zobrist;
    tt.move = bestMove;
    return bestVal;
}

int ComputerPlayer::quiesce(Board& bd, int alpha, int beta)
{
    return Eval(bd)+bd.GetLegalMoves().size();
}

int ComputerPlayer::Eval(Board& bd) const
{
	int val = 0, mg=0, eg=0, mg_progress = 0;
	int attacks, index, eg_progress;
    bool isWhite = true;
    auto pvals = PieceVals;
	Bitboard kingSafety, tmp;
	for (Bitboard side : {bd.getWhiteBitboard(), bd.getBlackBitboard()}) {
		kingSafety = bd.getBitboard(King) & side;
		kingSafety |= kingSafety << 8 | kingSafety << 16 | kingSafety >> 16 | kingSafety >> 8;
		kingSafety |= kingSafety << 1 | kingSafety >> 1;
		attacks = 0;
        auto pvals = PieceVals;
		for (PieceType p = Pawn; p < King; p++) {
			tmp = bd.getBitboard(p) & side;
            if (p == Pawn) {
                int pwns = BitboardHelpers::GetNumOfBitsSet(tmp) - 5;
                pvals[Knight] += pwns * 6;
                pvals[Rook] -= pwns * 13;
            }
            else if (p == Bishop && BitboardHelpers::GetNumOfBitsSet(tmp) > 1) val += 50;
			while (tmp) {
				index = BitboardHelpers::getAndClearIndexOfLSB(tmp);
				val += pvals[p];
                mg_progress += mgIncrement[p];
                mg+=mg_table[p][index | (isWhite ? White : 0)];
                eg+=eg_table[p][index | (isWhite ? White : 0)];
				attacks += BitboardHelpers::GetNumOfBitsSet(bd.getPieceAttacks(bd.getPieceOnSquare(index), index) & kingSafety);
			}
		}
		val += static_cast<int>(std::pow(attacks, 2));
		val = -val;
        mg = -mg;
        eg = -eg;
        isWhite = !isWhite;
	}
    if(mg_progress>24) mg_progress = 24;
    eg_progress = 24 - mg_progress;
    return (val + (mg * mg_progress + eg * eg_progress) / 24)*(bd.isWhiteToMove ? 1 : -1);
}

ComputerPlayer::ComputerPlayer(uint8_t max_depth, Clock maxTime) : max_depth(max_depth), maxTime(maxTime)
{
	transposition->fill(MoveEval{ 0,NullMove,0,0,transpositionFlag::NONE });
	for (int p = PieceUtils::Pawn; p <= PieceUtils::King; p++) {
		for (int sq = 0; sq < 64; sq++) {
			mg_table[p | PieceUtils::White][sq] = mg_pesto_table[p][sq];
			eg_table[p | PieceUtils::White][sq] = eg_pesto_table[p][sq];
			mg_table[p][sq] = mg_pesto_table[p][sq ^ 56];
			eg_table[p][sq] = eg_pesto_table[p][sq ^ 56];
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
        NegaMax(bd, -Checkmate/10, Checkmate/10, 2);
    if (BitboardHelpers::getBit(av, tt.move.to))
        return tt.move.to;
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
    auto sTime = std::chrono::system_clock::now();
    std::cout << "Computer thinks..." << std::endl;
    curr = bd.getZobristKey();
    int depth = 0;
    while (depth<max_depth){
        depth++;
        if (std::chrono::system_clock::now() - sTime > maxTime || NegaMax(bd, Checkmate, Checkmate, depth) > Checkmate - max_depth) break;
    }
    if (curr != bd.getZobristKey()) {
        std::cout << "WTF?" << std::endl;
    }
	auto tt = transposition->at(curr & mask);
    std::cout << "Computer" << (bd.isWhiteToMove ? "(w)" : "(b)") << " thinks: " << static_cast<char>(tt.move.to % 8 + 'a') << static_cast<char>(tt.move.to / 8 + '1') << '\t' << tt.eval * (bd.isWhiteToMove ? 1 : -1) << "\tat depth: " << depth << std::endl;
    return tt.move;
}

ComputerPlayer::~ComputerPlayer()
{
}
