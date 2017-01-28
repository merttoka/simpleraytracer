#ifndef _SHADEREC_H
#define _SHADEREC_H

class Material;
class World;

#include "Vector3D.hpp"

class ShadeRec {
	public:
	
		bool				hit_an_object;		// Did the ray hit an object?
		Vector3D			o;					// Ray's origin
		Vector3D			d;					// Ray's direction
		float				t;					// ray parameter
		Vector3D			normal;				// Normal at hit point
		Material* 			material_ptr;		// Pointer to the nearest object's material
		Vector3D			hit_point;			// World coordinates of intersection
		World&				w;					// World reference
		Vector3D			local_hit_point;	// World coordinates of hit point on generic object (used for texture transformations)

		int					depth;				// recursion depth
		
				
		ShadeRec(World& wr)
			: 	hit_an_object(false),
				material_ptr(NULL),
				hit_point(),
				local_hit_point(),
				normal(),
				o(),
				d(),
				depth(0),
				t(0.0),
				w(wr)
		{}

		ShadeRec(const ShadeRec& sr)
			: 	hit_an_object(sr.hit_an_object),
				material_ptr(sr.material_ptr),
				hit_point(sr.hit_point),
				local_hit_point(sr.local_hit_point),
				normal(sr.normal),
				o(sr.o),
				d(sr.d),
				depth(sr.depth),
				t(sr.t),
				w(sr.w)
		{}

		/*ShadeRec::~ShadeRec() {
			if (material_ptr) {
				delete material_ptr;
				material_ptr = NULL;
			}
		}*/
};

#endif