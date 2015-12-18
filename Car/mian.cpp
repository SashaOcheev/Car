#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

const int ANTIALIASING_LEVEL = 8;
const int WIN_SIZE = 600;
const std::string TITLE = "Car";
const std::string CAR_FILE = "images/car.png";
const std::string WHEEL_FILE = "images/wheel.png";
const float MAX_VELOCITY = 0.2;
const float BOOST = 0.0001;

//        car sizes   ///////////////////////////////////
const struct
{
	int LENGTH = 207;
	int FRONT_WHEEL_CENTER = 34;
	int BACK_WHEEL_CENTER = 155;
	int WHEEL_RADIUS = 14;
} CAR_IMAGE;
////////////////////////////////////////////////////////////

//        map            ///////////////////////////////////
const struct
{
	float START = WIN_SIZE - CAR_IMAGE.LENGTH;
	float STOP = 0.f;
	float HEIGHT = 250.f;
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
		if (position.x >= MAP.START)
			direction = LEFT;
		if (position.x <= MAP.STOP)
			direction = RIGHT;

		boost = (direction == LEFT) ? -BOOST : BOOST;
		
		if (abs(velocity + boost) <= MAX_VELOCITY)
			velocity += boost;
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
	Car.updateSprite(sf::Vector2f(MAP.START, MAP.HEIGHT), 0.f);

	sf::ContextSettings settings;
	settings.antialiasingLevel = ANTIALIASING_LEVEL;
	sf::RenderWindow window(sf::VideoMode(WIN_SIZE, WIN_SIZE), TITLE, sf::Style::Default, settings);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		window.clear(sf::Color::Magenta);

		Velocity.setVelocity(Car.position);
		Car.updateSprite(sf::Vector2f(Velocity.velocity, 0.f), 0.f);

		window.draw(Car.sprite);
		
		window.display();

	}
	return 0;
}