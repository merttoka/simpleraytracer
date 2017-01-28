#ifndef _OBJECTS_H
#define _OBJECTS_H

#include "Vector3D.hpp"
#include "Material.h"
#include "ShadeRec.h"

class Objects	{
public:
	Objects()	{}

	virtual ~Objects()	{}

	Objects(const Objects& obj) 				
	{
		material = obj.material;
	}

	virtual void set_material(Material* mPtr)	{
		material = mPtr;
	}

	virtual bool Hit(ShadeRec& sr, float min = kEpsilon, float max = 1.0E10) = 0;

	virtual void Normal(ShadeRec& sr) = 0;

	virtual void set_local_hit_point(ShadeRec& sr) = 0;

public:
	Material* material;
};

class Sphere: public Objects	{
public:
	Vector3D c;
	int R;
public:
	Sphere()	{}
	Sphere(Vector3D cen, int rad): Objects(), c(cen), R(rad)
	{}
	virtual ~Sphere() {}

	virtual void set_local_hit_point(ShadeRec& sr)
	{
		sr.local_hit_point.setX(sr.hit_point.x() - c.x());
		sr.local_hit_point.setY((sr.hit_point.y() - c.y())/R);
		sr.local_hit_point.setZ(sr.hit_point.z() - c.z());
	}

	virtual bool Hit(ShadeRec& sr, float min = kEpsilon, float max = 1.0E10)
	{
		float disc = ((sr.d.dot(sr.o-this->c))*(sr.d.dot(sr.o-this->c))) - (sr.d.dot(sr.d))*((sr.o-this->c).dot(sr.o-this->c)-this->R*this->R);

		if(disc > 0)
		{
			float t1 = (-sr.d.dot(sr.o - this->c) + sqrt(disc))/sr.d.dot(sr.d);
			float t2 = (-sr.d.dot(sr.o - this->c) - sqrt(disc))/sr.d.dot(sr.d);

			if(t1 > 0 || t2 > 0)	{
				if((t1 < min) || (t1 > max) || (t2 < min) || (t2 > max))	{
					return false;
				}
				else	{
					if(t1>t2){
						sr.t = t2;
						return true;
					}
					else	{
						sr.t = t1;
						return true;
					}
				}
			}
			else	{
				return false;}
		}
		else if(disc < 0)	{
			return false;
		}
	}

	virtual void Normal(ShadeRec& sr)
	{
		Vector3D n((sr.o.x()+sr.t*sr.d.x() - this->c.x())/this->R,(sr.o.y()+sr.t*sr.d.y() - this->c.y())/this->R,(sr.o.z()+sr.t*sr.d.z() - this->c.z())/this->R);
		n.normalize();
		sr.normal = n;
	}
};

class Plane: public Objects	{
public:
	Vector3D a;
	Vector3D b;
	Vector3D c;
public:
	Plane(Vector3D _a, Vector3D _b, Vector3D _c): Objects(), a(_a),b(_b),c(_c)	{}

	virtual ~Plane()	{}

	virtual void set_local_hit_point(ShadeRec& sr)
	{
		sr.local_hit_point = sr.hit_point;
	}

	virtual bool Hit(ShadeRec& sr, float min = kEpsilon, float max = 1.0E10)	{
		float _a = this->a.x()-this->b.x(),		_b = this->a.y()-this->b.y(),	_c = this->a.z()-this->b.z();
		float _d = this->a.x()-this->c.x(),		_e = this->a.y()-this->c.y(),	_f = this->a.z()-this->c.z();
		float _g = sr.d.x(),					_h = sr.d.y(),				_i = sr.d.z();
		float _j = this->a.x()-sr.o.x(),		_k = this->a.y()-sr.o.y(),		_l = this->a.z()-sr.o.z();

		float t_val =-(_f*(_a*_k - _j*_b) + _e*(_j*_c - _a*_l) + _d*(_b*_l - _k*_c))/
					  (_a*(_e*_i - _h*_f) + _b*(_g*_f - _d*_i) + _c*(_d*_h - _e*_g));
		if((t_val < min) || (t_val > max))	{
			return false;
		}

		sr.t = t_val;
		return true;
	}

