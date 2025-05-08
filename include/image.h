#ifndef IMAGE_H
#define IMAGE_H

#include <fstream>
#include "color.h"


class Image {
protected:
	unsigned char* data;
	int width;
	int height;
	int bytespp;

public:
	enum Format {
		GRAYSCALE=1, RGB=3, RGBA=4
	};

	Image();
    Image(int w, int h, int bpp);
	Image(const char *filename);
	Image(const Image &img);
    void read_file(const char *filename);
	void write_png_file(const char *filename);
	Color get(int x, int y);
	bool set(int x, int y, const Color &c);
	~Image();
	int get_width();
	int get_height();
	int get_bytespp();
	unsigned char *buffer();
	void clear();
    bool flip_horizontally();
    bool flip_vertically();
};

#endif //__IMAGE_H__
