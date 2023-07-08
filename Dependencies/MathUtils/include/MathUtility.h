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
} // namespace MathUtils