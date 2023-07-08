#include "../include/Scene.h"


namespace RayTracerApp
{

Scene::Scene() :
	mNumberOfSpheres(0),
	mNumberOfLightSources(0)
{}

const int Scene::GetNumberOfSpheres() const
{
	return mNumberOfSpheres;
}

const int Scene::GetNumberOfLightSources() const
{
	return mNumberOfLightSources;
}

const MathUtils::Vector3d& Scene::GetSkyColor() const
{
	// TODO: insert return statement here
	return mSkyColor;
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

} // namespace RayTracerApp