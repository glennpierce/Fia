#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Logic.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Particle.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Arithmetic.h"
#include "FreeImageAlgorithms_Convolution.h"

#include <math.h>

#define MAX_REGIONGROW_CALLS 5000

typedef float (*GetPixelValueFunction) (FIBITMAP *src, int x, int y);

class FindMaxima
{
	public:

		FIBITMAP* FindImageMaxima(FIBITMAP* src, FIBITMAP *mask, unsigned char threshold,
            int min_separation, FIAPeak **peaks, int number, int *peaks_found);

	private:

		void NonMaxSupression();
		inline int NeighbourhoodNMS(unsigned char *ptr);
		inline void SetNeighbours(unsigned char *ptr);
		void SetNeigbourPixels();
		void RegionGrow(int x, int y);
		void PerformRegionGrow();
		void DrawMaxima (int size);
		int StoreBrightestPeaks (int number, FIAPeak **peaks);

		unsigned char min_separation;
		unsigned char threshold;
		unsigned int pitch_in_pixels;
		unsigned char *original_first_pixel_address_ptr;
		unsigned char *processing_first_pixel_address_ptr;
		int width;
		int height;
		int regionGrowCount;

		FIBITMAP *original_image;
		FIBITMAP *processing_image;
		FIBITMAP *peek_image;
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
	tmp_ptr = ptr - 1;

	if(*ptr < tmp_ptr[0])
		return 0;

	if(*ptr < tmp_ptr[2])
		return 0;

	// Next kernel line
	tmp_ptr = ptr + this->pitch_in_pixels - 1;

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
		src_ptr = this->original_first_pixel_address_ptr + y * this->pitch_in_pixels + 1;
		dst_ptr = this->processing_first_pixel_address_ptr + y * this->pitch_in_pixels + 1;

		for (register int x=1; x < width - 1; x++) 
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
		src_ptr = (this->processing_first_pixel_address_ptr + y * this->pitch_in_pixels) + 1;

		for (register int x=1; x < width - 1; x++) 
		{
			if(*src_ptr == 1)
				this->SetNeighbours(src_ptr);

			src_ptr++;
		}
	}
}

// Region growing downhill
void
FindMaxima::RegionGrow(int x, int y)
{
	if (regionGrowCount++ > MAX_REGIONGROW_CALLS)
		return;

	register unsigned char* optr = this->original_first_pixel_address_ptr;
	register unsigned char* pptr = this->processing_first_pixel_address_ptr;

	int pos, n_pos;

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
		
		n_pos = pos - 1;

		if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
			RegionGrow(x-1,y);
		
		n_pos+=2;
		
		if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
			RegionGrow(x+1,y);
		
		n_pos = pos + this->pitch_in_pixels - 1; 
		
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
		src_ptr = processing_first_pixel_address_ptr + y * this->pitch_in_pixels;
	
		for (register int x=1; x < width - 1; x++) 
		{
			if(src_ptr[x] == 2) {
				regionGrowCount=0;
				RegionGrow(x, y);
			}
		}
	}
}

void
FindMaxima::DrawMaxima (int size)
{
	// A large size allows the peask to merge together if this is desirable
	
	if (size < 1)
		size = 1; // Just a check as this has created much confusion

	this->peek_image = FreeImage_Allocate(width, height , 8, 0, 0, 0);

	FreeImageAlgorithms_SetGreyLevelPalette(this->peek_image);

	register unsigned char *src_ptr, *dst_ptr;
	FIARECT rect;

	for (register int y=1; y < height - 1; y++)
	{		
		src_ptr = this->processing_first_pixel_address_ptr + y * this->pitch_in_pixels;
		dst_ptr = FreeImage_GetScanLine(this->peek_image, y);

		for (register int x=1; x < width - 1; x++) 
		{
			if(src_ptr[x] == 1) {

				rect.left = x;
				rect.bottom = this->height - y - 1;
				rect.right = rect.left + size - 1;
				rect.top = rect.bottom - size + 1;

				FreeImageAlgorithms_DrawSolidGreyscaleRect (this->peek_image, rect, 255); 

				dst_ptr[x] = 255;

			}
		}
	}
}


static inline int __cdecl ComparePeaks (const void *element1, const void *element2)
{
	FIAPeak peak1, peak2;
	
	peak1 = *(FIAPeak *) element1;
	peak2 = *(FIAPeak *) element2;

	if (peak1.value > peak2.value)
		return 1;
	else if (peak2.value > peak1.value)
		return -1;

	return 0;
}

