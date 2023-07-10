#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "Vector3d.h"
#include "Scene.h"
#include "Renderer.h"
#include "Camera.h"

namespace RayTracerApp
{

// making typedef for short declaration
typedef std::unordered_multimap<std::string, std::string> SceneDefininition;

class App
{
public:
   static App& GetInstance();

   [[nodiscard]] uint32_t GetWidth() const;
      
   void SetWidth(const uint32_t width); 
      
   [[nodiscard]] uint32_t GetHeigth() const;
   
   void SetHeight(const uint32_t height);    
   

   [[nodiscard]] const bool SetUpScene(const SceneDefininition& scene);

	void InitilizeRenderer();

	const uint8_t* Draw() const;

private :
   App() = default;

   static App* instance; 
   
   std::unique_ptr<Camera> mCamera;

	std::unique_ptr<Renderer> mRenderer;

	std::unique_ptr<Scene> mScene;
   
   uint32_t mWidth = 100;
   
   uint32_t mHeight = 100;

   uint16_t mNumberSpheres = 0;

   uint16_t mNumberLights = 0;

   std::vector<Sphere> mSpheres;

   std::vector<LightSource> mLights;
   
   void SetUpSphere(const std::string& sphere);

   void SetUpLight(const std::string& light);

	void SetUpMaterial(const std::string& material);
};

} // namespace RayTracer




