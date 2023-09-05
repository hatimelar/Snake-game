#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics.hpp>
#include "GameMap.h"
#include "GameTile.h"
#include "Snake.h"
#include <SFML/Window/Keyboard.hpp>
#include <chrono>
#include <thread>
#include <queue>

void emptyQueue(std::queue<Direction> queue);

sf::RenderWindow window {sf::VideoMode(800, 600), "test"};

sf::Texture gameOverTexture;
sf::Sprite gameOverSprite;
sf::Font font;
int main()
{
	window.setFramerateLimit(10);

	gameOverTexture.loadFromFile("GameOver3.png");
	gameOverSprite.setTexture(gameOverTexture);
	sf::FloatRect gameOverBounds = gameOverSprite.getLocalBounds();
	gameOverSprite.setPosition((800.f - gameOverBounds.width) / 2.f, (600.f - gameOverBounds.height) / 2.f);

	font.loadFromFile("C:\\Windows\\Fonts\\impact.ttf");

	GameMap map{ 20.0f, 800, 600, "primaryTile.jpg", "secondaryTile.jpg", "circle2.png", "Wall3.jpg" };

	Snake snake = Snake(&map, "SnakeSolid.png", "SnakeSolid.png");
	map.addSnake(&snake);
	Direction snakeDirection = NONE;
	std::queue<Direction> inputQueue;
	bool isQueueFresh = false;
	// Game loop
	while (window.isOpen())
	{
		// Event polling
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed) {
				if (!isQueueFresh && !inputQueue.empty()) emptyQueue(inputQueue);
				switch (event.key.code) {
					case sf::Keyboard::Key::Left:
						inputQueue.push(LEFT);
						isQueueFresh = true;
						break;
					case sf::Keyboard::Key::Right:
						inputQueue.push(RIGHT);
						isQueueFresh = true;
						break;
					case sf::Keyboard::Key::Up:
						inputQueue.push(UP);
						isQueueFresh = true;
						break;
					case sf::Keyboard::Key::Down:
						inputQueue.push(DOWN);
						isQueueFresh = true;
				}
			}
		}
		isQueueFresh = false;
		if (!inputQueue.empty()) { 
			snakeDirection = inputQueue.front();
			inputQueue.pop();
		}
		window.clear();
		//Drawing
		map.m_snake->move(snakeDirection);
		map.drawTiles(&window);
		map.m_snake->drawSnake(&window);
		
		if (snake.m_dead) {

			sf::Text score{std::to_string(snake.m_length), font, 26};
			score.setFillColor(sf::Color::Black);
			sf::Vector2f gameOverPos = gameOverSprite.getPosition();
			score.setPosition(235.f + gameOverPos.x, 102.f + gameOverPos.y);
			window.draw(gameOverSprite);
			window.draw(score);
			window.display();
			while (true) {
				if (window.pollEvent(event)) {
					if (event.type == sf::Event::KeyPressed) break;
					if (event.type == sf::Event::Closed)
						window.close();
				}
			}
			snake.reset();
			map.relocateFeed();
			snakeDirection = NONE;
		}
		window.display();
	}
	return 0;

}
void emptyQueue(std::queue<Direction> queue) {
	while (!queue.empty()) {
		queue.pop();
	}
}
void reset();