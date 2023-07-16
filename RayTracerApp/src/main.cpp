#pragma once
#include "../include/App.h"
#include "../include/FileUtilities.h"
#include <iostream>
#include "../include/ppm.h"


int main(int argc, char* argv[])
{
   if (argc <= 1)
   {
     std::cout << "NO COMMAND LINE ARGUMENT PROVIDED.\n"
               << "Enter any key to move on" << std::endl;
     char end;
     std::cin >> end;
     return -1;
   }
   std::string fileName = argv[1];
   const RayTracerApp::SceneDefininition sceneDefinition = FileUtility::ParseFile(fileName);
   auto scene = RayTracerApp::App::GetInstance().SetUpScene(sceneDefinition);
	RayTracerApp::App::GetInstance().InitilizeRenderer();
   
	if (scene) 
   {
		auto  it = sceneDefinition.find("OUTPUT"); 
		char OutFileName [20];
		if (it != sceneDefinition.end()) 
		{
		  strcpy_s(OutFileName, it->second.c_str());
		}
		else 
		{
		  strcpy_s(OutFileName, "Test.ppm");
		}
		auto pixels = (unsigned char*) RayTracerApp::App::GetInstance().Draw();
      //save_imageP3(Width, Height, fname3, pixels);
		std::cout << "Saving File... " << OutFileName << std::endl;
		save_imageP6(RayTracerApp::App::GetInstance().GetWidth(),
		             RayTracerApp::App::GetInstance().GetHeigth(),
		             OutFileName, pixels);
		return 1;
	}
   
      //do shit!!

   //const auto aspectRatio = RayTracerApp::App::GetInstance().GetHeigth() /
   //                         RayTracerApp::App::GetInstance().GetWidth();
    
//   float W = abs(left - right) / 2.0f;
//   float H = abs(bottom - top) / 2.0f;
//   
//   glm::vec3 eye(0, 0, 0);
//   glm::vec3 u(1, 0, 0);
//   glm::vec3 v(0, 1, 0);
//   glm::vec3 n(0, 0, 1);
//   
//   unsigned char* pixels;
//   // This will be your image. Note that pixels[0] is the top left of the image
//   // and pixels[3*Width*Height-1] is the bottom right of the image.
//   pixels = new unsigned char[3 * width * height];
//   
//   // This loop just creates a gradient for illustration purposes only. You will
//   // not use it.
//   float scale = (float)height / (float)width;
//   int k = 0;
//   for (float j = height - 1; j >= 0; j--) {
//     for (float i = 0; i < width; i++) {
//       // int c = (i + j) * scale;
//       auto uc = -W + W * (2.0f * i / width);
//       auto ur = -H + H * (2.0f * j / height);
//   
//       glm::vec3 dir = -1.0f * near * n + uc * u + ur * v;
//       ray r(eye, dir);
//       r.SetDepth(1);
//       glm::vec3 colors = RayTrace(r);
//       // colors = ReScaleColor(colors);
//       pixels[k] = colors.r * 255.0;
//       pixels[k + 1] = colors.g * 255.0;
//       pixels[k + 2] = colors.b * 255.0;
//       k = k + 3;
//     }
//   }
//   
//   // char fname6[20] = "sceneP6.ppm";
//   // save_imageP3(width, height, outFileName, pixels);
//   save_imageP6(width, height, outFileName, pixels);
   return 0;
}
