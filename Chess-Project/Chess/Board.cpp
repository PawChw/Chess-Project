#include "Board.h"
#include "Generator.h"
#include "Board.h"
#include "Board.h"

void Board::MakeMove(Move move)
{
    // Check if the move is legal
    if (legalMovesCache.empty()) GetLegalMoves();
    if (!isMoveInVector(legalMovesCache, move)) {
        int i = 0;
        isMoveInVector(legalMovesCache, move);
        throw std::invalid_argument("Illegal move");
    }

    return ForceMakeMove(move);
}

void Board::MoveBlocker(Square newBlockerSquare)
{
    if (!isValidSquare(newBlockerSquare)  || blockerSquare == newBlockerSquare) throw std::invalid_argument("Illegal blocker move");
    Bitboard bb = getAllPiecesBitboard();
    if(BitboardHelpers::getBit(bb, newBlockerSquare)) throw std::invalid_argument("Illegal blocker move");
    blockerSquare = newBlockerSquare;
    legalMovesCache.clear();
}

void Board::ForceMakeMove(Move move)
{
    // Move the piece and promote if promotion
    board[move.to] = changePieceTypeIfNotNone(board[move.from], move.promotedToPieceType);
    board[move.from] = None;

    // Handle en passant capture
    if (move.epSquare == move.to && PieceUtils::getPieceType(move.movedPiece) == Pawn) {
        board[getSquare(GetFile(move.to), GetRank(move.from))] = None;
    }
    // handle castling
    else if (move.isCastle.get(0)) {
        int rookFrom = move.from + 3;
        int rookTo = move.from + 1;
        board[rookTo] = board[rookFrom];
        board[rookFrom] = None;
    }
    else if (move.isCastle.get(1)) {
        int rookFrom = move.from - 4;
        int rookTo = move.from - 1;
        board[rookTo] = board[rookFrom];
        board[rookFrom] = None;
    }

    // Update the en passant square
    epSquare = -1;
    if (getPieceType(move.movedPiece) == Pawn) {
        int diff = move.to - move.from;
        if (diff == 16 || diff == -16) {
            epSquare = move.from + (diff / 2);
        }
    }
    // update king squares
    else if (getPieceType(move.movedPiece) == King) {
        if (getColor(move.movedPiece) == White) whiteKing = move.to;
        else blackKing = move.to;
    }

    //handle halfmoveclock
    if (getPieceType(move.movedPiece) == Pawn || move.capturedPiece != None) {
        historicHalfMoves.push_back(halfMoveClock);
        halfMoveClock = 0;
    }
    else {
        halfMoveClock++;
    }

    //handle castle rights
    if (move.castleRightsLost.get(0, 0)) castleRights[0][0] = false;
    if (move.castleRightsLost.get(0, 1)) castleRights[0][1] = false;
    if (move.castleRightsLost.get(1, 0)) castleRights[1][0] = false;
    if (move.castleRightsLost.get(1, 1)) castleRights[1][1] = false;


    gameMoveHistory.push_back(move);
    gameHistory.push_back(currHash);
    if (getColor(move.movedPiece) == Black) fullMoveClock++;
    legalMovesCache.clear();
    ZobristKey::Move(currHash, move.from, move.to, move.movedPiece);
    isWhiteToMove = !isWhiteToMove;
    ply_count++;
}

void Board::UndoMove() {
    if (gameMoveHistory.size() == 0) return;
    Move& move = gameMoveHistory.back();
    board[move.from] = move.movedPiece;
    if (move.to == move.epSquare && PieceUtils::getPieceType(move.movedPiece) == Pawn) {
        board[getSquare(GetFile(move.to), GetRank(move.from))] = move.capturedPiece;
        board[move.to] = None;
    }
    else
        board[move.to] = move.capturedPiece;
    if (move.isCastle.get(0)) {
        int rookTo = move.from + 3;
        int rookFrom = move.from + 1;
        board[rookTo] = board[rookFrom];
        board[rookFrom] = None;
    }else if(move.isCastle.get(1)) {
        int rookTo = move.from - 4;
        int rookFrom = move.from - 1;
        board[rookTo] = board[rookFrom];
        board[rookFrom] = None;
    }

    //handle castle rights
    if (move.castleRightsLost.get(0,0)) castleRights[0][0] = true;
    if (move.castleRightsLost.get(0,1)) castleRights[0][1] = true;
    if (move.castleRightsLost.get(1,0)) castleRights[1][0] = true;
    if (move.castleRightsLost.get(1,1)) castleRights[1][1] = true;

    //return epSqure
    epSquare = move.epSquare;

    //return king squares
    if (getPieceType(move.movedPiece) == King) {
        if (getColor(move.movedPiece) == White) whiteKing = move.from;
        else blackKing = move.from;
    }

    if (halfMoveClock == 0) {
        halfMoveClock = historicHalfMoves.back();
        historicHalfMoves.pop_back();
    }
    else {
        halfMoveClock--;
    }
    gameMoveHistory.pop_back();
    if (gameHistory.empty()) {
        ZobristKey::Move(currHash, move.from, move.to, move.movedPiece);
    }
    else {
        currHash = gameHistory.back();
        gameHistory.pop_back();
    }
    if (getColor(move.movedPiece) == Black) fullMoveClock--;
    legalMovesCache.clear();
    isWhiteToMove = !isWhiteToMove;
    ply_count--;

}

