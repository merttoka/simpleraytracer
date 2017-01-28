#ifndef _WORLD_H
#define _WORLD_H

#include "tvector.h"
#include <math.h>

#include "Vector3D.hpp"
#include "Objects.h"
#include "Camera.h"
#include "Light.h"
#include "BMP.h"
using namespace std;

class World {	
public:
	int							numSampler;
	double						invNumSampler;
	int							sqrtNumSampler;
	double						invSqrtNumSampler;

	Vector3D					background_color;
	Light*   					ambient_ptr;
	Camera*						camera_ptr;
	BMP*						image_ptr;
	tvector<Objects*>			objects;		
	tvector<Light*> 			lights;
		
public:
	
	World(char* name, const int& width, const int& height, const int& ns = 1)
		:  	background_color(black),
			camera_ptr(new Camera),
			ambient_ptr(new PointLight),
			image_ptr(new BMP(name, width, height)),
			numSampler(ns)
	{
		sqrtNumSampler = (int)sqrt((float)numSampler);
		invNumSampler  = 1.0/numSampler;
		invSqrtNumSampler = 1.0/sqrtNumSampler;
	}												
		
	~World(void) {
		if (ambient_ptr) {
			delete ambient_ptr;
			ambient_ptr = NULL;
		}

		if (camera_ptr)	 {
			delete camera_ptr;
			camera_ptr = NULL;
		}
	
		for (int j = 0; j < objects.size(); j++) {
			delete objects[j];
			objects[j] = NULL;
		}
		for (int j = 0; j < lights.size(); j++) {
			delete lights[j];
			lights[j] = NULL;
		}				
	}
								
	void add_object(Objects* object_ptr)	{  
		objects.push_back(object_ptr);	
	}

	void add_light(Light* light_ptr){  
		lights.push_back(light_ptr);
	} 
		
	void set_ambient_light(Light* light_ptr) {
		ambient_ptr = light_ptr;
	}

	void set_camera(Camera* cam_ptr)	{
		camera_ptr = cam_ptr;
	}

	void set_sampler(const int& i) {
		numSampler = i;
		sqrtNumSampler = (int)sqrt((float)i);
		invNumSampler  = 1.0/i;
		invSqrtNumSampler = 1.0/sqrtNumSampler;
	}	
};

#endif
