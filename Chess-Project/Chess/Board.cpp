#include "Board.h"
#include "Generator.h"
#include "Board.h"
#include "Board.h"

void Board::MakeMove(Move move)
{
    // Check if the move is legal
    if (m_legal_moves_cache.empty()) GetLegalMoves();
    if (!IsMoveInVector(m_legal_moves_cache, move)) {
        throw std::invalid_argument("Illegal move");
    }

    return ForceMakeMove(move);
}

void Board::MoveBlocker(Square newBlockerSquare)
{
    if (!IsValidSquare(newBlockerSquare)  || blocker_square == newBlockerSquare) throw std::invalid_argument("Illegal blocker move");
    Bitboard bb = GetAllPiecesBitboard();
    if(BitboardHelpers::GetBit(bb, newBlockerSquare)) throw std::invalid_argument("Illegal blocker move");
    blocker_square = newBlockerSquare;
    m_legal_moves_cache.clear();
}

void Board::ForceMakeMove(Move move)
{
    // Move the piece and promote if promotion
    BitboardHelpers::ClearBit(m_board[PieceToBitboardIndex(move.moved_piece)], move.from);
    BitboardHelpers::SetBit(m_board[PieceToBitboardIndex(ChangePieceTypeIfNotNone(move.moved_piece, move.promoted_to_piece_type))], move.to);

    // Handle en passant capture
    if (move.ep_square == move.to && PieceUtils::GetPieceType(move.moved_piece) == Pawn) {
        BitboardHelpers::ClearBit(m_board[PieceToBitboardIndex(move.moved_piece^(White|Black))], GetSquare(GetFile(move.to), GetRank(move.from)));
    }
    // handle capture
    else if (move.captured_piece) {
        BitboardHelpers::ClearBit(m_board[PieceToBitboardIndex(move.captured_piece)], move.to);
    }
    // handle castling
    else if (move.is_castle.Get(0)) {
        int rookFrom = move.from + 3;
        int rookTo = move.from + 1;
        int bbIndex = PieceToBitboardIndex(GetColor(move.moved_piece) | Rook);
        BitboardHelpers::ClearBit(m_board[bbIndex], rookFrom);
        BitboardHelpers::SetBit(m_board[bbIndex], rookTo);
    }
    else if (move.is_castle.Get(1)) {
        int rookFrom = move.from - 4;
        int rookTo = move.from - 1;
        int bbIndex = PieceToBitboardIndex(GetColor(move.moved_piece) | Rook);
        BitboardHelpers::ClearBit(m_board[bbIndex], rookFrom);
        BitboardHelpers::SetBit(m_board[bbIndex], rookTo);
    }

    // Update the en passant square
    ep_square = -1;
    if (GetPieceType(move.moved_piece) == Pawn) {
        int diff = move.to - move.from;
        if (diff == 16 || diff == -16) {
            ep_square = move.from + (diff / 2);
        }
    }
    // update king squares
    else if (GetPieceType(move.moved_piece) == King) {
        if (GetColor(move.moved_piece) == White) white_king = move.to;
        else black_king = move.to;
    }

    //handle halfmoveclock
    if (GetPieceType(move.moved_piece) == Pawn || move.captured_piece != None) {
        m_historic_half_moves.push_back(m_half_move_clock);
        m_half_move_clock = 0;
    }
    else {
        m_half_move_clock++;
    }

    //handle castle rights
    if (move.castle_rights_lost.Get(0, 0)) castle_rights[0][0] = false;
    if (move.castle_rights_lost.Get(0, 1)) castle_rights[0][1] = false;
    if (move.castle_rights_lost.Get(1, 0)) castle_rights[1][0] = false;
    if (move.castle_rights_lost.Get(1, 1)) castle_rights[1][1] = false;


    m_game_move_history.push_back(move);
    m_game_history.push_back(m_curr_hash);
    if (GetColor(move.moved_piece) == Black) m_full_move_clock++;
    m_legal_moves_cache.clear();
    ZobristKey::Move(m_curr_hash, move.from, move.to, move.moved_piece);
    is_white_to_move = !is_white_to_move;
    ply_count++;
}