	virtual void Normal(ShadeRec& sr)	{
		Vector3D normal((b-c).cross(a-c));
		normal.normalize();
		sr.normal = -normal;
	}
};

class Tri: public Objects	{
public:
	Vector3D a;
	Vector3D b;
	Vector3D c;
public:
	Tri(Vector3D _a, Vector3D _b, Vector3D _c): Objects(), a(_a),b(_b),c(_c)
	{}

	virtual ~Tri() {}

	virtual void set_local_hit_point(ShadeRec& sr)
	{
		sr.local_hit_point = sr.hit_point;
	}

	virtual bool Hit(ShadeRec& sr, float min = kEpsilon, float max = 1.0E10)
	{
		float _a = this->a.x()-this->b.x(),		_b = this->a.y()-this->b.y(),	_c = this->a.z()-this->b.z();
		float _d = this->a.x()-this->c.x(),		_e = this->a.y()-this->c.y(),	_f = this->a.z()-this->c.z();
		float _g = sr.d.x(),					_h = sr.d.y(),				_i = sr.d.z();
		float _j = this->a.x()-sr.o.x(),		_k = this->a.y()-sr.o.y(),		_l = this->a.z()-sr.o.z();

		float t_val =-(_f*(_a*_k - _j*_b) + _e*(_j*_c - _a*_l) + _d*(_b*_l - _k*_c))/
					  (_a*(_e*_i - _h*_f) + _b*(_g*_f - _d*_i) + _c*(_d*_h - _e*_g));
		if((t_val < min) || (t_val > max))	{
			return false;
		}

		float gamma = (_i*(_a*_k - _j*_b) + _h*(_j*_c - _a*_l) + _g*(_b*_l - _k*_c))/
					  (_a*(_e*_i - _h*_f) + _b*(_g*_f - _d*_i) + _c*(_d*_h - _e*_g));
		if((gamma < 0) || (gamma > 1))	{
			return false;
		}

		float beta = (_j*(_e*_i - _h*_f) + _k*(_g*_f - _d*_i) + _l*(_d*_h - _e*_g))/
					 (_a*(_e*_i - _h*_f) + _b*(_g*_f - _d*_i) + _c*(_d*_h - _e*_g));
		if ((beta < 0) || (beta > 1 - gamma))	{
			return false;
		}

		sr.t = t_val;
		return true;
	}

	virtual void Normal(ShadeRec& sr)
	{
		Vector3D h(sr.o.x()+sr.t*sr.d.x(), sr.o.y()+sr.t*sr.d.y(), sr.o.z()+sr.t*sr.d.z());
		Vector3D temp((b-h).cross(c-h));
		temp.normalize();
		sr.normal = temp;
	}
};

class Rectangle: public Objects
{
public:
	//const float kEpsilon = 0.001;

	Rectangle() : 	Objects(),
		p0(-1, 0, -1), 
		a(0, 0, 2), b(2, 0, 0), 
		a_len_squared(4.0), 
		b_len_squared(4.0)
	{}	 									
				
	Rectangle(const Vector3D& _p0, const Vector3D& _a, const Vector3D& _b) :	Objects(),
		p0(_p0),
		a(_a),
		b(_b),
		a_len_squared(a.length()*a.length()), 
		b_len_squared(b.length()*b.length())
	{}
	
	Rectangle(const Rectangle& r) :	Objects(r),
		p0(r.p0), 
		a(r.a),
		b(r.b),
		a_len_squared(r.a_len_squared), 
		b_len_squared(r.b_len_squared)
	{}					

	virtual	~Rectangle(void)	{}   									
	
	virtual void Normal(ShadeRec& sr)	{
		Vector3D temp(a .cross( b ));
		temp.normalize();
		sr.normal = temp;
	}

