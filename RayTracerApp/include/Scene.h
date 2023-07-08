#pragma once
#include "Vector3d.h"
#include "Camera.h"
#include <string>;
#include <vector>
#include <memory>

namespace RayTracerApp
{

struct Sphere
{
public:
	Sphere() = default;

	Sphere(const Sphere& other)
	{
		Name = other.Name;
		Position = other.Position;
		Albedo = other.Albedo;
		Radius = other.Radius;
		SpecularComponent = other.SpecularComponent;
	}

	std::string Name;
	
	MathUtils::Vector3d Position;
	
	MathUtils::Vector3d Albedo;
	
	float Radius;
	
	float SpecularComponent;
};

struct LightSource
{
public:
	std::string Name;

	MathUtils::Vector3d Position;

	MathUtils::Vector3d Color;
};

class Scene
{
public:
	Scene();
	
	~Scene() = default;

	void AddSphere(const Sphere& sphere);

	void AddLightSource(const LightSource& lightSource);

	const std::vector<Sphere>& GetSpheres() const;

	const std::vector<LightSource>& GetLightSources() const;
	
	const int GetNumberOfSpheres() const;

	const int GetNumberOfLightSources() const;

	const MathUtils::Vector3d& GetSkyColor() const;

	void SetSkyColor(const MathUtils::Vector3d& skyColor);
private:
	int mNumberOfSpheres;

	int mNumberOfLightSources;

	std::vector<Sphere> mSpheres;

	std::vector<LightSource> mLightSources;

	MathUtils::Vector3d mSkyColor;
};
} // namespace RayTracerApp
