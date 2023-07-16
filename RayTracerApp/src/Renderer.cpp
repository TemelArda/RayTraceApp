#include "../include/Renderer.h"
#include "Vector3d.h"
#include "MathUtility.h"
#include <iostream>
#include <chrono>
#include <execution>

namespace RayTracerApp
{
constexpr double POINT_OFFSET = 0.0001;

Renderer::~Renderer() 
{
	delete [] mImageData;
	delete [] mAccumulationBuffer;
}

void Renderer::SetImageSize(uint32_t width, uint32_t height) 
{
	delete [] mImageData;
	mImageData = new uint8_t[width * height * 3]();
	
	delete [] mAccumulationBuffer;
	mAccumulationBuffer = new MathUtils::Vector3d[width * height]();

	mVerticalIterator.resize(height);
	mHorizontalIterator.resize(width);
	for (size_t i = 0; i < width; i++)
	{
		mHorizontalIterator[i] = i;
	}
	for (size_t y = height - 1; y >= 0 && y < height; --y)
	{
		mVerticalIterator[(height - 1) - y] = y;
	}
}

void Renderer::Render(const std::weak_ptr<Camera> camera, const std::weak_ptr<Scene> scene, uint32_t width, uint32_t height)
{
	if (scene.expired() || camera.expired())
	{
		std::cout << "Scene or camera is expired" << std::endl;
		return;
	}
	
	auto scenePtr = scene.lock().get();
	auto cameraPtr = camera.lock().get();
	MathUtils::Vector3d pixelColor{0};
	
	while (mFrameIndex < mSamples)
	{
		if(mFrameIndex == 1)
			memset(mAccumulationBuffer, 0, width * height * sizeof(MathUtils::Vector3d));
		auto start = std::chrono::steady_clock::now();

		std::for_each(std::execution::par, mVerticalIterator.begin(), mVerticalIterator.end(),
		[=](uint32_t y)
		{
			for (size_t x = 0; x < width; ++x)
			{
				int rayIndex = x + y * width;
				int k = 3 * (x + ((height - 1) - y ) * width);
				auto color = this->PerPixel(cameraPtr, scenePtr, rayIndex);
				mAccumulationBuffer[rayIndex] += color;
				auto pixelColor = mAccumulationBuffer[rayIndex] * (1.0 / (float)mFrameIndex);
				pixelColor.x = MathUtils::Clamp(pixelColor.x, 0.0, 1.0);
				pixelColor.y = MathUtils::Clamp(pixelColor.y, 0.0, 1.0);
				pixelColor.z = MathUtils::Clamp(pixelColor.z, 0.0, 1.0);
				mImageData[k] = pixelColor.x * 255.0;
				mImageData[k + 1] = pixelColor.y * 255.0;
				mImageData[k + 2] = pixelColor.z * 255.0;
			}
			//std::for_each(std::execution::par, mHorizontalIterator.begin(), mHorizontalIterator.end(),
			//[=](uint32_t x)
			//{
			//	int rayIndex = x + y * width;
			//	int k = 3 * (x + ((height - 1) - y ) * width);
			//	auto color = this->PerPixel(camera, scene, rayIndex);
			//	mAccumulationBuffer[rayIndex] += color;
			//	auto pixelColor = mAccumulationBuffer[rayIndex] * (1.0 / (float)mFrameIndex);
			//	pixelColor.x = MathUtils::Clamp(pixelColor.x, 0.0, 1.0);
			//	pixelColor.y = MathUtils::Clamp(pixelColor.y, 0.0, 1.0);
			//	pixelColor.z = MathUtils::Clamp(pixelColor.z, 0.0, 1.0);
			//	mImageData[k] = pixelColor.x * 255.0;
			//	mImageData[k + 1] = pixelColor.y * 255.0;
			//	mImageData[k + 2] = pixelColor.z * 255.0;
			//});	
		});
		mFrameIndex++;
		auto end = std::chrono::steady_clock::now();
		std::cout << "Samples: " << mFrameIndex;
		std::cout << ", Elapsed(ms)= " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
	}
	PostProcess(width, height);
}

const uint8_t* Renderer::GetImageData() const 
{
	return mImageData; 
}

void Renderer::SetNumSamples(uint32_t numSamples)
{
	mSamples = numSamples;
}

const MathUtils::Vector3d& Renderer::PerPixel(const Camera* camera, const Scene* scene, uint32_t rayIndex) const
{
	Ray ray;
	ray.Origin = camera->GetPosition();
	ray.Direction = camera->GetRayDirections()[rayIndex];

	int rayBounces = 5;
	MathUtils::Vector3d color{0};
	MathUtils::Vector3d contribution{1};
	
	uint32_t seed = rayIndex * mFrameIndex;
	for (size_t i = 0; i < rayBounces; ++i)
	{
		seed += i;
		HitInfo payload = TraceRay(ray, scene);
		if (payload.Distance < 0)
		{
			color += scene->GetSkyColor() ;//* contribution;
			break;
		}
		const Material& mat = scene->GetMaterials()[payload.MaterialIndex];
		for(size_t i = 0; i < scene->GetNumberOfLightSources(); ++i)
		{
			const LightSource& light = scene->GetLightSources()[i];
			Ray rayToLight;
			rayToLight.Direction = light.Position - payload.HitPoint;
			rayToLight.Origin = payload.HitPoint + rayToLight.Direction * POINT_OFFSET;
			if (IsInShadow(rayToLight, payload, scene))
			{
				continue;
			}
			rayToLight.Direction.Normilize();
			MathUtils::Vector3d V = payload.HitPoint * -1;
			MathUtils::Vector3d R = MathUtils::Vector3d::Reflect(rayToLight.Direction * -1.0f, payload.Normal);
			const double NdotL = MathUtils::Max(0, MathUtils::Vector3d::DotProduct(rayToLight.Direction, payload.Normal));
			//const double RdotV = MathUtils::Max(0, MathUtils::Vector3d::DotProduct(V, R));
			//float s = powf(RdotV, (int)mat.Specular);

			MathUtils::Vector3d diffuse = light.Color * NdotL * mat.Albedo;
			//MathUtils::Vector3d specular = light.Color * s;
			color += diffuse; //;+ specular;
		}
		//auto lightDirection =  payload.HitPoint - scene->GetLightSources()[0].Position;
		//lightDirection.Normilize();
		//const double dot = MathUtils::Max(0, 
		//	MathUtils::Vector3d::DotProduct(payload.Normal, lightDirection * -1));
		//color += mat.Albedo * dot;
		//color *= contribution;

		//color = payload.Normal * .5 + .5;
		ray.Origin = payload.HitPoint + payload.Normal * POINT_OFFSET;
		ray.Direction = (payload.Normal + mat.Roughness * MathUtils::RandomFloatInRange(0.5, -0.5, seed));
		ray.Direction.Normilize();
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
	hitInfo.MaterialIndex = sphere.MaterialIndex;

	return hitInfo;
}


const bool Renderer::IsInShadow(const Ray& toLight, const HitInfo& hitInfo, const Scene* scene) const
{
	for (size_t i = 0; i < scene->GetNumberOfSpheres(); ++i)
	{
		if (hitInfo.ObjectIndex == i)
		{
			continue;
		}
		const Sphere& sphere  = scene->GetSpheres()[i];
		MathUtils::Vector3d origin = toLight.Origin - sphere.Position;
		float a = MathUtils::Vector3d::DotProduct(toLight.Direction, toLight.Direction);
		float b = 2 * MathUtils::Vector3d::DotProduct(toLight.Direction, origin);
		float c = MathUtils::Vector3d::DotProduct(origin, origin) - sphere.Radius * sphere.Radius;
		float discriminant = b * b - 4.0 * a * c;
		if (discriminant > 0)
		{
			//float t2 = (-b + sqrt(discriminant)) / (2.0 * a);
			float t1 = (-b - sqrt(discriminant)) / (2.0 * a);
			if (t1 > 0 && t1 < 1)
			{
				return true;
			}
		}

	}
	return false;
}

void Renderer::PostProcess(uint32_t width, uint32_t height)
{
	for (size_t x = 1; x < width - 1; ++x)
	{
		for (size_t y = 1; y < height - 1; ++y)
		{
			MathUtils::Vector3d finalColor{0};
			//Get the color of the pixel
			for (int j = -1; j < 2; ++j)
			{
				for (int i = -1; i < 2; ++i)
				{
					uint32_t pixel = 3 * ((x + i) + (y + j) * width);
					MathUtils::Vector3d color{(double)mImageData[pixel], (double)mImageData[pixel + 1],
						(double)mImageData[pixel + 2]};
					finalColor += color;
				}
			}
			//Average the colors
			finalColor *= 1.0f / 9.0f;
			mImageData[3 * (x + y * width)] = finalColor.x;
			mImageData[3 * (x + y * width) + 1] = finalColor.y;
			mImageData[3 * (x + y * width) + 2] = finalColor.z;
		}
	}
	

	
}


}// namespace RaytracerApp
