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
	sf::Vector2u m_size = window.getSize();
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
	std::unique_ptr<BoardView> m_board = std::make_unique<BoardView>(sf::Vector2f(0, 0), std::min(m_size.y * 1.f, m_size.x * 2.f / 3), human, computer, true);
	float board_x_offset = m_board->GetPosition().x + m_board->GetSize();
	Button nextGameButton(font);
	nextGameButton.SetSize(sf::Vector2f{ 250,50 });
	nextGameButton.setString("Start next game");
	nextGameButton.SetPosition(sf::Vector2f{ board_x_offset + 100, 300 });
	Button humanVsComputerButton(font);
	humanVsComputerButton.SetSize(sf::Vector2f{ 250,50 });
	humanVsComputerButton.setString("Human vs Computer");
	humanVsComputerButton.SetPosition(sf::Vector2f{ board_x_offset + 100, 360 });
	Button humanVsHumanButton(font);
	humanVsHumanButton.SetSize(sf::Vector2f{ 250,50 });
	humanVsHumanButton.setString("Human vs Human");
	humanVsHumanButton.SetPosition(sf::Vector2f{ board_x_offset + 100, 420 });
	Button computerVsComputerButton(font);
	computerVsComputerButton.SetSize(sf::Vector2f{ 250,50 });
	computerVsComputerButton.setString("Computer vs Computer");
	computerVsComputerButton.SetPosition(sf::Vector2f{ board_x_offset + 100, 480 });
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
	std::thread gameThread(&IGame::StartGame, m_board->game.get());
	while (!m_board->game->is_game_on) {
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
				m_board->HandleMouseMove(e.mouseMove, window);
				break;
			case sf::Event::MouseButtonPressed:
				m_board->HandleMousePress(e.mouseButton, window);
				humanVsComputerButton.HandleMousePress(e.mouseButton, window);
				humanVsHumanButton.HandleMousePress(e.mouseButton, window);
				computerVsComputerButton.HandleMousePress(e.mouseButton, window);
				nextGameButton.HandleMousePress(e.mouseButton, window);
				break;
			case sf::Event::MouseButtonReleased:
				m_board->HandleMouseRealease(e.mouseButton, window);
				humanVsComputerButton.HandleMouseRealease(e.mouseButton, window);
				humanVsHumanButton.HandleMouseRealease(e.mouseButton, window);
				computerVsComputerButton.HandleMouseRealease(e.mouseButton, window);
				nextGameButton.HandleMouseRealease(e.mouseButton, window);
				if (humanVsComputerButton.Click()) {
					m_board->game->is_game_on = false;
					human->TryMove();
					human->TryMoveBlocker();
					if (gameThread.joinable())
						gameThread.join();
					m_board = std::make_unique<BoardView>(m_board->GetPosition(), m_board->GetSize(), human, computer, true);
					gameThread = std::thread(&IGame::StartGame, m_board->game.get());
				}
				else if (humanVsHumanButton.Click()) {
					m_board->game->is_game_on = false;
					human->TryMove();
					human->TryMoveBlocker();
					if (gameThread.joinable())
						gameThread.join();
					m_board = std::make_unique<BoardView>(m_board->GetPosition(), m_board->GetSize(), human, human, true);
					gameThread = std::thread(&IGame::StartGame, m_board->game.get());
				}
				else if (computerVsComputerButton.Click()) {
					m_board->game->is_game_on = false;
					human->TryMove();
					human->TryMoveBlocker();
					if (gameThread.joinable())
						gameThread.join();
					m_board = std::make_unique<BoardView>(m_board->GetPosition(), m_board->GetSize(), computer, computer, true);
					gameThread = std::thread(&IGame::StartGame, m_board->game.get());
				}
				else if (nextGameButton.Click()) {
					m_board->game->is_game_on = false;
					human->TryMove();
					human->TryMoveBlocker();
					if (gameThread.joinable())
						gameThread.join();
					auto rs = m_board->game->rs.winner;
					gameThread = std::thread(&IGame::RestartGame, m_board->game.get());
					while (!m_board->game->is_game_on) {
						std::this_thread::sleep_for(std::chrono::milliseconds(33));
					}
					if (round) {
						whitePlayerNote.setPosition(sf::Vector2f{ m_size.x * 2.f / 3 + 20, 50 });
						blackPlayerNote.setPosition(sf::Vector2f{ m_size.x * 2.f / 3 + 20, 700 });
					}
					else {
						whitePlayerNote.setPosition(sf::Vector2f{ m_size.x * 2.f / 3 + 20, 700 });
						blackPlayerNote.setPosition(sf::Vector2f{ m_size.x * 2.f / 3 + 20, 50 });
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
		if (!update && !m_board->game->state_changed) {
			if (human->blocker_move) {
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
		m_board->Update();
		window.clear();
		//re-render
		window.draw(*m_board.get());
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
		if (!m_board->game->is_game_on) {
			if (gameThread.joinable()) {
				gameThread.join();
				whiteWinsString = whiteWinsString.substr(0, 14) + std::to_string(m_board->game->player1_score);
				blackWinsString = blackWinsString.substr(0, 14) + std::to_string(m_board->game->player2_score);
				drawsString = drawsString.substr(0, 6) + std::to_string(m_board->game->draw_score);
				round = !round;
				whites.setString(whiteWinsString);
				blacks.setString(blackWinsString);
				draws.setString(drawsString);
				update = true;
			}
		}
	}
	m_board->game->is_game_on = false;
	human->TryMove();
	human->TryMoveBlocker();
	if (gameThread.joinable()) {
		gameThread.join();
	}

}

int main() {
	GameLoop();
}