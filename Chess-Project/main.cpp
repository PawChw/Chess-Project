#include<iostream>
#include<thread>
#include<chrono>
#include "Game.h"
#include <SFML/Graphics.hpp>
#include "UI/BoardView.h"
#include "Chess.h"
#include <string>
#include <algorithm>
#include <Windows.h>
#include "Players/CompuerPlayer.h"

void GameLoop() {
	sf::RenderWindow window(sf::VideoMode(1200, 800), "Chess");
	sf::Vector2u size = window.getSize();
	sf::Text whites;
	sf::Text blacks;
	sf::Text draws;
	sf::Font font;
	sf::Text player1;
	sf::Text player2;
	std::string whiteWinsString = "Player1 wins: 0";
	std::string blackWinsString = "Player2 wins: 0";
	std::string drawsString = "Draw: 0";
	bool round = true;
	BoardView board(sf::Vector2f(0, 0), std::min(size.y * 1.f, size.x * 2.f / 3), Players::HumanPlayer, Players::ComputerPlayer, true);
	font.loadFromFile("Assets/Roboto-Regular.ttf");
	window.setFramerateLimit(30);
	whites.setPosition(sf::Vector2f{ size.x * 2.f / 3 + 100, 150 });
	whites.setCharacterSize(20);
	whites.setString(whiteWinsString);
	whites.setFont(font);
	blacks.setPosition(sf::Vector2f{ size.x * 2.f / 3 + 100, 200 });
	blacks.setCharacterSize(20);
	blacks.setFont(font);
	blacks.setString(blackWinsString);
	draws.setPosition(sf::Vector2f{ size.x * 2.f / 3 + 100, 250 });
	draws.setCharacterSize(20);
	draws.setFont(font);
	draws.setString(drawsString);
	player1.setPosition(sf::Vector2f{ size.x * 2.f / 3 + 20, 50 });
	player1.setCharacterSize(20);
	player1.setFont(font);
	player1.setString("Player1");
	player2.setPosition(sf::Vector2f{ size.x * 2.f / 3 + 20, 700 });
	player2.setCharacterSize(20);
	player2.setFont(font);
	player2.setString("Player2");
	bool update = true;
	std::thread gameThread(&IGame::StartGame, board.game.get());
	while (!board.game->isGameOn) {
		std::this_thread::sleep_for(std::chrono::milliseconds(33));
	}
	while (window.isOpen()) {
		sf::Event e;
		while (window.pollEvent(e)) {
			update = true;
			switch (e.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseMoved:
				board.HandleMouseMove(e.mouseMove, window);
				break;
			case sf::Event::MouseButtonPressed:
				board.HandleMousePress(e.mouseButton, window);
				break;
			case sf::Event::MouseButtonReleased:
				board.HandleMouseRealease(e.mouseButton, window);
				break;
			case sf::Event::Resized:
				window.setSize(sf::Vector2u{ 1200, 800 });
				break;
			default:
				break;
			}
		}
		if (!update && !board.game->stateChanged) {
			std::this_thread::sleep_for(std::chrono::milliseconds(33));
			continue;
		}
		board.Update();
		window.clear();
		//re-render
		window.draw(board);
		window.draw(draws);
		window.draw(whites);
		window.draw(blacks);
		window.draw(player1);
		window.draw(player2);
		window.display();
		update = false;
		if (!board.game->isGameOn) {
			if (gameThread.joinable())
				gameThread.join();
			auto rs = board.game->rs.winner;
			gameThread = std::thread(&IGame::RestartGame, board.game.get());
			while (!board.game->isGameOn) {
				std::this_thread::sleep_for(std::chrono::milliseconds(33));
			}
			whiteWinsString = whiteWinsString.substr(0, 14) + std::to_string(board.game->player1);
			blackWinsString = blackWinsString.substr(0, 14) + std::to_string(board.game->player2);
			drawsString = drawsString.substr(0, 6) + std::to_string(board.game->draw);
			if (round) {
				player2.setPosition(sf::Vector2f{ size.x * 2.f / 3 + 20, 50 });
				player1.setPosition(sf::Vector2f{ size.x * 2.f / 3 + 20, 700 });
			}
			else {
				player1.setPosition(sf::Vector2f{ size.x * 2.f / 3 + 20, 50 });
				player2.setPosition(sf::Vector2f{ size.x * 2.f / 3 + 20, 700 });
			}
			round = !round;
			whites.setString(whiteWinsString);
			blacks.setString(blackWinsString);
			draws.setString(drawsString);
			update = true;
		}
	}
	board.game->isGameOn = false;
	if (auto d = dynamic_cast<HumanPlayer*>(board.white.get()); d != nullptr) {
		d->TryMove();
		d->TryMoveBlocker();
	}
	if (auto d = dynamic_cast<HumanPlayer*>(board.black.get()); d != nullptr) {
		d->TryMove();
		d->TryMoveBlocker();
	}
	if (gameThread.joinable())
		gameThread.join();
}

int main() {
	GameLoop();
}