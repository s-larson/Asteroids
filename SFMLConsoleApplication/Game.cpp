#include "SFML/Graphics.hpp"
#include <string>
#include "Game.h"
#include <vector>
#include <cstdlib>

using namespace sf;
using namespace std;

namespace { 

	//Inställningar för fönstret
	const string windowTitle = "Asteroids"; //Titel på spelfönstret
	const VideoMode videoMode = VideoMode(768, 1024); //Storlek på spelfönstret
	const Color backgroundColor = Color::Black; //Bakgrundsfärg för spelfönstret
	const unsigned int FRAMERATE_LIMIT = 60; //Högst antal uppdateringar per sekund
	const int START_LEVEL = 1;


	//Inställningar för mynt
	const float COIN_RADIUS = 16.0f;
	const float COIN_VELOCITY = 80.0f;

	//Inställningar för skepp
	const float SHIP_RADIUS = 20.0f;
	const float SHIP_VELOCITY = 400.0f;
	const Vector2f SHIP_SPAWN_POINT = Vector2f(videoMode.width / 2, videoMode.height*0.75);
	
	//Inställningar för asteroider
	const float ASTEROID_RADIUS = 32.0f;
	const float ASTEROID_SPAWN_DELTA = 1.0f;
	const float ASTEROID_SPAWN_COUNT_BASE = 1.0f;
	const float ASTEROID_SPAWN_COUNT_INCREMENT = 0.5f;
	const float ASTEROID_MIN_VELOCITY = 80.0f;
	const float ASTEROID_DELTA_VELOCITY = 200.0f;
}


Game::Game() : //Konstruktor för objekt, fönster och texturer

	mRenderWindow(videoMode, windowTitle, Style::Titlebar | Style::Close),
	mCoinTexture(loadTexture("CoinSprite.psd")),
	mShipTexture(loadTexture("ShipSprite.psd")),
	mAsteroidTexture(loadTexture("AsteroidSprite.psd")),
	mAsteroidSpawnClock(),
	mCoin(0), //Skapar pekarobjekt (pekar på ingenting, tills objekten skapas)
	mShip(0),
	mAsteroids(),
	mLevel(START_LEVEL), //Initiera nivå ett
	mGameOver(false){
	createCoin();
	createShip();
	mRenderWindow.setFramerateLimit(FRAMERATE_LIMIT); //Uppdateringsfrekvens för fönstret
}

void Game::run() { 

	Clock frameClock; //SFML-klocka. Håller reda på hur lång en "spelloop" är
	
	while (mRenderWindow.isOpen() && !mGameOver) { //Huvudloopen
		float deltaTime = frameClock.restart().asSeconds();
		clearWindow();
		updateCoin(deltaTime);
		updateShip(deltaTime);
		updateAsteroids(deltaTime);
		handleCoinPickup();
		handleAsteroidCollisions();
		handleLostCoin();
		handleWindowEvents();
		pruneAsteroids();
		createAsteroids();
		drawCoin();
		drawShip();
		drawAsteroids();
		displayWindow();
	}
}

/////////////// WINDOW ///////////////////

void Game::clearWindow() { //Rensar fönstret
	mRenderWindow.clear(backgroundColor);
}

void Game::displayWindow() { //Ritar ut innehållet på skärmen. Stänger mRenderWindow om fönstret stängs
	mRenderWindow.display();
}

int Game::getWindowHeight() { // Hämtar fönstrets storlek i Y-led
	int WindowSize = mRenderWindow.getSize().y;
	return WindowSize;
}

int Game::getWindowWidth() { // Hämtar fönstrets storlek i Y-led
	int WindowSize = mRenderWindow.getSize().x;
	return WindowSize;
}

void Game::handleWindowEvents() { //Stänger fönstret om användaren kryssar rutan
	Event event;
	while (mRenderWindow.pollEvent(event)) {
		if (event.type == Event::Closed) {
			mRenderWindow.close();
		}
	}
}

/////////////////// HJÄLPFUNKTIONER ///////////////////

Texture Game::loadTexture(string texture) { //Läser in texturer till objekten

	Texture text;

	if (!text.loadFromFile(texture)) {
		cout << "error";
	}
	return text;
}

Vector2f Game::getRandomCoinPosition() { //Slumpar fram en X och Y position för myntet.
	Vector2f position;
	position.x = rand() % videoMode.width - COIN_RADIUS + COIN_RADIUS;
	position.y = 0;
	return position;
}

Vector2f Game::getRandomAsteroidPosition() { //Slumpar fram en X och Y kordinat för varje asteroid
	Vector2f position;
	position.x = rand() % videoMode.width + ASTEROID_RADIUS; 
	position.y = 0 - ASTEROID_RADIUS;
	return position;
}

Vector2f Game::randomAsteroidVelocity() { //Slumpar fram hastigheten för asteroiderna
	Vector2f velocity;
	int maxV = ASTEROID_DELTA_VELOCITY; //Omvandlar float till int för att rand() funktionen ska kunna användas
	int minV = ASTEROID_MIN_VELOCITY;
	velocity.x = 0;
	velocity.y = rand() % maxV + minV;
	return velocity;
}

