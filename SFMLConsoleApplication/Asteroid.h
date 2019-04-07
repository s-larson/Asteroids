#ifndef INCLUDED_ASTEROID
#define INCLUDED_ASTEROID

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>

class Asteroid {

public:
	//Konstruktor. Tar emot mål för utritning (renderwindow), textur, position, hastighet och radie.
	Asteroid(sf::RenderWindow &renderWindow, sf::Texture &texture, sf::Vector2f position, 
		sf::Vector2f velocity, float radius);
	
	void draw();							//Ritar ut spriten
	void update(float deltaTime);			//Uppdaterar positionen
	void centerSprite();					//Centrera sprite utifrån storlek
	void setPosition(sf::Vector2f position);//Sätt position
	
	float getRadius();						//Läs storlek
	sf::Vector2f getVelocity();				//Läs hastighet
	sf::Vector2f getPosition();				//Läs position

private:
	sf::RenderWindow &mRenderWindow;		//Referensobjekt till spelfönstret
	sf::Sprite mSprite;						//Spriteobjekt
	sf::Vector2f mVelocity;					//Hastighet
	float mRadius;							//Storlek
};

#endif