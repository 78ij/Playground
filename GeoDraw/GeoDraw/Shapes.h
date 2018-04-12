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
		AABB getAABB() const { return aabb; }
		vector2 translate(vector2 offset);
		virtual vector2 rotate(float angle);
	protected:
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

	class Line : public Shapes {
	public:
		Line() {}
		Line(const Line &other) : start(other.start), end(other.end),thickness(other.thickness) {
			aabb = other.aabb;
		}
		Line(vector2 _start, vector2 _end,float _thickness = 2) {
			start = _start;
			end = _end;
			aabb = AABB(_start, _end);
			thickness = _thickness;
		}
		Line &operator=(const Line &other) {
			start = other.start;
			end = other.end;
			aabb = other.aabb;
			return *this;
		}
		vector2 getstart() const { return start; }
		vector2 getend() const { return end; }
		float getthickness() const { return thickness; }
	private:
		/*The points of the line *relative to* the origin of the shape, 
		a.k.a the left top point of the AABB box *in its own space*. */
		vector2 start; 
		vector2 end;
		float thickness;
	};


} // namespace GeoDraw

#endif //SHAPES_H