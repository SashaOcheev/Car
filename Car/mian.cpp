#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

const int ANTIALIASING_LEVEL = 8;
const sf::Vector2f WIN_SIZE = sf::Vector2f(800, 600);
const std::string TITLE = "Car";
const std::string CAR_FILE = "images/car.png";
const std::string WHEEL_FILE = "images/wheel.png";
const float MAX_VELOCITY = 0.5;
const float BOOST = 0.0005;

//        car sizes   ///////////////////////////////////
const struct
{
	float LENGTH = 207.f;
	float WHEEL_RADIUS = 14;
} CAR_IMAGE;
////////////////////////////////////////////////////////////

//        map            ///////////////////////////////////
struct
{
	float START = WIN_SIZE.x - CAR_IMAGE.LENGTH;
	float STOP = 0.f;
	float HEIGHT = 250.f;
	sf::Vector2f FRONT_WHEEL_POS = sf::Vector2f(607.f, 282.f);
	sf::Vector2f BACK_WHEEL_POS = sf::Vector2f(728.f, 282.f);
} MAP;
////////////////////////////////////////////////////////////

//       velocity     //////////////////////////////////////
struct velocity_init
{
	enum { LEFT, RIGHT } direction;
	float velocity;
	float angle_velocity;
	float boost;

	velocity_init()
	{
		velocity = 0.f;
		angle_velocity = 0.f;
		boost = 0.f;
		direction = LEFT;
	}

	void setVelocity(sf::Vector2f position)
	{
		if (position.x + (velocity * velocity / 2 / boost) >= MAP.START)
			direction = LEFT;
		if (position.x + (velocity * velocity / 2 / boost) <= MAP.STOP)
			direction = RIGHT;

		boost = (direction == LEFT) ? -BOOST : BOOST;
		
		if (abs(velocity + boost) <= MAX_VELOCITY)
			velocity += boost;
	}

	void setAngle()
	{

	}

};
////////////////////////////////////////////////////////////

//       sprite       //////////////////////////////////////
struct sprite_init
{
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f position;
	float angle;
	
	sprite_init(std::string texture_file)
	{
		texture.loadFromFile(texture_file);
		sprite.setTexture(texture);
		position = sf::Vector2f(0, 0);
		angle = 0.f;
	}

	void updateSprite(sf::Vector2f move, float rotation)
	{
		position.x += move.x;
		position.y += move.y;
		angle += rotation;
		sprite.setPosition(position);
		sprite.setRotation(angle);
	}
};
////////////////////////////////////////////////////////////


int main()
{
	velocity_init Velocity;
	sprite_init Car(CAR_FILE);
	sprite_init Front_wheel(WHEEL_FILE);
    sprite_init Back_wheel(WHEEL_FILE);

	Car.updateSprite(sf::Vector2f(MAP.START, MAP.HEIGHT), 0.f);
	Front_wheel.updateSprite(MAP.FRONT_WHEEL_POS, 0.f);
	Back_wheel.updateSprite(MAP.BACK_WHEEL_POS, 0.f);

	sf::ContextSettings settings;
	settings.antialiasingLevel = ANTIALIASING_LEVEL;
	sf::RenderWindow window(sf::VideoMode(WIN_SIZE.x, WIN_SIZE.y), TITLE, sf::Style::Default, settings);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		window.clear(sf::Color::Magenta);

		Velocity.setVelocity(Car.position);
		Car.updateSprite(sf::Vector2f(Velocity.velocity, 0.f), 0.f);
		Front_wheel.updateSprite(sf::Vector2f(Velocity.velocity, 0.f), 0.f);
		Back_wheel.updateSprite(sf::Vector2f(Velocity.velocity, 0.f), 0.f);

		window.draw(Car.sprite);
		window.draw(Front_wheel.sprite);
		window.draw(Back_wheel.sprite);
		window.display();

	}
	return 0;
}