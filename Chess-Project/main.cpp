#include<iostream>
#include<thread>
#include<chrono>
#include "Game.h"
#include "Players/HumanPlayer.h"

inline Square parseSquare(char sq[3]) {
	return ((sq[1] - '1') << 3) + sq[0] - 'a';
}

int main() {
	HumanPlayer p1, p2;
	BlockerGame game(p1, p2);
	std::thread gameThread = std::thread(&BlockerGame::StartGame, &game);
	char from[3] = { 0 };
	char to[3] = { 0 };
	std::cout << "Game start" << std::endl;
	while (!game.isGameOn) {
		std::this_thread::sleep_for(std::chrono::milliseconds(33));
	}
	while (game.isGameOn)
	{
		if (p1.move) {
			std::cout << "White to move" << std::endl;
			do {
				std::cin >> from;
				std::cin >> to;
			} while (!p1.TryMove(parseSquare(from), parseSquare(to)));
		}
		else if (p1.blockerMove) {
			std::cout << "White to move blocker" << std::endl;
			do {
				std::cin >> to;
			} while (!p1.TryMoveBlocker(parseSquare(to)));
		}
		else if (p2.move) {
			std::cout << "Black to move" << std::endl;
			do {
				std::cin >> from;
				std::cin >> to;
			} while (!p2.TryMove(parseSquare(from), parseSquare(to)));
		}
		else if (p2.blockerMove) {
			std::cout << "Black to move blocker" << std::endl;
			do {
				std::cin >> to;
			} while (!p2.TryMoveBlocker(parseSquare(to)));
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