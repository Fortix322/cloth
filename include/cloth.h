#pragma once

#include <vector>

#include "physicsSolver.h"

class Cloth
{
public:

	Cloth(size_t density, size_t height, size_t durability, glm::fvec3 leftTop, glm::fvec3 rightTop)
	{
		m_count = density * height;
		m_points = new VerletObject[m_count];

		for (size_t i = 0; i < m_count; i++)
		{
			m_points[i].IsStatic = true;
		}

		float offset = (rightTop.x - leftTop.x) / density;
		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < density; j++)
			{
				m_points[j + i * density].SetPosition({ leftTop.x + offset * j, leftTop.y - offset * i, leftTop.z});
			}
		}

		/*for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < density; j++)
			{
				if (j != density - 1)
				{
					m_links.push_back({ &m_points[j + i * density], &m_points[j + i * density + 1], offset });
				}

				if (i != height - 1)
				{
					m_links.push_back({ &m_points[j + i * density], &m_points[j + (i + 1) * density], offset });
				}
			}
		}*/

	}

	~Cloth()
	{
		delete[] m_points;
	}

	void Update(float dt)
	{
		PhysicsSolver solver;

		solver.Update(m_points, m_count, dt);
	}

	const VerletObject* GetPoints(unsigned int& count)
	{
		count = m_count;
		return m_points;
	}

	const Link* GetLinks(unsigned int& count)
	{
		count = m_links.size();
		return m_links.data();
	}

private:

	VerletObject* m_points;
	std::vector<Link> m_links;
	unsigned int m_count;
};