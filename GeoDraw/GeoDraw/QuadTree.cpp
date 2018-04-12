#include "QuadTree.h"

namespace GeoDraw {

	bool AABB::isintersect(const AABB &other) {
		float inleft = left > other.left ? left : other.left;
		float inright = left + width < other.left + other.width ?
			left + width : other.left + other.width;
		float intop = top < other.top ? top : other.top;
		float inbottom = top - height > other.top - other.height ? 
			top - height : other.top - other.height;
		return inleft < inright && intop > inbottom;
	}

	void QuadTree::insert(AABB *aabb) {
		insertaabb(&root, aabb);
	}

	void QuadTree::insertaabb(QuadTreeNode *node, AABB *aabb) {
		if (node->isleaf) {
			if (node->containlist.size() + 1 > maxchildcount) {
				split(node);
				insertaabb(node, aabb);
			}
			else{
				node->containlist.push_back(aabb);
			}
		}
		else {
			float midwidth = node->aabb.getwidth() / 2;
			float midheight = node->aabb.getheight() / 2;
			if (aabb->isintersect(node->childs[QuadTreeNode::LB]->aabb)) {
				insertaabb(node->childs[QuadTreeNode::LB],aabb);
			}
			if (aabb->isintersect(node->childs[QuadTreeNode::LT]->aabb)) {
				insertaabb(node->childs[QuadTreeNode::LT], aabb);
			}
			if (aabb->isintersect(node->childs[QuadTreeNode::RB]->aabb)) {
				insertaabb(node->childs[QuadTreeNode::RB], aabb);
			}
			if (aabb->isintersect(node->childs[QuadTreeNode::RT]->aabb)) {
				insertaabb(node->childs[QuadTreeNode::RT], aabb);
			}
		}
	}

	void QuadTree::split(QuadTreeNode *node) {
		if (!node->isleaf) return;
		float midwidth = node->aabb.getwidth() / 2;
		float midheight = node->aabb.getheight() / 2;
		node->childs = new QuadTreeNode *[4];
		node->isleaf = false;
		node->childs[QuadTreeNode::LT] =
			new QuadTreeNode(AABB(node->aabb.getleft(), node->aabb.gettop(), midwidth, midheight));
		node->childs[QuadTreeNode::LB] =
			new QuadTreeNode(AABB(node->aabb.getleft(), node->aabb.gettop() - midheight, midwidth, midheight));
		node->childs[QuadTreeNode::RT] =
			new QuadTreeNode(AABB(node->aabb.getleft() + midwidth, node->aabb.gettop(), midwidth, midheight));
		node->childs[QuadTreeNode::RB] =
			new QuadTreeNode(AABB(node->aabb.getleft() + midwidth, node->aabb.gettop() - midheight, midwidth, midheight));

		for(auto aabb : node->containlist) {
			insertaabb(node, aabb);
		}
		node->containlist.clear();
	}

	void QuadTree::printtree(QuadTreeNode *node,int depth) {
		if (node == nullptr) return;
		if (node->isleaf) {
			for (auto aabb : node->containlist) {
				std::cout << "left: " << aabb->getleft() << "top: " << aabb->gettop()
					<< "width :" << aabb->getwidth() << "height :" << aabb->getheight() << " depth:" << depth << std::endl;
			}
		}
		else {
			if (node->childs == nullptr) return;
			printtree(node->childs[QuadTreeNode::LB],depth + 1);
			printtree(node->childs[QuadTreeNode::LT], depth + 1);
			printtree(node->childs[QuadTreeNode::RB], depth + 1);
			printtree(node->childs[QuadTreeNode::RT], depth + 1);
		}
	}

	void QuadTree::print() {
		printtree(&root,0);
	}

} // namespace GeoDraw