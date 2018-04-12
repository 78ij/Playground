#ifndef QUADTREE_H
#define QUADTREE_H

#include "Base.h"

namespace GeoDraw {
	
	class AABB {
	public:
		AABB() {}
		AABB(float _left, float _top, float _width, float _height) :
			left(_left), top(_top), width(_width), height(_height) {}
		AABB(vector2 lefttop, vector2 rightbottom) :
			left(lefttop.x), top(lefttop.y),
			width(rightbottom.x - lefttop.x), height(lefttop.y - rightbottom.y) {}
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
		QuadTree() : maxchildcount(8),root(QuadTreeNode(AABB(-1000,1000,1000,1000))){}
		QuadTree(const QuadTree &other) = delete; // no copying
		void insert(AABB *aabb);
		void print();

	};

} // namespace GeoDraw

#endif // QUADTREE_H
