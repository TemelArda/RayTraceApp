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
	int k = 0;
	for (size_t y = height - 1; y >= 0 && y < height ; --y) 
	{
		for (size_t x = 0; x >= 0 && x < width; ++x)
		{
			int rayIndex = x + y * width;
			auto pixelColor = this->PerPixel(camera, scene, rayIndex);
			pixelColor.x = MathUtils::Clamp(pixelColor.x, 0.0, 1.0);
			pixelColor.y = MathUtils::Clamp(pixelColor.y, 0.0, 1.0);
			pixelColor.z = MathUtils::Clamp(pixelColor.z, 0.0, 1.0);

			mImageData[k] = pixelColor.x * 255.0;
			mImageData[k + 1] = pixelColor.y * 255.0;
			mImageData[k + 2] = pixelColor.z * 255.0;
			k+=3;
		}
	}
}

const uint8_t* Renderer::GetImageData() const 
{
	return mImageData; 
}

const MathUtils::Vector3d& Renderer::PerPixel(const Camera* camera, const Scene* scene, uint32_t rayIndex) const
{
	Ray ray;
	ray.Origin = camera->GetPosition();
	ray.Direction = camera->GetRayDirections()[rayIndex];
	
	int rayBounces = 2;
	MathUtils::Vector3d color{0};
	float multiplier = 1;
	LightSource lightSource = scene->GetLightSources()[0];

	for (size_t i = 0; i < rayBounces; ++i)
	{
		HitInfo payload = TraceRay(ray, scene);
		if (payload.Distance < 0)
		{
			color += scene->GetSkyColor() * multiplier;
			break;
		}
		auto lightDirection =  payload.HitPoint - lightSource.Position;
		lightDirection.Normilize();
		const double dot = MathUtils::Max(0, 
			MathUtils::Vector3d::DotProduct(payload.Normal, lightDirection * -1));
		
		color += scene->GetSpheres()[payload.ObjectIndex].Albedo * dot;
		color *= multiplier;
		multiplier *= 0.7;

		//color = payload.Normal * .5 + .5;
		ray.Origin = payload.HitPoint + payload.Normal * 0.0001;
		ray.Direction = MathUtils::Vector3d::Reflect(ray.Direction, payload.Normal);
	}
	return color;
}

const HitInfo& Renderer::TraceRay(const Ray& ray, const Scene* scene) const
{
	if (scene->GetNumberOfSpheres() <= 0 || scene->GetNumberOfLightSources() <= 0 )
	{
		return Missed(ray);
	}
	int hitIndex = -1;
	float hitDistance = std::numeric_limits<float>::max();

	for (int i  = 0; i < scene->GetNumberOfSpheres(); ++i)
	{
		const Sphere& sphere = scene->GetSpheres()[i];
		
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
		if (t1 > 0 && t1 < hitDistance)
		{
			hitDistance = t1;
			hitIndex = i;
		}
	}
	if (hitIndex < 0 )
	{
		return Missed(ray);
	}
	return OnHit(ray, scene, hitDistance, hitIndex);
}

const HitInfo& Renderer::Missed(const Ray& ray) const
{
	HitInfo hitInfo;
	hitInfo.Distance = -1;
	return hitInfo;
}

const HitInfo& Renderer::OnHit(const Ray& ray, const Scene* scene, const double HitDistance, const int ObjectIndex) const
{
	const Sphere& sphere = scene->GetSpheres()[ObjectIndex];
	HitInfo hitInfo;

	hitInfo.HitPoint = ray.GetPointAt(HitDistance);
	hitInfo.Normal = hitInfo.HitPoint - sphere.Position;
	hitInfo.Normal.Normilize();
	hitInfo.Distance = HitDistance;
	hitInfo.ObjectIndex = ObjectIndex;
	
	return hitInfo;
}
}// namespace RaytracerApp
