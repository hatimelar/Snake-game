#include <SFML/Graphics.hpp>
#include "Direction.h"

#ifndef SNAKEBLOCK_h
#define SNAKEBLOCK_h

class SnakeBlock
{
	float m_dimension;
	sf::Texture* m_texture;
	
public:
	sf::Sprite m_sprite;
	sf::Vector2f m_pos;
	Direction m_currentDirection; // The current direction the block is headed in
	Direction m_prevDirection;
	SnakeBlock* m_next; // Pointer to the next SnakeBlock
	SnakeBlock* m_prev;

	SnakeBlock(sf::Vector2f pos, Direction currentDirection, sf::Texture* texture, SnakeBlock* prev, float dimension);

	// Updates the current and previous member direction variables when a new direction is given
	// and calls the method on the SnakeBlock pointed to by m_next if it is not null
	void updateDirection(Direction direction);

	// Draws the SnakeBlock
	void drawBlock(sf::RenderWindow* window);
	sf::Vector2f getPos() { return m_pos; };
	void move(sf::Vector2f pos, Direction direction);
	void rotateSprite();
};

#endif