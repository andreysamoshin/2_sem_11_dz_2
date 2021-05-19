#include "system.hpp"

const float cartesian_stiffness = 0.04f; // вертикальная и горизональная жёсткости
const float diagonal_stiffness = 0.02f; // диагональная жёсткость

void System::initialize()
{
    const auto size_x = std::size(m_particles[0]);
    const auto size_y = std::size(m_particles);

	for (auto i = 0U; i < size_y-1; ++i) // минус 1 из-за границ - они не зацикливаются на начало как было с шариком
	{
	    for (auto j = 0U; j < size_x-1; ++j) // минус 1 из-за границ - они не зацикливаются на начало как было с шариком
	    {
		    m_links.push_back(Link(particle(i,j), particle(i+1, j), cartesian_stiffness));
		    m_links.push_back(Link(particle(i,j), particle(i, j+1), cartesian_stiffness));
		    m_links.push_back(Link(particle(i,j), particle(i+1, j+1), diagonal_stiffness));
            m_links.push_back(Link(particle(i,j+1), particle(i+1, j), diagonal_stiffness));

            if (j == size_x-2)
            { //дополнительное соединение по крайней вертикали
                m_links.push_back(Link(particle(i,j+1), particle(i+1, j+1), cartesian_stiffness));
            }
        }
	}
}

void System::push(const sf::Vector2f force) const
{
    const auto size_x = std::size(m_particles[0]);
    const auto size_y = std::size(m_particles);

	for (auto i = 0U; i < size_y; ++i)
	{
	    for (auto j = 0U; j < size_x; ++j)
	    {
		    m_particles[i][j]->move(force);
        }
	}
}

void System::update() const
{
    const auto size_x = std::size(m_particles[0]);
    const auto size_y = std::size(m_particles);

    // взаимодействие со стенкой
    for (auto i = 0U; i < size_y; ++i)
    {
        for (auto j = 0U; j < size_x; ++j)
        {
            m_particles[i][j]->move(0.25f);

            if (m_particles[i][j]->position().y + m_particles[i][j]->radius() > m_max_point.y) {
                m_particles[i][j]->set_y(m_max_point.y - m_particles[i][j]->radius());
            }

            if (m_particles[i][j]->position().y - m_particles[i][j]->radius() < m_min_point.y) {
                m_particles[i][j]->set_y(m_min_point.y + m_particles[i][j]->radius());
            }

            if (m_particles[i][j]->position().x + m_particles[i][j]->radius() > m_max_point.x) {
                m_particles[i][j]->set_x(m_max_point.x - m_particles[i][j]->radius());
            }

            if (m_particles[i][j]->position().x - m_particles[i][j]->radius() < m_min_point.x) {
                m_particles[i][j]->set_x(m_min_point.x + m_particles[i][j]->radius());
            }

            for (auto k = 0U; k < std::size(m_links); ++k) // подновляем связи после взаимодействия со стенкой
            {
                m_links[k].update();
            }
        }
	}

	for (auto i = 0U; i < std::size(m_links); ++i)
	{
		m_links[i].update();
	}

}