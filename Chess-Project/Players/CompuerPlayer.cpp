#include "CompuerPlayer.h"
const int ComputerPlayer::mg_pawn_table[64] = {
      0,   0,   0,   0,   0,   0,  0,   0,
     98, 134,  61,  95,  68, 126, 34, -11,
     -6,   7,  26,  31,  65,  56, 25, -20,
    -14,  13,   6,  21,  23,  12, 17, -23,
    -27,  -2,  -5,  12,  17,   6, 10, -25,
    -26,  -4,  -4, -10,   3,   3, 33, -12,
    -35,  -1, -20, -23, -15,  24, 38, -22,
      0,   0,   0,   0,   0,   0,  0,   0,
};
const int ComputerPlayer::eg_pawn_table[64] = {
      0,   0,   0,   0,   0,   0,   0,   0,
    178, 173, 158, 134, 147, 132, 165, 187,
     94, 100,  85,  67,  56,  53,  82,  84,
     32,  24,  13,   5,  -2,   4,  17,  17,
     13,   9,  -3,  -7,  -7,  -8,   3,  -1,
      4,   7,  -6,   1,   0,  -5,  -1,  -8,
     13,   8,   8,  10,  13,   0,   2,  -7,
      0,   0,   0,   0,   0,   0,   0,   0,
};
const int ComputerPlayer::mg_knight_table[64] = {
    -167, -89, -34, -49,  61, -97, -15, -107,
     -73, -41,  72,  36,  23,  62,   7,  -17,
     -47,  60,  37,  65,  84, 129,  73,   44,
      -9,  17,  19,  53,  37,  69,  18,   22,
     -13,   4,  16,  13,  28,  19,  21,   -8,
     -23,  -9,  12,  10,  19,  17,  25,  -16,
     -29, -53, -12,  -3,  -1,  18, -14,  -19,
    -105, -21, -58, -33, -17, -28, -19,  -23,
};
const int ComputerPlayer::eg_knight_table[64] = {
    -58, -38, -13, -28, -31, -27, -63, -99,
    -25,  -8, -25,  -2,  -9, -25, -24, -52,
    -24, -20,  10,   9,  -1,  -9, -19, -41,
    -17,   3,  22,  22,  22,  11,   8, -18,
    -18,  -6,  16,  25,  16,  17,   4, -18,
    -23,  -3,  -1,  15,  10,  -3, -20, -22,
    -42, -20, -10,  -5,  -2, -20, -23, -44,
    -29, -51, -23, -15, -22, -18, -50, -64,
};
const int ComputerPlayer::mg_bishop_table[64] = {
    -29,   4, -82, -37, -25, -42,   7,  -8,
    -26,  16, -18, -13,  30,  59,  18, -47,
    -16,  37,  43,  40,  35,  50,  37,  -2,
     -4,   5,  19,  50,  37,  37,   7,  -2,
     -6,  13,  13,  26,  34,  12,  10,   4,
      0,  15,  15,  15,  14,  27,  18,  10,
      4,  15,  16,   0,   7,  21,  33,   1,
    -33,  -3, -14, -21, -13, -12, -39, -21,
};
const int ComputerPlayer::eg_bishop_table[64] = {
    -14, -21, -11,  -8, -7,  -9, -17, -24,
     -8,  -4,   7, -12, -3, -13,  -4, -14,
      2,  -8,   0,  -1, -2,   6,   0,   4,
     -3,   9,  12,   9, 14,  10,   3,   2,
     -6,   3,  13,  19,  7,  10,  -3,  -9,
    -12,  -3,   8,  10, 13,   3,  -7, -15,
    -14, -18,  -7,  -1,  4,  -9, -15, -27,
    -23,  -9, -23,  -5, -9, -16,  -5, -17,
};
const int ComputerPlayer::mg_rook_table[64] = {
     32,  42,  32,  51, 63,  9,  31,  43,
     27,  32,  58,  62, 80, 67,  26,  44,
     -5,  19,  26,  36, 17, 45,  61,  16,
    -24, -11,   7,  26, 24, 35,  -8, -20,
    -36, -26, -12,  -1,  9, -7,   6, -23,
    -45, -25, -16, -17,  3,  0,  -5, -33,
    -44, -16, -20,  -9, -1, 11,  -6, -71,
    -19, -13,   1,  17, 16,  7, -37, -26,
};
const int ComputerPlayer::eg_rook_table[64] = {
    13, 10, 18, 15, 12,  12,   8,   5,
    11, 13, 13, 11, -3,   3,   8,   3,
     7,  7,  7,  5,  4,  -3,  -5,  -3,
     4,  3, 13,  1,  2,   1,  -1,   2,
     3,  5,  8,  4, -5,  -6,  -8, -11,
    -4,  0, -5, -1, -7, -12,  -8, -16,
    -6, -6,  0,  2, -9,  -9, -11,  -3,
    -9,  2,  3, -1, -5, -13,   4, -20,
};
const int ComputerPlayer::mg_queen_table[64] = {
    -28,   0,  29,  12,  59,  44,  43,  45,
    -24, -39,  -5,   1, -16,  57,  28,  54,
    -13, -17,   7,   8,  29,  56,  47,  57,
    -27, -27, -16, -16,  -1,  17,  -2,   1,
     -9, -26,  -9, -10,  -2,  -4,   3,  -3,
    -14,   2, -11,  -2,  -5,   2,  14,   5,
    -35,  -8,  11,   2,   8,  15,  -3,   1,
     -1, -18,  -9,  10, -15, -25, -31, -50,
};
const int ComputerPlayer::eg_queen_table[64] = {
     -9,  22,  22,  27,  27,  19,  10,  20,
    -17,  20,  32,  41,  58,  25,  30,   0,
    -20,   6,   9,  49,  47,  35,  19,   9,
      3,  22,  24,  45,  57,  40,  57,  36,
    -18,  28,  19,  47,  31,  34,  39,  23,
    -16, -27,  15,   6,   9,  17,  10,   5,
    -22, -23, -30, -16, -16, -23, -36, -32,
    -33, -28, -22, -43,  -5, -32, -20, -41,
};
const int ComputerPlayer::mg_king_table[64] = {
    -65,  23,  16, -15, -56, -34,   2,  13,
     29,  -1, -20,  -7,  -8,  -4, -38, -29,
     -9,  24,   2, -16, -20,   6,  22, -22,
    -17, -20, -12, -27, -30, -25, -14, -36,
    -49,  -1, -27, -39, -46, -44, -33, -51,
    -14, -14, -22, -46, -44, -30, -15, -27,
      1,   7,  -8, -64, -43, -16,   9,   8,
    -15,  36,  12, -54,   8, -28,  24,  14,
};
const int ComputerPlayer::eg_king_table[64] = {
    -74, -35, -18, -18, -11,  15,   4, -17,
    -12,  17,  14,  17,  17,  38,  23,  11,
     10,  17,  23,  15,  20,  45,  44,  13,
     -8,  22,  24,  27,  26,  33,  26,   3,
    -18,  -4,  21,  24,  27,  23,   9, -11,
    -19,  -3,  11,  21,  23,  16,   7,  -9,
    -27, -11,   4,  13,  14,   4,  -5, -17,
    -53, -34, -21, -11, -28, -14, -24, -43
};


