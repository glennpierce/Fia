Copyright 2007 Glenn Pierce

This file is part of FreeImageAlgorithms.

FreeImageAlgorithms is free software: you can redistribute it and/or modify
it under the terms of the Lesser GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

FreeImageAlgorithms is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
Lesser GNU General Public License for more details.

You should have received a copy of the Lesser GNU General Public License
along with FreeImageAlgorithms.  If not, see <http://www.gnu.org/licenses/>.

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Statistics.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#include <assert.h>
#include <math.h>

template<class Tsrc>
class Statistic
{
public:
	int CalculateHistogram(FIBITMAP *src, double min, double max, int number_of_bins,
		unsigned long *hist);

	int CalculateStatisticReport(FIBITMAP *src, StatisticReport *report);

	int Centroid(FIBITMAP *src, float *x_centroid, float *y_centroid);

    double CalculateGreyLevelAverage(FIBITMAP *src);
};


template<class Tsrc> int
Statistic<Tsrc>::CalculateHistogram(FIBITMAP *src, double min, double max, int number_of_bins,
									unsigned long *hist)
{
	if (hist == NULL)      
		return FREEIMAGE_ALGORITHMS_ERROR;

	// We need to find the min and max in the image.
	if(min == 0 && max == 0)
		FreeImageAlgorithms_FindMinMax(src, &min, &max);

	if(min >= max)
		return FREEIMAGE_ALGORITHMS_ERROR;
 
	// Clear histogram array
	memset(hist, 0, number_of_bins * sizeof(unsigned long) );

	Tsrc tmp_min = (Tsrc) min;
	Tsrc tmp_max = (Tsrc) max;
	Tsrc range = tmp_max - tmp_min; 
	Tsrc range_per_bin = range / (number_of_bins - 1);     

	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);   

	Tsrc *bits = (Tsrc *) FreeImage_GetBits(src);
	Tsrc pixel;
	unsigned int bin;

	for(register int y=0; y < height ; y++) {
		
		bits = (Tsrc*) FreeImage_GetScanLine(src, y);

		for(register int x=0; x < width ; x++) {
		
			pixel = bits[x];
					
			if(pixel >= tmp_min && pixel <= tmp_max) {
		
				// If range_per_bin == 1 with dont need the divide. The divide is very slow.
				if(range_per_bin == 1)
					bin = (int) (pixel - tmp_min);
				else
					bin = (int) ((pixel - tmp_min) / range_per_bin);

				hist[bin]++;
			}
		}
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


template<class Tsrc> int
Statistic<Tsrc>::CalculateStatisticReport(FIBITMAP *src, StatisticReport *report)
{
	if (report == NULL)      
		return FREEIMAGE_ALGORITHMS_ERROR;

	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);   

	double sum = 0.0, sd_sum = 0.0;
	Tsrc *bits;

	for(register int y=0; y < height ; y++) {
		
		bits = (Tsrc*) FreeImage_GetScanLine(src, y);

		for(register int x=0; x < width ; x++)	
			sum += bits[x];				
	}

	report->area = width * height;
	report->mean = (float) sum / report->area; 

	for(register int y=0; y < height ; y++) {
		
		bits = (Tsrc*) FreeImage_GetScanLine(src, y);

		for(register int x=0; x < width ; x++)	
			sd_sum += ((bits[x] - report->mean) * (bits[x] - report->mean));				
	}

	report->stdDeviation = sqrt((float) sd_sum / (report->area - 1));

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


template<class Tsrc> int
Statistic<Tsrc>::Centroid(FIBITMAP *src, float *x_centroid, float *y_centroid)
{
	*x_centroid = 0;
	*y_centroid = 0;

	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);   

	Tsrc *bits;
	double sum_x = 0.0, sum_y = 0.0, sum_i = 0.0;
	int row;

	for(register int y=0; y < height ; y++) {
		
		bits = (Tsrc*) FreeImage_GetScanLine(src, y);
		
		row = height - y;

		for(register int x=0; x < width ; x++) {	
			sum_i += (double) bits[x];
			sum_x += (double)((x + 1) * bits[x]);		
			sum_y += (double)(row * bits[x]);		
		}
	}

	*x_centroid = (float) (sum_x / sum_i);
	*y_centroid = (float) (sum_y / sum_i);

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

template<class Tsrc> double
Statistic<Tsrc>::CalculateGreyLevelAverage(FIBITMAP *src)
{
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);   
    int total = width * height;

	Tsrc *bits;
	double sum = 0.0;

	for(register int y=0; y < height ; y++) {
		
		bits = (Tsrc*) FreeImage_GetScanLine(src, y);
		
		for(register int x=0; x < width ; x++) {	
			sum += (double) bits[x];
		}
	}

	return sum / total;
}