void Board::UndoMove() {
    if (m_game_move_history.size() == 0) return;
    Move& move = m_game_move_history.back();
    m_game_move_history.pop_back();

    BitboardHelpers::SetBit(m_board[PieceToBitboardIndex(move.moved_piece)], move.from);
    BitboardHelpers::ClearBit(m_board[PieceToBitboardIndex(ChangePieceTypeIfNotNone(move.moved_piece, move.promoted_to_piece_type))], move.to);

    // Handle en passant capture
    if (move.ep_square == move.to && PieceUtils::GetPieceType(move.moved_piece) == Pawn) {
        BitboardHelpers::SetBit(m_board[PieceToBitboardIndex(move.moved_piece ^ (White | Black))], GetSquare(GetFile(move.to), GetRank(move.from)));
    }
    // handle capture
    else if (move.captured_piece) {
        BitboardHelpers::SetBit(m_board[PieceToBitboardIndex(move.captured_piece)], move.to);
    }
    // handle castling
    else if (move.is_castle.Get(0)) {
        int rookFrom = move.from + 3;
        int rookTo = move.from + 1;
        int bbIndex = PieceToBitboardIndex(GetColor(move.moved_piece) | Rook);
        BitboardHelpers::SetBit(m_board[bbIndex], rookFrom);
        BitboardHelpers::ClearBit(m_board[bbIndex], rookTo);
    }
    else if (move.is_castle.Get(1)) {
        int rookFrom = move.from - 4;
        int rookTo = move.from - 1;
        int bbIndex = PieceToBitboardIndex(GetColor(move.moved_piece) | Rook);
        BitboardHelpers::SetBit(m_board[bbIndex], rookFrom);
        BitboardHelpers::ClearBit(m_board[bbIndex], rookTo);
    }

    //handle castle rights
    if (move.castle_rights_lost.Get(0,0)) castle_rights[0][0] = true;
    if (move.castle_rights_lost.Get(0,1)) castle_rights[0][1] = true;
    if (move.castle_rights_lost.Get(1,0)) castle_rights[1][0] = true;
    if (move.castle_rights_lost.Get(1,1)) castle_rights[1][1] = true;

    //return epSqure
    ep_square = move.ep_square;

    //return king squares
    if (GetPieceType(move.moved_piece) == King) {
        if (GetColor(move.moved_piece) == White) white_king = move.from;
        else black_king = move.from;
    }

    if (m_half_move_clock == 0) {
        m_half_move_clock = m_historic_half_moves.back();
        m_historic_half_moves.pop_back();
    }
    else {
        m_half_move_clock--;
    }

    if (m_game_history.empty()) {
        ZobristKey::Move(m_curr_hash, move.from, move.to, move.moved_piece);
    }
    else {
        m_curr_hash = m_game_history.back();
        m_game_history.pop_back();
    }

    if (GetColor(move.moved_piece) == Black) m_full_move_clock--;
    m_legal_moves_cache.clear();
    is_white_to_move = !is_white_to_move;
    ply_count--;

}

void Board::ParseFEN(std::string FEN)
{
    int i = 0;
    auto ps = Parser(FEN);
    char val;
    while (!ps.Finished() && i < 64) {
        val = ps.Consume();
        switch (val) {
        case 'K': 
            BitboardHelpers::SetBit(m_board[11], i); white_king = i; i++; break;
        case 'Q': 
            BitboardHelpers::SetBit(m_board[10], i); i++; break;
        case 'R': 
            BitboardHelpers::SetBit(m_board[9], i); i++; break;
        case 'B': 
            BitboardHelpers::SetBit(m_board[8], i); i++; break;
        case 'N': 
            BitboardHelpers::SetBit(m_board[7], i); i++; break;
        case 'P':
            BitboardHelpers::SetBit(m_board[6], i); i++; break;
        case 'k': 
            BitboardHelpers::SetBit(m_board[5], i); black_king = i; i++; break;
        case 'q': 
            BitboardHelpers::SetBit(m_board[4], i); i++; break;
        case 'r': 
            BitboardHelpers::SetBit(m_board[3], i); i++; break;
        case 'b': 
            BitboardHelpers::SetBit(m_board[2], i); i++; break;
        case 'n': 
            BitboardHelpers::SetBit(m_board[1], i); i++; break;
        case 'p': 
            BitboardHelpers::SetBit(m_board[0], i); i++; break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            i += val - '0'; break;
        case '/':
            if (i % 8 != 0) throw std::invalid_argument("Invalid FEN(board invalid)");
        }
    }
    if (!ps.Finished())
        if (ps.Consume() != ' ') throw std::invalid_argument("Invalid FEN(space required 0)");
    if (!ps.Finished()) {
        val = ps.Consume();
        if (val == 'w') is_white_to_move = true;
        else if (val == 'b') is_white_to_move = false;
        else throw std::invalid_argument("Invalid FEN(who's to move)");
    }
    if (!ps.Finished())
        if (ps.Consume() != ' ') throw std::invalid_argument("Invalid FEN(space required 1)");
    castle_rights = { {{false, false},{false, false }} };
    while (!ps.Finished()) {
        val = ps.Consume();
        if (val == 'K') castle_rights[1][0] = true;
        else if (val == 'Q') castle_rights[1][1] = true;
        else if (val == 'k') castle_rights[0][0] = true;
        else if (val == 'q') castle_rights[0][1] = true;
        else if (val == '-') {
            ps.Consume();
            break;
        }
        else if (val == ' ') break;
        else throw std::invalid_argument("Invalid FEN(error with castle rights)");
    }
    while (!ps.Finished()) {
        val = ps.Consume();
        Square pEpSquare = 0;
        if (val >= 'a' && val <= 'h') pEpSquare += val - 'a';
        else if (val >= '1' && val <= '8') pEpSquare += (val - '1') * 8;
        else if (val == '-') {
            ep_square = -1;
            ps.Consume();
            break;
        }
        else if (val == ' ') {
            if (pEpSquare < 16 || pEpSquare > 47) throw std::invalid_argument("Invalid FEN(invalid enPassant square)");
            ep_square = pEpSquare;
            break;
        }
        else throw std::invalid_argument("Invalid FEN(error with enPassant)");
    }
    while (!ps.Finished()) {
        val = ps.Consume();
        if (val >= '0' && val <= '9') m_half_move_clock = m_half_move_clock * 10 + (val - '0');
        else if (val == '-') m_half_move_clock = 0;
        else if (val == ' ') {
            break;
        }
        else throw std::invalid_argument("Invalid FEN(error with halfMoveClock)");
    }
    while (!ps.Finished()) {
        val = ps.Consume();
        if (val >= '0' && val <= '9') m_full_move_clock = m_full_move_clock * 10 + (val - '0');
        else if (val == '-') m_full_move_clock = 0;
        else if (val == ' ') {
            break;
        }
        else throw std::invalid_argument("Invalid FEN(error with fullMoveClock)");
    }
}

