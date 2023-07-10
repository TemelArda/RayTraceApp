#pragma once
#include <string>

namespace MathUtils 
{

class Vector3d {
public:
   Vector3d();

   Vector3d(double value);

   Vector3d(double x, double y, double z);

	Vector3d(const Vector3d& other);

   ~Vector3d() = default;

	Vector3d operator +(const Vector3d& other) const;

	void operator +=(const Vector3d& other);

	Vector3d operator-(const Vector3d& other) const;

	void operator-=(const Vector3d& other);

	Vector3d operator *(const double other) const;

	void operator *=(const double other);

	Vector3d operator *(const Vector3d& other) const;

	void operator *=(const Vector3d& other);

	double GetMagnitude() const;

	void Normilize(); 

	static double DotProduct(const Vector3d& v1, const Vector3d& v2);

	static Vector3d CrossProduct(const Vector3d& v1, const Vector3d& v2); 

	static Vector3d FromString(const std::string& s);

	static Vector3d Reflect(const Vector3d& v1, const Vector3d& v2);
public:
     double x;

     double y;
     
	  double z;
private:
   
};

}// namespace MathUtils