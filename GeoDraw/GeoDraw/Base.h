#ifndef BASE_H
#define BASE_H

#include <list>
#include <algorithm>
#include <set>
#include <iostream>
#include <typeinfo>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace GeoDraw {

	typedef		glm::vec3 vector3;
	typedef		glm::vec2 vector2;
	typedef		glm::mat2x2 matrix22;
	typedef		glm::mat3x3 matrix33;
	using		glm::sin;
	using		glm::cos;
	using		glm::clamp;
	using		glm::cross;
	using		glm::dot;
	using		glm::length;
	using		glm::normalize;
	using		std::list;
	using		std::set;
	using		std::sort;
} // namespace GeoDraw

#endif // BASE_H
