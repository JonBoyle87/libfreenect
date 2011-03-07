#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include "libfreenect_cv.h"

IplImage *GlViewColor(IplImage *depth)
{
	static IplImage *image = 0;
	unsigned char *depth_mid;
	int i;

	if (!image) image = cvCreateImage(cvSize(640,480), 8, 3);
	depth_mid = (unsigned char*)(image->imageData);

	if (!image) image = cvCreateImage(cvSize(640,480), 8, 3);
	depth_mid = (unsigned char *) image->imageData;
	for (i = 0; i < 640*480; i++) {
		int lb = ((short *)depth->imageData)[i] % 256;
		int ub = ((short *)depth->imageData)[i] / 256;
		switch (ub) {
			case 0:
				depth_mid[3*i+2] = 255;
				depth_mid[3*i+1] = 255-lb;
				depth_mid[3*i+0] = 255-lb;
				break;
			case 1:
				depth_mid[3*i+2] = 255;
				depth_mid[3*i+1] = lb;
				depth_mid[3*i+0] = 0;
				break;
			case 2:
				depth_mid[3*i+2] = 255-lb;
				depth_mid[3*i+1] = 255;
				depth_mid[3*i+0] = 0;
				break;
			case 3:
				depth_mid[3*i+2] = 0;
				depth_mid[3*i+1] = 255;
				depth_mid[3*i+0] = lb;
				break;
			case 4:
				depth_mid[3*i+2] = 0;
				depth_mid[3*i+1] = 255-lb;
				depth_mid[3*i+0] = 255;
				break;
			case 5:
				depth_mid[3*i+2] = 0;
				depth_mid[3*i+1] = 0;
				depth_mid[3*i+0] = 255-lb;
				break;
			default:
				depth_mid[3*i+2] = 0;
				depth_mid[3*i+1] = 0;
				depth_mid[3*i+0] = 0;
				break;
		}
	}
	return image;
}

//Do any processing you want on the images here, or rewrite as you wish.
IplImage* ProcessRGB(IplImage* rgb)
{
	unsigned char *r,*g,*b;
	int x,y,p;
	int index;

	unsigned char *data = (unsigned char *)rgb->imageData;

	/* There are two ways that you can access the data. The first way is a very slow way of doing things, as you have to
	   calculate the index every time and have two loops to go through. This however can be easier to understand, and the
	   equation for the index can be used to access the co-ordinates anywhere by substituting the y and x variables. 
	   
	   Make sure to remove what you aren't using.
	*/
	for(y = 0; y < 480; y++)
	{
		for(x = 0; x < 640; x++)
		{
			index = 3 * (y * 640 + x);

			r = &data[index]; 
			g = &data[index + 1];
			b = &data[index + 2];

			*r = *r; //The data can be accessed here
			*g = *g;
			*b = *b;
		}
	}

	/* The second way is a much faster way of doing things, but involves using pointers. As you don't have to recalculate
	   the index every time, only adding 1 each time which is a fast operation, it's a lot faster. The order the pixels are stored are
	   by row e.g. If there the image were 3 x 3 (x,y):

	   (0,0) (1,0) (2,0) (0,1) (1,1) (2,1) (0,2) (1,2) (2,2)
	*/

	for(p = 0; p < 640 * 480; p++)
	{
		r = data++; 
		g = data++;
		b = data++;

		*r = *r; //The data can be accessed here
		*g = *g;
		*b = *b;
	}

	return rgb;
}

IplImage* ProcessDepth(IplImage* depth)
{
	int x,y,p,index;
	unsigned char *v;
	unsigned char *data = (unsigned char *)depth->imageData;

	//This function turns the depth image into colour. It's probably best that you do the processing on the original image rather than the colour.
	IplImage* Colour = GlViewColor(depth);

	/* If you want to do your processing on the colour image, then use the example of code from the ProcessRGB function. Otherwise, you'll
	   be wanting to use what's below. The main difference here is that the depth image is Grayscale, so it only has one value per pixel.
	   The first way is the slow way, and the second way is the fast way.

	   Make sure to remove what you aren't using.
	*/

	for(y = 0; y < 480; y++)
	{
		for(x = 0; x < 640; x++)
		{
			index = 3 * (y * 640 + x);

			v = &data[index]; 

			*v = *v; //The data can be accessed here
		}
	}

	for(p = 0; p < 640 * 480; p++)
	{
		v = data++;

		*v = *v; //The data can be accessed here
	}

	return Colour;
}

int main(int argc, char **argv)
{
	IplImage *depth;
	IplImage *image;

	IplImage *displayedDepth;
	IplImage *displayedImage;

	while (cvWaitKey(10) < 0) {
		image = freenect_sync_get_rgb_cv(0);
		if (!image) {
		    printf("Error: Kinect not connected?\n");
		    return -1;
		}
		cvCvtColor(image, image, CV_RGB2BGR);
		depth = freenect_sync_get_depth_cv(0);
		if (!depth) {
		    printf("Error: Kinect not connected?\n");
		    return -1;
		}

		displayedImage = ProcessRGB(image);
		displayedDepth = ProcessDepth(depth);

		cvShowImage("RGB", displayedImage);
		cvShowImage("Depth", displayedDepth);
	}
	return 0;
}
