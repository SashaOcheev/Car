#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

const int ANTIALIASING_LEVEL = 8;
const sf::Vector2f WIN_SIZE = { 800.f, 600.f };
const std::string TITLE = "Car";
const std::string CAR_FILE = "images/car.png";
const std::string WHEEL_FILE = "images/wheel.png";
const float MAX_VELOCITY = 0.5f;
const float BOOST = 0.0005f;
const float CAR_LENGTH = 300.f;
const float pi = 3.14f;

//        map            ///////////////////////////////////
const struct
{
	float START = WIN_SIZE.x - CAR_LENGTH;
	float STOP = 0.f;
	float HEIGHT = 250.f;
	float WHEEL_RADIUS = 30.f;
	sf::Vector2f FRONT_WHEEL_POS = { 540.f, 345.f };
	sf::Vector2f BACK_WHEEL_POS = { 745.f, 345.f };
	sf::Vector2f WHEEL_CENTER = { 30.f, 30.f };
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

		angle_velocity = velocity * 180 / pi / MAP.WHEEL_RADIUS;
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

	sprite_init() {}

	void setSprite(std::string texture_file, sf::Vector2f center)
	{
		texture.loadFromFile(texture_file);
		sprite.setTexture(texture);
		position = sf::Vector2f(0, 0);
		angle = 0.f;
		sprite.setOrigin(center);
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

//          shapes         /////////////////////////////////
struct Init
{
	velocity_init Velocity;
	sprite_init Car;
	sprite_init Front_wheel;
	sprite_init Back_wheel;
	sf::RectangleShape road;

	Init()
	{
		Car.setSprite(CAR_FILE, sf::Vector2f(0, 0));
		Front_wheel.setSprite(WHEEL_FILE, MAP.WHEEL_CENTER);
		Back_wheel.setSprite(WHEEL_FILE, MAP.WHEEL_CENTER);

		Car.updateSprite(sf::Vector2f(MAP.START, MAP.HEIGHT), 0.f);
		Front_wheel.updateSprite(MAP.FRONT_WHEEL_POS, 0.f);
		Back_wheel.updateSprite(MAP.BACK_WHEEL_POS, 0.f);

		road.setPosition(0.f, MAP.FRONT_WHEEL_POS.y + MAP.WHEEL_RADIUS);
		road.setSize(WIN_SIZE);
		road.setFillColor(sf::Color::Black);
	}

	void update()
	{
		Velocity.setVelocity(Car.position);
		Car.updateSprite(sf::Vector2f(Velocity.velocity, 0.f), 0.f);
		Front_wheel.updateSprite(sf::Vector2f(Velocity.velocity, 0.f), Velocity.angle_velocity);
		Back_wheel.updateSprite(sf::Vector2f(Velocity.velocity, 0.f), Velocity.angle_velocity);
	}

	void drawShapes(sf::RenderWindow &window)
	{
		window.draw(road);
		window.draw(Car.sprite);
		window.draw(Front_wheel.sprite);
		window.draw(Back_wheel.sprite);
	}
};
////////////////////////////////////////////////////////////

int main()
{
	Init init;

	sf::ContextSettings settings;
	settings.antialiasingLevel = ANTIALIASING_LEVEL;
	sf::RenderWindow window(sf::VideoMode(int(WIN_SIZE.x), int(WIN_SIZE.y)), TITLE, sf::Style::Default, settings);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		window.clear(sf::Color::White);

		init.update();
		init.drawShapes(window);

		window.display();

	}
	return 0;
}