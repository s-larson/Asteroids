#include "Ship.h"
#include "SFML/Graphics.hpp"
#include <string>

using namespace std;
using namespace sf;


//Template för att multiplicera vectorer (behövs i updatePosition())
template <typename T>
Vector2<T> operator *(const Vector2<T>& left, const Vector2<T>& right)
{
	T X = left.x * right.x;
	T Y = left.y * right.y;
	return Vector2<T>(X, Y);
}
// Konverterar int till float
Vector2f toFloat(Vector2u ints) {
	Vector2f floats;
	floats.x = static_cast <float> (ints.x);
	floats.y = static_cast <float> (ints.y);
	return floats;
}


Ship::Ship(sf::RenderWindow &renderWindow, sf::Texture &texture, sf::Vector2f position, sf::Vector2f velocity, float radius) :
	mRadius(radius),
	mVelocity(velocity),
	mRenderWindow(renderWindow),
	mSprite(texture)
{
	centerSprite();
	setPosition(position);
}

void Ship::draw() { 
	mRenderWindow.draw(mSprite);
}

void Ship::setPosition(Vector2f position) { 
	mSprite.setPosition(position);
}


Vector2f Ship::getPosition() { 
	return mSprite.getPosition();
}

Vector2f Ship::getVelocity() { 
	return mVelocity;
}


void Ship::updatePosition(float timeDelta) {
	float directionX = 0.0f;
	float directionY = 0.0f;

	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		directionX -= 1.0f;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		directionX += 1.0f;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		directionY -= 1.0f;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down)) {
		directionY += 1.0f;
	}
	Vector2f direction(directionX, directionY);
	mSprite.move(timeDelta * mVelocity * direction);
}

void Ship::constrainPosition() {
	Vector2f windowSize = toFloat(mRenderWindow.getSize());
	float minX = mRadius;
	float minY = mRadius;
	float maxX = windowSize.x - mRadius;
	float maxY = windowSize.y - mRadius;
	float shipX = mSprite.getPosition().x;
	float shipY = mSprite.getPosition().y;
	if (shipX < minX) {
		shipX = minX;
	}
	else if (maxX < shipX) {
		shipX = maxX;
	}
	if (shipY < minY) {
		shipY = minY;
	}
	else if (maxY < shipY) {
		shipY = maxY;
	}
	Vector2f shipPosition(shipX, shipY);
	mSprite.setPosition(shipPosition);
}

void Ship::centerSprite() {
	mSprite.setOrigin(Vector2f(mSprite.getTexture()->getSize().x * 0.5f, mSprite.getTexture()->getSize().y * 0.5f));
}


float Ship::getRadius() {
	return mRadius;
}

void Ship::update(float deltaTime) {
	updatePosition(deltaTime);
	constrainPosition();
}