#include "GameMap.h"
#include "GameTile.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include "Snake.h"


GameMap::GameMap(float tileDimension, int windowWidth, int windowHeight, std::string tileTexture1, std::string tileTexture2, std::string feedTexture, std::string wallTexture) :
	m_tileDimension{ tileDimension }, m_cols{ windowWidth / static_cast<int>(tileDimension) }, m_rows{ windowHeight / static_cast<int>(tileDimension) },
	m_rd{}, m_rng{ m_rd() }, m_rowDistribution{ 1, m_rows - 2 }, m_colDistribution{ 1, m_cols - 2 }
{
	if (!m_tileTexture1.loadFromFile(tileTexture1)) std::cout << "Error loading tileTexture1 file\n";
	if (!m_tileTexture2.loadFromFile(tileTexture2)) std::cout << "Error loading tileTexture2 file\n";
	if (!m_wallTexture.loadFromFile(wallTexture)) std::cout << "Error loading wallTexture file\n";
	if (!m_feedTexture.loadFromFile(feedTexture)) std::cout << "Error loading feedTexture file\n";
	bool primaryTile; // Specifies if the next tile should be colored with the primary color instead of the secondary one
	for (int i = 0; i < m_rows; i++) {
		primaryTile = (i % 2 == 0);
		m_gameTiles.push_back(std::vector<GameTile*>());
		for (int j = 0; j < m_cols; j++) {
			//if the tile is a wall
			if (i == 0 || j == 0 || i == m_rows - 1 || j == m_cols - 1)
				m_gameTiles[i].push_back(new GameTile(tileDimension, static_cast<float> (j) * tileDimension, static_cast<float>(i) * tileDimension, &m_wallTexture, true, false));
			else {
				if (primaryTile)
					m_gameTiles[i].push_back(new GameTile(tileDimension, static_cast<float>(j) * tileDimension, static_cast<float>(i) * tileDimension, &m_tileTexture1, false, false));
				else
					m_gameTiles[i].push_back(new GameTile(tileDimension, static_cast<float>(j) * tileDimension, static_cast<float>(i) * tileDimension, &m_tileTexture2, false, false));
			}
			primaryTile = !primaryTile;
		}
	}
	m_feedTexture.setSmooth(true);
	// Setting the feed's Sprite with the texture provided
	m_feedSprite.setTexture(m_feedTexture);
	// Scalling the sprite so it has the right dimension
	float xScale = m_tileDimension / (m_feedTexture.getSize().x*2.0f);
	float yScale = m_tileDimension / (m_feedTexture.getSize().y *2.0f);
	m_feedSprite.setScale(xScale, yScale);

	sf::Vector2u feedTextureSize = m_feedTexture.getSize();
	sf::Vector2f feedPosition = generateFeed();
	m_feedSpriteXOffset= (m_tileDimension - (xScale*static_cast<float>(feedTextureSize.x)))/2.0f;
	m_feedSpriteYOffset = (m_tileDimension - (yScale * static_cast<float>(feedTextureSize.y))) / 2.0f;
	m_feedSprite.setPosition(feedPosition.y+ m_feedSpriteXOffset, feedPosition.x+ m_feedSpriteYOffset);
}
void GameMap::drawTiles(sf::RenderWindow* window) {
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_cols; j++) {
			(*m_gameTiles[i][j]).drawTile(window);
		}
	}
	window->draw(m_feedSprite);
}

void GameMap::addSnake(Snake* snake) {
	m_snake = snake;
}

bool GameMap::isWall(sf::Vector2f pos) {
	int x = static_cast<int>(pos.x / m_tileDimension);
	int y = static_cast<int>(pos.y / m_tileDimension);
	return (*m_gameTiles[y][x]).isWall();
}
// Generates a random location for the feed and changes the tile's state to reflect the feed that was spawned
sf::Vector2f GameMap::generateFeed() {
	int row = m_rowDistribution(m_rng);
	int col = m_colDistribution(m_rng);
	if (m_snake != nullptr) {
		while ((*m_snake).intersectsSnake(sf::Vector2f((static_cast<float>(col) * m_tileDimension) + (m_tileDimension / 2.f), (static_cast<float>(row) * m_tileDimension) + (m_tileDimension / 2.f)))) {
			row = m_rowDistribution(m_rng);
			col = m_colDistribution(m_rng);
		}
	}
	(*m_gameTiles[row][col]).m_containsFeed = true;
	m_feedRow = row;
	m_feedCol = col;
	return sf::Vector2f(static_cast<float>(row) * m_tileDimension, static_cast<float>(col) * m_tileDimension);
}
// Relocates the feed by generating a random new position for it and moving the sprite to that location
void GameMap::relocateFeed() {
	m_gameTiles[m_feedRow][m_feedCol]->m_containsFeed = false;
	sf::Vector2f feedPosition = generateFeed();
	m_feedSprite.setPosition(feedPosition.y + m_feedSpriteXOffset, feedPosition.x + m_feedSpriteYOffset);
}