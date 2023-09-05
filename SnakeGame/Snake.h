
#include "SnakeBlock.h"
#include "Direction.h"
#include "GameMap.h"
#ifndef SNAKE_H
#define SNAKE_H

class Snake {
	GameMap* m_gameMap;
	float m_dimension;
	
	bool m_isMoving;
	SnakeBlock* m_tail;
	sf::Texture m_headTexture;
	sf::Texture m_bodyTexture;
	bool m_grow;
	SnakeBlock* m_head;
	
	
public:
	int m_length;
	bool m_dead;
	Snake(GameMap* gameMap, std::string headTexture, std::string bodyTexture);
	void drawSnake(sf::RenderWindow* window);
	void move(Direction direction);
	bool canMove(Direction direction);
	void grow(sf::Vector2f tailPos);
	bool intersectsSnake(sf::Vector2f pos);
	void reset();
};


#endif