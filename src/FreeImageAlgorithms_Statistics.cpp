#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Statistics.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#include <assert.h>
#include <math.h>

template<class Tsrc>
class Histogram
{
public:
	int CalculateHistogram(FIBITMAP *src, double min, double max, int number_of_bins, unsigned long *hist);
};


template<class Tsrc> int
Histogram<Tsrc>::CalculateHistogram(FIBITMAP *src, double min, double max, int number_of_bins, unsigned long *hist)
{
	if (hist == NULL)      
		return FREEIMAGE_ALGORITHMS_ERROR;

	// clear histogram array
	memset(hist, 0, number_of_bins * sizeof(unsigned long) );

	double range = max - min; 
	double range_per_bin = range / (number_of_bins - 1);     

	long total_pixels = FreeImage_GetWidth(src) * FreeImage_GetHeight(src);   

	Tsrc *bits = (Tsrc *) FreeImage_GetBits(src);
	Tsrc pixel;
	int bin;

	for(int x=0; x < total_pixels ; x++) {
	
		pixel = (Tsrc) *bits;
				
		if(pixel >= (Tsrc)min && pixel <= (Tsrc)max) {
	
			bin = (int) floor( (pixel - min) / range_per_bin );

			hist[bin]++;
		}

		bits++;
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


Histogram<unsigned char>		histogramUCharImage;
Histogram<unsigned short>		histogramUShortImage;
Histogram<short>				histogramShortImage;
Histogram<unsigned long>		histogramULongImage;
Histogram<long>					histogramLongImage;
Histogram<float>				histogramFloatImage;
Histogram<double>				histogramDoubleImage;


/** 
 * Calculate the histogram for the image.
 * Does not work with colour images.
*/
int DLL_CALLCONV
FreeImageAlgorithms_Histogram(FIBITMAP *src, double min, double max, int number_of_bins, unsigned long *hist)
{
	if(!src)
		return NULL;

	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				return histogramUCharImage.CalculateHistogram(src, min, max, number_of_bins, hist);

		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
			return histogramUShortImage.CalculateHistogram(src, min, max, number_of_bins, hist);

		case FIT_INT16:		// array of short: signed 16-bit
			return histogramShortImage.CalculateHistogram(src, min, max, number_of_bins, hist);

		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
			return histogramULongImage.CalculateHistogram(src, min, max, number_of_bins, hist);

		case FIT_INT32:		// array of long: signed 32-bit
			return histogramLongImage.CalculateHistogram(src, min, max, number_of_bins, hist);

		case FIT_FLOAT:		// array of float: 32-bit
			return histogramFloatImage.CalculateHistogram(src, min, max, number_of_bins, hist);

		case FIT_DOUBLE:	// array of double: 64-bit
			return histogramDoubleImage.CalculateHistogram(src, min, max, number_of_bins, hist);

		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	return FREEIMAGE_ALGORITHMS_ERROR;
}


int DLL_CALLCONV
FreeImageAlgorithms_RGBHistogram(FIBITMAP *src, double min, double max, int number_of_bins,
			unsigned long *rhist, unsigned long *ghist, unsigned long *bhist)
{
	if (rhist == NULL || ghist == NULL || bhist == NULL)      
		return FREEIMAGE_ALGORITHMS_ERROR;

	// clear histogram arrays
	memset(rhist, 0, number_of_bins * sizeof(unsigned long) );
	memset(ghist, 0, number_of_bins * sizeof(unsigned long) );
	memset(bhist, 0, number_of_bins * sizeof(unsigned long) );

	double range = max - min; 
	double range_per_bin = range / (number_of_bins - 1);     

	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	unsigned int *bits;
	unsigned char red_pixel, green_pixel, blue_pixel;
	int bin;

	for(int y = 0; y < height; y++) {

		bits = reinterpret_cast<unsigned int *>(FreeImage_GetScanLine(src, y));

		for(int x = 0; x < width; x++) {

			red_pixel = *bits >> FI_RGBA_RED_SHIFT;
			green_pixel = *bits >> FI_RGBA_GREEN_SHIFT;
			blue_pixel = *bits >> FI_RGBA_BLUE_SHIFT;

			if(red_pixel >= (unsigned int) min && red_pixel <= (unsigned int) max) {
	
				bin = (int) floor( (red_pixel - min) / range_per_bin );
		
				rhist[bin]++;
			}

			if(green_pixel >= (unsigned int) min && green_pixel <= (unsigned int) max) {
	
				bin = (int) floor( (green_pixel - min) / range_per_bin );
		
				ghist[bin]++;
			}

			if(blue_pixel >= (unsigned int) min && blue_pixel <= (unsigned int) max) {
	
				bin = (int) floor( (blue_pixel - min) / range_per_bin );
		
				bhist[bin]++;
			}
		}
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_HistEq_Random_Additions(FIBITMAP *src)
{
	FIBITMAP *dib;
	int i, R;
	const int number_grey_levels = 256;
	int type, bpp, image_width, image_height, *left, *width;
	unsigned char *src_bits, *dib_bits;
	long total, Hsum, Havg, *histogram;

	bpp = FreeImage_GetBPP(src);
	type = FreeImage_GetImageType(src);
	
	// The image is not 8bit greyscale return.
	if( bpp != 8 || type != FIT_BITMAP )
		return NULL;

	image_width = FreeImage_GetWidth(src);
	image_height = FreeImage_GetHeight(src);

	// Allocate a 8-bit dib
	dib = FreeImage_AllocateT(FIT_BITMAP, image_width, image_height, 8, 0, 0, 0);
	FreeImageAlgorithms_CopyPalette(src, dib);

	total = (long) image_width * image_height;
	
	left =  (int *) malloc (sizeof(int) * number_grey_levels);
	width = (int *) malloc (sizeof(int) * number_grey_levels);
	histogram = (long *) malloc (sizeof(long) * number_grey_levels);
	
	/* Clear histogram */ 
	memset(histogram, 0, number_grey_levels); 

	FreeImage_GetHistogram(src, (DWORD *)histogram, FICC_BLACK );  

	/* right end of interval */ 
	R    = 0;		
	
	/* cumulative value for interval */ 
	Hsum = 0;		
	
	/* interval value for uniform histogram	*/
	Havg = total / number_grey_levels;	

	/* evaluate remapping of all input gray levels;
	 * Each input gray value maps to an interval of valid output values.
	 * The endpoints of the intervals are left[] and left[]+width[].
	 */
	for(i=0; i < number_grey_levels; i++) {
	
		left[i] = R;			/* left end of interval	*/
		
		Hsum += histogram[i];
		
		while( Hsum > Havg && R < (number_grey_levels - 1) ) { /* make interval wider	*/
		
			Hsum -= Havg;		/* adjust Hsum		*/
			R++;			/* update right end	*/
		}
		
		width[i] = R - left[i] + 1;	/* width of interval	*/
	}

	src_bits = (unsigned char *) FreeImage_GetBits(src);   
	dib_bits = (unsigned char *) FreeImage_GetBits(dib); 

	/* visit all input pixels and remap intensities */
	for(i=0; i < total; i++) {
	
		const unsigned char pixel = *src_bits++;
	
		if( width[pixel] == 1) {
		
			*dib_bits++ = left[pixel];
		}
		else {
		
			/* pixel spills over into width[] possible values */
			/* randomly pick from 0 to width[i] */
			
			/* 0 <= R < width */ 
			R = ( (rand() & 0x7fff) * width[pixel] ) >> 15;	
			
			*dib_bits++ = left[ pixel ] + R;
		}
	}
	
	free(histogram);
	
	return dib;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_HistEq(FIBITMAP *src)
{
	FIBITMAP *dst;
	int i;
	const int number_grey_levels = 256;
	int type, bpp, image_width, image_height;
	unsigned char *src_bits, *dst_bits;
	long total, Hsum;
	DWORD *histogram;
	double Havg;
	long *mapping;

	bpp = FreeImage_GetBPP(src);
	type = FreeImage_GetImageType(src);
	
	// The image is not 8bit greyscale return.
	if( bpp != 8 || type != FIT_BITMAP )
		return NULL;

	image_width = FreeImage_GetWidth(src);
	image_height = FreeImage_GetHeight(src);

	// Allocate a 8-bit dib
	dst = FreeImage_AllocateT(FIT_BITMAP, image_width, image_height, 8, 0, 0, 0);

	FreeImageAlgorithms_SetGreyLevelPalette(dst);

	total = image_width * image_height;
	
	mapping =  (long *) malloc (sizeof(long) * number_grey_levels);
	histogram = (DWORD *) malloc (sizeof(long) * number_grey_levels);
	
	/* Clear histogram */ 
	memset(histogram, 0, number_grey_levels); 
	memset(mapping, 0, number_grey_levels); 

	FreeImage_GetHistogram(src, histogram, FICC_BLACK );  
	
	/* cumulative value for interval */ 
	Hsum = 0;		
	
	/* interval value for uniform histogram	*/
	Havg = (double) total / (number_grey_levels - 1);	

	for(i=0; i < number_grey_levels; i++) {   
	
		Hsum += histogram[i];
		
		mapping[i] = Hsum;
	}

	src_bits = (unsigned char *) FreeImage_GetBits(src);   
	dst_bits = (unsigned char *) FreeImage_GetBits(dst); 

	/* visit all input pixels and remap intensities */
	for(i=0; i < total; i++) {
	
		const unsigned char pixel = *src_bits++;
	
		*dst_bits++ = (const unsigned char) (int) ((mapping[pixel] / Havg) + 0.5);
	}
	
	free(histogram);
	
	return dst;
}


double DLL_CALLCONV
FreeImageAlgorithms_GetGreyLevelAverage(FIBITMAP *src)
{
	int bpp = FreeImage_GetBPP(src);
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);
	long size = width * height;  	
   	
	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src); 

	switch(type)
	{
		case FIT_BITMAP:
		{
			if(bpp == 8)
			{
				unsigned char *bits = (unsigned char *) FreeImage_GetBits(src);
				return MeanAverage(bits, size);
			}
		}

		case FIT_INT16:
		{
			short *bits = (short *) FreeImage_GetBits(src);
			return MeanAverage(bits, size);
		}
		
		case FIT_UINT16:
		{
			unsigned short *bits = (unsigned short *) FreeImage_GetBits(src);
			return MeanAverage(bits, size);
		}

		case FIT_FLOAT:
		{
			float *bits = (float *) FreeImage_GetBits(src);
			return MeanAverage(bits, size);
		}
	}

	return 0.0;
}


int DLL_CALLCONV
FreeImageAlgorithms_MonoImageFindWhiteFraction(FIBITMAP *src, double *white_area, double *black_area)
{
	int bpp = FreeImage_GetBPP(src);
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);
	long size = width * height;  	
   	
	int whites = 0;

	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src); 

	if(type != FIT_BITMAP || bpp > 8)
		return FREEIMAGE_ALGORITHMS_ERROR;

	unsigned char *src_bits = (unsigned char *) FreeImage_GetBits(src);   

	for(int i=0; i < size; i++) {
	
		const unsigned char pixel = *src_bits++;
	
		if(pixel >= 1)
			whites++;
	}

	*white_area = (double) whites / size;
	*black_area = 1 - *white_area;

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageAlgorithms_MonoTrueFalsePositiveComparison(FIBITMAP *src, FIBITMAP *result,
													int *tp, int *tn, int *fp, int *fn)
{
	*tp = 0; *tn = 0, *fp = 0, *fn = 0;

	if(!src || !result)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(FreeImage_GetBPP(src) != FreeImage_GetBPP(result))
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(FreeImage_GetImageType(src) != FreeImage_GetImageType(result))
		return FREEIMAGE_ALGORITHMS_ERROR;

	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);
	int pitch = FreeImage_GetPitch(src);

	if(width != FreeImage_GetWidth(result))
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(height != FreeImage_GetHeight(result))
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(pitch != FreeImage_GetPitch(result))
		return FREEIMAGE_ALGORITHMS_ERROR;

	BYTE src_tmp, result_tmp;
	BYTE *src_bits, *result_bits;

	for(int y = 0; y < height; y++) {

		src_bits = reinterpret_cast<BYTE *>(FreeImage_GetScanLine(src, y));
		result_bits = reinterpret_cast<BYTE *>(FreeImage_GetScanLine(result, y));
			
		for(int x = 0; x < width; x++) {

			src_tmp = src_bits[x];
			result_tmp = result_bits[x];

			if(src_tmp > 1)
				src_tmp = 1;

			if(result_tmp > 1)
				result_tmp = 1;

			// Pixels are the same and 1 increase true positive count.
			if(src_tmp == 1 && result_tmp == 1)
				(*tp)++;		
		
			// Pixels are the same and 0 increase true negative count.
			if(src_tmp == 0 && result_tmp == 0)
				(*tn)++;		

			// Pixel not in src but in result increase false positive.
			if(src_tmp == 0 && result_tmp == 1) 
				(*fp)++;

			// Pixel in src image not in result increase false negative.
			if(src_tmp == 1 && result_tmp == 0) 
				(*fn)++;		
		}
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}