Statistic<unsigned char>		statisticUCharImage;
Statistic<unsigned short>		statisticUShortImage;
Statistic<short>				statisticShortImage;
Statistic<unsigned long>		statisticULongImage;
Statistic<long>					statisticLongImage;
Statistic<float>				statisticFloatImage;
Statistic<double>				statisticDoubleImage;


/** 
 * Calculate the histogram for the image.
 * Does not work with colour images.
*/
int DLL_CALLCONV
FreeImageAlgorithms_Histogram(FIBITMAP *src, double min, double max, int number_of_bins,
							  unsigned long *hist)
{
	if(!src)
		return FREEIMAGE_ALGORITHMS_ERROR;

	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				return statisticUCharImage.CalculateHistogram(src, min, max, number_of_bins, hist);

		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
			return statisticUShortImage.CalculateHistogram(src, min, max, number_of_bins, hist);

		case FIT_INT16:		// array of short: signed 16-bit
			return statisticShortImage.CalculateHistogram(src, min, max, number_of_bins, hist);

		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
			return statisticULongImage.CalculateHistogram(src, min, max, number_of_bins, hist);

		case FIT_INT32:		// array of long: signed 32-bit
			return statisticLongImage.CalculateHistogram(src, min, max, number_of_bins, hist);

		case FIT_FLOAT:		// array of float: 32-bit
			return statisticFloatImage.CalculateHistogram(src, min, max, number_of_bins, hist);

		case FIT_DOUBLE:	// array of double: 64-bit
			return statisticDoubleImage.CalculateHistogram(src, min, max, number_of_bins, hist);

		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	return FREEIMAGE_ALGORITHMS_ERROR;
}


int DLL_CALLCONV
FreeImageAlgorithms_StatisticReport(FIBITMAP *src, StatisticReport *report)
{
	if(!src)
		return FREEIMAGE_ALGORITHMS_ERROR;

	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				return statisticUCharImage.CalculateStatisticReport(src, report);

		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
			return statisticUShortImage.CalculateStatisticReport(src, report);

		case FIT_INT16:		// array of short: signed 16-bit
			return statisticShortImage.CalculateStatisticReport(src, report);

		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
			return statisticULongImage.CalculateStatisticReport(src, report);

		case FIT_INT32:		// array of long: signed 32-bit
			return statisticLongImage.CalculateStatisticReport(src, report);

		case FIT_FLOAT:		// array of float: 32-bit
			return statisticFloatImage.CalculateStatisticReport(src, report);

		case FIT_DOUBLE:	// array of double: 64-bit
			return statisticDoubleImage.CalculateStatisticReport(src, report);

		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	return FREEIMAGE_ALGORITHMS_ERROR;
}


int DLL_CALLCONV
FreeImageAlgorithms_Centroid(FIBITMAP *src, float *x_centroid, float *y_centroid)
{
	if(!src)
		return  FREEIMAGE_ALGORITHMS_ERROR;

	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				return statisticUCharImage.Centroid(src, x_centroid, y_centroid);

		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
			return statisticUShortImage.Centroid(src, x_centroid, y_centroid);

		case FIT_INT16:		// array of short: signed 16-bit
			return statisticShortImage.Centroid(src, x_centroid, y_centroid);

		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
			return statisticULongImage.Centroid(src, x_centroid, y_centroid);

		case FIT_INT32:		// array of long: signed 32-bit
			return statisticLongImage.Centroid(src, x_centroid, y_centroid);

		case FIT_FLOAT:		// array of float: 32-bit
			return statisticFloatImage.Centroid(src, x_centroid, y_centroid);

		case FIT_DOUBLE:	// array of double: 64-bit
			return statisticDoubleImage.Centroid(src, x_centroid, y_centroid);

		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	return FREEIMAGE_ALGORITHMS_ERROR;
}


