#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Morphology.h"
#include "FreeImageAlgorithms_FindImageMaxima.h"

#define MAX_REGIONGROW_CALLS 5000

class FindMaxima
{
	public:

		FIBITMAP* FindImageMaxima(FIBITMAP* src, unsigned char threshold, int min_separation);

	private:

		void NonMaxSupression();
		inline int NeighbourhoodNMS(unsigned char *ptr);
		inline void SetNeighbours(unsigned char *ptr);
		void SetNeigbourPixels();
		void RegionGrow(int x, int y);
		void PerformRegionGrow();
		FIBITMAP* DrawMaxima (int size);

		unsigned char min_separation;
		unsigned char threshold;
		unsigned int pitch_in_pixels;
		unsigned char *original_first_pixel_address_ptr;
		unsigned char *processing_first_pixel_address_ptr;
		int width;
		int height;
		int regionGrowCount;

		FIABITMAP *original_image;
		FIBITMAP *processing_image;
};

inline int
FindMaxima::NeighbourhoodNMS(unsigned char *ptr)
{
	unsigned char *tmp_ptr = ptr - this->pitch_in_pixels - 1;

	if(*ptr < tmp_ptr[0])
		return 0;

	if(*ptr < tmp_ptr[1])
		return 0;

	if(*ptr < tmp_ptr[2])
		return 0;

	// Next kernel line
	tmp_ptr += this->pitch_in_pixels;

	if(*ptr < tmp_ptr[0])
		return 0;

	if(*ptr < tmp_ptr[2])
		return 0;

	// Next kernel line
	tmp_ptr += this->pitch_in_pixels;

	if(*ptr < tmp_ptr[0])
		return 0;

	if(*ptr < tmp_ptr[1])
		return 0;

	if(*ptr < tmp_ptr[2])
		return 0;

	return 1;
}

void
FindMaxima::NonMaxSupression()
{
	register unsigned char *src_ptr, *dst_ptr;

	for (register int y=1; y < height - 1; y++)
	{		
		src_ptr = (this->original_first_pixel_address_ptr + y * this->pitch_in_pixels + 1);
		dst_ptr = (this->processing_first_pixel_address_ptr + y * this->pitch_in_pixels + 1);

		for (register int x=0; x < width - 1; x++) 
		{
			if(*src_ptr > this->threshold)
				*dst_ptr = NeighbourhoodNMS(src_ptr);

			src_ptr++;
			dst_ptr++;
		}
	}
}

inline void
FindMaxima::SetNeighbours(unsigned char *ptr)
{
	unsigned char *tmp_ptr = ptr - this->pitch_in_pixels - 1;

	if(!tmp_ptr[0])
		tmp_ptr[0] = 2;

	if(!tmp_ptr[1])
		tmp_ptr[1] = 2;
	
	if(!tmp_ptr[2])
		tmp_ptr[2] = 2;

	// Next kernel line
	tmp_ptr += this->pitch_in_pixels;

	if(!tmp_ptr[0])
		tmp_ptr[0] = 2;

	if(!tmp_ptr[2])
		tmp_ptr[2] = 2;

	// Next kernel line
	tmp_ptr += this->pitch_in_pixels;

	if(!tmp_ptr[0])
		tmp_ptr[0] = 2;

	if(!tmp_ptr[1])
		tmp_ptr[1] = 2;
	
	if(!tmp_ptr[2])
		tmp_ptr[2] = 2;
}

// Set adjoining pixels to 2
void 
FindMaxima::SetNeigbourPixels()
{
	register unsigned char *src_ptr;

	for (register int y=1; y < height - 1; y++)
	{		
		src_ptr = (this->processing_first_pixel_address_ptr + y * this->pitch_in_pixels + 1);

		for (register int x=0; x < width - 1; x++) 
		{
			if(src_ptr[x] == 1)
				this->SetNeighbours(src_ptr);
		}
	}
}

