#include <string.h>
#include "tvector.h"

#include <math.h>   //Had to use in order to take square roots of numbers.
#include "Maths.h"
#include "Constants.h"

#include "BMP.h"
#include "Vector3D.hpp"
#include "Objects.h"
#include "Material.h"
#include "ShadeRec.h"
#include "Light.h"
#include "Camera.h"
#include "World.h"

#include "tinyxml\tinyxml.h"
#include "Timing\ctimer.h"
#include <fstream>
#include <sstream>

// Default initializations of some scene entities.
int		  W			   = 100;
int		  H			   = 100;
bool	  CAST_SHADOWS = 1;
int		  SAMPLER      = 1;
char*	  FILENAME;

template<class T>
float Convert(T c)
{
	stringstream strValue;
	strValue << c;
	float intValue;
	strValue >> intValue;

	return intValue;
}

Vector3D ConvertVec(const char* c)
{
	float x,y,z;

	stringstream strValue;
	strValue << c;

	std::string s;
	strValue >> s;

	int index = 0;
	int index_space = s.find(',');
	x = Convert(s.substr(index, index_space));
	index = index_space;
	index_space = s.find(',', index+1);
	y = Convert(s.substr(index + 1, index_space-index-1));
	index = index_space;
	z = Convert(s.substr(index+1,s.length()));

	return Vector3D(x, y, z);
}

