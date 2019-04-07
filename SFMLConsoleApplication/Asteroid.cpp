#include "Asteroid.h"
#include "SFML/Graphics.hpp"
#include <string>

using namespace std;
using namespace sf;

Asteroid::Asteroid(sf::RenderWindow &renderWindow, sf::Texture &texture, sf::Vector2f position, sf::Vector2f velocity, float radius) :
	mRenderWindow(renderWindow),
	mSprite(texture),
	mVelocity(velocity),
	mRadius(radius)
{
	centerSprite();
	setPosition(position);
}

void Asteroid::draw() {	
	mRenderWindow.draw(mSprite);
}

void Asteroid::update(float deltaTime) {
	mSprite.move(deltaTime*mVelocity);
}

void Asteroid::setPosition(Vector2f position) {
	mSprite.setPosition(position);
}

void Asteroid::centerSprite() {
	mSprite.setOrigin(Vector2f(mSprite.getTexture()->getSize().x * 0.5f, mSprite.getTexture()->getSize().y * 0.5f));
}

Vector2f Asteroid::getPosition() {
	return mSprite.getPosition();
}

Vector2f Asteroid::getVelocity() {
	return mVelocity;
}

float Asteroid::getRadius() {
	return mRadius;
}
