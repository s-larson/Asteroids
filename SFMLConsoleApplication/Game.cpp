#include "SFML/Graphics.hpp"
#include <string>
#include "Game.h"
#include <vector>
#include <cstdlib>

using namespace sf;
using namespace std;

namespace { 

	//Inst�llningar f�r f�nstret
	const string windowTitle = "Asteroids"; //Titel p� spelf�nstret
	const VideoMode videoMode = VideoMode(768, 1024); //Storlek p� spelf�nstret
	const Color backgroundColor = Color::Black; //Bakgrundsf�rg f�r spelf�nstret
	const unsigned int FRAMERATE_LIMIT = 60; //H�gst antal uppdateringar per sekund
	const int START_LEVEL = 1;


	//Inst�llningar f�r mynt
	const float COIN_RADIUS = 16.0f;
	const float COIN_VELOCITY = 80.0f;

	//Inst�llningar f�r skepp
	const float SHIP_RADIUS = 20.0f;
	const float SHIP_VELOCITY = 400.0f;
	const Vector2f SHIP_SPAWN_POINT = Vector2f(videoMode.width / 2, videoMode.height*0.75);
	
	//Inst�llningar f�r asteroider
	const float ASTEROID_RADIUS = 32.0f;
	const float ASTEROID_SPAWN_DELTA = 1.0f;
	const float ASTEROID_SPAWN_COUNT_BASE = 1.0f;
	const float ASTEROID_SPAWN_COUNT_INCREMENT = 0.5f;
	const float ASTEROID_MIN_VELOCITY = 80.0f;
	const float ASTEROID_DELTA_VELOCITY = 200.0f;
}


Game::Game() : //Konstruktor f�r objekt, f�nster och texturer

	mRenderWindow(videoMode, windowTitle, Style::Titlebar | Style::Close),
	mCoinTexture(loadTexture("CoinSprite.psd")),
	mShipTexture(loadTexture("ShipSprite.psd")),
	mAsteroidTexture(loadTexture("AsteroidSprite.psd")),
	mAsteroidSpawnClock(),
	mCoin(0), //Skapar pekarobjekt (pekar p� ingenting, tills objekten skapas)
	mShip(0),
	mAsteroids(),
	mLevel(START_LEVEL), //Initiera niv� ett
	mGameOver(false){
	createCoin();
	createShip();
	mRenderWindow.setFramerateLimit(FRAMERATE_LIMIT); //Uppdateringsfrekvens f�r f�nstret
}

void Game::run() { 

	Clock frameClock; //SFML-klocka. H�ller reda p� hur l�ng en "spelloop" �r
	
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

void Game::clearWindow() { //Rensar f�nstret
	mRenderWindow.clear(backgroundColor);
}

void Game::displayWindow() { //Ritar ut inneh�llet p� sk�rmen. St�nger mRenderWindow om f�nstret st�ngs
	mRenderWindow.display();
}

int Game::getWindowHeight() { // H�mtar f�nstrets storlek i Y-led
	int WindowSize = mRenderWindow.getSize().y;
	return WindowSize;
}

int Game::getWindowWidth() { // H�mtar f�nstrets storlek i Y-led
	int WindowSize = mRenderWindow.getSize().x;
	return WindowSize;
}

void Game::handleWindowEvents() { //St�nger f�nstret om anv�ndaren kryssar rutan
	Event event;
	while (mRenderWindow.pollEvent(event)) {
		if (event.type == Event::Closed) {
			mRenderWindow.close();
		}
	}
}

/////////////////// HJ�LPFUNKTIONER ///////////////////

Texture Game::loadTexture(string texture) { //L�ser in texturer till objekten

	Texture text;

	if (!text.loadFromFile(texture)) {
		cout << "error";
	}
	return text;
}

Vector2f Game::getRandomCoinPosition() { //Slumpar fram en X och Y position f�r myntet.
	Vector2f position;
	position.x = rand() % videoMode.width - COIN_RADIUS + COIN_RADIUS;
	position.y = 0;
	return position;
}

Vector2f Game::getRandomAsteroidPosition() { //Slumpar fram en X och Y kordinat f�r varje asteroid
	Vector2f position;
	position.x = rand() % videoMode.width + ASTEROID_RADIUS; 
	position.y = 0 - ASTEROID_RADIUS;
	return position;
}

Vector2f Game::randomAsteroidVelocity() { //Slumpar fram hastigheten f�r asteroiderna
	Vector2f velocity;
	int maxV = ASTEROID_DELTA_VELOCITY; //Omvandlar float till int f�r att rand() funktionen ska kunna anv�ndas
	int minV = ASTEROID_MIN_VELOCITY;
	velocity.x = 0;
	velocity.y = rand() % maxV + minV;
	return velocity;
}

/////////////// COIN ///////////////////

Vector2f Game::getCoinVelocity() { // Returnerar myntets hastighet som s�tts i inst�llningar h�gst upp i Game.cpp. X-hastighet s�tts till noll.
	return Vector2f(0, COIN_VELOCITY); 
}

void Game::createCoin() { //Skapar myntet och s�tter positionen till det slumpm�ssiga fr�n getRandomCoinPosition.
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

void Game::handleLostCoin() { //S�tter gameover om myntet �ker nedanf�r sk�rmkanten
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

void Game::updateAsteroids(float deltaTime){ //Uppdaterar positionen f�r hela vektorn med asteroider
	for (AsteroidVector::size_type i = 0; i < mAsteroids.size(); i++) {
		mAsteroids[i]->update(deltaTime);
	}
}

void Game::pruneAsteroids() { //Tar bort asteroider n�r de faller under f�nstrets kant
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

bool Game::overlap(sf::Vector2f position0, float radius0, sf::Vector2f position1, float radius1) { //formeln f�r hit-detection
	float deltaX = position0.x - position1.x;
	float deltaY = position0.y - position1.y;
	float radiusSum = radius0 + radius1;
	return deltaX * deltaX + deltaY * deltaY < radiusSum* radiusSum;
}

bool Game::overlap(Ship *ship, Coin *coin) { //Hit detection f�r skepp och mynt
	Vector2f shipPosition = ship->getPosition();
	float shipRadius = ship->getRadius();
	Vector2f coinPosition = coin->getPosition();
	float coinRadius = coin->getRadius();
	return overlap(shipPosition, shipRadius, coinPosition, coinRadius);
}

bool Game::overlap(Ship *ship, Asteroid *asteroid) { //Hit detection f�r skepp och asteroid
	Vector2f shipPosition = ship->getPosition();
	float shipRadius = ship->getRadius();
	Vector2f asteroidPosition = asteroid->getPosition();
	float asteroidRadius = asteroid->getRadius();
	return overlap(shipPosition, shipRadius, asteroidPosition, asteroidRadius);
}