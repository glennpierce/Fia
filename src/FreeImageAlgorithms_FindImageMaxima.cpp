#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Morphology.h"
#include "FreeImageAlgorithms_FindImageMaxima.h"

#define MAX_REGIONGROW_CALLS 5000
static int	regionGrowCount=0;

static inline int NeighbourhoodNMS(unsigned char *ptr, unsigned int pitch_in_pixels)
{
	unsigned char *tmp_ptr = ptr - pitch_in_pixels - 1;

	if(*ptr < tmp_ptr[0])
		return 0;

	if(*ptr < tmp_ptr[1])
		return 0;

	if(*ptr < tmp_ptr[2])
		return 0;

	// Next kernel line
	tmp_ptr += pitch_in_pixels;

	if(*ptr < tmp_ptr[0])
		return 0;

	if(*ptr < tmp_ptr[2])
		return 0;

	// Next kernel line
	tmp_ptr += pitch_in_pixels;

	if(*ptr < tmp_ptr[0])
		return 0;

	if(*ptr < tmp_ptr[1])
		return 0;

	if(*ptr < tmp_ptr[2])
		return 0;

	return 1;
}

static FIBITMAP* NonMaxSupression(FIBITMAP *src, int threshold)
{
	FIABITMAP *bordered_src = FreeImageAlgorithms_SetBorder(src, 1, 1);

	const int width = FreeImage_GetWidth(bordered_src->fib);
	const int height = FreeImage_GetHeight(bordered_src->fib);

	FIBITMAP *dst = FreeImage_Allocate(width, height, 8, 0, 0, 0);

	assert(dst != NULL);

	const int pitch_in_pixels = FreeImage_GetPitch(dst) / sizeof(unsigned char);

	register unsigned char *src_ptr, *dst_ptr;

	unsigned char *src_first_pixel_address_ptr = (unsigned char*) FreeImage_GetBits(bordered_src->fib);
	unsigned char *dst_first_pixel_address_ptr = (unsigned char*) FreeImage_GetBits(dst);

	for (register int y=1; y < height - 1; y++)
	{		
		src_ptr = (src_first_pixel_address_ptr + y * pitch_in_pixels + 1);
		dst_ptr = (dst_first_pixel_address_ptr + y * pitch_in_pixels + 1);

		for (register int x=0; x < width - 1; x++) 
		{
			if(*src_ptr > threshold)
				*dst_ptr = NeighbourhoodNMS(src_ptr, pitch_in_pixels);

			src_ptr++;
			dst_ptr++;
		}
	}

	return dst;
}

static inline void SetNeighbours(unsigned char *ptr, unsigned int pitch_in_pixels)
{
	unsigned char *tmp_ptr = ptr - pitch_in_pixels - 1;

	if(!tmp_ptr[0])
		tmp_ptr[0] = 2;

	if(!tmp_ptr[1])
		tmp_ptr[1] = 2;
	
	if(!tmp_ptr[2])
		tmp_ptr[2] = 2;

	// Next kernel line
	tmp_ptr += pitch_in_pixels;

	if(!tmp_ptr[0])
		tmp_ptr[0] = 2;

	if(!tmp_ptr[2])
		tmp_ptr[2] = 2;

	// Next kernel line
	tmp_ptr += pitch_in_pixels;

	if(!tmp_ptr[0])
		tmp_ptr[0] = 2;

	if(!tmp_ptr[1])
		tmp_ptr[1] = 2;
	
	if(!tmp_ptr[2])
		tmp_ptr[2] = 2;
}

// Set adjoining pixels to 2
static void SetNeigbourPixels(FIBITMAP *src)
{
	const int width = FreeImage_GetWidth(src);
	const int height = FreeImage_GetHeight(src);

	const int pitch_in_pixels = FreeImage_GetPitch(src) / sizeof(unsigned char);

	register unsigned char *src_ptr;

	unsigned char *src_first_pixel_address_ptr = (unsigned char*) FreeImage_GetBits(src);

	for (register int y=1; y < height - 1; y++)
	{		
		src_ptr = (src_first_pixel_address_ptr + y * pitch_in_pixels + 1);
		
		for (register int x=0; x < width - 1; x++) 
		{
			if(*src_ptr== 1)
				SetNeighbours(src_ptr, pitch_in_pixels);
			
			src_ptr++;
		}
	}

	return dst;
}

