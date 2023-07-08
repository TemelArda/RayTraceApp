#pragma once
#include <string>;
#include "Vector3d.h"

struct Sphere 
{
public:
   std::string name;
   
   MathUtils::Vector3d position;
   
   MathUtils::Vector3d color;
   
   MathUtils::Vector3d scale;
   
   float specularComponent;
};

struct LightSource {
 public:
   
  std::string name;

  MathUtils::Vector3d position;

  MathUtils::Vector3d color;

};