Move ComputerPlayer::Think(Board bd)
{
    std::cout << "Computer thinks..." << std::endl;
    curr = bd.GetZobristKey();
    int depth = 1;
    int mateMaxDepth = -checkmate - bd.ply_count - max_depth;
    auto sTime = std::chrono::system_clock::now();
    auto& tt = transposition->at(mask & curr);
    if (tt.depth > 1 && tt.hash == curr) {
        depth = tt.depth;
    }
    while (depth < max_depth && std::chrono::system_clock::now() - sTime < (desired_time/3)) {
        std::cout << "Computer thinks at depth " << depth << std::endl;
        if (NegaMax(bd, mateMaxDepth, -mateMaxDepth, depth) > checkmate-bd.ply_count-max_depth) break;
        depth++;
    }
    std::cout << "Computer thinks(m)" << (bd.is_white_to_move ? "(w): " : "(b): ") << static_cast<char>(GetFile(tt.move.to) + 'a') << static_cast<char>('8' - GetRank(tt.move.to)) << "\tvalue: " << static_cast<int>(tt.eval) << "\tat depth: " << static_cast<int>(tt.depth) << "\tin: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - sTime).count() << "ms" << std::endl;
    return tt.move;
}

int ComputerPlayer::NegaMax(Board& bd, int alpha, int beta, int depth)
{
    if (bd.IsCheckMate()) return -checkmate + bd.ply_count;
    if (bd.IsKingCapturd()) return checkmate - bd.ply_count;
    if (bd.IsDraw()) {
        if (Eval(bd) < -2000) return 100;
        return -100;
    }
    Zobrist zobrist = bd.GetZobristKey();
    auto& tt = transposition->at(mask & zobrist);
    std::vector<Move> legalMoves = bd.GetLegalMoves();
    if (tt.hash == zobrist && tt.depth >= depth && IsMoveInVector(legalMoves, tt.move)) {
        if (tt.flag == TranspositionFlag::EXACT) return tt.eval;
        if (tt.flag == TranspositionFlag::UPPERBOUND && tt.eval >= beta) return tt.eval;
        if (tt.flag == TranspositionFlag::LOWERBOUND && tt.eval <= alpha) return tt.eval;
    }
    if (depth == 0) {
        return Quiesce(bd, alpha, beta);
    }
    int bestVal = -checkmate - bd.ply_count;
    int value = bestVal;
    std::sort(legalMoves.begin(), legalMoves.end(), [this, zobrist](const Move& mx, const Move& my)->bool {
        int xval = GetPieceType(mx.captured_piece) + GetPieceType(mx.promoted_to_piece_type) - GetPieceType(mx.moved_piece);
        int yval = GetPieceType(my.captured_piece) + GetPieceType(my.promoted_to_piece_type) - GetPieceType(my.moved_piece);

        Zobrist zx = ZobristKey::GetMove(zobrist, mx.from, mx.to, mx.moved_piece);
        auto tx = transposition->at(mask & zx);
        if (tx.hash == zx) {
            xval = tx.eval;
        }

        Zobrist zy = ZobristKey::GetMove(zobrist, my.from, my.to, my.moved_piece);
        auto ty = transposition->at(mask & zy);
        if (ty.hash == zy) {
            yval = ty.eval;
        }

        return xval > yval;
        });
    Move bestMove = legalMoves.front();
    TranspositionFlag bestFlag = TranspositionFlag::LOWERBOUND;
    for (auto move : legalMoves) {
        bd.ForceMakeMove(move);
        value = -NegaScout(bd, -(alpha+1), -alpha, depth - 1);
        if (value > alpha && value < beta)
            value = -NegaMax(bd, -beta, -alpha, depth - 1) + (move.is_castle.toBool() ? 500 : 0);
        bd.UndoMove();

        if (value >= beta) {
            bestVal = value;
            bestFlag = TranspositionFlag::UPPERBOUND;
            break;
        }
        if (value > bestVal) {
            bestVal = value;
            bestMove = move;
            if (value > alpha) {
                bestFlag = TranspositionFlag::EXACT;
                alpha = bestVal;
            }
        }
    }
    tt.depth = depth;
    tt.flag = bestFlag;
    tt.eval = bestVal;
    tt.hash = zobrist;
    tt.move = bestMove;
    return bestVal;
}