static void regionGrow(int x, int y)
// region growing downhill
{
	int pos, n_pos;

	regionGrowCount++;
	
	if (regionGrowCount > MAX_REGIONGROW_CALLS)
		return;

	if (x>0 && x<x_points-1 && y>0 && y<y_points-1)
	{
		pos = y * x_points + x;
		n_pos = pos - x_points - 1 ; // position of neighbour
		maxima[pos]=3;
		
		if (imageArray[n_pos]<=imageArray[pos] && imageArray[n_pos]!=0 && maxima[n_pos]!=3) regionGrow(x-1,y-1);
		n_pos++;
		if (imageArray[n_pos]<=imageArray[pos] && imageArray[n_pos]!=0 && maxima[n_pos]!=3) regionGrow(x,y-1);
		n_pos++;
		if (imageArray[n_pos]<=imageArray[pos] && imageArray[n_pos]!=0 && maxima[n_pos]!=3) regionGrow(x+1,y-1);
		n_pos+=x_points;
		if (imageArray[n_pos]<=imageArray[pos] && imageArray[n_pos]!=0 && maxima[n_pos]!=3) regionGrow(x+1,y);
		n_pos-=2;
		if (imageArray[n_pos]<=imageArray[pos] && imageArray[n_pos]!=0 && maxima[n_pos]!=3) regionGrow(x-1,y);
		n_pos+=x_points;
		if (imageArray[n_pos]<=imageArray[pos] && imageArray[n_pos]!=0 && maxima[n_pos]!=3) regionGrow(x-1,y+1);
		n_pos++;
		if (imageArray[n_pos]<=imageArray[pos] && imageArray[n_pos]!=0 && maxima[n_pos]!=3) regionGrow(x,y+1);
		n_pos++;
		if (imageArray[n_pos]<=imageArray[pos] && imageArray[n_pos]!=0 && maxima[n_pos]!=3) regionGrow(x+1,y+1);
	}
}

