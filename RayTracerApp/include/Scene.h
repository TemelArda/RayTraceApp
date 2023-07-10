#pragma once
#include "Vector3d.h"
#include "Camera.h"
#include <string>;
#include <vector>
#include <memory>

namespace RayTracerApp
{

struct Material
{
public:
	std::string name = "Material";

	MathUtils::Vector3d Albedo{1.0};
	
	float Roughness = 1.0;
	
	float Metallic = 0.0;

	float Specular = 1;
};

struct Sphere
{
public:
	Sphere() = default;

	Sphere(const Sphere& other)
	{
		Name = other.Name;
		Position = other.Position;
		Radius = other.Radius;
		MaterialIndex = other.MaterialIndex;
	}

	std::string Name = "sphere";
	
	MathUtils::Vector3d Position{0};
	
	float Radius = 1;

	int MaterialIndex = 0;
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

	void AddMaterial(const Material& material);

	const std::vector<Sphere>& GetSpheres() const;

	const std::vector<LightSource>& GetLightSources() const;

	const std::vector<Material>& GetMaterials() const;
	
	const int GetNumberOfSpheres() const;

	const int GetNumberOfLightSources() const;

	const int GetNumberOfMaterials() const;

	const MathUtils::Vector3d& GetSkyColor() const;

	int GetMaterialIndex(const std::string& materialName) const;

	void SetSkyColor(const MathUtils::Vector3d& skyColor);
private:
	int mNumberOfSpheres;

	int mNumberOfLightSources;

	int mNumberOfMaterials;

	std::vector<Sphere> mSpheres;

	std::vector<LightSource> mLightSources;

	std::vector<Material> mMaterials;

	MathUtils::Vector3d mSkyColor;
};
} // namespace RayTracerApp
