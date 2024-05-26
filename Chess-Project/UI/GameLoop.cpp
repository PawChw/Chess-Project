#include "GameLoop.h"



void GameLoop(BoardView& board)
{
	sf::RenderWindow window(sf::VideoMode(800, 640), "Test");
	Textures textures;
	loadTextures(textures);
	const int squareSize = 80;
	window.setFramerateLimit(30);
	sf::Event e;
	std::array<sf::RectangleShape, 64> boardView;
	auto boardState = board.getBoardState();
	boardView.fill(sf::RectangleShape(sf::Vector2f(squareSize, squareSize)));
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			int index = i * 8 + j;
			boardView.at(index).setPosition(sf::Vector2f(j * squareSize, i * squareSize));
			if ((i+j) % 2 == 1) boardView.at(index).setFillColor(sf::Color(47, 47, 110));
			else boardView.at(index).setFillColor(sf::Color(255, 255, 255));
		}
	}
	sf::Clock clock;
	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			switch (e.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}
		boardState = board.getBoardState();
		window.clear();
		sf::RectangleShape piece(sf::Vector2f(squareSize, squareSize));
		// re-render logic start
		for (int i = 0; i < 64; i++) {
			window.draw(boardView.at(i));
			piece.setPosition(boardView.at(i).getPosition());
			switch (boardState.at(i).pieceType){
			case PieceType::Pawn:
				if (boardState.at(i).isWhite) {
					piece.setTexture(&textures.whitePawn);
				}
				else {
					piece.setTexture(&textures.blackPawn);
				}
				window.draw(piece);
				break;
			case PieceType::Knight:
				if (boardState.at(i).isWhite) {
					piece.setTexture(&textures.whiteKnight);
				}
				else {
					piece.setTexture(&textures.blackKnight);
				}
				window.draw(piece);
				break;
			case PieceType::Bishop:
				if (boardState.at(i).isWhite) {
					piece.setTexture(&textures.whiteBishop);
				}
				else {
					piece.setTexture(&textures.blackBishop);
				}
				window.draw(piece);
				break;
			case PieceType::Rook:
				if (boardState.at(i).isWhite) {
					piece.setTexture(&textures.whiteRook);
				}
				else {
					piece.setTexture(&textures.blackRook);
				}
				window.draw(piece);
				break;
			case PieceType::Queen:
				if (boardState.at(i).isWhite) {
					piece.setTexture(&textures.whiteQueen);
				}
				else {
					piece.setTexture(&textures.blackQueen);
				}
				window.draw(piece);
				break;
			case PieceType::King:
				if (boardState.at(i).isWhite) {
					piece.setTexture(&textures.whiteKing);
				}
				else {
					piece.setTexture(&textures.blackKing);
				}
				window.draw(piece);
				break;
			default:
				break;
		}
		}
		// re-render logic end
		window.display();
		std::cout << "FPS: " << 1000 / clock.restart().asMilliseconds() << std::endl;
	}
}