void Board::ParseFEN(std::string FEN)
{
    int i = 0;
    auto ps = Parser(FEN);
    char val;
    while (!ps.finished() && i < 64) {
        val = ps.consume();
        switch (val) {
        case 'K': 
            board[i] = White | King; whiteKing = i; i++; break;
        case 'Q': 
            board[i] = White | Queen; i++; break;
        case 'R': 
            board[i] = White | Rook; i++; break;
        case 'B': 
            board[i] = White | Bishop; i++; break;
        case 'N': 
            board[i] = White | Knight; i++; break;
        case 'P':
            board[i] = White | Pawn; i++; break;
        case 'k': 
            board[i] = Black | King; blackKing = i; i++; break;
        case 'q': 
            board[i] = Black | Queen; i++; break;
        case 'r': 
            board[i] = Black | Rook; i++; break;
        case 'b': 
            board[i] = Black | Bishop; i++; break;
        case 'n': 
            board[i] = Black | Knight; i++; break;
        case 'p': 
            board[i] = Black | Pawn; i++; break;
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
    if (!ps.finished())
        if (ps.consume() != ' ') throw std::invalid_argument("Invalid FEN(space required 0)");
    if (!ps.finished()) {
        val = ps.consume();
        if (val == 'w') isWhiteToMove = true;
        else if (val == 'b') isWhiteToMove = false;
        else throw std::invalid_argument("Invalid FEN(who's to move)");
    }
    if (!ps.finished())
        if (ps.consume() != ' ') throw std::invalid_argument("Invalid FEN(space required 1)");
    castleRights = { {{false, false},{false, false }} };
    while (!ps.finished()) {
        val = ps.consume();
        if (val == 'K') castleRights[1][0] = true;
        else if (val == 'Q') castleRights[1][1] = true;
        else if (val == 'k') castleRights[0][0] = true;
        else if (val == 'q') castleRights[0][1] = true;
        else if (val == '-') {
            ps.consume();
            break;
        }
        else if (val == ' ') break;
        else throw std::invalid_argument("Invalid FEN(error with castle rights)");
    }
    while (!ps.finished()) {
        val = ps.consume();
        Square pEpSquare = 0;
        if (val >= 'a' && val <= 'h') pEpSquare += val - 'a';
        else if (val >= '1' && val <= '8') pEpSquare += (val - '1') * 8;
        else if (val == '-') {
            epSquare = -1;
            ps.consume();
            break;
        }
        else if (val == ' ') {
            if (pEpSquare < 16 || pEpSquare > 47) throw std::invalid_argument("Invalid FEN(invalid enPassant square)");
            epSquare = pEpSquare;
            break;
        }
        else throw std::invalid_argument("Invalid FEN(error with enPassant)");
    }
    while (!ps.finished()) {
        val = ps.consume();
        if (val >= '0' && val <= '9') halfMoveClock = halfMoveClock * 10 + (val - '0');
        else if (val == '-') halfMoveClock = 0;
        else if (val == ' ') {
            break;
        }
        else throw std::invalid_argument("Invalid FEN(error with halfMoveClock)");
    }
    while (!ps.finished()) {
        val = ps.consume();
        if (val >= '0' && val <= '9') fullMoveClock = fullMoveClock * 10 + (val - '0');
        else if (val == '-') fullMoveClock = 0;
        else if (val == ' ') {
            break;
        }
        else throw std::invalid_argument("Invalid FEN(error with fullMoveClock)");
    }
}

Board::Board()
{
    ParseFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    legalMovesCache.clear();
    ZobristKey::Init();
    Bitboard wb = getWhiteBitboard(), bb = getBlackBitboard(),
        p = getBitboard(Pawn), n = getBitboard(Knight), b = getBitboard(Bishop),
        r = getBitboard(Rook), q = getBitboard(Queen), k = getBitboard(King);
    currHash = ZobristKey::Compute({
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
        }, !isWhiteToMove);
}

Board::Board(std::string toParse)
{
    ParseFEN(toParse);
    legalMovesCache.clear();
    ZobristKey::Init();
    Bitboard wb = getWhiteBitboard(), bb = getBlackBitboard(),
        p = getBitboard(Pawn), n = getBitboard(Knight), b = getBitboard(Bishop),
        r = getBitboard(Rook), q = getBitboard(Queen), k = getBitboard(King);
    currHash = ZobristKey::Compute({
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
        }, !isWhiteToMove);
}

const std::vector<Move>& Board::GetLegalMoves()
{
    if (legalMovesCache.empty()) {
        auto gen = Generator(*this);
        legalMovesCache = gen.GenerateLegalMoves();
    }
    return legalMovesCache;
}

Bitboard Board::getBitboard(PieceType pieceType) const
{
    Bitboard bitboard = 0;

    for (int i = 63; i >= 0; i--) {
        bitboard <<= 1;
        if (getPieceType(board[i]) == pieceType)
            bitboard |= 1;
    }
    return bitboard;
}

Bitboard Board::getBitboard(PieceType pieceType, Color color) const
{
    Bitboard bitboard = 0;
    Piece piece = pieceType | color;
    for (int i = 63; i >= 0; i--) {
        bitboard <<= 1;
        if (board[i] == piece)
            bitboard |= 1;
    }
    return bitboard;
}

Bitboard Board::getPieceAttacks(Piece piece, Square square) const
{
    bool isWhite = (getColor(piece) == White);
    return GetPieceMoves(getPieceType(piece), square, (isWhite ? blackKing : whiteKing), getAllPiecesBitboard(), isWhite, epSquare, true);
}

Bitboard Board::getWhiteBitboard() const
{
    Bitboard bitboard = 0;

    for (int i = 63; i >= 0; i--) {
        bitboard <<= 1;
        if (board[i] & White)
            bitboard |= 1;
    }
    return bitboard;
}

Bitboard Board::getBlackBitboard() const
{
    Bitboard bitboard = 0;

    for (int i = 63; i >= 0; i--) {
        bitboard <<= 1;
        if (board[i] & Black)
            bitboard |= 1;
    }
    return bitboard;
}

Bitboard Board::getAllPiecesBitboard() const
{
    Bitboard bitboard = 0;

    for (int i = 63; i >= 0; i--) {
        bitboard <<= 1;
        if(board[i])
            bitboard |= 1;
    }
    return bitboard;
}

Piece Board::getPieceOnSquare(Square square) const
{
    if (square == blockerSquare) return Blocker;
    if (isValidSquare(square)) return board[square];
    return None;
}

bool Board::isSquareAttacked(Square square, bool byWho) const
{
    Square opposite = byWho ? whiteKing : blackKing;
    Bitboard allPieces = getAllPiecesBitboard(), currCheckBtboard;
    for (int i = 1; i < 7; i++) {
        currCheckBtboard = getBitboard(static_cast<PieceType>(i), byWho ? White : Black);
        Bitboard bb = GetPieceMoves(static_cast<PieceType>(i), square, opposite, allPieces, !byWho, epSquare, true);
        if (static_cast<bool>(bb & currCheckBtboard)) return true;
    }
    return false;
}

bool Board::isInsufficientMaterial()
{
    if (!static_cast<bool>(getBitboard(Pawn))) return false;
    auto wb = getWhiteBitboard();
    auto bb = getBlackBitboard();
    auto K = getBitboard(Knight);
    auto KnB = K | getBitboard(Bishop);
    BitboardHelpers::clearBit(wb, whiteKing);
    BitboardHelpers::clearBit(bb, blackKing);
    auto wP = BitboardHelpers::GetNumOfBitsSet(wb);
    auto bP = BitboardHelpers::GetNumOfBitsSet(bb);
    bool whiteInsufficient = false;
    bool blackInsufficient = false;
    if (wP == 0 || (wP < 2 && wb == (wb & KnB)) || (wP == 2 && wb == (wb & K) && bP == 0)) whiteInsufficient = true;
    if (bP == 0 || (bP < 2 && bb == (bb & KnB)) || (bP == 2 && bb == (bb & K) && wP == 0)) blackInsufficient = true;
    return whiteInsufficient && blackInsufficient;
}

bool Board::is50MoveRule() const
{
    return halfMoveClock == 100;
}

bool Board::isSteelMate()
{
    if (isInCheck(isWhiteToMove)) return false;
    Bitboard myBB = isWhiteToMove ? getWhiteBitboard() : getBlackBitboard(),
        myPawns = getBitboard(Pawn, isWhiteToMove ? White : Black);
    BitboardHelpers::clearBit(myBB, isWhiteToMove ? whiteKing : blackKing);
    if (myPawns ^ myBB) return false;
    return GetLegalMoves().size() == 0;
}

bool Board::isRepetition()
{
    int count = 0;
    for (int i = gameHistory.size() - halfMoveClock; i < gameHistory.size(); i++) {
        if (gameHistory[i] == currHash) {
            count++;
        }
    }
    return count > 1;
}

bool Board::isDraw()
{
    return is50MoveRule() || isSteelMate() || isRepetition() || isInsufficientMaterial();
}

bool Board::isInCheck(bool whoIs) const
{
    Square interesetedIn = whoIs ? whiteKing : blackKing;
    return isSquareAttacked(interesetedIn, !whoIs);
}

bool Board::isCheckMate()
{
    return isInCheck(isWhiteToMove) && GetLegalMoves().size() == 0;
}

Zobrist Board::getZobristKey() const
{
    return currHash;
}

std::array<Piece, 64> Board::GetBoard() const
{
    std::array<Piece, 64> r;
    std::copy(std::begin(board), std::end(board), r.begin());
    return r;
}
