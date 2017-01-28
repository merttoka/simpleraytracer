#ifndef _MAPPING_H
#define _MAPPING_H

class Mapping
{
public:
	Mapping() {}

	virtual ~Mapping()	{}

	virtual void get_texel_coordinates (const Vector3D& local_hit_point, 
										const int hres,
										const int vres,
										int& row,
										int& column) const = 0;
};

class SphericalMap: public Mapping
{
public:
	SphericalMap(): Mapping()	{}
	virtual ~SphericalMap()	{}

	virtual void get_texel_coordinates (const Vector3D& local_hit_point, 
										const int hres,
										const int vres,
										int& row,
										int& column) const	
	{
		float theta = acos(local_hit_point.y());
		float phi   = atan2(local_hit_point.x(), local_hit_point.z());

		if(phi < 0.0)
			phi += 2*PI;

		float u = phi/(2*PI);
		float v = (PI - theta)/PI;

		column = (int)((hres - 1) * u); 
		row = (int)((vres - 1) * v); 
	}
};

class RectangularMapping: public Mapping
{
public:
	RectangularMapping(): Mapping()	{}
	virtual ~RectangularMapping()	{}

	virtual void get_texel_coordinates (const Vector3D& local_hit_point, 
										const int hres,
										const int vres,
										int& row,
										int& column) const	
	{
		float u = (local_hit_point.z() + 1) * 0.5;
		float v = (local_hit_point.x() + 1) * 0.5;

		column = (int)((hres - 1) * u); 
		row = (int)((vres - 1) * v); 
	}
};

class CylindricalMapping: public Mapping
{
public: 
	CylindricalMapping(): Mapping()	{}
	virtual ~CylindricalMapping()	{}

	virtual void get_texel_coordinates (const Vector3D& local_hit_point, 
										const int hres,
										const int vres,
										int& row,
										int& column) const	
	{
		float phi = atan(local_hit_point.x() / local_hit_point.z());

		if(phi < 0.0)
			phi += PI/2;

		float u = phi * invTWO_PI; 
		float v = (local_hit_point.y() + 1) * 0.5;

		column = (int)((hres - 1) * u); 
		row = (int)((vres - 1) * v); 
	}

};

#endif