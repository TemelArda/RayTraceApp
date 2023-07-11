#pragma once
#include <cstdint>
#include <tuple>
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"

namespace RayTracerApp
{
class Renderer {

public:
	Renderer() = default;
 
	~Renderer();
	
	void SetImageSize(uint32_t width, uint32_t height);

	void Render(const Camera* camera, const Scene* scene, uint32_t width, uint32_t height);

	[[nodiscard]] const uint8_t* GetImageData() const;

	void SetNumSamples(uint32_t numSamples);
private:
	uint8_t *mImageData;
	
	MathUtils::Vector3d *mAccumulationBuffer;

	uint32_t mFrameIndex = 1;

	uint32_t mSamples = 1;

	std::vector<uint32_t> mVerticalIterator;

	std::vector<uint32_t> mHorizontalIterator;
private:
	[[nodiscard]] const MathUtils::Vector3d& PerPixel(const Camera* camera, const Scene* scene, uint32_t rayIndex) const;

	[[nodiscard]] const HitInfo& TraceRay(const Ray& ray, const Scene* scene) const;

	[[nodiscard]] const bool IsInShadow(const Ray& toLight, const HitInfo& hitInfo, const Scene* scene) const;

	const HitInfo& Missed(const Ray& ray) const;

	const HitInfo& OnHit(const Ray& ray, const Scene* scene, const double HitDistance, const int ObjectIndex) const;

};

}  // namespace RaytracerApp