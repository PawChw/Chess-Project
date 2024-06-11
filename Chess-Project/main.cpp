#include<iostream>
#include<thread>
#include<chrono>
#include "Game.h"
#include "Players/HumanPlayer.h"
#include "Players//CompuerPlayer.h"

inline Square parseSquare(char sq[3]) {
	return ((sq[1] - '1') << 3) + sq[0] - 'a';
}

int main() {
	HumanPlayer p1;
	ComputerPlayer p2(4);
	BlockerGame game(p1, p2);
	int ply_count = -1;
	std::thread gameThread = std::thread(&BlockerGame::StartGame, &game);
	char from[3] = { 0 };
	char to[3] = { 0 };
	std::cout << "Game start" << std::endl;
	while (!game.isGameOn) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	while (game.isGameOn)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(33));
		if (ply_count != game.bd.ply_count) {
			ply_count = game.bd.ply_count;
			{
				std::unique_lock lk(p1.m);
				for (Square sq = 0; sq < 64; sq++) {
					Square sq1 = (8 * (7 - sq / 8)) + sq % 8;
					if (sq % 8 == 0)
						std::cout << '\n';
					switch (game.bd.getPieceOnSquare(sq1))
					{
					case White | Pawn: std::cout << 'P'; break;
					case White | Knight: std::cout << 'N'; break;
					case White | Bishop: std::cout << 'B'; break;
					case White | Rook: std::cout << 'R'; break;
					case White | Queen: std::cout << 'Q'; break;
					case White | King: std::cout << 'K'; break;
					case Black | Pawn: std::cout << 'p'; break;
					case Black | Knight: std::cout << 'n'; break;
					case Black | Bishop: std::cout << 'b'; break;
					case Black | Rook: std::cout << 'r'; break;
					case Black | Queen: std::cout << 'q'; break;
					case Black | King: std::cout << 'k'; break;
					case Blocker: std::cout << 'X'; break;
					default:std::cout << '.'; break;
					}
				}
				std::cout << std::endl;
			}
		}
		if (p1.move) {
			std::cout << (game.bd.isWhiteToMove ? "White" : "Black") << " to move" << std::endl;
			do {
				std::cin >> from;
				std::cin >> to;
			} while (!p1.TryMove(parseSquare(from), parseSquare(to)));
		}
		else if (p1.blockerMove) {
			std::cout << (!game.bd.isWhiteToMove ? "White" : "Black") << " to move blocker" << std::endl;
			do {
				std::cin >> to;
			} while (!p1.TryMoveBlocker(parseSquare(to)));
		}
	}
	gameThread.join();
	
	if (game.rs.winner == Winner::White)
		std::cout << "White wins ";
	else if (game.rs.winner == Winner::Black)
		std::cout << "White wins ";
	else
		std::cout << "Draw ";

	if (game.rs.reason == Reason::Checkmate)
		std::cout << "by checkmate!";
	else if(game.rs.reason == Reason::IllegalMove)
		std::cout << "because enemy made Illegal Move!";
	else if (game.rs.reason == Reason::Repetition)
		std::cout << "due to repetition!";
	else if (game.rs.reason == Reason::FiftyMoveRule)
		std::cout << "due to 50 move rule!";
	else if (game.rs.reason == Reason::InsufficientMaterial)
		std::cout << "due to insufficient material!";
	else if (game.rs.reason == Reason::SteelMate)
		std::cout << "due to steelmate!";
}