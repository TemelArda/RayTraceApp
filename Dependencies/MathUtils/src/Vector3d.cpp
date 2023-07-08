#include "../include/Vector3d.h"
#include <cmath>

namespace MathUtils 
{
Vector3d::Vector3d() : 
	x(0.0),
	y(0.0), 
	z(0.0) 
{}

Vector3d::Vector3d(double value) :
	x(value),
	y(value), 
	z(value) 
	{}

Vector3d::Vector3d(double x, double y, double z):
	x(x),
	y(y), 
	z(z) 
	{}

Vector3d::Vector3d(const Vector3d& other) 
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

Vector3d Vector3d::operator+(const Vector3d& other) const 
{ 
	return Vector3d(x + other.x, y + other.y, z + other.z); 
}

void Vector3d::operator+=(const Vector3d& other) 
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
}

Vector3d Vector3d::operator-(const Vector3d& other) const 
{ 
	return Vector3d(x - other.x, y - other.y, z - other.z); 
}

void Vector3d::operator-=(const Vector3d& other) 
{
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
}

Vector3d Vector3d::operator* (const double other) const
{
	return Vector3d(x * other, y * other, z * other); 
}

void Vector3d::operator*=(const double other) 
{
  this->x *= other;
  this->y *= other;
  this->z *= other;
}

double Vector3d::DotProduct(const Vector3d& v1, const Vector3d& v2) 
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3d Vector3d::CrossProduct(const Vector3d& v1, const Vector3d& v2) 
{
	Vector3d result;
	
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;

	return result;
}

double Vector3d::GetMagnitude() const 
{ 
	return std::sqrt(Vector3d::DotProduct(*this, *this)); 
}

void Vector3d::Normilize() 
{ 
	double magnitude = this->GetMagnitude();
	if (magnitude <= 0 )
		return;
	this->x /= magnitude;
	this->y /= magnitude;
	this->z /= magnitude;
}


}// namespace MathUtils 
