#ifndef SHAPES_H
#define SHAPES_H

#include "Base.h"
#include "QuadTree.h"

namespace GeoDraw {

	class Shapes {
	public:
		Shapes() {}
		Shapes(vector2 _position, vector2 _orientation) :
			position(_position), orientation(_orientation) {}
		vector2 getposition() const { return position; }
		vector2 setposition(vector2 newposition) { position = newposition; }
		vector2 getorientation() const { return orientation; }
		vector2 setorientation(vector2 neworientation) { orientation = neworientation; }
		AABB getAABB() { return aabb; }
		vector2 translate(vector2 offset);
		vector2 rotate(float angle);
	private:
		AABB aabb;
		// the left top point of the AABB box
		vector2 position;
		vector2 orientation;
		matrix22 modelmatrix;
	};

	class Point : Shapes {
		Point(){}
		Point(const Point & other):size(other.size) {}
		Point &operator=(const Point &other) {
			size = other.size;
			return *this;
		}
	private:
		int size;
	};

	class Line : Shapes {
	public:
		Line() {}
		Line(const Line &other) : start(other.start), end(other.end),thickness(other.thickness) {}
		Line &operator=(const Line &other) {
			start = other.start;
			end = other.end;
			return *this;
		}
	private:
		/*The points of the line *relative to* the origin of the shape, 
		a.k.a the left top point of the AABB box *in its own space*. */
		vector2 start; 
		vector2 end;
		int thickness;
	};


} // namespace GeoDraw

#endif //SHAPES_H