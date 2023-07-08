#pragma once
#include "Vector3d.h"
#include <cstdint>
#include <vector>


namespace RayTracerApp{
class Camera {
public:
	Camera();

	~Camera() = default;

	void SetNear(const float near);
	
	const float GetNear() const;

	const MathUtils::Vector3d& GetPosition() const;

	const MathUtils::Vector3d& GetForwatdDirection() const;

	const MathUtils::Vector3d GetRightDirection() const;

	const std::vector<MathUtils::Vector3d>& GetRayDirections() const;

	void CalculateRayDirections(uint32_t width, uint32_t height);

public:
	int8_t top = 1;

	int8_t bottom = -1;
	
	int8_t right = 1;

	int8_t left = -1;

private:
	MathUtils::Vector3d mPosition;

	MathUtils::Vector3d mForwardDirection;

	float mNear;

	std::vector<MathUtils::Vector3d> mRayDirections;
};
} //namespace RayTracerApp