#include "GameTile.h"
#include <SFML/Graphics.hpp>
#include <iostream>

GameTile::GameTile(float dimension, float xPos, float yPos, sf::Texture* tileTexture, bool isWall, bool containsFeed) :
	m_dimension{ dimension }, m_pos{ sf::Vector2f(xPos, yPos) }, m_isWall{ isWall }, m_containsFeed{ containsFeed }, m_tileTexture{ tileTexture }
{
	// Setting the tile's Sprite with the texture provided
	m_sprite.setTexture(*m_tileTexture);
	// Scalling the sprite so it has the dimension of the tile
	m_sprite.setScale(m_dimension / (*m_tileTexture).getSize().x, m_dimension / (*m_tileTexture).getSize().y);
	m_sprite.setPosition(m_pos);
}
// Draws the tile on the given window reference
void GameTile::drawTile(sf::RenderWindow* window) {
	(*window).draw(m_sprite);
}

bool GameTile::isWall() {
	return m_isWall;
}

void GameTile::addFeed() {
	m_containsFeed = true;
}