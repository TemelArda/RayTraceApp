#pragma once
namespace MathUtils
{
// static clamp function for double
static double Clamp(double value, double min, double max)
{
	if (value < min)
	{
		return min;
	}
	if (value > max)
	{
		return max;
	}
	return value;
}
// static max function for double
static double Max(double value1, double value2)
{
	return value1 > value2 ? value1 : value2;
}

static uint32_t PCG_Hash(uint32_t seed)
{
	uint32_t state = seed * 747796405u + 2891336453u;
	uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
	return (word >> 22u) ^ word;
}

static float RandomFloat(uint32_t& seed)
{
	seed = PCG_Hash(seed);
	return (float)seed / std::numeric_limits<uint32_t>::max();
}

static Vector3d RandomInUnitSphere(uint32_t& seed)
{
	Vector3d v 
	{ 
		RandomFloat(seed) * 2 - 1,
		RandomFloat(seed) * 2 - 1, 
		RandomFloat(seed) * 2 - 1 
	};
	v.Normilize();
	return v;
}

static float RandomFloatInRange(float min, float max, uint32_t& seed)
{
	return min + (max - min) * RandomFloat(seed);
}

} // namespace MathUtils