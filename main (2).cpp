#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "functions.hpp"
#include "system.hpp"

int main(int argc, char ** argv)
{
	sf::RenderWindow window(sf::VideoMode(800U, 600U), "PHYSICS");

	sf::Vector2f min_point(  0.0f,   0.0f);
	sf::Vector2f max_point(775.0f, 575.0f);

    const int N = 7; // количество частичек в одном слое (квадратная решётка)

    const auto R = length(max_point - min_point) * 0.1f;

    const auto C = (min_point + max_point) * 0.5f;

    const auto r = 2.5f;

    // для прямоугольного желе
//	std::vector < std::vector < System::particle_t >  > particles
//	        (N-2, std::vector(N, std::make_shared<Particle>()));

    std::vector < std::vector < System::particle_t >  > particles
            (N, std::vector(N, std::make_shared<Particle>()));

    // для прямоугольного желе
//	for (int i = 0; i < N-2; ++i)
//	{
//	    for (int j = 0; j < N; ++j)
//	    {
//            auto x = static_cast <float> (j - (N/2));
//            auto y = static_cast <float> (i - ((N-2)/2));
//
////          auto position = sf::Vector2f(std::cos(angle), std::sin(angle)) * R + C;
//            auto position = sf::Vector2f(x, y) * (R/N) + C;
//            particles[i][j] = std::make_shared < Particle > (position, position,
//                    sf::Vector2f(0.0f, 8.0f), r);
//	    }
//        std::cout << std::endl;
//	}

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            auto x = (j - (N/2)) * 1.0f;
            auto y = (i - (N/2)) * 1.0f;

            std::cout << '(' <<  x << ',' << y << ")\t";

            auto position = sf::Vector2f(x, y) * (R/N) + C;
            particles[i][j] = std::make_shared < Particle > (position, position,
                                                             sf::Vector2f(0.0f, 10.0f), r);
        }
        std::cout << std::endl;
    }

	System system(min_point, max_point, particles); // вся система частиц

	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		system.update();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			system.push(sf::Vector2f(0.0f, -1.5f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			system.push(sf::Vector2f(0.0f, 1.5f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			system.push(sf::Vector2f(-1.5f, 0.0f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			system.push(sf::Vector2f(1.5f, 0.0f));
		}

		window.clear();
		
		for (auto i = 0U; i < system.particles().size(); ++i)
		{
            for (auto j = 0U; j < system.particles()[0].size(); ++j)
            {
                sf::CircleShape circle(2.0f * r);

                circle.setPosition(system.particle(i,j)->position() + sf::Vector2f(r, r)); // j - index_x
                    // i - index_y

                circle.setFillColor(sf::Color::Green);

                window.draw(circle);
            }
		}
	
		window.display();
	}
	
//	std::system("pause");

	return EXIT_SUCCESS;
}