static FIBITMAP* DrawMaxima (FIBITMAP* src, int size )
{
	// A large size allows the peask to merge together if this is desirable
	
	if (size < 1)
		size = 1; // Just a check as this has created much confusion

	const int width = FreeImage_GetWidth(src);
	const int height = FreeImage_GetHeight(src);

	FIBITMAP *dst = FreeImage_Allocate(width, height, 8, 0, 0, 0);

	FreeImageAlgorithms_SetGreyLevelPalette(dst);

	const int pitch_in_pixels = FreeImage_GetPitch(dst) / sizeof(unsigned char);

	register unsigned char *src_ptr;

	unsigned char *src_first_pixel_address_ptr = (unsigned char*) FreeImage_GetBits(src);

	RECT rect;

	for (register int y=0; y < height; y++)
	{		
		src_ptr = (src_first_pixel_address_ptr + y * pitch_in_pixels);
	
		for (register int x=0; x < width; x++) 
		{
			if(src_ptr[0] == 1) {

				rect.left = x-size/2;
				rect.top = y-size/2;
				rect.right = rect.left + size;
				rect.bottom = rect.top + size;

				FreeImageAlgorithms_Draw8BitSolidGreyscaleRect (dst, rect, 255); 
			}

			src_ptr++;
		}
	}

	return dst;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_FindImageMaxima(FIBITMAP* src, int min_separation)
{
	
	FIBITMAP* nms = NonMaxSupression(src, 50);

	SetNeigbourPixels(nms);


	FIBITMAP* drawnMaxima = DrawMaxima (nms, min_separation);

	FreeImage_Unload(nms);

	return drawnMaxima;
}



//********************************* regionGrow **********************************************************************
/*
static void regionGrow(int x, int y)
// region growing downhill
{
	int pos, n_pos;

	regionGrowCount++;
	if (regionGrowCount>MAX_REGIONGROW_CALLS) return;
	if (x>0 && x<x_points-1 && y>0 && y<y_points-1)
	{
		pos = y * x_points + x;
		n_pos = pos - x_points - 1 ; // position of neighbour
		maxima[pos]=3;
		
		if (imageArray[n_pos]<=imageArray[pos] && imageArray[n_pos]!=0 && maxima[n_pos]!=3) regionGrow(x-1,y-1);
		n_pos++;
		if (imageArray[n_pos]<=imageArray[pos] && imageArray[n_pos]!=0 && maxima[n_pos]!=3) regionGrow(x,y-1);
		n_pos++;
		if (imageArray[n_pos]<=imageArray[pos] && imageArray[n_pos]!=0 && maxima[n_pos]!=3) regionGrow(x+1,y-1);
		n_pos+=x_points;
		if (imageArray[n_pos]<=imageArray[pos] && imageArray[n_pos]!=0 && maxima[n_pos]!=3) regionGrow(x+1,y);
		n_pos-=2;
		if (imageArray[n_pos]<=imageArray[pos] && imageArray[n_pos]!=0 && maxima[n_pos]!=3) regionGrow(x-1,y);
		n_pos+=x_points;
		if (imageArray[n_pos]<=imageArray[pos] && imageArray[n_pos]!=0 && maxima[n_pos]!=3) regionGrow(x-1,y+1);
		n_pos++;
		if (imageArray[n_pos]<=imageArray[pos] && imageArray[n_pos]!=0 && maxima[n_pos]!=3) regionGrow(x,y+1);
		n_pos++;
		if (imageArray[n_pos]<=imageArray[pos] && imageArray[n_pos]!=0 && maxima[n_pos]!=3) regionGrow(x+1,y+1);
	}
}

*/

//********************************* FindOnlyMaxima **********************************************************************

/*
static void FindOnlyMaxima(double threshold, double *imageArray, int *maxima)
// find peaks and plateaux(?), no shoulders, no local minima.
{
	int i,j,k; // i and j step through the pixels. 
	int f_pos;
		

	// nms
	for (j = 1; j< y_points-1; j++)
	{
		f_pos = j * x_points + 1; // position in array which is really 1D
		for (i = 1; i< x_points-1; i++)
		{

			if (imageArray[f_pos]>threshold)
				maxima[f_pos] = nms (f_pos); 
			f_pos++;
		} // i loop which is for x
	} // j loop which is for y

	// set adjoining pixels
	for (j = 1; j< y_points-1; j++)
	{
		f_pos = j * x_points + 1; // position in array which is really 1D
		for (i = 1; i< x_points-1; i++)
		{
			if (maxima[f_pos]==1)
			{
				if (maxima[f_pos-1]==0) maxima[f_pos-1]=2;
				if (maxima[f_pos+1]==0) maxima[f_pos+1]=2;
				k=-x_points;
				if (maxima[f_pos+k-1]==0) maxima[f_pos+k-1]=2;
				if (maxima[f_pos+k]==0)   maxima[f_pos+k]=2;
				if (maxima[f_pos+k+1]==0) maxima[f_pos+k+1]=2;
				k=x_points;
				if (maxima[f_pos+k-1]==0) maxima[f_pos+k-1]=2;
				if (maxima[f_pos+k]==0)   maxima[f_pos+k]=2;
				if (maxima[f_pos+k+1]==0) maxima[f_pos+k+1]=2;
			}
		f_pos++;
		}
	}
   
	// grow region down hill
	for (j = 1; j< y_points-1; j++)
	{
		for (i = 1; i< x_points-1; i++)
		{
			f_pos = j * x_points + i; // position in array which is really 1D
			if (maxima[f_pos]==2)
			{
				regionGrowCount=0;
				regionGrow(i,j);
			}
		}
	
	}
	
}
*/

//********************************* DrawMaxima **********************************************************************

/*
static void DrawMaxima ( IPIImageRef input, int rectOrOval, int size )
// draw these maxima in an IPI image
// specify marker type and size
// a large size allows the peask to merge together if this is desirable
{
	int x, y, f_pos;

	if (size<1) size=1; // just a check as this has created much confusion

	IPI_DrawRect (input, input, IPI_FULL_RECT, IPI_DRAW_PAINT, 0.0);
	for (y = 1; y< y_points-1; y++)
	{
		f_pos = y * x_points; // position in array which is really 1D
		for (x = 1; x< x_points-1; x++)
		{
			// only true maxima are labelled with 1
			if (maxima[f_pos]==1) 
			{
				if (!rectOrOval)    IPI_DrawRect (input, input, MakeRect (y-size/2, x-size/2, size, size), IPI_DRAW_PAINT, 1.0);
				else				IPI_DrawOval (input, input, MakeRect (y-size/2, x-size/2, size, size), IPI_DRAW_PAINT, 1.0);
				
//				Favourites were:
//				IPI_DrawRect (input, input, MakeRect (y-1, x-1, 3, 3), IPI_DRAW_PAINT, 1.0);
//				IPI_DrawOval (input, input, MakeRect (y-5, x-5, 11, 11), IPI_DRAW_PAINT, 1.0);
			}
			f_pos++;
		} 
	} 
}

*/

//********************************* comparePeaks **********************************************************************

/*
static int __cdecl comparePeaks (const void *element1, const void *element2)
{
	GlPeak peak1, peak2;
	
	peak1 = *(GlPeak *)element1;
	peak2 = *(GlPeak *)element2;

	if (peak1.value > peak2.value) return(+1);
	else if (peak2.value > peak1.value) return(-1);
	else return(0);
}
*/

//********************************* StoreBrightestPeaks **********************************************************************

/*
static int StoreBrightestPeaks (IPIImageRef peakImage, IPIImageRef brightnessImage, int number, GlPeak **pCentres)
{
	// store a number of brightest peaks from peakImage according to brightnessImage
	// if number<=0 then all the peaks are stored
	// the actual number of peaks in the centres list is returned
	
	IPIFullPReportPtr particleReportArray=NULL;
	int parameterArray[] = {IPI_PP_CenterMassX, IPI_PP_CenterMassY};
	int i, noOfParticles, count;
	float *particleCoeffArray;
	GlPeak *peakList;
	
	// characterize the peaks in the peak image
	IPI_Particle (peakImage, TRUE, &particleReportArray, &noOfParticles);
	particleCoeffArray = (float *) calloc (2*noOfParticles, sizeof(float)); if (particleCoeffArray==NULL) return(0);
	IPI_ParticleCoeffs (peakImage, parameterArray, 2, particleReportArray, noOfParticles, particleCoeffArray);

	// load up the list of peaks
	peakList = (GlPeak *) calloc (noOfParticles, sizeof(GlPeak)); if (peakList==NULL) return(0);
	for (i=0; i<noOfParticles; i++) 
	{
		peakList[i].centre.x = particleCoeffArray[i*2];
		peakList[i].centre.y = particleCoeffArray[i*2+1];
		IPI_GetPixelValue (brightnessImage, peakList[i].centre.x, peakList[i].centre.y, &(peakList[i].value));
	}
	
	// sort the peaks
	qsort (peakList, noOfParticles, sizeof(GlPeak), comparePeaks);   // sort into assending order

	// store top "number" of particles
	if (number<=0) number = noOfParticles;
	if (*pCentres==NULL) *pCentres = (GlPeak *) calloc (number, sizeof(GlPeak)); if (*pCentres==NULL) return(-1);
	for (i=noOfParticles-1, count=0; i>=0 && count<number; i--, count++)
	{
		(*pCentres)[count].centre.x = peakList[i].centre.x;
		(*pCentres)[count].centre.y = peakList[i].centre.y;
		(*pCentres)[count].value    = peakList[i].value;
	}
	
	IPI_free(particleReportArray);
	free (particleCoeffArray);
	free (peakList);
	return(count);
}
*/


//********************************* findImageMaxima Public fns **********************************************************************

/*
int findImageMaxima_imageOut (FIBITMAP* image, FIBITMAP* mask, int number, double threshold_in, int minSeparation, GlPeak **pCentresIn, FIBITMAP** imageOut)

// number is the number of maxima to try and find, if number<=0 all peaks are found
// cetres can be already allocated to take a maximum of number points.
// the actual number of maxima found is returned.
{
	int maximaFound;
	FIBITMAP* temp=NULL;
	float threshold;
	GlPeak **pCentres=NULL;

	PROFILE_START("findImageMaxima");

	if (pCentresIn!=NULL)
		pCentres = pCentresIn;

	if (threshold_in<0)
		threshold = 10.0;
	else 
		threshold = threshold_in;
	
	x_points = FreeImage_GetWidth(image)
	y_points = FreeImage_GetHeight(image)
	
	imageArray = (double *) calloc (x_points * y_points, sizeof(double));
	maxima     = (int *)    calloc (x_points * y_points, sizeof(int));
	IPI_ImageToArray (image, IPI_FULL_RECT, VAL_DOUBLE, imageArray, &x_points, &y_points);
	
	// stores the maxima as single points in the maxima array
	FindOnlyMaxima(threshold, imageArray, maxima);

	IPI_Create (&temp, IPI_PIXEL_U8, 2);
	IPI_SetImageSize (temp, info.width, info.height);

	// draw these maxima in an IPI image
	// specify marker type and size
	// a large size allows the peask to merge together if this is desirable
	DrawMaxima ( temp, 1, minSeparation );

	// allow for masking of the image
	if (mask>0)
		IPI_Mask (temp, mask, temp);

	free (imageArray);
	free (maxima);

	// stores the peaks from the temp image as a list of centres
	maximaFound = StoreBrightestPeaks (temp, image, number, pCentres);
	
	if (imageOut>0)
		IPI_Copy(temp, imageOut);
	
	IPI_Dispose(temp);
	PROFILE_STOP("findImageMaxima");
	return (maximaFound);
}

int findImageMaxima_Thresholded (IPIImageRef image, IPIImageRef mask,  int number, double threshold_in, int minSeparation, GlPeak **pCentres)
{
	return (findImageMaxima_imageOut (image, mask, number, threshold_in, minSeparation, pCentres, 0));
}

int findImageMaxima (IPIImageRef image, int number, IPIImageRef mask,  GlPeak *centres)
{   // this is for lagacy cell finding where centres is pre allocated and minSeparation = 11 pixels, and no mask
	return (findImageMaxima_imageOut (image, 0, number, 0, 11, &centres, 0));
}
*/