inline int Board::PieceToBitboardIndex(Piece piece)
{
    return (GetColor(piece) == White ? 6 : 0) + (GetPieceType(piece) - 1);
}

Board::Board()
{
    ParseFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    m_legal_moves_cache.clear();
    ZobristKey::Init();
    Bitboard wb = GetWhiteBitboard(), bb = GetBlackBitboard(),
        p = GetBitboard(Pawn), n = GetBitboard(Knight), b = GetBitboard(Bishop),
        r = GetBitboard(Rook), q = GetBitboard(Queen), k = GetBitboard(King);
    m_curr_hash = ZobristKey::Compute({
        wb & p,
        wb & n,
        wb & b,
        wb & r,
        wb & q,
        wb & k,
        bb & p,
        bb & n,
        bb & b,
        bb & r,
        bb & q,
        bb & k,
        }, !is_white_to_move);
}

Board::Board(std::string to_parse)
{
    ParseFEN(to_parse);
    m_legal_moves_cache.clear();
    ZobristKey::Init();
    Bitboard wb = GetWhiteBitboard(), bb = GetBlackBitboard(),
        p = GetBitboard(Pawn), n = GetBitboard(Knight), b = GetBitboard(Bishop),
        r = GetBitboard(Rook), q = GetBitboard(Queen), k = GetBitboard(King);
    m_curr_hash = ZobristKey::Compute({
        wb & p,
        wb & n,
        wb & b,
        wb & r,
        wb & q,
        wb & k,
        bb & p,
        bb & n,
        bb & b,
        bb & r,
        bb & q,
        bb & k,
        }, !is_white_to_move);
}

const std::vector<Move>& Board::GetLegalMoves()
{
    if (m_legal_moves_cache.empty()) {
        auto gen = Generator(*this);
        m_legal_moves_cache = gen.GenerateLegalMoves();
    }
    return m_legal_moves_cache;
}

Bitboard Board::GetBitboard(PieceType pieceType) const
{
    int bitboardIndex = pieceType - 1;
    return m_board[bitboardIndex] | m_board[bitboardIndex+6];
}

Bitboard Board::GetBitboard(PieceType pieceType, Color color) const
{
    int bitboardIndex = (color == White ? 6 : 0) + (pieceType - 1);
    return m_board[bitboardIndex];
}

Bitboard Board::GetPieceAttacks(Piece piece, Square square) const
{
    bool isWhite = (GetColor(piece) == White);
    return GetPieceMoves(GetPieceType(piece), square, (isWhite ? black_king : white_king), GetAllPiecesBitboard(), isWhite, ep_square, true);
}