int
FindMaxima::StoreBrightestPeaks (int number, FIAPeak **peaks_ref)
{
	// Store a number of brightest peaks from peakImage according to brightnessImage
	// if number<=0 then all the peaks are stored
	// the actual number of peaks in the centres list is returned
	PARTICLEINFO* info;

	FreeImageAlgorithms_ParticleInfo(this->peek_image, &info, 1);

	int total_blobs = info->number_of_blobs;

	if (number <= 0)
	    number = total_blobs;

    if (*peaks_ref == NULL) {
		*peaks_ref = (FIAPeak *) malloc (number * sizeof(FIAPeak));
        CheckMemory(*peaks_ref);
    }
    else
        return -1;

	FIAPeak *peaks = *peaks_ref;

	if (peaks == NULL)
	    return -1;
	
	int bpp = FreeImage_GetBPP(this->original_image);

	FREE_IMAGE_TYPE type = FreeImage_GetImageType(this->original_image);

	for(int i=0; i < number; i++)
	{
		BLOBINFO blobinfo = info->blobs[i];
 	
		peaks[i].centre.x = blobinfo.center_x;
		peaks[i].centre.y = blobinfo.center_y;

		FreeImageAlgorithms_GetPixelValue(
			this->original_image,peaks[i].centre.x,
			peaks[i].centre.y, &(peaks[i].value));
	}

	FreeImageAlgorithms_FreeParticleInfo(info);

	// Sort the peaks
	qsort (peaks, number, sizeof(FIAPeak), ComparePeaks);   // sort into assending order

	return MIN(number, total_blobs);
}


FIBITMAP*
FindMaxima::FindImageMaxima(FIBITMAP* src, FIBITMAP *mask, unsigned char threshold,
                            int min_separation, FIAPeak **peaks, int number, int *peaks_found)
{
	this->regionGrowCount=0;
	this->threshold = threshold;
	this->min_separation;

	this->original_image = src;

	this->width = FreeImage_GetWidth(this->original_image);
	this->height = FreeImage_GetHeight(this->original_image);

	this->processing_image = FreeImage_Allocate(width, height, 8, 0, 0, 0);

	this->pitch_in_pixels = FreeImage_GetPitch(this->processing_image) / sizeof(unsigned char);
	
	this->original_first_pixel_address_ptr = (unsigned char*) FreeImage_GetBits(this->original_image);
	this->processing_first_pixel_address_ptr = (unsigned char*) FreeImage_GetBits(this->processing_image);

	this->NonMaxSupression();

	SetNeigbourPixels();

	PerformRegionGrow();

	DrawMaxima (min_separation);

	// allow for masking of the image
	if (mask != NULL)
		FreeImageAlgorithms_MaskImage(src, mask);

	*peaks_found = StoreBrightestPeaks (number, peaks);      

	FreeImage_Unload(this->processing_image);

	return this->peek_image;
}

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_FindImageMaxima(FIBITMAP* src, FIBITMAP *mask, unsigned char threshold, int min_separation, FIAPeak **peaks, int number, int *peaks_found)
{
	FindMaxima maxima;

    // Make sure we have the 8bit greyscale image.
	if(FreeImage_GetBPP(src) != 8 && FreeImage_GetImageType(src) != FIT_BITMAP)
		return NULL;

	return maxima.FindImageMaxima(src, mask, threshold, min_separation, peaks, number, peaks_found);
}


void DLL_CALLCONV
FreeImageAlgorithms_FreePeaks(FIAPeak *peaks)
{
	free(peaks);
}


static FIBITMAP* GetApproximationImage(FIBITMAP *src, double *kernel, int border_size)
{
    FilterKernel kern1, kern2;

    kern1 = FreeImageAlgorithms_NewKernel(border_size, 0, kernel, 1.0);
    kern2 = FreeImageAlgorithms_NewKernel(0, border_size, kernel, 1.0);

    FIABITMAP* bordered_image = FreeImageAlgorithms_SetBorder(src, border_size, border_size,
                                    BorderType_Mirror, 0.0);
        
    FIBITMAP *approxImage = FreeImageAlgorithms_SeparableConvolve(bordered_image, kern1, kern2);

    FreeImageAlgorithms_Unload(bordered_image);

    //FreeImageAlgorithms_InPlaceConvertToStandardType(&(approxImage), 1);

    return approxImage;
}

