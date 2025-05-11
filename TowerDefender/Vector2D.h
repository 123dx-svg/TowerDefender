#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

#include <cmath>
class Vector2D
{
public:
	double x=0, y=0;
public:
	Vector2D()=default;
	~Vector2D()=default;
	Vector2D(double x,double y):x(x),y(y){};

	Vector2D operator+(const Vector2D& other) const
	{
		return Vector2D(x+other.x,y+other.y);
	}
	Vector2D operator-(const Vector2D& other) const
	{
		return Vector2D(x-other.x,y-other.y);
	}
	Vector2D operator*(double scalar) const
	{
		return Vector2D(x*scalar,y*scalar);
	}
	double operator*(const Vector2D& other) const
	{
		return x*other.x+y*other.y;
	}
	Vector2D operator/(double scalar) const
	{
		return Vector2D(x/scalar,y/scalar);
	}
	void operator+= (const Vector2D& other)
	{ 
		x+=other.x;
		y+=other.y;
		
	}
	void operator-= (const Vector2D& other)
	{ 
		x-=other.x;
		y-=other.y;
		
	}
	void operator*= (double scalar)
	{ 
		x*=scalar;
		y*=scalar;
		
	}
	void operator/= (double scalar)
	{ 
		x/=scalar;
		y/=scalar;
		
	}
	bool operator==(const Vector2D& other) const
	{
		return (x==other.x && y==other.y);
	}

	bool operator>(const Vector2D& other) const
	{
		return Length() > other.Length();
	}

	bool operator<(const Vector2D& other) const
	{
		return Length() < other.Length();
	}

	double Length() const
	{
		return sqrt(x*x+y*y);
	}
	
	Vector2D Normailize() const
	{
		double length = Length();
		if (length==0)
		 return Vector2D(0,0);

		return Vector2D(x/length,y/length);
	}

	bool ApproxZero() const
	{
		return Length()<0.0001;
	}
};



#endif