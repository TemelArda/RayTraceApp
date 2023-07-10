#include "../include/Camera.h"
#include <cstdlib>

namespace RayTracerApp{
Camera::Camera() : 
	mForwardDirection{0, 0, -1}, 
	mNear(1), 
	mPosition(0, 0, 6) {}

void Camera::SetNear(const float near) 
{
	mNear = near; }

const float Camera::GetNear() const 
{ 
	return mNear; 
}

const MathUtils::Vector3d& Camera::GetPosition() const 
{
	return mPosition;
}

const MathUtils::Vector3d& Camera::GetForwatdDirection() const 
{
	return mForwardDirection;
}

const MathUtils::Vector3d Camera::GetRightDirection() const 
{
	MathUtils::Vector3d UpDirection(0.0, 1.0, 0.0);
	return MathUtils::Vector3d::CrossProduct(mForwardDirection, UpDirection);
}

const std::vector<MathUtils::Vector3d>& Camera::GetRayDirections() const
{
  return mRayDirections;
}

void Camera::CalculateRayDirections(uint32_t width, uint32_t height) 
{
	float W = (float) std::abs(left - right) / 2.0f;
	float H = (float) std::abs(bottom - top) / 2.0f;

	mRayDirections.resize((size_t) width * height);

	for (size_t y = 0; y < height; ++y) 
	{
		for (size_t x = 0; x < width; ++x) 
		{
			auto uc = -W + W * (2.0f * x / (float) width);
			auto ur = -H + H * (2.0f * y / (float) height);
			//glm::vec3 dir = -1.0f * near * n + uc * u + ur * v;
			
			MathUtils::Vector3d rayDirection= mForwardDirection * mNear;
			rayDirection += this->GetRightDirection() * uc;
			rayDirection += MathUtils::Vector3d(0, 1, 0) * ur; 

			mRayDirections[x + y * width] = rayDirection;
		}
	}
}
} //namespace RayTrcerApp