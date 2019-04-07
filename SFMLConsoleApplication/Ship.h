#ifndef INCLUDED_SHIP
#define INCLUDED_SHIP

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>

class Ship {

public:
	//Konstruktor. Tar emot m�l f�r utritning (renderwindow), textur, position, hastighet och radie.
	Ship(sf::RenderWindow &renderWindow, sf::Texture &texture, sf::Vector2f position, 
		sf::Vector2f velocity, float radius);
	
	void draw();							//Ritar ut spriten f�r myntet
	void update(float deltaTime);			//Uppdaterar positionen
	void centerSprite();					//Centrera sprite utifr�n storlek
	void updatePosition(float deltaTime);	//Skeppets styrning
	void setPosition(sf::Vector2f position);//S�tt position
	void constrainPosition();				//Begr�nsar skeppet till f�nstrets kanter
	
	float getRadius();						//L�s storlek
	sf::Vector2f getVelocity();				//L�s hastighet
	sf::Vector2f getPosition();				//L�s position

	
private:
	sf::RenderWindow &mRenderWindow;		//Referens till spelf�nstret
	sf::Sprite mSprite;						//Spriteobjekt
	sf::Vector2f mVelocity;					//Hastighet
	float mRadius;							//Storlek
};

#endif