int ComputerPlayer::NegaScout(Board& bd, int alpha, int beta, int depth)
{
    if (bd.IsCheckMate()) return -checkmate + bd.ply_count;
    if (bd.IsKingCapturd()) return checkmate - bd.ply_count;
    if (bd.IsDraw()) {
        if (Eval(bd) < -1000) return 100;
        return -1000;
    }
    Zobrist zobrist = bd.GetZobristKey();
    int bestVal = -checkmate - bd.ply_count;
    int value = bestVal;
    if (depth == 0) {
        return Eval(bd);
    }
    std::vector<Move> legalMoves = bd.GetLegalMoves();
    std::sort(legalMoves.begin(), legalMoves.end(), [this, zobrist](const Move& mx, const Move& my)->bool {
        int xval = GetPieceType(mx.captured_piece) + GetPieceType(mx.promoted_to_piece_type) - GetPieceType(mx.moved_piece);
        int yval = GetPieceType(my.captured_piece) + GetPieceType(my.promoted_to_piece_type) - GetPieceType(my.moved_piece);

        Zobrist zx = ZobristKey::GetMove(zobrist, mx.from, mx.to, mx.moved_piece);
        auto tx = transposition->at(mask & zx);
        if (tx.hash == zx) {
            xval = tx.eval;
        }

        Zobrist zy = ZobristKey::GetMove(zobrist, my.from, my.to, my.moved_piece);
        auto ty = transposition->at(mask & zy);
        if (ty.hash == zy) {
            yval = ty.eval;
        }

        return xval > yval;
        });
    for (auto move : legalMoves) {
        bd.ForceMakeMove(move);
        value = -NegaScout(bd, -beta, -alpha, depth - 1);
        bd.UndoMove();
        if (value >= beta) {
            bestVal = value;
            break;
        }
        if (value > bestVal) {
            bestVal = value;
            if (value > alpha) {
                alpha = bestVal;
            }
        }
    }
    return bestVal;
}

