//
// RayTracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include "../include/App.h"
#include "FileUtility.h"
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
   if (!mCamera)
      mCamera = std::make_unique<Camera>();
   
   auto it = scene.find("NEAR");
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
     SetHeight(
         stoi(it->second.substr(it->second.find(" "), it->second.length())));
   } 
   else 
   {
     std::cout << "Error when retriving resolution" << std::endl;
     return false;
   }

   auto range = scene.equal_range("SPHERE");
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
		mRenderer = std::make_unique<Renderer>();

	mRenderer->SetImageSize(this->mWidth, this->mHeight);
}

const uint8_t* App::Draw() const
{
	mRenderer->Render(mCamera.get(), mWidth, mHeight); 
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
   
   newSphere.name = v[0];
   
   MathUtils::Vector3d pos{stod(v[1]), stod(v[2]), stod(v[3])};
   newSphere.position = pos;
   MathUtils::Vector3d scale{stod(v[4]), stod(v[5]), stod(v[6])};
   newSphere.scale = scale;
   MathUtils::Vector3d col{stod(v[7]), stod(v[8]), stod(v[9])};
   newSphere.color = col;   
   newSphere.specularComponent = stod(v[14]);

   mSpheres.push_back(newSphere);
   mNumberSpheres++;
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
   
   newLight.name = v[0];
   MathUtils::Vector3d pos{stod(v[1]), stod(v[2]), stod(v[3])};
   newLight.position = pos;
   MathUtils::Vector3d col{stod(v[4]), stod(v[5]), stod(v[6])};
   newLight.color = col;
   
   mLights.push_back(newLight);
   mNumberLights++;
}

 
}  // namespace RayTracerApp