#ifndef GEOCONTEXT_H
#define GEOCONTEXT_H

#include "Base.h"
#include "Shapes.h"

namespace GeoDraw {

	class Geocontext {
	public:
		Geocontext() :LineDraw(nullptr), PointDraw(nullptr), AABBDraw(nullptr) {}
		void(*LineDraw)(const Line &line);
		void(*PointDraw)(const Point &point);
		void(*AABBDraw)(const AABB &aabb);
		void Draw();
		void AddLine(vector2 start, vector2 end);
		~Geocontext() {
			for (Shapes *shape : shapes) {
				delete shape;
				shape = nullptr;
			}
		}
	private:
		set<Shapes *> shapes;
		QuadTree quadtree;
	};

	void Geocontext::AddLine(vector2 start,vector2 end) {
		Line *line = new Line(start, end);
		shapes.insert(static_cast<Shapes *>(line));
		quadtree.insert(&line->getAABB());
	}

	void Geocontext::Draw() {
		for(Shapes *shape : shapes) {
			if (typeid (*shape) == typeid(Line)) {
				LineDraw(*dynamic_cast<Line *>(shape));
			}
			if (typeid (*shape) == typeid(Point)) {
				PointDraw(*dynamic_cast<Point *>(shape));
			}
			if (typeid (*shape) == typeid(AABB)) {
				AABBDraw(*dynamic_cast<AABB *>(shape));
			}
		}
	}


} // namespace GeoDraw


#endif // GEOCONTEXT_H