int ComputerPlayer::Quiesce(Board& bd, int alpha, int beta, int checks)
{
    if (bd.IsCheckMate()) return -checkmate + bd.ply_count;
    if (bd.IsKingCapturd()) return checkmate - bd.ply_count;
    int standPat = Eval(bd);
    if (bd.IsDraw()) {
        if (standPat < -1000) return 100;
        return -1000;
    }
    Zobrist zobrist = bd.GetZobristKey();
    auto& tt = transposition->at(mask & zobrist);
    if (tt.hash == zobrist && tt.depth >= max_depth + qseDepth - checks) {
        return tt.eval;
    }
    if (standPat >= beta) {
        return beta;
    }
    int BIG_DELTA = piece_vals[Queen]; 
    if (bd.WasProomtion()) BIG_DELTA += piece_vals[Queen]- 2*piece_vals[Pawn];
    if (standPat + BIG_DELTA < alpha) {
        return alpha;
    }
    if (alpha < standPat) {
        alpha = standPat;
    }
    bool filter = true;
    auto legalMoves = bd.GetLegalMoves();
    if (bd.IsInCheck(bd.is_white_to_move)) {
        checks++;
        if(checks == qseDepth) return -checkmate + bd.ply_count;
    }
    else legalMoves = FilterSilentMoves(bd, legalMoves);
    int val = alpha;

    if (legalMoves.empty()) return alpha;
    Move bestMove = legalMoves.front();
    std::sort(legalMoves.begin(), legalMoves.end(), [](const Move& mx, const Move& my)->bool {
        int xval = 0, yval = 0;
        xval = GetPieceType(mx.captured_piece) + GetPieceType(mx.promoted_to_piece_type) - GetPieceType(mx.moved_piece);
        yval = GetPieceType(my.captured_piece) + GetPieceType(mx.promoted_to_piece_type) - GetPieceType(my.moved_piece);
        return xval > yval;
        });

    for (auto capture : legalMoves) {
        bd.ForceMakeMove(capture);
        val = -Quiesce(bd, -beta, -alpha, checks);
        bd.UndoMove();
        if (val >= beta) {
            val = beta;
            bestMove = capture;
            break;
        }
        if (val > alpha) {
            val = alpha;
            bestMove = capture;
        }
    }
    tt.eval = val;
    tt.flag = TranspositionFlag::QUIESCE;
    tt.hash = zobrist;
    tt.depth = max_depth + qseDepth - checks;
    tt.move = bestMove;
    return alpha;
}

inline int ComputerPlayer::Clamp(int min, int max, int val)
{
    return (val > max ? max : (val < min ? min : val));
}

std::vector<Move> ComputerPlayer::FilterSilentMoves(Board& bd, std::vector<Move> m_moves)
{
    std::vector<Move> captures;
    for (auto mv : m_moves) {
        if (mv.captured_piece)
            captures.push_back(mv);
        else if (GetPieceType(mv.moved_piece) ^ King) {
            {
                Square interestedIn = bd.is_white_to_move ? bd.black_king : bd.white_king;
                Bitboard allPieces = bd.GetAllPiecesBitboard(), currCheckBitboard, bb;
                BitboardHelpers::ClearBit(allPieces, mv.from);
                if (GetPieceType(mv.moved_piece) == Pawn && mv.to == bd.ep_square) {
                    BitboardHelpers::ClearBit(allPieces, bd.ep_square);
                }
                for (PieceType i = Pawn; i < King; i++) {
                    currCheckBitboard = bd.GetBitboard(i, bd.is_white_to_move ? White : Black);
                    if (i == GetPieceType(mv.moved_piece)) {
                        BitboardHelpers::ClearBit(allPieces, mv.from);
                        BitboardHelpers::SetBit(allPieces, mv.to);
                    }
                    bb = GetPieceMoves(i, interestedIn, 0, allPieces, !bd.is_white_to_move, 0, true);
                    if (static_cast<bool>(bb & currCheckBitboard)) {
                        captures.push_back(mv);
                        break;
                    }
                }
            }
        }
    }
    return captures;
}

