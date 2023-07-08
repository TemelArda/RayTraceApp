#pragma once
#include <cstdint>
#include <tuple>
#include "Camera.h"
#include "Ray.h"

namespace RayTracerApp
{
class Renderer {

public:
	Renderer() = default;
 
	~Renderer();
	
	void SetImageSize(uint32_t width, uint32_t height);

	void Render(const Camera* camera, uint32_t width, uint32_t height);

	[[nodiscard]] const uint8_t* GetImageData() const;

private:
	const MathUtils::Vector3d& TraceRay(const Ray& ray) const;
	
	uint8_t *mImageData;
};

}  // namespace RaytracerApp