// Region growing downhill
void
FindMaxima::RegionGrow(int x, int y)
{
	int pos, n_pos;

	regionGrowCount++;
	
	if (regionGrowCount > MAX_REGIONGROW_CALLS)
		return;

	register unsigned char* optr = this->original_first_pixel_address_ptr;
	register unsigned char* pptr = this->processing_first_pixel_address_ptr;

	if (x > 0 && x < (this->width - 1) && y > 0 && y < (this->height - 1))
	{
		pos = y * this->pitch_in_pixels + x;
		n_pos = pos - this->pitch_in_pixels - 1; // Position of neighbour
		pptr[pos] = 3;
		
		if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
			RegionGrow(x-1,y-1);

		n_pos++;

		if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
			RegionGrow(x,y-1);

		n_pos++;

		if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
			RegionGrow(x+1,y-1);
		
		n_pos += this->pitch_in_pixels;

		if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
			RegionGrow(x+1,y);
		
		n_pos-=2;
		
		if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
			RegionGrow(x-1,y);
		
		n_pos += this->pitch_in_pixels;
		
		if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
			RegionGrow(x-1,y+1);
		
		n_pos++;

		if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
			RegionGrow(x,y+1);
		
		n_pos++;
		
		if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
			RegionGrow(x+1,y+1);
	}
}

void
FindMaxima::PerformRegionGrow()
{
	register unsigned char *src_ptr;

	for (register int y=1; y < height - 1; y++)
	{		
		src_ptr = (processing_first_pixel_address_ptr + y * this->pitch_in_pixels + 1);
	
		for (register int x=0; x < width - 1; x++) 
		{
			if(src_ptr[x] == 2) {
				regionGrowCount++;
				RegionGrow(x, y);
			}
		}
	}
}

FIBITMAP*
FindMaxima::DrawMaxima (int size)
{
	// A large size allows the peask to merge together if this is desirable
	
	if (size < 1)
		size = 1; // Just a check as this has created much confusion

	// Remove border
	FIBITMAP *dst = FreeImage_Allocate(width - 2, height - 2, 8, 0, 0, 0);

	FreeImageAlgorithms_SetGreyLevelPalette(dst);

	register unsigned char *src_ptr;
	RECT rect;

	for (register int y=1; y < height - 1; y++)
	{		
		src_ptr = this->processing_first_pixel_address_ptr + y * this->pitch_in_pixels;
	
		for (register int x=1; x < width - 1; x++) 
		{
			if(src_ptr[x] == 1) {

				rect.left = x-size/2 - 1;
				rect.top = y-size/2 - 1;
				rect.right = rect.left + size;
				rect.bottom = rect.top + size;

				FreeImageAlgorithms_Draw8BitSolidGreyscaleRect (dst, rect, 255); 
			}
		}
	}

	return dst;
}


FIBITMAP*
FindMaxima::FindImageMaxima(FIBITMAP* src, unsigned char threshold, int min_separation)
{
	this->regionGrowCount=0;
	this->threshold = threshold;
	this->min_separation;

	this->original_image = FreeImageAlgorithms_SetBorder(src, 1, 1);

	this->width = FreeImage_GetWidth(this->original_image->fib);
	this->height = FreeImage_GetHeight(this->original_image->fib);

	this->processing_image = FreeImage_Allocate(width, height, 8, 0, 0, 0);

	this->pitch_in_pixels = FreeImage_GetPitch(this->processing_image) / sizeof(unsigned char);
	
	this->original_first_pixel_address_ptr = (unsigned char*) FreeImage_GetBits(this->original_image->fib);
	this->processing_first_pixel_address_ptr = (unsigned char*) FreeImage_GetBits(this->processing_image);

	this->NonMaxSupression();

	SetNeigbourPixels();

	PerformRegionGrow();

	FIBITMAP* drawnMaxima = DrawMaxima (min_separation);

	FreeImage_Unload(this->processing_image);

	return drawnMaxima;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_FindImageMaxima(FIBITMAP* src, unsigned char threshold, int min_separation)
{
	FindMaxima maxima;

	return maxima.FindImageMaxima(src, threshold, min_separation);
}




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
	peakList = (GlPeak *) calloc (noOfParticles, sizeof(GlPeak));
	
	if (peakList==NULL)
	    return(0);
	
	for (i=0; i<noOfParticles; i++) 
	{
		peakList[i].centre.x = particleCoeffArray[i*2];
		peakList[i].centre.y = particleCoeffArray[i*2+1];
		IPI_GetPixelValue (brightnessImage, peakList[i].centre.x, peakList[i].centre.y, &(peakList[i].value));
	}
	
	// sort the peaks
	qsort (peakList, noOfParticles, sizeof(GlPeak), comparePeaks);   // sort into assending order

	// store top "number" of particles
	
	if (number<=0)
	    number = noOfParticles;
	
	if (*pCentres==NULL) *pCentres = (GlPeak *) calloc (number, sizeof(GlPeak));
	
	if (*pCentres==NULL)
	    return(-1);

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



*/