Bitboard Board::GetWhiteBitboard() const
{
    Bitboard bitboard = 0;

    for (int i = 6; i < 12; i++) {
        bitboard |= m_board[i];
    }
    return bitboard;
}

Bitboard Board::GetBlackBitboard() const
{
    Bitboard bitboard = 0;

    for (int i = 0; i < 6; i++) {
        bitboard |= m_board[i];
    }
    return bitboard;
}

Bitboard Board::GetAllPiecesBitboard() const
{
    Bitboard bitboard = 0;

    for (auto& bb : m_board) {
        bitboard |= bb;
    }
    return bitboard;
}

Piece Board::GetPieceOnSquare(Square square) const
{
    if (square == blocker_square) return Blocker;
    if (IsValidSquare(square)) {
        for (int i = 0; i < 12; i++) {
            if (BitboardHelpers::GetBit(m_board[i], square)) {
                return (i > 5 ? White : Black) | ((i % 6) + 1);
            }
        }
    }
    return None;
}

bool Board::IsSquareAttacked(Square square, bool byWho) const
{
    Bitboard allPieces = GetAllPiecesBitboard(), currCheckBtboard;
    for (int i = Pawn; i < King; i++) {
        currCheckBtboard = GetBitboard(static_cast<PieceType>(i), byWho ? White : Black);
        Bitboard bb = GetPieceMoves(static_cast<PieceType>(i), square, 0, allPieces, !byWho, ep_square, true);
        if (static_cast<bool>(bb & currCheckBtboard)) return true;
    }
    return false;
}

bool Board::IsInsufficientMaterial()
{
    if (static_cast<bool>(GetBitboard(Pawn))) 
        return false;
    auto wb = GetWhiteBitboard();
    auto bb = GetBlackBitboard();
    auto K = GetBitboard(Knight);
    auto KnB = K | GetBitboard(Bishop);
    BitboardHelpers::ClearBit(wb, white_king);
    BitboardHelpers::ClearBit(bb, black_king);
    auto wP = BitboardHelpers::GetNumOfBitsSet(wb);
    auto bP = BitboardHelpers::GetNumOfBitsSet(bb);
    bool whiteInsufficient = false;
    bool blackInsufficient = false;
    if (wP == 0 || (wP < 2 && wb == (wb & KnB)) || (wP == 2 && wb == (wb & K) && bP == 0)) whiteInsufficient = true;
    if (bP == 0 || (bP < 2 && bb == (bb & KnB)) || (bP == 2 && bb == (bb & K) && wP == 0)) blackInsufficient = true;
    return whiteInsufficient && blackInsufficient;
}

bool Board::Is50MoveRule() const
{
    return m_half_move_clock == 100;
}

bool Board::IsSteelMate()
{
    if (IsInCheck(is_white_to_move)) return false;
    Bitboard myBB = is_white_to_move ? GetWhiteBitboard() : GetBlackBitboard(),
        myPawns = GetBitboard(Pawn, is_white_to_move ? White : Black);
    BitboardHelpers::ClearBit(myBB, is_white_to_move ? white_king : black_king);
    if ((myPawns ^ myBB) & 4359202964317896252ull) return false; //magic bitboard to prevent stuck in corner in blocker games
    return GetLegalMoves().size() == 0;
}

bool Board::IsRepetition()
{
    int count = 0;
    for (int i = m_game_history.size() - m_half_move_clock; i < m_game_history.size(); i++) {
        if (m_game_history[i] == m_curr_hash) {
            count++;
        }
    }
    return count > 1;
}

bool Board::IsDraw()
{
    return Is50MoveRule() || IsSteelMate() || IsRepetition() || IsInsufficientMaterial();
}

bool Board::IsInCheck(bool whoIs) const
{
    Square interesetedIn = whoIs ? white_king : black_king;
    return IsSquareAttacked(interesetedIn, !whoIs);
}

bool Board::IsCheckMate()
{
    return IsInCheck(is_white_to_move) && GetLegalMoves().size() == 0;
}

bool Board::IsKingCapturd() const
{
    if (blocker_square == -1) return false;
    return BitboardHelpers::GetNumOfBitsSet(GetBitboard(King)) != 2;
}

bool Board::WasProomtion() const
{
    return static_cast<bool>(m_game_move_history.back().captured_piece);
}

Zobrist Board::GetZobristKey() const
{
    return m_curr_hash;
}

std::array<Piece, 64> Board::GetBoard() const
{
    std::array<Piece, 64> r = { 0 };
    for (int i = 0; i < 64; i++) {
        r[i] = GetPieceOnSquare(i);
    }
    return r;
}
