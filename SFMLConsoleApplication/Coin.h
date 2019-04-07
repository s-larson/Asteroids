#ifndef INCLUDED_COIN
#define INCLUDED_COIN

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>

class Coin {

public:
	//Konstruktor. Tar emot mål för utritning (renderwindow), textur, position, hastighet och radie.
	Coin(sf::RenderWindow &renderWindow, sf::Texture &texture, sf::Vector2f position, 
		sf::Vector2f velocity, float radius);
	
	void draw();							//Ritar ut spriten för myntet
	void centerSprite();					//Centrera sprite utifrån storlek
	void update(float deltaTime);			//Uppdaterar positionen
	void setPosition(sf::Vector2f position);//Sätt position	
	
	float getRadius();						//Läs storlek
	sf::Vector2f getVelocity();				//Läs hastighet
	sf::Vector2f getPosition();				//Läs position

private:
	sf::RenderWindow &mRenderWindow;		//Referens till spelfönstret
	sf::Sprite mSprite;						//Spriteobjekt
	sf::Vector2f mVelocity;					//Hastighet
	float mRadius;							//Storlek
};

#endif