	virtual bool Hit(ShadeRec& sr, float min = kEpsilon, float max = 1.0E10) {
		Vector3D temp(a .cross( b ));
		temp.normalize();
		Vector3D normal = temp;

		double t = (p0 - sr.o).dot(normal)/ (sr.d.dot(normal));
	
		if (t <= kEpsilon)
			return (false);
			
		Vector3D p = sr.o + sr.d * t;
		Vector3D d = p - p0;
	
		double ddota = d.dot(a);
	
		if (ddota < 0.0 || ddota > a_len_squared)
			return (false);
		
		double ddotb = d.dot(b);
	
		if (ddotb < 0.0 || ddotb > b_len_squared)
			return (false);
		
		sr.t = t;
		return (true);
	}

	virtual void set_local_hit_point(ShadeRec& sr) {
		sr.local_hit_point = sr.hit_point;
	}
		
private:
	Vector3D p0;   			// corner vertex 
	Vector3D a;				// side
	Vector3D b;				// side
	double a_len_squared;	// square of the length of side a
	double b_len_squared;	// square of the length of side b

	//static const float kEpsilon;
};

class OpenCylinder: public Objects {
public:
	OpenCylinder() : 	Objects(),
			y0(-1.0),
			y1(1.0),
			radius(1.0),
			inv_radius(1.0)			
	{}							

	OpenCylinder(const double bottom, const double top, const double r) :  	Objects(),
			y0(bottom),
			y1(top),	
			radius(r),
			inv_radius(1.0 / radius)
	{}

	OpenCylinder(const OpenCylinder& c) : 	Objects(c),
			y0(c.y0),
			y1(c.y1),
			radius(c.radius), 
			inv_radius(c.inv_radius) 			
	{}						
		
	virtual ~OpenCylinder() {}
						
	virtual void Normal(ShadeRec& sr)	{
		if (-sr.d.dot(sr.normal) >= 0.0)
			sr.normal = Vector3D((sr.o.x() + sr.t * sr.d.x())/radius, 0.0, (sr.o.z() + sr.t * sr.d.z())/radius);
		else
			sr.normal = Vector3D(-(sr.o.x() + sr.t * sr.d.x())/radius, 0.0, -(sr.o.z() + sr.t * sr.d.z())/radius);
	}

	virtual bool Hit(ShadeRec& sr, float min = kEpsilon, float max = 1.0E10) {
		double t;
		double ox = sr.o.x();
		double oy = sr.o.y();
		double oz = sr.o.z();
		double dx = sr.d.x();
		double dy = sr.d.y();
		double dz = sr.d.z();
	
		double a = dx * dx + dz * dz;  	
		double b = 2.0 * (ox * dx + oz * dz);					
		double c = ox * ox + oz * oz - radius * radius;
		double disc = b * b - 4.0 * a * c ;

		if (disc < 0.0)
			return(false);
		else {	
			double e = sqrt(disc);
			double denom = 2.0 * a;
			t = (-b - e) / denom;    // smaller root
		
			if (t > kEpsilon) {
				double yhit = oy + t * dy;
			
				if (yhit > y0 && yhit < y1) {
					sr.t = t;
					sr.normal = Vector3D((ox + t * dx) * inv_radius, 0.0, (oz + t * dz) * inv_radius);
				
					// test for hitting from inside
			
					if (-sr.d.dot(sr.normal) < 0.0)
						sr.normal = -sr.normal;
					return (true);
				}
			} 
		
			t = (-b + e) / denom;    // larger root
		
			if (t > kEpsilon) {
				double yhit = oy + t * dy;
			
				if (yhit > y0 && yhit < y1) {
					sr.t = t;
					sr.normal = Vector3D((ox + t * dx) * inv_radius, 0.0, (oz + t * dz) * inv_radius);
				
					// test for hitting inside surface
			
					if (-sr.d.dot(sr.normal) < 0.0)
						sr.normal = -sr.normal;
					return (true);
				}
			} 
		}
	
		return (false);			
	}

	virtual void set_local_hit_point(ShadeRec& sr) {
		sr.local_hit_point.setX(sr.hit_point.x() * inv_radius);
		sr.local_hit_point.setY((1-2*(y1-sr.hit_point.y())/(y1-y0)));
		sr.local_hit_point.setZ(sr.hit_point.z() * inv_radius);
	}
		
protected:
	
	double		y0;				// bottom y value
	double		y1;				// top y value
	double		radius;			// radius
	double		inv_radius;  	// one over the radius	
};

#endif