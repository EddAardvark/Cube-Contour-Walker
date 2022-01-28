#pragma once

#include <list>
#include <functional>

#include "VLInt.h"
#include "FourPointCubic.h"


class CubicSpotter
{
	class Point
	{
	public:
		VLInt x, y1, y2;
		
		Point(const VLInt& vx, const VLInt& vy1, const VLInt& vy2) : x(vx), y1(vy1), y2(vy2) {}
	};

	__int64 delta{ -1 };
	std::list <Point*> points;

public:

	~CubicSpotter()
	{
		while (!points.empty())
		{
			RemoveFirstPoint();
		}
	}

	inline void SetDelta(__int64 d) { delta = d; }
	inline __int64 Delta() const { return delta; }

	void Add(const VLInt& vlx, const VLInt& y1, const VLInt& y2)
	{
		if (!points.empty())
		{
			auto del = (vlx - points.back()->x).ToInt();
			if (del == 1)
			{
				std::cout << "Ignoring " << vlx << ", delta = " << del << std::endl;
				return;
			}
		}
		Point * pt = new Point (vlx, y1, y2);

		points.emplace_back(pt);

		TestSequence(delta-1);
		TestSequence(delta);
	}

	void TestSequence(__int64 del)
	{
		if (points.size() >= 4)
		{
			std::cout << "Checking ";

			for (const auto& it : points)
				std::cout << it->x << ",";

			std::cout << std::endl;

			auto n = points.size();
			Point* test[4];
			int pos = 0;

			for (const auto& pt : points)
			{
				if (pos == 0 || (pt->x - test[pos - 1]->x).ToInt() == del)
				{
					test[pos] = pt;
					pos++;

					if (pos == 4)
					{
						if (pos == 4)
						{
							TestCubic(*test[0], *test[1], *test[2], *test[3]);
							std::cout << "Discarding x = " << points.front()->x << " (used)" << std::endl;
							RemoveFirstPoint();
							return;
						}
					}
				}
			}

			if (n > 7)
			{
				std::cout << "Discarding x = " << points.front()->x << " (no match)" << std::endl;
				RemoveFirstPoint();
			}
		}
	}

	void RemoveFirstPoint()
	{
		if (!points.empty())
		{
			delete points.front();
			points.pop_front();
		}
	}

	void TestCubic(const Point & p1, const Point& p2, const Point& p3, const Point& p4)
	{
		FourPointCubic c1(p1.y1, p2.y1, p3.y1, p4.y1);
		FourPointCubic c2(p1.y1, p2.y1, p3.y1, p4.y2);
		FourPointCubic c3(p1.y1, p2.y1, p3.y2, p4.y2);
		FourPointCubic c4(p1.y1, p2.y2, p3.y2, p4.y2);
		FourPointCubic c5(p1.y2, p2.y2, p3.y2, p4.y2);
		FourPointCubic c6(p1.y2, p2.y2, p3.y2, p4.y1);
		FourPointCubic c7(p1.y2, p2.y2, p3.y1, p4.y1);
		FourPointCubic c8(p1.y2, p2.y1, p3.y1, p4.y1);

		if (c1.IsNatural())
		{
			std::cout << "Row 0 = " << p1.y1 << ", " << p2.y1 << ", " << p3.y1 << ", " << p4.y1 << std::endl;
			std::cout << "C1 = " << c1 << std::endl;
		}
		if (c2.IsNatural())
		{
			std::cout << "Row 0 = " << p1.y1 << ", " << p2.y1 << ", " << p3.y1 << ", " << p4.y2 << std::endl;
			std::cout << "C2 = " << c2 << std::endl;
		}
		if (c3.IsNatural())
		{
			std::cout << "Row 0 = " << p1.y1 << ", " << p2.y1 << ", " << p3.y2 << ", " << p4.y2 << std::endl;
			std::cout << "C3 = " << c3 << std::endl;
		}
		if (c4.IsNatural())
		{
			std::cout << "Row 0 = " << p1.y1 << ", " << p2.y2 << ", " << p3.y2 << ", " << p4.y2 << std::endl;
			std::cout << "C4 = " << c4 << std::endl;
		}
		if (c5.IsNatural())
		{
			std::cout << "Row 0 = " << p1.y2 << ", " << p2.y2 << ", " << p3.y2 << ", " << p4.y2 << std::endl;
			std::cout << "C5 = " << c5 << std::endl;
		}
		if (c6.IsNatural())
		{
			std::cout << "Row 0 = " << p1.y2 << ", " << p2.y2 << ", " << p3.y2 << ", " << p4.y1 << std::endl;
			std::cout << "C6 = " << c6 << std::endl;
		}
		if (c7.IsNatural())
		{
			std::cout << "Row 0 = " << p1.y2 << ", " << p2.y2 << ", " << p3.y1 << ", " << p4.y1 << std::endl;
			std::cout << "C7 = " << c7 << std::endl;
		}
		if (c8.IsNatural())
		{
			std::cout << "Row 0 = " << p1.y2 << ", " << p2.y1 << ", " << p3.y1 << ", " << p4.y1 << std::endl;
			std::cout << "C8 = " << c8 << std::endl;
		}
	}
};

