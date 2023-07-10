#include "../include/Scene.h"


namespace RayTracerApp
{

Scene::Scene() :
	mNumberOfSpheres(0),
	mNumberOfLightSources(0)
{
	Material defaultMaterial;
	defaultMaterial.name = "Default";
	defaultMaterial.Albedo = MathUtils::Vector3d(1.0);
	defaultMaterial.Roughness = 1.0;
	defaultMaterial.Metallic = 0.0;
	defaultMaterial.Specular = 1.0;
	mMaterials.push_back(defaultMaterial);
	mNumberOfMaterials = 1;
}

const int Scene::GetNumberOfSpheres() const
{
	return mNumberOfSpheres;
}

const int Scene::GetNumberOfLightSources() const
{
	return mNumberOfLightSources;
}

const int Scene::GetNumberOfMaterials() const
{
	return mNumberOfMaterials;
}

const MathUtils::Vector3d& Scene::GetSkyColor() const
{
	// TODO: insert return statement here
	return mSkyColor;
}

int Scene::GetMaterialIndex(const std::string& materialName) const
{
	for (int i = 0; i < mMaterials.size(); i++)
	{
		if (mMaterials[i].name == materialName)
		{
			return i;
		}
	}
	return 0;
}

void Scene::SetSkyColor(const MathUtils::Vector3d& skyColor)
{
	mSkyColor = skyColor;
}

const std::vector<Sphere>& Scene::GetSpheres() const
{
	return mSpheres;
}

const std::vector<LightSource>& Scene::GetLightSources() const
{
	return mLightSources;
}

const std::vector<Material>& Scene::GetMaterials() const
{
	return mMaterials;
}

void Scene::AddSphere(const Sphere& sphere)
{
	mNumberOfSpheres++;
	mSpheres.push_back(sphere);
}

void Scene::AddLightSource(const LightSource& lightSource)
{
	mNumberOfLightSources++;
	mLightSources.push_back(lightSource);
}

void Scene::AddMaterial(const Material& material)
{
	mNumberOfMaterials++;
	mMaterials.push_back(material);
}

} // namespace RayTracerApp