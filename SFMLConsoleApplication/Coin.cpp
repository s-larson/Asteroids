#include "Coin.h"
#include "SFML/Graphics.hpp"
#include <string>

using namespace std;
using namespace sf;

Coin::Coin(sf::RenderWindow &renderWindow, sf::Texture &texture, sf::Vector2f position, sf::Vector2f velocity, float radius) :
	mRadius(radius),
	mVelocity(velocity),
	mRenderWindow(renderWindow),
	mSprite(texture)
{
	centerSprite();
	setPosition(position);
}

void Coin::draw() {
	mRenderWindow.draw(mSprite);
}

void Coin::centerSprite() {
	mSprite.setOrigin(Vector2f(mSprite.getTexture()->getSize().x * 0.5f, mSprite.getTexture()->getSize().y * 0.5f));
}


void Coin::setPosition(Vector2f position) {
	mSprite.setPosition(position);
}


Vector2f Coin::getPosition(){ 
	return mSprite.getPosition();
}

Vector2f Coin::getVelocity() {
	return mVelocity;
}


void Coin::update(float timeDelta) {
	mSprite.move(timeDelta * mVelocity);
}

float Coin::getRadius() {
	return mRadius;
}