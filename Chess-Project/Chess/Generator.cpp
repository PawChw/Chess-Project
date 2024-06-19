#include "Generator.h"

void Generator::SeedBitboards()
{
	for (bool side : {true, false}) {
		for (PieceType i = Pawn; i <= King; i++) {
			bitboards[static_cast<int>(side)][static_cast<int>(i)] = m_board.GetBitboard(i, side ? White : Black);
			bitboards[static_cast<int>(side)][0] |= bitboards[static_cast<int>(side)][static_cast<int>(i)];
		}
	}
	if(IsValidSquare(m_board.blocker_square))
		BitboardHelpers::SetBit(bitboards[static_cast<int>(m_moves_for)][0], m_board.blocker_square);
	m_all_pieces_bitboard = bitboards[0][0] | bitboards[1][0];
}


bool Generator::CheckMove(Move& move) const
{
	Square interesetedIn, capturedSquare = move.to;
	PieceType movedPieceType = GetPieceType(move.moved_piece),
		captured_piece = GetPieceType(move.captured_piece);
	if (movedPieceType == King) {
		interesetedIn = move.to;
	}
	else {
		interesetedIn = m_moves_for ? m_board.white_king : m_board.black_king;
	}
	Bitboard blockers = m_all_pieces_bitboard, attacksFromKingSquare, enemyPiecesOfSameType;
	BitboardHelpers::ClearBit(blockers, move.from);
	if (captured_piece && (movedPieceType == Pawn && move.to == m_board.ep_square)) {
		capturedSquare = GetSquare(GetFile(move.to), GetRank(move.from));
		BitboardHelpers::ClearBit(blockers, capturedSquare);
	}
	BitboardHelpers::SetBit(blockers, move.to);
	for (PieceType i = Pawn; i < King; i++) {
		enemyPiecesOfSameType = bitboards[static_cast<int>(!m_moves_for)][static_cast<int>(i)];
		if (i == captured_piece) BitboardHelpers::ClearBit(enemyPiecesOfSameType, capturedSquare);
		attacksFromKingSquare = GetPieceMoves(i, interesetedIn, 0, blockers, m_moves_for, 0, true);
		if (static_cast<bool>(attacksFromKingSquare & enemyPiecesOfSameType)) return false;
	}
	return true;
}

Generator::Generator(const Board& m_board): m_board(m_board), m_moves_for(m_board.is_white_to_move)
{
	SeedBitboards();
}

Generator::Generator(const Board& m_board, bool m_moves_for) : m_board(m_board), m_moves_for(m_moves_for)
{
	SeedBitboards();
}