double DLL_CALLCONV
FreeImageAlgorithms_GetGreyLevelAverage(FIBITMAP *src)
{
	if(!src)
		return 0.0;

	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {

		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				return statisticUCharImage.CalculateGreyLevelAverage(src);

		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
			return statisticUShortImage.CalculateGreyLevelAverage(src);

		case FIT_INT16:		// array of short: signed 16-bit
			return statisticShortImage.CalculateGreyLevelAverage(src);

		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
			return statisticULongImage.CalculateGreyLevelAverage(src);

		case FIT_INT32:		// array of long: signed 32-bit
			return statisticLongImage.CalculateGreyLevelAverage(src);

		case FIT_FLOAT:		// array of float: 32-bit
			return statisticFloatImage.CalculateGreyLevelAverage(src);

		case FIT_DOUBLE:	// array of double: 64-bit
			return statisticDoubleImage.CalculateGreyLevelAverage(src);

		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	return 0.0;
}

int DLL_CALLCONV
FreeImageAlgorithms_RGBHistogram(FIBITMAP *src, unsigned char min, unsigned char max,
			int number_of_bins, unsigned long *rhist, unsigned long *ghist, unsigned long *bhist)
{
	if (rhist == NULL || ghist == NULL || bhist == NULL)      
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(min >= max)
		return FREEIMAGE_ALGORITHMS_ERROR;

	// clear histogram arrays
	memset(rhist, 0, number_of_bins * sizeof(unsigned long) );
	memset(ghist, 0, number_of_bins * sizeof(unsigned long) );
	memset(bhist, 0, number_of_bins * sizeof(unsigned long) );

	unsigned char range = max - min; 
	unsigned char range_per_bin = range / (number_of_bins - 1);     

	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	unsigned int *bits;
	unsigned char red_pixel, green_pixel, blue_pixel;
	unsigned int bin;

	for(int y = 0; y < height; y++) {

		bits = reinterpret_cast<unsigned int *>(FreeImage_GetScanLine(src, y));

		for(int x = 0; x < width; x++) {

			red_pixel = *bits >> FI_RGBA_RED_SHIFT;
			green_pixel = *bits >> FI_RGBA_GREEN_SHIFT;
			blue_pixel = *bits >> FI_RGBA_BLUE_SHIFT;

			if(red_pixel >= (unsigned int) min && red_pixel <= (unsigned int) max) {
	
				// If range_per_bin == 1 with dont need the divide. The divide is very slow.
				if(range_per_bin == 1)
					bin = (unsigned int) (red_pixel - min);
				else
					bin = (unsigned int) ((red_pixel - min) / range_per_bin);
		
				rhist[bin]++;
			}

			if(green_pixel >= (unsigned int) min && green_pixel <= (unsigned int) max) {
	
				if(range_per_bin == 1)
					bin = (unsigned int) (green_pixel - min);
				else
					bin = (unsigned int) ((green_pixel - min) / range_per_bin);
		
				ghist[bin]++;
			}

			if(blue_pixel >= (unsigned int) min && blue_pixel <= (unsigned int) max) {
	
				if(range_per_bin == 1)
					bin = (unsigned int) (blue_pixel - min);
				else
					bin = (unsigned int) ((blue_pixel - min) / range_per_bin);
		
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


int DLL_CALLCONV
FreeImageAlgorithms_MonoImageFindWhiteArea(FIBITMAP *src, unsigned int *white_area)
{
	int bpp = FreeImage_GetBPP(src);
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);	
   	
	int whites = 0;

	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src); 

	if(type != FIT_BITMAP || bpp > 8)
		return FREEIMAGE_ALGORITHMS_ERROR;

	BYTE *src_bits;   

	for(int y = 0; y < height; y++) {

		src_bits = reinterpret_cast<BYTE *>(FreeImage_GetScanLine(src, y));
	
		for(int x = 0; x < width; x++) {

			if(src_bits[x] >= 1)
				whites++;
		}
	}

	*white_area = whites;

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageAlgorithms_MonoImageFindWhiteFraction(FIBITMAP *src, double *white_area, double *black_area)
{
	int bpp = FreeImage_GetBPP(src);
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);
	long size = width * height;  	
   	
	unsigned int whites = 0;

	if(FreeImageAlgorithms_MonoImageFindWhiteArea(src, &whites) == FREEIMAGE_ALGORITHMS_ERROR)
		return FREEIMAGE_ALGORITHMS_ERROR;

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

	int bpp = FreeImage_GetBPP(src);

	if(bpp > 8)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(FreeImage_GetImageType(src) != FreeImage_GetImageType(result))
		return FREEIMAGE_ALGORITHMS_ERROR;

	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	if(width != FreeImage_GetWidth(result))
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(height != FreeImage_GetHeight(result))
		return FREEIMAGE_ALGORITHMS_ERROR;

	FIBITMAP *src_converted;

	if(bpp < 8)
		src_converted = FreeImage_ConvertTo8Bits(src);
	else
		src_converted = FreeImage_Clone(src);

	BYTE src_tmp, result_tmp;
	BYTE *src_bits, *result_bits;

	for(int y = 0; y < height; y++) {

		src_bits = reinterpret_cast<BYTE *>(FreeImage_GetScanLine(src_converted, y));
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
