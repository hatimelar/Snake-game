#include <iostream>
#include "Snake.h"
#include "SnakeBlock.h"
#include <SFML/Graphics.hpp>
#include "Direction.h"
#include "GameMap.h"


Snake::Snake(GameMap* gameMap, std::string headTexture, std::string bodyTexture) : m_grow{ false }, m_isMoving{ false }, m_dead{ false } {
	// Loading the texture file into the texture member variable
	if (!m_headTexture.loadFromFile(headTexture)) std::cout << "Error loading headTexture texture file\n";
	if (!m_bodyTexture.loadFromFile(bodyTexture)) std::cout << "Error loading headTexture texture file\n";
	m_gameMap = gameMap;
	m_dimension = (*gameMap).getTileDimension();
	m_head = new SnakeBlock(sf::Vector2f((19.f*m_dimension)+(m_dimension/2.f), (14.f * m_dimension) + (m_dimension / 2.f)), NONE, &m_headTexture, nullptr, m_dimension);
	m_length = 1;
	m_tail = m_head;
}

void Snake::reset() {
	m_grow = false;
	m_isMoving = false;
	m_dead = false;
	m_length = 1;
	
	SnakeBlock* temp =m_tail;
	SnakeBlock* prev;
	while (temp != m_head) {
		prev = temp->m_prev;
		delete temp;
		temp = prev;
	}
	m_head->m_next = nullptr;
	m_tail = m_head;

	m_head->m_pos = sf::Vector2f((19.f * m_dimension) + (m_dimension / 2.f), (14.f * m_dimension) + (m_dimension / 2.f));
	m_head->m_sprite.setPosition(m_head->m_pos);
	m_head->m_currentDirection = RIGHT;
	m_head->m_prevDirection = RIGHT;
}

// Draws the snake block by block by delegating the drawing to each block using the drawBlock method of the SnakeBlock class
void Snake::drawSnake(sf::RenderWindow* window) {
	SnakeBlock* temp = m_head;
	
	while (temp != nullptr) {
		(*temp).drawBlock(window);
		temp = temp->m_next;
	}
}

// Returns true if the snake can move in the given direction and false otherwise
bool Snake::canMove(Direction direction) {
	sf::Vector2f pos = (*m_head).getPos();
	switch (direction) {

	case LEFT:
		pos.x = pos.x - m_dimension;
		if (m_head->m_currentDirection == RIGHT && m_length !=1) {
			move(RIGHT);
			return false;
		}
		break;
	case RIGHT:
		pos.x = pos.x + m_dimension;
		if (m_head->m_currentDirection == LEFT && m_length != 1) {
			move(LEFT);
			return false;
		}
		break;
	case UP:
		pos.y = pos.y - m_dimension;
		if (m_head->m_currentDirection == DOWN && m_length != 1) {
			move(DOWN);
			return false;
		}
		break;
	case DOWN:
		pos.y = pos.y + m_dimension;
		if (m_head->m_currentDirection == UP && m_length != 1) {
			move(UP);
			return false;
		}
		break;

	default:
		return false;
	}
	if (intersectsSnake(pos)) {
		m_dead = true;
		return false;
	}
	return !(*m_gameMap).isWall(pos);
}

// Moves the snake in the given direction if possible
void Snake::move(Direction direction) {
	if (this->canMove(direction)) {
		sf::Vector2f pos;
		sf::Vector2f tailPos = m_tail->getPos(); //The tail position prior to moving the snake
		switch (direction) {
		case LEFT:
			pos = m_head->getPos();
			pos.x -= m_dimension;
			(*m_head).move(pos, direction);
			break;
		case RIGHT:
			pos = m_head->getPos();
			pos.x += m_dimension;
			(*m_head).move(pos, direction);
			break;
		case UP:
			pos = m_head->getPos();
			pos.y -= m_dimension;
			(*m_head).move(pos, direction);
			break;
		case DOWN:
			pos = m_head->getPos();
			pos.y += m_dimension;
			(*m_head).move(pos, direction);
			break;
		}
		// If the snake's head moved to a position containing feed in the previous move then the the snake will grow by calling grow()
		if (m_grow) grow(tailPos);
		if (m_gameMap->m_gameTiles[static_cast<int>(pos.y / m_dimension)][static_cast<int>(pos.x / m_dimension)]->m_containsFeed) {
			m_grow = true;
			m_gameMap->relocateFeed();
		}
	}
}

void Snake::grow(sf::Vector2f tailPos) {
	m_tail->m_next = new SnakeBlock(tailPos, RIGHT, &m_bodyTexture, m_tail, m_dimension);
	m_tail = m_tail->m_next;
	m_grow = false;
	m_length++;
}

// Returns true if the snake occupies that position and false otherwise
bool Snake::intersectsSnake(sf::Vector2f pos) {
	SnakeBlock *temp = m_head;
	while (temp != nullptr) { 
		sf::Vector2f tempPos = temp->getPos();
		if (tempPos.x == pos.x && tempPos.y == pos.y) return true;
		temp = temp->m_next;
	}
	return false;
}