std::vector<Move> Generator::GenerateLegalMoves(bool capturesOnly)
{
	const static std::array< std::array<Bitboard, 2>, 2> castleChecks = { { {96ull, 14ull}, {6917529027641081856ull, 1008806316530991104ull}} }; // K, Q, k, q
	std::vector<Move> m_moves;
	Bitboard tmp, movesbb, notOurBB = ~(bitboards[static_cast<int>(m_moves_for)][0]), enemyBB = bitboards[static_cast<int>(m_moves_for)][0];
	Square ourKingSquare = m_moves_for ? m_board.white_king : m_board.black_king, enemyKingSquare = m_moves_for ? m_board.black_king : m_board.white_king;
	Color c = m_moves_for ? White : Black;
	Move move{0};
	move.ep_square = m_board.ep_square;
	move.is_castle.Set({ false, false });
	for (PieceType i = Pawn; i <= King; i++) {
		tmp = bitboards[static_cast<int>(m_moves_for)][static_cast<int>(i)];
		while (static_cast<bool>(tmp)) {
			move.from = BitboardHelpers::GetAndClearIndexOfLSB(tmp);
			movesbb = GetPieceMoves(i, move.from, enemyKingSquare, m_all_pieces_bitboard, m_moves_for, m_board.ep_square, capturesOnly) & notOurBB;
			if (capturesOnly) movesbb &= m_all_pieces_bitboard;
			move.moved_piece = c | i;
			while (static_cast<bool>(movesbb)) {
				move.to = BitboardHelpers::GetAndClearIndexOfLSB(movesbb);
				move.captured_piece = m_board.GetPieceOnSquare(move.to);
				if (m_board.ep_square == move.to && GetPieceType(move.moved_piece) == Pawn) {
					move.captured_piece = ChangeColor(move.moved_piece);
				}

				//castle rights
				move.castle_rights_lost.Set({ false, false, false, false });
				if (i == King) {
					if (m_board.castle_rights[static_cast<int>(m_moves_for)][0])
						move.castle_rights_lost.Set(static_cast<int>(m_moves_for), 0, true);
					if (m_board.castle_rights[static_cast<int>(m_moves_for)][1])
						move.castle_rights_lost.Set(static_cast<int>(m_moves_for), 1, true);
				}
				else if (i == Rook) {
					if (m_board.castle_rights[static_cast<int>(m_moves_for)][0] && move.from == ourKingSquare + 3)
						move.castle_rights_lost.Set(static_cast<int>(m_moves_for), 0, true);
					if (m_board.castle_rights[static_cast<int>(m_moves_for)][1] && move.from == ourKingSquare - 4)
						move.castle_rights_lost.Set(static_cast<int>(m_moves_for), 1, true);
				}
				if (GetPieceType(move.captured_piece) == Rook) {
					if (m_board.castle_rights[static_cast<int>(!m_moves_for)][0] && move.to == enemyKingSquare + 3)
						move.castle_rights_lost.Set(static_cast<int>(!m_moves_for), 0, true);
					if (m_board.castle_rights[static_cast<int>(!m_moves_for)][1] && move.to == enemyKingSquare - 4)
						move.castle_rights_lost.Set(static_cast<int>(!m_moves_for), 1, true);
				}

				//promotion check
				if (i == Pawn && GetRank(move.to) == (m_moves_for ? 0 : 7)) {
					for (PieceType j = Knight; j < King; j++) {
						move.promoted_to_piece_type = j;
						if (CheckMove(move))
							m_moves.push_back(move);
					}
				}
				else {
					move.promoted_to_piece_type = None;
					if (CheckMove(move)) {
						m_moves.push_back(move);
					}
				}
			}
		}
	}
	move.captured_piece = None;
	move.castle_rights_lost.Set({ false, false, false, false });
	move.castle_rights_lost.Set(static_cast<int>(m_moves_for), { m_board.castle_rights[static_cast<int>(m_moves_for)][0], m_board.castle_rights[static_cast<int>(m_moves_for)][1] });
	if (m_board.castle_rights[static_cast<int>(m_moves_for)][1]
		&& !static_cast<bool>(m_all_pieces_bitboard & castleChecks[static_cast<int>(m_moves_for)][1])
		&& !m_board.IsSquareAttacked(ourKingSquare, !m_moves_for)
		&& !m_board.IsSquareAttacked(ourKingSquare - 1, !m_moves_for)
		&& !m_board.IsSquareAttacked(ourKingSquare - 2, !m_moves_for)
		&& !m_board.IsSquareAttacked(ourKingSquare - 3, !m_moves_for)
		&& !m_board.IsSquareAttacked(ourKingSquare - 4, !m_moves_for)) {
		move.to = ourKingSquare - 2;
		move.is_castle.Set({ false, true });
		m_moves.push_back(move);
	}
	if (m_board.castle_rights[static_cast<int>(m_moves_for)][0]
		&& !static_cast<bool>(m_all_pieces_bitboard & castleChecks[static_cast<int>(m_moves_for)][0])
		&& !m_board.IsSquareAttacked(ourKingSquare, !m_moves_for)
		&& !m_board.IsSquareAttacked(ourKingSquare + 1, !m_moves_for)
		&& !m_board.IsSquareAttacked(ourKingSquare + 2, !m_moves_for)
		&& !m_board.IsSquareAttacked(ourKingSquare + 3, !m_moves_for)) {
		move.to = ourKingSquare + 2;
		move.is_castle.Set({ true, false });
		m_moves.push_back(move);
	}
	return m_moves;
}
