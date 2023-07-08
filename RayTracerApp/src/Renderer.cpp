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

void Renderer::Render(const Camera* camera, const Scene* scene, uint32_t width, uint32_t height)
{
	Ray ray;
	ray.Origin = camera->GetPosition();
	int k = 0;
	for (size_t y = 0; y >= 0 && y < height ; ++y) 
	{
		for (size_t x = width - 1; x >= 0 && x < width; --x)
		{
			ray.Direction = camera->GetRayDirections()[x + y * width];
			auto pixelColor = this->TraceRay(ray, scene);

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

const MathUtils::Vector3d& Renderer::TraceRay(const Ray& ray, const Scene* scene) const
{
	if (scene->GetNumberOfSpheres() <= 0 || scene->GetNumberOfLightSources() <= 0 )
	{
		return scene->GetSkyColor() * 255.0;
	}
	const Sphere* hitSphere = nullptr;
	float hitDistance = std::numeric_limits<float>::max();

	for (Sphere sphere : scene->GetSpheres())
	{
		MathUtils::Vector3d origin = ray.Origin - sphere.Position;

		float a = MathUtils::Vector3d::DotProduct(ray.Direction, ray.Direction);
		float b = 2 * MathUtils::Vector3d::DotProduct(ray.Direction, origin);
		float c = MathUtils::Vector3d::DotProduct(origin, origin) - sphere.Radius * sphere.Radius;

		// b^2-4ac
		float discriminant = b * b - 4.0 * a * c;
		if (discriminant < 0)
		{
			continue;
		}

		//Calculate the two solutions
		//float t2 = (-b + sqrt(discriminant)) / (2.0 * a);
		float t1 = (-b - sqrt(discriminant)) / (2.0 * a);
		//Get the closest solution
		if (t1 < hitDistance)
		{
			delete hitSphere;
			hitDistance = t1;
			hitSphere = new Sphere(sphere);
		}
	}
	if (hitSphere == nullptr)
	{
		return scene->GetSkyColor() * 255.0;
	}
	MathUtils::Vector3d hitPoint = ray.GetPointAt(hitDistance);
	//Calculate the normal at the intersection point
	auto p = hitSphere->Position.x;
	MathUtils::Vector3d normal = hitPoint - hitSphere->Position;
	normal.Normilize();

	//Light direction 3d vector all values are -1.0
	MathUtils::Vector3d lightDir = hitPoint - scene->GetLightSources()[0].Position ;
	lightDir.Normilize();
	const auto dot = MathUtils::Max(MathUtils::Vector3d::DotProduct(normal,  lightDir * -1), 0);
	
	auto sphereColor = hitSphere->Albedo;
	sphereColor *= dot;
	
	delete hitSphere;
	return sphereColor * 255.0;
}
}// namespace RaytracerApp
