#pragma once
#include "Vector3d.h"
namespace RayTracerApp 
{

struct Ray 
{

	Ray() = default;

	Ray(const MathUtils::Vector3d& origin, const MathUtils::Vector3d& direction):
	Origin(origin), 
	Direction(direction) 
	{}
	
	MathUtils::Vector3d Origin{0};
	MathUtils::Vector3d Direction{0, 0, 1};
};

}// namespace RayTracerApp