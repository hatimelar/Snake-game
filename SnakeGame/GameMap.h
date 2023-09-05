
#include <SFML/Graphics.hpp>
#include "GameTile.h"
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>


#ifndef GAMEMAP_h
#define GAMEMAP_h

class Snake;
class GameMap
{
private:
	
	float m_tileDimension{};
	int m_rows{};
	int m_cols{};

	sf::Texture m_tileTexture1;
	sf::Texture m_tileTexture2;
	sf::Texture m_feedTexture;
	sf::Texture m_wallTexture;

	int m_feedRow;
	int m_feedCol;
	float m_feedSpriteXOffset;
	float m_feedSpriteYOffset;
	sf::Sprite m_feedSprite;

	std::random_device m_rd;
	std::mt19937 m_rng;
	std::uniform_int_distribution<int> m_rowDistribution;
	std::uniform_int_distribution<int> m_colDistribution;

public:
	std::vector<std::vector<GameTile*>> m_gameTiles;
	GameMap(float tileDimension, int windowWidth, int windowHeight, std::string tileTexture1, std::string tileTexture2, std::string feedTexture, std::string wallTexture);
	float getTileDimension() { return m_tileDimension; };
	int getNumRows() { return m_rows; }
	int getNumCols() { return m_cols; }
	void drawTiles(sf::RenderWindow* window);
	bool isWall(sf::Vector2f pos);
	sf::Vector2f generateFeed();
	Snake* m_snake;
	void addSnake(Snake* snake);
	void relocateFeed();
	
};

#endif