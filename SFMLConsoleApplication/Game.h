#ifndef INCLUDED_GAME
#define INCLUDED_GAME

#include <SFML/Graphics.hpp>
#include "Coin.h"
#include "Ship.h"
#include "Asteroid.h"


class Game {

public:
	Game(); //Konstruktor för objekt, samt fönster och texturer
	//~Game(); //Destruktor för skepp, asteroider och mynt
	void run(); //Huvudloop

private:	
	//Fönsterfunktioner
	void displayWindow();
	void clearWindow();
	void handleWindowEvents();
	sf::RenderWindow mRenderWindow;

	//Hjälpfunktioner
	int getWindowHeight();
	int getWindowWidth();
	sf::Texture loadTexture(std::string texture);
	sf::Vector2f getRandomCoinPosition();
	sf::Vector2f getRandomAsteroidPosition();
	sf::Vector2f randomAsteroidVelocity();

	//Texturobjekt
	sf::Texture mCoinTexture;
	sf::Texture mShipTexture;
	sf::Texture mAsteroidTexture;
	
	//Skapar/tar bort/ritar ut objekten
	void createCoin();
	void destroyCoin();
	void drawCoin();
	void updateCoin(float deltaTime);
	sf::Vector2f getCoinVelocity();

	void createShip();
	void destroyShip();
	void drawShip();
	void updateShip(float deltaTime);
	void constrainPosition();
	sf::Vector2f getShipVelocity();
	
	void createAsteroids();
	void drawAsteroids();
	void updateAsteroids(float deltaTime);
	sf::Clock mAsteroidSpawnClock; //Klocka som mäter intervallet mellan varje ny grupp asteroider
	void pruneAsteroids();
	
	//Pekarobjekt till spelobjekten
	Coin *mCoin;
	Ship *mShip;
	typedef std::vector<Asteroid*> AsteroidVector;
	AsteroidVector mAsteroids; //Vektor till alla asteroider

	//Spelregler
	bool mGameOver;
	int mLevel;
	void handleLostCoin();
	void handleCoinPickup();
	void handleAsteroidCollisions();
	bool overlap(sf::Vector2f position0, float radius0, sf::Vector2f position1, float radius1);
	bool overlap(Ship *ship, Coin *coin);
	bool overlap(Ship *ship, Asteroid *asteroid);
	
};

#endif