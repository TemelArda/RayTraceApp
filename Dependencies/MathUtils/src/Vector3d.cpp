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

Vector3d Vector3d::operator*(const Vector3d& other) const
{
	return Vector3d(x * other.x, y * other.y, z * other.z);
}

void Vector3d::operator*=(const Vector3d& other)
{
	this->x *= other.x;
	this->y *= other.y;
	this->z *= other.z;
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

Vector3d Vector3d::FromString(const std::string& s)
{
	//Build a new Vector3d from a string in the format "x y z"
	//Example: "1 1 1"
	std::string::size_type sz;
	std::string::size_type sz2;
	std::string::size_type sz3;

	double x = std::stod(s, &sz);
	double y = std::stod(s.substr(sz), &sz2);
	double z = std::stod(s.substr(sz + sz2), &sz3);

	return Vector3d(x, y, z);
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

Vector3d Vector3d::Reflect(const Vector3d& incoming, const Vector3d& surfaceNormal)
{
	//-2.0f * glm::dot(hit.normal, r.direction) * hit.normal + r.direction;
	// result = I - 2.0 * dot(N, I) * N
	Vector3d result = (incoming - 2.0 * Vector3d::DotProduct(incoming, surfaceNormal));
	return result * surfaceNormal;
}
}// namespace MathUtils 
