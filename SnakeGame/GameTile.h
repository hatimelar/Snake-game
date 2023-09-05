#include <SFML/Graphics.hpp>

#ifndef GAMETILE_H
#define GAMETILE_H

class GameTile
{
	float m_dimension{};
	sf::Sprite m_sprite{};
	sf::Sprite m_feedSprite{};
	sf::Vector2f m_pos{};
	sf::Texture* m_tileTexture{};
	bool m_isWall{};


public:
	bool m_containsFeed{};
	// Constructor
	GameTile(float dimension, float xPos, float yPos, sf::Texture* tileTexture, bool isWall, bool containsFeed);
	void drawTile(sf::RenderWindow* window);
	bool isWall();
	void addFeed();
};





#endif

