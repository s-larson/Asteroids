#ifndef INCLUDED_ASTEROID
#define INCLUDED_ASTEROID

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>

class Asteroid {

public:
	//Konstruktor. Tar emot m�l f�r utritning (renderwindow), textur, position, hastighet och radie.
	Asteroid(sf::RenderWindow &renderWindow, sf::Texture &texture, sf::Vector2f position, 
		sf::Vector2f velocity, float radius);
	
	void draw();							//Ritar ut spriten
	void update(float deltaTime);			//Uppdaterar positionen
	void centerSprite();					//Centrera sprite utifr�n storlek
	void setPosition(sf::Vector2f position);//S�tt position
	
	float getRadius();						//L�s storlek
	sf::Vector2f getVelocity();				//L�s hastighet
	sf::Vector2f getPosition();				//L�s position

private:
	sf::RenderWindow &mRenderWindow;		//Referensobjekt till spelf�nstret
	sf::Sprite mSprite;						//Spriteobjekt
	sf::Vector2f mVelocity;					//Hastighet
	float mRadius;							//Storlek
};

#endif