// Median Absolute Deviation
static double GetMADValue(FIBITMAP *src)
{
    int width = FreeImage_GetWidth(src);
    int height = FreeImage_GetHeight(src);
    int total = width * height;
    int line_in_bytes = FreeImage_GetLine(src);
    register int x, y;

    assert(FreeImage_GetImageType(src) == FIT_DOUBLE);

    double* data = new double[total];
    BYTE *data_ptr = (BYTE*) data;

    register double *src_ptr;

    for(y = 0; y < height; y++) {

        src_ptr = (double *)FreeImage_GetScanLine(src, y);

        memcpy(data_ptr + (y * line_in_bytes), src_ptr, line_in_bytes);
    }

    double median = quick_select_median(data, total);

    for(y = 0; y < height; y++) {

        src_ptr = (double *)FreeImage_GetScanLine(src, y);

         for(x = 0; x < width; x++) {

            data[y*width+x] = abs(src_ptr[x] - median);
         }
       
    }

   double ret = quick_select_median(data, total);

    delete [] data;

    return ret;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_FindImageMaxima2(FIBITMAP* src, int levels, unsigned char threshold, int *peaks_found)
{
    const int number_of_resolutions = 4, k = 3;
    FIBITMAP *A[number_of_resolutions + 1], *W[number_of_resolutions];
    double image_thresholds[number_of_resolutions];

   // double* kernels = new double*[levels];
    double* kernels[number_of_resolutions];

   // FIBITMAP* A = new FIBITMAP*[levels + 1];
   // FIBITMAP* W = new FIBITMAP*[levels];

    double kernel1[5] =  {1.0/16, 1.0/4, 3.0/8, 1.0/4, 1.0/6};
    double kernel2[9] =  {1.0/16, 0.0, 1.0/4, 0.0, 3.0/8, 0.0, 1.0/4, 0.0, 1.0/6};                      
    double kernel3[17] = {1.0/16, 0.0, 0.0, 0.0, 1.0/4, 0.0, 0.0, 0.0, 3.0/8,
                              0.0, 0.0, 0.0, 1.0/4, 0.0, 0.0, 0.0, 1.0/6};

    double kernel4[33] = {1.0/16, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0/4,
                          0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3.0/8,
                          0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0/4,
                          0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0/6};


    int borders[number_of_resolutions] = {2, 4, 8, 16};
    kernels[0] = kernel1;
    kernels[1] = kernel2;
    kernels[2] = kernel3;
    kernels[3] = kernel4;

    char name[1000];
    double min, max;

    A[0] = FreeImageAlgorithms_ConvertToGreyscaleFloatType(src, FIT_DOUBLE);

    FIBITMAP *test;

    for(int i=1; i <= number_of_resolutions; i++) {

        A[i] = GetApproximationImage(A[i-1], kernels[i-1], borders[i-1]);

        sprintf(name, "%s\\A_%d.bmp", "C:\\Temp\\FreeImageAlgorithms_Tests", i);
	    test = FreeImage_ConvertToStandardType(A[i], 0);
        FreeImageAlgorithms_SaveFIBToFile(test, name, BIT8); 


        W[i-1] = FreeImage_Clone(A[i-1]);       
        FreeImageAlgorithms_SubtractGreyLevelImages(W[i-1], A[i]);

        sprintf(name, "%s\\W_%d.bmp", "C:\\Temp\\FreeImageAlgorithms_Tests", i);
	    FreeImageAlgorithms_SaveFIBToFile(W[i-1], name, BIT8); 

        image_thresholds[i-1] = GetMADValue(W[i-1]) * k / 0.67;

        FreeImageAlgorithms_FindMinMax(W[i-1], &min, &max);

        FreeImageAlgorithms_InPlaceThreshold(W[i-1], min, image_thresholds[i-1], 0);
    
        FreeImageAlgorithms_FindMinMax(W[i-1], &min, &max);

        test = FreeImage_ConvertToStandardType(W[i-1], 1);

        sprintf(name, "%s\\Wthresold_%d.bmp", "C:\\Temp\\FreeImageAlgorithms_Tests", i);
	    FreeImageAlgorithms_SaveFIBToFile(test, name, BIT8); 

    }

    FIBITMAP *product_image = FreeImage_Clone(W[0]);

    for(int i=1; i < number_of_resolutions; i++) {
 
        FreeImageAlgorithms_MultiplyGreyLevelImages(product_image, W[i]);
    }

    sprintf(name, "%s\\find_image_maxima2_final.bmp", "C:\\Temp\\FreeImageAlgorithms_Tests");
	FreeImageAlgorithms_SaveFIBToFile(product_image, name, BIT8); 

	return NULL;

}