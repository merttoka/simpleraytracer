#include "tmatrix.h"
#include "Vector3D.hpp"

#ifndef _BMP_H
#define _BMP_H

class BMP {
public:
	BMP(char* name, const int& width, const int& height): file_name(name), w(width), h(height) {
		red.resize(width,height);
		green.resize(width,height);
		blue.resize(width,height);
	}

	BMP(const BMP& rhs)
	{
		w = rhs.w; h = rhs.h;
		file_name = rhs.file_name;
		red = rhs.red; green = rhs.green; blue = rhs.blue;
	}

	void SetColorPixel(int x, int y, float r, float g, float b)	{red[x][y]   = r;
																 green[x][y] = g;
																 blue[x][y]  = b;}

	void SetColorPixel(int x, int y, const Vector3D& color)			{red[x][y]   = color.x();
																	 green[x][y] = color.y();
																	 blue[x][y]  = color.z();}

	void SetColorAll(float r=1.0, float g=1.0, float b=1.0)
	{
		for (int row=0; row<w; row++) {
			for (int col=0; col<h; col++) {
				red[row][col] = r;
				green[row][col] = g;
				blue[row][col] = b;
			}
		}
	}

	void SetName(char* name)
	{
		file_name = name;
	}

	void CreateImage()
	{
		FILE *f;
		int x, y;
		int yres = h;
		int r,g,b;
		unsigned char *img = NULL;
		int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int
		if( img )
			free( img );
		img = (unsigned char *)malloc(3*w*h);
		memset(img,0,sizeof(img));

		for(int i=0; i<w; i++)
		{
			for(int j=0; j<h; j++)
			{
				x=i; 
				y=(yres-1)-j;
				r = red[i][j]*255;
				g = green[i][j]*255;
				b = blue[i][j]*255;
				if (r > 255) r=255;
				if (g > 255) g=255;
				if (b > 255) b=255;
				img[(x+y*w)*3+2] = (unsigned char)(r);
				img[(x+y*w)*3+1] = (unsigned char)(g);
				img[(x+y*w)*3+0] = (unsigned char)(b);
			}
		}

		unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
		unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
		unsigned char bmppad[3] = {0,0,0};

		bmpfileheader[ 2] = (unsigned char)(filesize    );
		bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
		bmpfileheader[ 4] = (unsigned char)(filesize>>16);
		bmpfileheader[ 5] = (unsigned char)(filesize>>24);

		bmpinfoheader[ 4] = (unsigned char)(       w    );
		bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
		bmpinfoheader[ 6] = (unsigned char)(       w>>16);
		bmpinfoheader[ 7] = (unsigned char)(       w>>24);
		bmpinfoheader[ 8] = (unsigned char)(       h    );
		bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
		bmpinfoheader[10] = (unsigned char)(       h>>16);
		bmpinfoheader[11] = (unsigned char)(       h>>24);

		f = fopen(file_name,"wb");
		fwrite(bmpfileheader,1,14,f);
		fwrite(bmpinfoheader,1,40,f);
		for(int i=0; i<h; i++)
		{
			fwrite(img+(w*(h-i-1)*3),3,w,f);
			fwrite(bmppad,1,(4-(w*3)%4)%4,f);
		}
		fclose(f);
	}

private:
	char* file_name;
	int w;
	int h;
	tmatrix<float> red;
	tmatrix<float> green;
	tmatrix<float> blue;
};

class Image
{
public:
	Image()	{}

	void read_bmp_file(char* filename)
	{
		int i;
		FILE* f = fopen(filename, "rb");
		unsigned char info[54];
		fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

		// extract image height and width from header
		width = *(int*)&info[18];
		height = *(int*)&info[22];

		int size = 3 * width * height;
		data = new unsigned char[size]; // allocate 3 bytes per pixel
		fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
		fclose(f);

		for(i = 0; i < size; i += 3)
		{
			unsigned char tmp = data[i];
			data[i] = data[i+2];
			data[i+2] = tmp;
		}
		/*Now data should contain the (R, G, B) values of the pixels. The color of pixel (i, j) is stored at 
		data[j * 3* width + 3 * i], data[j * 3 * width + 3 * i + 1] and data[j * 3 * width + 3*i + 2].

		In the last part, the swap between every first and third pixel is done because windows stores the 
		color values as (B, G, R) triples, not (R, G, B).*/
	}

public:
	int width;
	int height;
	unsigned char* data;
};

#endif
