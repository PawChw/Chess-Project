#include "GameLoop.h"

void GameLoop(BoardView& board)
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Test");
	window.setFramerateLimit(30);
	sf::Event e;

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
		window.clear();
		// re-render logic start

		// re-render logic end
		window.display();
		std::cout << "FPS: " << 1000 / clock.restart().asMilliseconds() << std::endl;
	}
}