int ComputerPlayer::Eval(Board& bd) const
{
    int mgProgress = 0, egProgress, val, mg=0, eg=0, attacks;

    for (bool side : {bd.is_white_to_move, !bd.is_white_to_move}) {
        attacks = 0;
        Bitboard safety = GetKingSafety(side, bd);
        Bitboard pieces = side ? bd.GetWhiteBitboard() : bd.GetBlackBitboard();
        while (pieces) {
            Square square = BitboardHelpers::GetAndClearIndexOfLSB(pieces);
            Piece piece = bd.GetPieceOnSquare(square);
            attacks += BitboardHelpers::GetNumOfBitsSet(bd.GetPieceAttacks(piece, square) & safety) * atk_vals[GetPieceType(piece)];
            mg += mg_table[piece & 15][square] + piece_vals[GetPieceType(piece)]; //ignore C6385, piece & 15 will always be lower than 15, because its max value is King(6) | White(8) = 14
            eg += eg_table[piece & 15][square] + piece_vals[GetPieceType(piece) + 6]; //ignore C6385, read above
            mgProgress += mg_increment[GetPieceType(piece)];
        }
        mg = -mg-SafetyTable[Clamp(0,61, attacks)];
        eg = -eg;
    }
   
    if(mgProgress>24) mgProgress = 24;
    egProgress = 24 - mgProgress;
    val = (mg * mgProgress + eg * egProgress) / 24;
    return val;
}

Bitboard ComputerPlayer::GetKingSafety(bool side, Board& bd) const {
    int i = bd.is_white_to_move ? bd.white_king : bd.black_king;
    Bitboard kingSafety = GetKingMoves(i, (i + 32) % 64);
    BitboardHelpers::SetBit(kingSafety, i);
    if (bd.is_white_to_move) {
        kingSafety |= kingSafety >> 16;
    }
    else {
        kingSafety |= kingSafety << 16;
    }
    return kingSafety;
}

ComputerPlayer::ComputerPlayer(uint8_t max_depth, Clock desired_time, int qseDepth) : max_depth(max_depth), desired_time(desired_time), qseDepth(qseDepth)
{
	transposition->fill(MoveEval{ 0,NullMove,0,0,TranspositionFlag::NONE });
	for (int p = PieceUtils::Pawn; p <= PieceUtils::King; p++) {
		for (int sq = 0; sq < 64; sq++) {
			mg_table[p | PieceUtils::White][sq] = mg_pesto_table[p][sq ^ 56];
			eg_table[p | PieceUtils::White][sq] = eg_pesto_table[p][sq ^ 56];
			mg_table[p][sq] = mg_pesto_table[p][sq];
			eg_table[p][sq] = eg_pesto_table[p][sq];
		}
	}
}

Square ComputerPlayer::ThinkBlocker(Board bd)
{
	auto& tt = transposition->at(bd.GetZobristKey() & mask);
	Bitboard av = ~bd.GetAllPiecesBitboard();
	if(IsValidSquare(bd.blocker_square))
		BitboardHelpers::ClearBit(av, bd.blocker_square);
	if (tt.hash != bd.GetZobristKey())
        NegaMax(bd, -checkmate/10, checkmate/10, 2);
    if (BitboardHelpers::GetBit(av, tt.move.to))
        return tt.move.to;
	Square to = tt.move.from;
	auto fileDiff = GetFile(tt.move.to) - GetFile(tt.move.from),
		rankDiff = GetRank(tt.move.to) - GetRank(tt.move.from);
	if (fileDiff > 1) to += 1;
	else if(fileDiff < -1) to -= 1;
	if(rankDiff > 1) to += 8;
	else if(rankDiff < -1) to -= 8;
	if(BitboardHelpers::GetBit(av, to))
		return to;
	return BitboardHelpers::GetIndexOfLSB(av);
}


ComputerPlayer::~ComputerPlayer()
{
}