int main(int argc, char* argv[])
{
	// Read command line command
	if(argc > 0){
		FILENAME = argv[1];
	}
	
	CTimer time;
	time.Start();

	///// -XML READING- ///////////
	TiXmlDocument doc("..\\world.xml");
	if (doc.LoadFile())	{cout << "DEBUG: Successfully loaded file 'world.xml'" << endl;}
	else				{cout << "ERROR: Failed to load file 'world.xml'" << endl; exit(0);}
	
	TiXmlElement *pRoot, *pParm, *pFeat;
	pRoot = doc.FirstChildElement("scene");

	// creating the world according to width and height data
	pParm = pRoot->FirstChildElement("image");
	W = Convert(pParm->Attribute("width"));
	H = Convert(pParm->Attribute("height"));
	World wr(FILENAME,W,H); 

	// setting sampler value of world
	pParm = pParm->NextSiblingElement("sampler");
	SAMPLER = Convert(pParm->Attribute("value"));
	wr.set_sampler(SAMPLER); 

	// setting the camera's eye and lookat values
	pParm = pParm->NextSiblingElement("camera"); 
	wr.set_camera(new Camera(ConvertVec(pParm->Attribute("eye")), ConvertVec(pParm->Attribute("lookat")), ConvertVec(pParm->Attribute("up"))));

	// setting background color
	pParm = pParm->NextSiblingElement("back_color");
	wr.background_color = ConvertVec(pParm->Attribute("rgb"));

	// setting ambient lights position and intensity
	pParm = pParm->NextSiblingElement("ambient");
	wr.set_ambient_light(new PointLight(ConvertVec(pParm->Attribute("position")), ConvertVec(pParm->Attribute("intensity"))));

	// adding point light source(s)
	pParm = pParm->NextSiblingElement("light");
	pFeat  = pParm->FirstChildElement("P");
	while(pFeat){
		wr.add_light(new PointLight(ConvertVec(pFeat->Attribute("position")), ConvertVec(pFeat->Attribute("color"))));
		pFeat = pFeat->NextSiblingElement();
	}
	pFeat = pParm->FirstChildElement("A");
	while(pFeat){
		wr.add_light(new FakeAreaLight(ConvertVec(pFeat->Attribute("position")), ConvertVec(pFeat->Attribute("color")), Convert(pFeat->Attribute("radius"))));
		pFeat = pFeat->NextSiblingElement();
	}

	// objects and material setting
	  // There are also texture mappings implemented, but not included in xml parser.
	  // Example use: Texture* t1 = new Texture("Texture\\earthmap2.bmp");
	  //              SphericalMap* sm = new SphericalMap();
	  //              t1->set_mapping(sm);
	pParm = pParm->NextSiblingElement("objects");
	pFeat = pParm->FirstChildElement("surface");
	while(pFeat)
	{
		TiXmlElement *pIn = pFeat->FirstChildElement("material");
		const char* charValue = pFeat->Attribute("type");

		stringstream strValue;
		strValue << charValue;
		std::string name;
		strValue >> name;

		if(name == "sphere")
		{
			Matte* m = new Matte();
			m->set_color(ConvertVec(pIn->Attribute("color")));
			m->set_ka(Convert(pIn->Attribute("ka")));
			m->set_ks(Convert(pIn->Attribute("ks")));

			pIn = pIn->NextSiblingElement();
			Sphere* s = new Sphere(ConvertVec(pIn->Attribute("center")), Convert(pIn->Attribute("radius")));
			s->set_material(m);
			wr.add_object(s);
		}
		else if(name == "plane")
		{
			Matte* m2 = new Matte();
			m2->set_color(ConvertVec(pIn->Attribute("color")));
			m2->set_ka(Convert(pIn->Attribute("ka")));
			m2->set_ks(Convert(pIn->Attribute("ks")));

			pIn = pIn->NextSiblingElement();
			Plane * p = new Plane(ConvertVec(pIn->Attribute("v1")), ConvertVec(pIn->Attribute("v2")), ConvertVec(pIn->Attribute("v3")));
			p->set_material(m2);
			wr.add_object(p);
		}
		else if(name == "triangle")
		{
			Matte* m3 = new Matte();
			m3->set_color(ConvertVec(pIn->Attribute("color")));
			m3->set_ka(Convert(pIn->Attribute("ka")));
			m3->set_ks(Convert(pIn->Attribute("ks")));

			pIn = pIn->NextSiblingElement();
			Tri * t = new Tri(ConvertVec(pIn->Attribute("v1")), ConvertVec(pIn->Attribute("v2")), ConvertVec(pIn->Attribute("v3")));
			t->set_material(m3);
			wr.add_object(t);
		}
		else if(name == "rectangle")
		{
			Matte* m4 = new Matte();
			m4->set_color(ConvertVec(pIn->Attribute("color")));
			m4->set_ka(Convert(pIn->Attribute("ka")));
			m4->set_ks(Convert(pIn->Attribute("ks")));

			pIn = pIn->NextSiblingElement();
			Rectangle * r = new Rectangle(ConvertVec(pIn->Attribute("p0")), ConvertVec(pIn->Attribute("a")), ConvertVec(pIn->Attribute("b")));
			r->set_material(m4);
			wr.add_object(r);
		}
		else if(name == "cylinder")
		{
			Matte* m5 = new Matte();
			m5->set_color(ConvertVec(pIn->Attribute("color")));
			m5->set_ka(Convert(pIn->Attribute("ka")));
			m5->set_ks(Convert(pIn->Attribute("ks")));

			pIn = pIn->NextSiblingElement();
			OpenCylinder * c = new OpenCylinder(Convert(pIn->Attribute("bottom")), Convert(pIn->Attribute("top")), Convert(pIn->Attribute("radius")));
			c->set_material(m5);
			wr.add_object(c);
		}
		pFeat = pFeat->NextSiblingElement("surface");
	}
	// - END OF READING FILE - ////////////////////////////

	cout << "DEBUG: Ray-tracing: " << endl; // For console view
	int l = -W/2, r=W/2, t=H/2, b=-H/2;
	for(int i = 0; i < W; i++)
	{
		for(int j = 0; j < H; j++)
		{
			if((i*H+j) % int(W/100) == 0) 
			{
				cout << "[ " <<int((i*H+j)*100/(W*H)) << "% ]\r";
				cout.flush();
			}
			float u = l + ((r - l) * (i + 0.5)) / W;
			float v = b + ((t - b) * (j + 0.5)) / H;

			// -------------------------------------- Anti-Alising ---------
			Vector3D aa;	
			double v_new = (v - 0.5) + wr.invSqrtNumSampler/2;
			int c_y = 0;
			while(c_y < wr.sqrtNumSampler)
			{
				double u_new = (u - 0.5) + wr.invSqrtNumSampler/2;
				int c_x = 0;
				while(c_x < wr.sqrtNumSampler)	
				{	// -----------------------------------------------------
					Vector3D e = wr.camera_ptr->camera_position;
					Vector3D d = wr.camera_ptr->ray_direction(u_new,v_new);

					ShadeRec sr(wr);
					sr.o = e; 
					sr.d = d;

					float t1 = kHugeValue;
					Objects* hitObject;
					int num_obj = wr.objects.size();
					for(int a = 0; a < num_obj; a++)	// Object intersection
						if(wr.objects[a]->Hit(sr, kEpsilon, t1))	{
							sr.hit_an_object = true;
							hitObject = wr.objects[a];
							t1 = sr.t;
						}

					if(sr.hit_an_object)	{
						sr.material_ptr = hitObject->material;
						Vector3D a(-sr.t*d.x(),-sr.t*d.y(), -sr.t*d.z());  a.normalize();
						Vector3D p(e.x()+sr.t*d.x(), e.y()+sr.t*d.y(), e.z()+sr.t*d.z());  sr.hit_point = p;
						hitObject->Normal(sr);

						hitObject->set_local_hit_point(sr);// For Textures

						double xShaded = 0, yShaded = 0, zShaded = 0;

						xShaded += sr.material_ptr->ka * wr.ambient_ptr->intensity.x();
						yShaded += sr.material_ptr->ka * wr.ambient_ptr->intensity.y();
						zShaded += sr.material_ptr->ka * wr.ambient_ptr->intensity.z();

						for(int i = 0; i < wr.lights.size(); i++)	//----- Lights
						{
							Vector3D l = wr.lights[i]->get_direction(sr);
							l.normalize();
							Vector3D h = l + (-a);	h.normalize();

							ShadeRec sr_shadow(wr);
							if(CAST_SHADOWS)	{				//----- Shadows
								Objects* hitObjectShadow;
								sr_shadow.o = p;	sr_shadow.d = l;	sr_shadow.t = kEpsilon;

								for(int a = 0; a < num_obj; a++)	{
									if(wr.objects[a]->Hit(sr_shadow) && sr_shadow.t < sr.o.getDistance(wr.lights[i]->position))	{
										sr_shadow.hit_an_object = true;
										hitObjectShadow = wr.objects[a];
									}
								}
							}

							if(!sr_shadow.hit_an_object)
							{
								Vector3D in = wr.lights[i]->intensity;

								xShaded +=  sr.material_ptr->get_color(sr).x() * in.x() * max(0.0, sr.normal.dot(l)) + 
											sr.material_ptr->ks * in.x() * pow((max(0.0,sr.normal.dot(h))),1000);
								yShaded +=  sr.material_ptr->get_color(sr).y() * in.y() * max(0.0, sr.normal.dot(l)) + 
						   					sr.material_ptr->ks * in.y() * pow((max(0.0,sr.normal.dot(h))),1000);
								zShaded +=  sr.material_ptr->get_color(sr).z() * in.z() * max(0.0, sr.normal.dot(l)) + 
											sr.material_ptr->ks * in.z() * pow((max(0.0,sr.normal.dot(h))),1000);
							}
						}

						aa.setX(aa.x() + wr.invNumSampler*xShaded);
						aa.setY(aa.y() + wr.invNumSampler*yShaded);
						aa.setZ(aa.z() + wr.invNumSampler*zShaded);
					}
					else	{
						aa.setX(aa.x() + wr.invNumSampler*wr.background_color.x());
						aa.setY(aa.y() + wr.invNumSampler*wr.background_color.y());
						aa.setZ(aa.z() + wr.invNumSampler*wr.background_color.z());
					}

					c_x++;
					u_new += wr.invSqrtNumSampler;
				}
				c_y++;
				v_new += wr.invSqrtNumSampler;
			}
			wr.image_ptr->SetColorPixel((int)(floor(u)+W/2),(int)(floor(v)+H/2), aa);
		}	
	}

	cout << "[ 100% ]";
	cout << endl << "DEBUG: " <<FILENAME << " is in directory folder.\n";
	wr.image_ptr->CreateImage();

	time.Stop();

	int h, m; double s;
	time.ElapsedTimeNormalized(h, m, s);
	ofstream out("..\\timing\\timelog.txt", ios::app);
	out.precision(4);
	out << "AAx"<< wr.numSampler << ":\t" << h << " hour, " << m << " minutes, " << s << " seconds." << endl;
	out.close();
	
	return 0;
}