/////////////// COIN ///////////////////

Vector2f Game::getCoinVelocity() { // Returnerar myntets hastighet som sätts i inställningar högst upp i Game.cpp. X-hastighet sätts till noll.
	return Vector2f(0, COIN_VELOCITY); 
}

void Game::createCoin() { //Skapar myntet och sätter positionen till det slumpmässiga från getRandomCoinPosition.
	mCoin = new Coin(mRenderWindow, mCoinTexture, getRandomCoinPosition(), getCoinVelocity(), COIN_RADIUS);
}

void Game::destroyCoin() {
	delete mCoin;
}

void Game::drawCoin() {
	mCoin->draw();
}

void Game::updateCoin(float deltaTime) { //Uppdaterar myntets position enligt hastighet & deltatid
	mCoin->update(deltaTime);
}

void Game::handleLostCoin() { //Sätter gameover om myntet åker nedanför skärmkanten
	if (COIN_RADIUS + getWindowHeight() < mCoin->getPosition().y) {
		mGameOver = true;
	}
}

/////////////// SHIP ///////////////////

void Game::createShip() {
	mShip = new Ship(mRenderWindow, mShipTexture, SHIP_SPAWN_POINT, getShipVelocity(), SHIP_RADIUS);
}

Vector2f Game::getShipVelocity() {
	return Vector2f(SHIP_VELOCITY, SHIP_VELOCITY);
}

void Game::drawShip() {
	mShip->draw();
}

void Game::updateShip(float deltaTime) {
	mShip->update(deltaTime);
}

void Game::destroyShip() {
	delete mShip;
}

void Game::constrainPosition() {
	mShip->constrainPosition();
}

/////////////// ASTEROIDS ///////////////////

void Game::createAsteroids() { //Skapar vektorn med asteroider
	if (ASTEROID_SPAWN_DELTA < mAsteroidSpawnClock.getElapsedTime().asSeconds()) {
		int spawnCount = int(ASTEROID_SPAWN_COUNT_BASE + mLevel * ASTEROID_SPAWN_COUNT_INCREMENT);
		for (int i = 0; i < spawnCount; i++) {
			Asteroid *asteroidEntity = new Asteroid(mRenderWindow, mAsteroidTexture, getRandomAsteroidPosition(), randomAsteroidVelocity(), ASTEROID_RADIUS);
			mAsteroids.push_back(asteroidEntity);
		}
		mAsteroidSpawnClock.restart();
	}
}


void Game::drawAsteroids() { //Ritar ut alla asteroider
	for (AsteroidVector::size_type i = 0; i < mAsteroids.size(); i++) {
		mAsteroids[i]->draw();
	}
}

void Game::updateAsteroids(float deltaTime){ //Uppdaterar positionen för hela vektorn med asteroider
	for (AsteroidVector::size_type i = 0; i < mAsteroids.size(); i++) {
		mAsteroids[i]->update(deltaTime);
	}
}

void Game::pruneAsteroids() { //Tar bort asteroider när de faller under fönstrets kant
	float maxY = getWindowHeight() + ASTEROID_RADIUS;
	AsteroidVector remainingAsteroids;

	for (AsteroidVector::size_type i = 0; i < mAsteroids.size(); i++) {
	
		Asteroid *asteroid = mAsteroids[i];

		if (asteroid->getPosition().y < maxY) {
				remainingAsteroids.push_back(asteroid);
		}

		else {
				delete asteroid;
		}
	}
		mAsteroids = remainingAsteroids;
}

/////////////// GAME FUNCTIONS ///////////////////

void Game::handleCoinPickup() {
	if (overlap(mShip, mCoin)) {
		mLevel++;
		destroyCoin();
		createCoin();
	}
}

void Game::handleAsteroidCollisions() {
	for (AsteroidVector::size_type i = 0; i < mAsteroids.size(); i++) {
		if (overlap(mShip, mAsteroids[i])) {
			mGameOver = true;
		}
	}
}

bool Game::overlap(sf::Vector2f position0, float radius0, sf::Vector2f position1, float radius1) { //formeln för hit-detection
	float deltaX = position0.x - position1.x;
	float deltaY = position0.y - position1.y;
	float radiusSum = radius0 + radius1;
	return deltaX * deltaX + deltaY * deltaY < radiusSum* radiusSum;
}

bool Game::overlap(Ship *ship, Coin *coin) { //Hit detection för skepp och mynt
	Vector2f shipPosition = ship->getPosition();
	float shipRadius = ship->getRadius();
	Vector2f coinPosition = coin->getPosition();
	float coinRadius = coin->getRadius();
	return overlap(shipPosition, shipRadius, coinPosition, coinRadius);
}

bool Game::overlap(Ship *ship, Asteroid *asteroid) { //Hit detection för skepp och asteroid
	Vector2f shipPosition = ship->getPosition();
	float shipRadius = ship->getRadius();
	Vector2f asteroidPosition = asteroid->getPosition();
	float asteroidRadius = asteroid->getRadius();
	return overlap(shipPosition, shipRadius, asteroidPosition, asteroidRadius);
}