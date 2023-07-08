#include "../include/Renderer.h"
#include "Vector3d.h"
#include "MathUtility.h"
#include <iostream>

namespace RayTracerApp
{
Renderer::~Renderer() 
{
	delete [] mImageData;
}

void Renderer::SetImageSize(uint32_t width, uint32_t height) 
{
	delete [] mImageData;
	mImageData = new uint8_t[width * height * 3]();
}

void Renderer::Render(const Camera* camera, uint32_t width, uint32_t height)
{
	Ray ray;
	ray.Origin = camera->GetPosition();
	int k = 0;
	for (size_t y = 0; y >= 0 && y < height ; ++y) 
	{
		for (size_t x = width - 1; x >= 0 && x < width; --x)
		{
			ray.Direction = camera->GetRayDirections()[x + y * width];
			auto pixelColor = this->TraceRay(ray);

			mImageData[k] = pixelColor.x;
			mImageData[k + 1] = pixelColor.y;
			mImageData[k + 2] = pixelColor.z;
			k+=3;
		}
	}
}

const uint8_t* Renderer::GetImageData() const 
{
	return mImageData; 
}

const MathUtils::Vector3d& Renderer::TraceRay(const Ray& ray) const 
{
	float radius = 1;
  
	float a = MathUtils::Vector3d::DotProduct(ray.Direction, ray.Direction);
	float b = 2 * MathUtils::Vector3d::DotProduct(ray.Direction, ray.Origin);
	float c = MathUtils::Vector3d::DotProduct(ray.Origin, ray.Origin) - radius * radius;

	// b^2-4ac
	float discriminant = b * b -4.0 * a * c;

	uint8_t red = 0.0;
	uint8_t green = 0.0;
	uint8_t blue = 0.0;
	if (discriminant < 0) 
	{
		return MathUtils::Vector3d{0};
   }
	//Calculate the two solutions
	float t1 = (-b + sqrt(discriminant)) / (2.0 * a);
	float t2 = (-b - sqrt(discriminant)) / (2.0 * a);
	//Get the closest solution
	float t = t1 < t2 ? t1 : t2;
	//Calculate the intersection point
	MathUtils::Vector3d hitPoint = ray.GetPointAt(t);
	//Calculate the normal at the intersection point
	MathUtils::Vector3d normal = hitPoint;
	normal.Normilize();

	//Light direction 3d vector all values are -1.0
	MathUtils::Vector3d lightDir(-1.0);
	lightDir.Normilize();
	const auto dot = MathUtils::Max(MathUtils::Vector3d::DotProduct(normal,  lightDir * -1), 0);
	
	auto sphere_color = normal * 0.5 + 0.5;
	sphere_color *= dot;
	return sphere_color * 255.0;
}
}// namespace RaytracerApp
