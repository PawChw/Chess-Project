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
#include "Player.h"
#include "UI/Button.h"

void GameLoop() {
	sf::RenderWindow window(sf::VideoMode(1200, 800), "Chess");
	sf::Vector2u size = window.getSize();
	sf::Text whites;
	sf::Text blacks;
	sf::Text draws;
	sf::Font font;
	sf::Text whitePlayerNote;
	sf::Text blackPlayerNote;
	std::string whiteWinsString = "Player1 wins: 0";
	std::string blackWinsString = "Player2 wins: 0";
	std::string drawsString = "Draw: 0";
	bool round = true;
	std::shared_ptr<HumanPlayer> human = std::make_shared<HumanPlayer>();
	std::shared_ptr<ComputerPlayer> computer = std::make_shared<ComputerPlayer>();
	std::unique_ptr<BoardView> board = std::make_unique<BoardView>(sf::Vector2f(0, 0), std::min(size.y * 1.f, size.x * 2.f / 3), human, computer, true);
	float board_x_offset = board->getPosition().x + board->getSize();
	Button nextGameButton(font);
	nextGameButton.setSize(sf::Vector2f{ 250,50 });
	nextGameButton.setString("Start next game");
	nextGameButton.setPosition(sf::Vector2f{ board_x_offset + 100, 300 });
	Button humanVsComputerButton(font);
	humanVsComputerButton.setSize(sf::Vector2f{ 250,50 });
	humanVsComputerButton.setString("Human vs Computer");
	humanVsComputerButton.setPosition(sf::Vector2f{ board_x_offset + 100, 360 });
	Button humanVsHumanButton(font);
	humanVsHumanButton.setSize(sf::Vector2f{ 250,50 });
	humanVsHumanButton.setString("Human vs Human");
	humanVsHumanButton.setPosition(sf::Vector2f{ board_x_offset + 100, 420 });
	Button computerVsComputerButton(font);
	computerVsComputerButton.setSize(sf::Vector2f{ 250,50 });
	computerVsComputerButton.setString("Computer vs Computer");
	computerVsComputerButton.setPosition(sf::Vector2f{ board_x_offset + 100, 480 });
	font.loadFromFile("Assets/Roboto-Regular.ttf");
	window.setFramerateLimit(30);
	whites.setPosition(sf::Vector2f{ board_x_offset + 100, 150 });
	whites.setCharacterSize(20);
	whites.setString(whiteWinsString);
	whites.setFont(font);
	blacks.setPosition(sf::Vector2f{ board_x_offset + 100, 200 });
	blacks.setCharacterSize(20);
	blacks.setFont(font);
	blacks.setString(blackWinsString);
	draws.setPosition(sf::Vector2f{ board_x_offset + 100, 250 });
	draws.setCharacterSize(20);
	draws.setFont(font);
	draws.setString(drawsString);
	whitePlayerNote.setPosition(sf::Vector2f{ board_x_offset + 10, 700 });
	whitePlayerNote.setCharacterSize(20);
	whitePlayerNote.setFont(font);
	whitePlayerNote.setString("Player1");
	blackPlayerNote.setPosition(sf::Vector2f{ board_x_offset + 10, 50 });
	blackPlayerNote.setCharacterSize(20);
	blackPlayerNote.setFont(font);
	blackPlayerNote.setString("Player2");
	bool update = true, humanBlockerMoveRendered = false;
	std::thread gameThread(&IGame::StartGame, board->game.get());
	while (!board->game->isGameOn) {
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
				board->HandleMouseMove(e.mouseMove, window);
				break;
			case sf::Event::MouseButtonPressed:
				board->HandleMousePress(e.mouseButton, window);
				humanVsComputerButton.HandleMousePress(e.mouseButton, window);
				humanVsHumanButton.HandleMousePress(e.mouseButton, window);
				computerVsComputerButton.HandleMousePress(e.mouseButton, window);
				nextGameButton.HandleMousePress(e.mouseButton, window);
				break;
			case sf::Event::MouseButtonReleased:
				board->HandleMouseRealease(e.mouseButton, window);
				humanVsComputerButton.HandleMouseRealease(e.mouseButton, window);
				humanVsHumanButton.HandleMouseRealease(e.mouseButton, window);
				computerVsComputerButton.HandleMouseRealease(e.mouseButton, window);
				nextGameButton.HandleMouseRealease(e.mouseButton, window);
				if (humanVsComputerButton.Click()) {
					board->game->isGameOn = false;
					human->TryMove();
					human->TryMoveBlocker();
					if (gameThread.joinable())
						gameThread.join();
					board = std::make_unique<BoardView>(board->getPosition(), board->getSize(), human, computer, true);
					gameThread = std::thread(&IGame::StartGame, board->game.get());
				}
				else if (humanVsHumanButton.Click()) {
					board->game->isGameOn = false;
					human->TryMove();
					human->TryMoveBlocker();
					if (gameThread.joinable())
						gameThread.join();
					board = std::make_unique<BoardView>(board->getPosition(), board->getSize(), human, human, true);
					gameThread = std::thread(&IGame::StartGame, board->game.get());
				}
				else if (computerVsComputerButton.Click()) {
					board->game->isGameOn = false;
					human->TryMove();
					human->TryMoveBlocker();
					if (gameThread.joinable())
						gameThread.join();
					board = std::make_unique<BoardView>(board->getPosition(), board->getSize(), computer, computer, true);
					gameThread = std::thread(&IGame::StartGame, board->game.get());
				}
				else if (nextGameButton.Click()) {
					board->game->isGameOn = false;
					human->TryMove();
					human->TryMoveBlocker();
					if (gameThread.joinable())
						gameThread.join();
					auto rs = board->game->rs.winner;
					gameThread = std::thread(&IGame::RestartGame, board->game.get());
					while (!board->game->isGameOn) {
						std::this_thread::sleep_for(std::chrono::milliseconds(33));
					}
					if (round) {
						whitePlayerNote.setPosition(sf::Vector2f{ size.x * 2.f / 3 + 20, 50 });
						blackPlayerNote.setPosition(sf::Vector2f{ size.x * 2.f / 3 + 20, 700 });
					}
					else {
						whitePlayerNote.setPosition(sf::Vector2f{ size.x * 2.f / 3 + 20, 700 });
						blackPlayerNote.setPosition(sf::Vector2f{ size.x * 2.f / 3 + 20, 50 });
					}
					round = !round;
					whites.setString(whiteWinsString);
					blacks.setString(blackWinsString);
					draws.setString(drawsString);
					update = true;
				}
				break;
			case sf::Event::Resized:
				window.setSize(sf::Vector2u{ 1200, 800 });
				break;
			default:
				break;
			}
		}
		if (!update && !board->game->stateChanged) {
			if (human->blockerMove) {
				if (humanBlockerMoveRendered) {
					std::this_thread::sleep_for(std::chrono::milliseconds(33));
					continue;
				}
				else {
					humanBlockerMoveRendered = true;
				}
			}
			else {
				humanBlockerMoveRendered = false;
			}

		}
		board->Update();
		window.clear();
		//re-render
		window.draw(*board.get());
		window.draw(draws);
		window.draw(whites);
		window.draw(blacks);
		window.draw(whitePlayerNote);
		window.draw(blackPlayerNote);
		window.draw(humanVsComputerButton);
		window.draw(humanVsHumanButton);
		window.draw(computerVsComputerButton);
		window.draw(nextGameButton);
		//end re-render
		window.display();
		update = false;
		if (!board->game->isGameOn) {
			if (gameThread.joinable()) {
				gameThread.join();
				whiteWinsString = whiteWinsString.substr(0, 14) + std::to_string(board->game->player1);
				blackWinsString = blackWinsString.substr(0, 14) + std::to_string(board->game->player2);
				drawsString = drawsString.substr(0, 6) + std::to_string(board->game->draw);
				round = !round;
				whites.setString(whiteWinsString);
				blacks.setString(blackWinsString);
				draws.setString(drawsString);
				update = true;
			}
		}
	}
	board->game->isGameOn = false;
	human->TryMove();
	human->TryMoveBlocker();
	if (gameThread.joinable()) {
		gameThread.join();
		whiteWinsString = whiteWinsString.substr(0, 14) + std::to_string(board->game->player1);
		blackWinsString = blackWinsString.substr(0, 14) + std::to_string(board->game->player2);
		drawsString = drawsString.substr(0, 6) + std::to_string(board->game->draw);
		update = true;
	}

}

int main() {
	GameLoop();
}