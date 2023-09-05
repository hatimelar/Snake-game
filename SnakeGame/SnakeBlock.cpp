#include <SFML/Graphics.hpp>
#include "Direction.h"
#include "SnakeBlock.h"
#include <iostream>

// The pos member variable refers to the center of the snake block
SnakeBlock::SnakeBlock(sf::Vector2f pos, Direction currentDirection, sf::Texture* texture, SnakeBlock* prev, float dimension) :
	m_pos{ pos }, m_currentDirection{ currentDirection }, m_prevDirection{ currentDirection }, m_prev{ prev }, m_next{ nullptr },
	m_dimension{ dimension }, m_texture{ texture }
{
	// Setting the blocks's Sprite with the texture provided
	m_sprite.setTexture(*m_texture);

	// Scalling the sprite so it has the dimension of the block
	m_sprite.setScale(dimension / (*m_texture).getSize().x, dimension / (*m_texture).getSize().y);
	
	// Setting the center of the sprite as the origin
	sf::FloatRect spriteRect = m_sprite.getLocalBounds();
	m_sprite.setOrigin(spriteRect.width / 2.f, spriteRect.height / 2.f);
	m_sprite.setPosition(m_pos);
}

void SnakeBlock::updateDirection(Direction direction) {
	m_prevDirection = m_currentDirection;
	m_currentDirection = direction;
	if (m_next != nullptr) (*m_next).updateDirection(m_prevDirection);
}

// Draws the snakeblock on the window
void SnakeBlock::drawBlock(sf::RenderWindow* window) {
	(*window).draw(m_sprite);
}

// Moves the snake block to the specified position
void SnakeBlock::move(sf::Vector2f pos, Direction direction) {
	sf::Vector2f lastPos = m_pos;
	m_pos = pos;
	
	m_prevDirection = m_currentDirection;
	m_currentDirection = direction;
	//rotateSprite();
	m_sprite.setPosition(m_pos);
	
	// Calls the method on the snake block refrenced to by the next memeber variable
	if (m_next != nullptr) (*m_next).move(lastPos, m_prevDirection);
}

// Rotates the sprite according to the direction of the snake block
void SnakeBlock::rotateSprite() {
	switch (m_currentDirection) {
		case (RIGHT) :
			m_sprite.setRotation(0.f);
			break;
		case (LEFT) : 
			m_sprite.setRotation(180.f);
			break;
		case (UP) :
			m_sprite.setRotation(270.f);
			break;
		case (DOWN) :
			m_sprite.setRotation(90.f);
	}
}