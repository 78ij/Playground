#ifndef QUADTREE_H
#define QUADTREE_H

#include "Base.h"

namespace GeoDraw {
	
	class AABB {
	public:
		AABB() {}
		AABB(float _left, float _top, float _width, float _height) :
			left(_left), top(_top), width(_width), height(_height) {}
		AABB(vector2 a, vector2 b) {
			left = a.x > b.x ? b.x : a.x;
			top = a.y > b.y ? a.y : b.y;
			float down = a.y + b.y - top;
			float right = a.x + b.x - left;
			width = right - left;
			height = top - down;
		}
		AABB(const AABB &other) :
			left(other.left), top(other.top), width(other.width), height(other.height) {}
		AABB &operator=(const AABB &other) {
			left = other.left;
			top = other.top;
			width = other.width;
			height = other.height;
			return *this;
		}
		float getleft() const { return left; }
		float gettop() const { return top; }
		float getwidth() const { return width; }
		float getheight() const { return height; }
		bool isintersect(const AABB &other);
	private:
		float left;
		float top;
		float width;
		float height;
	};

	class QuadTree {
	private:
		int maxchildcount;
		class QuadTreeNode {
		public:
			enum Childs{
				LT, RT, LB, RB
			};
			list<AABB *> containlist;
			QuadTreeNode **childs;
			AABB aabb;
			QuadTreeNode() : childs(nullptr), isleaf(true){}
			QuadTreeNode(AABB aabb) : childs(nullptr), isleaf(true){
				this->aabb = aabb; 
			}
			bool isleaf;
			~QuadTreeNode() {
				for (auto aabb : containlist) {
					delete aabb;
					aabb = nullptr;
				}
				if (childs != nullptr) {
					delete childs[LT];
					delete childs[LB];
					delete childs[RT];
					delete childs[RB];
				}
				delete[] childs;
			}
		};

		QuadTreeNode root;
		void split(QuadTreeNode *node);
		void insertaabb(QuadTreeNode *node, AABB *aabb);
		void printtree(QuadTreeNode *node,int depth);
	public:
		QuadTree() : maxchildcount(3),root(QuadTreeNode(AABB(-1000,1000,2000,2000))), AABBDraw(nullptr){}
		QuadTree(const QuadTree &other) = delete; // no copying
		void insert(AABB *aabb);
		void print();
		void(*AABBDraw)(const AABB &aabb);
	};

} // namespace GeoDraw

#endif // QUADTREE_H
