#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "Vector3D.hpp"
#include "Mapping.h"
#include "ShadeRec.h"

class Material
{
public:
	Material()	{}			
	Material(const Material& material) {}
	virtual ~Material()	{}
	
	Material& operator= (const Material& rhs)
	{
		if (this == &rhs)
			return (*this);

		return (*this);
	}

	void set_ka(const float k)	{ka=k;}
	void set_ks(const float k)	{ks=k;}

	void set_color(const Vector3D c)	{color = c;}
	void set_color(const float r, const float g, const float b)	{color.setX(r); color.setY(g); color.setZ(b);}
	void set_color(const float c)	{color.setX(c); color.setY(c); color.setZ(c);}

	virtual Vector3D get_color(const ShadeRec& sr) = 0;

public:
	float ka;
	float ks;
	Vector3D color;
};

class Matte: public Material
{
public:
	Matte(): Material()	{}																			
	Matte& operator= (const Matte& rhs) {
		if (this == &rhs)
			return (*this);

		return (*this);
	}
	virtual ~Matte()	{}											
		
	virtual Vector3D get_color(const ShadeRec& sr)	{return color;}
};

class Texture: public Material
{
public:
	Texture(char* filename): Material()	{
		image_ptr = new Image;
		image_ptr->read_bmp_file(filename);
	}
	virtual ~Texture() {}

	virtual void set_mapping(Mapping* mapping)
	{	mapping_ptr = mapping;}

	virtual Vector3D get_color(const ShadeRec& sr)	{
		int row, col;

		if(mapping_ptr)
			mapping_ptr->get_texel_coordinates(sr.local_hit_point, image_ptr->width, image_ptr->height, row, col);

		return Vector3D (image_ptr->data[row * 3 * image_ptr->width + 3*col  ]/255.0, 
						 image_ptr->data[row * 3 * image_ptr->width + 3*col+1]/255.0,
						 image_ptr->data[row * 3 * image_ptr->width + 3*col+2]/255.0);
	}
public:
	Image* image_ptr;
	Mapping* mapping_ptr;
};

#endif