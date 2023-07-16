#pragma once
#include "../include/App.h"
#include <iostream>
#include <sstream>

namespace RayTracerApp {

 App& App::GetInstance() 
{
  static App instance;
  return instance;
}

uint32_t App::GetWidth() const
{ 
   return mWidth; 
}

void App::SetWidth(const uint32_t width) 
{
   mWidth = width;
}

uint32_t App::GetHeigth() const 
{ 
   return mHeight; 
}

void App::SetHeight(const uint32_t height) 
{
   mHeight = height;
}


const bool App::SetUpScene(const SceneDefininition& scene) 
{
	if (!mScene)
		mScene = std::make_shared<Scene>();
   
	if (!mCamera)
      mCamera = std::make_shared<Camera>();
	
	if(!mRenderer)
		mRenderer = std::make_shared<Renderer>();

	auto it = scene.find("SAMPLES");
	if (it != scene.end())
	{
		mRenderer->SetNumSamples(stoi(it->second));
	}
	else
	{
		std::cout << "Error when retriving TOP of mCamera" << std::endl;
		return false;
	}

   it = scene.find("NEAR");
   if (it != scene.end()) 
   {
		mCamera->SetNear(stoi(it->second));
   } 
   else 
   {
     std::cout << "Error when retriving Near Plane of mCamera" << std::endl;
      return false;
   }

   it = scene.find("LEFT");
   if (it != scene.end()) 
   {
     mCamera->left = stoi(it->second);
   } 
   else 
   {
     std::cout << "Error when retriving LEFT of mCamera" << std::endl;
     return false;
   }

   it = scene.find("RIGHT");
   if (it != scene.end()) 
   {
     mCamera->right = stoi(it->second);
   }
   else 
   {
     std::cout << "Error when retriving RIGHT of mCamera" << std::endl;
     return false;
   }

   it = scene.find("TOP");
   if (it != scene.end()) 
   {
     mCamera->top = stoi(it->second);
   } 
   else 
   {
     std::cout << "Error when retriving TOP of mCamera" << std::endl;
     return false;
   }
   
   it = scene.find("BOTTOM");
   if (it != scene.end()) 
   {
     mCamera->bottom = stoi(it->second);
   } 
   else 
   {
     std::cout << "Error when retriving BOTTOM of mCamera" << std::endl;
     return false;
   }
   
   it = scene.find("RES");
   if (it != scene.end()) 
   {
      SetWidth(stoi(it->second.substr(0, it->second.find(" "))));
		SetHeight(stoi(it->second.substr(it->second.find(" "), it->second.length())));
   } 
   else 
   {
     std::cout << "Error when retriving resolution" << std::endl;
     return false;
   }

	it = scene.find("AMBIENT");
	if (it != scene.end())
	{
		auto skyColor = MathUtils::Vector3d::FromString(it->second);
		mScene->SetSkyColor(skyColor);
	}
	else
	{
		std::cout << "Error when retriving Sky Color" << std::endl;
		return false;
	}

	auto range = scene.equal_range("MATERIAL");
	if (range.first != scene.end())
	{
		for (auto it = range.first; it != range.second; ++it)
		{
			SetUpMaterial(it->second);
		}
	}
	else
	{
		std::cout << "Error when retriving MATERIAL" << std::endl;
		return false;
	}

   range = scene.equal_range("SPHERE");
   if (range.first != scene.end())
   {
      for (auto it = range.first; it != range.second; ++it) 
      {
         SetUpSphere(it->second);
      }
   } 
   else
   {
     std::cout << "Error when retriving SPHERE" << std::endl;
     return false;
   }
   
   range = scene.equal_range("LIGHT");
   if (range.first != scene.end()) 
   {
      for (auto it = range.first; it != range.second; ++it)
      {
         SetUpLight(it->second);
      }
   } 
   else
   {
     std::cout << "Error when retriving LIGHT" << std::endl;
     return false;
   }
   
	mCamera->CalculateRayDirections(mWidth, mHeight);
   
	return true;
}

void App::InitilizeRenderer() 
{
	if (!mRenderer) 
		mRenderer = std::make_shared<Renderer>();

	mRenderer->SetImageSize(this->mWidth, this->mHeight);
}

const uint8_t* App::Draw() const
{
	mRenderer->Render(mCamera, mScene, mWidth, mHeight); 
	return mRenderer->GetImageData();
}

void App::SetUpSphere(const std::string& sphere) 
{
   std::cout<<sphere<<std::endl;
   std::stringstream ss(sphere);
   std::vector<std::string> v;
   std::string tmp;

   while (getline(ss, tmp, ' ')) 
      v.push_back(tmp);

   Sphere newSphere;
   
   newSphere.Name = v[0];
   
   MathUtils::Vector3d pos{stod(v[1]), stod(v[2]), stod(v[3])};
   newSphere.Position = pos;
   newSphere.Radius = stod(v[4]);
	newSphere.MaterialIndex = mScene->GetMaterialIndex(v[5]);
	mScene->AddSphere(newSphere);
}

void App::SetUpLight(const std::string& light) 
{
   std::cout << light << std::endl;
   
   std::stringstream ss(light);
   std::vector<std::string> v;
   std::string tmp;
   
   while (getline(ss, tmp, ' ')) 
      v.push_back(tmp);

   LightSource newLight;
   
   newLight.Name = v[0];
   MathUtils::Vector3d pos{stod(v[1]), stod(v[2]), stod(v[3])};
   newLight.Position = pos;
   MathUtils::Vector3d col{stod(v[4]), stod(v[5]), stod(v[6])};
   newLight.Color = col;

	mScene->AddLightSource(newLight);
}

void App::SetUpMaterial(const std::string& material)
{
	std::cout << material << std::endl;
	std::stringstream ss(material);
	std::vector<std::string> v;
	std::string tmp;

	while (getline(ss, tmp, ' '))
		v.push_back(tmp);

	Material newMaterial;
	newMaterial.name = v[0];
	MathUtils::Vector3d col{stod(v[1]), stod(v[2]), stod(v[3])};
	newMaterial.Albedo = col;
	newMaterial.Roughness = stod(v[4]);
	newMaterial.Metallic = stod(v[5]);
	newMaterial.Specular = stod(v[6]);
	mScene->AddMaterial(newMaterial);
}
}  // namespace RayTracerApp
