/* 
 * Copyright 2007-2010 Glenn Pierce, Paul Barber,
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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

#include <sstream>
#include <iostream>
#include <math.h>

#define MAX_REGIONGROW_CALLS 5000

typedef float (*GetPixelValueFunction) (FIBITMAP * src, int x, int y);

class FindMaxima
{
  public:

    FIBITMAP * FindImageMaxima (FIBITMAP * src, FIBITMAP * mask, double threshold,
                                int min_separation, int oval_draw, FIAPeak ** peaks, int number, int *peaks_found);

  private:

    void NonMaxSupression ();
    inline unsigned char NeighbourhoodNMS (double *ptr);
    inline void SetNeighbours (unsigned char *ptr);
    void SetNeigbourPixels ();
    void RegionGrow (int x, int y);
    void PerformRegionGrow ();
    void DrawMaxima (int size);
    int StoreBrightestPeaks (int number, FIAPeak ** peaks);

    unsigned char min_separation;
    double threshold;

	int oval_draw;
    int width;
    int height;
    int regionGrowCount;

    FIBITMAP *original_image;
    FIBITMAP *original_image_double;
    FIBITMAP *processing_image;
    FIBITMAP *peek_image;
};

inline unsigned char
FindMaxima::NeighbourhoodNMS (double *ptr)
{
    int pitch_in_pixels = FIA_GetPitchInPixels (this->original_image_double);
    
    // Top left of 3x3 kernel
    double *tmp_ptr = ptr - pitch_in_pixels - 1;

    if (*ptr < tmp_ptr[0])
    {
        return 0;
    }

    if (*ptr < tmp_ptr[1])
    {
        return 0;
    }

    if (*ptr < tmp_ptr[2])
    {
        return 0;
    }

    // Next kernel line
    tmp_ptr = ptr - 1;

    if (*ptr < tmp_ptr[0])
    {
        return 0;
    }

    if (*ptr < tmp_ptr[2])
    {
        return 0;
    }

    // Next kernel line
    tmp_ptr = ptr + pitch_in_pixels - 1;

    if (*ptr < tmp_ptr[0])
    {
        return 0;
    }

    if (*ptr < tmp_ptr[1])
    {
        return 0;
    }

    if (*ptr < tmp_ptr[2])
    {
        return 0;
    }

    return 1;
}

void
FindMaxima::NonMaxSupression ()
{
    register double *src_ptr;
    register unsigned char *dst_ptr;

    for(register int y = 1; y < height - 1; y++)
    {
        src_ptr = (double *) FreeImage_GetScanLine(this->original_image_double, y);
        dst_ptr = (unsigned char *) FreeImage_GetScanLine(this->processing_image, y);

        for(register int x = 1; x < width - 1; x++)
        {
            if (src_ptr[x] > this->threshold)
            {
                dst_ptr[x] = NeighbourhoodNMS (src_ptr + x);
            }
        }
    }
}

inline void
FindMaxima::SetNeighbours (unsigned char *ptr)
{
    int pitch_in_pixels = FIA_GetPitchInPixels (this->processing_image);
    
    unsigned char *tmp_ptr = ptr - pitch_in_pixels - 1;

    if (!tmp_ptr[0])
    {
        tmp_ptr[0] = 2;
    }

    if (!tmp_ptr[1])
    {
        tmp_ptr[1] = 2;
    }

    if (!tmp_ptr[2])
    {
        tmp_ptr[2] = 2;
    }

    // Next kernel line
    tmp_ptr += pitch_in_pixels;

    if (!tmp_ptr[0])
    {
        tmp_ptr[0] = 2;
    }

    if (!tmp_ptr[2])
    {
        tmp_ptr[2] = 2;
    }

    // Next kernel line
    tmp_ptr += pitch_in_pixels;

    if (!tmp_ptr[0])
    {
        tmp_ptr[0] = 2;
    }

    if (!tmp_ptr[1])
    {
        tmp_ptr[1] = 2;
    }

    if (!tmp_ptr[2])
    {
        tmp_ptr[2] = 2;
    }
}

// Set adjoining pixels to 2
void
FindMaxima::SetNeigbourPixels ()
{
    register BYTE *src_ptr;

    for(register int y = 1; y < height - 1; y++)
    {
        src_ptr = (BYTE *) FreeImage_GetScanLine(this->processing_image, y);
    
        for(register int x = 1; x < width - 1; x++)
        {
            if (src_ptr[x] == 1)
            {
                this->SetNeighbours (src_ptr + x);
            }
        }
    }
}

/*
// Region growing downhill
void
FindMaxima::RegionGrow (int x, int y)
{
    if (regionGrowCount++ > MAX_REGIONGROW_CALLS)
        return;

    register double *optr = this->original_first_pixel_address_ptr;
    register byte *pptr = this->processing_first_pixel_address_ptr;

    int pos, n_pos;

    if (x > 0 && x < (this->width - 1) && y > 0 && y < (this->height - 1))
    {
        pos = y * this->pitch_in_pixels + x;
        n_pos = pos - this->pitch_in_pixels - 1;        // Position of neighbour
        pptr[pos] = 3;

        if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
        {
            RegionGrow (x - 1, y - 1);
        }



        n_pos++;

        if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
        {
            RegionGrow (x, y - 1);
        }

        n_pos++;

        if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
        {
            RegionGrow (x + 1, y - 1);
        }

        n_pos = pos - 1;

        if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
        {
            RegionGrow (x - 1, y);
        }

        n_pos += 2;

        if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
        {
            RegionGrow (x + 1, y);
        }

        n_pos = pos + this->pitch_in_pixels - 1;

        if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
        {
            RegionGrow (x - 1, y + 1);
        }

        n_pos++;

        if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
        {
            RegionGrow (x, y + 1);
        }

        n_pos++;

        if (optr[n_pos] <= optr[pos] && optr[n_pos] && pptr[n_pos] != 3)
        {
            RegionGrow (x + 1, y + 1);
        }
    }
}
*/

static inline bool CheckToGrowDownHill(double kernel_centre_val, double kernel_neighbour_val)
{
    if(kernel_neighbour_val != 0.0 && kernel_neighbour_val <= kernel_centre_val)
        return true;

    return false;
}

// Region growing downhill
inline void
FindMaxima::RegionGrow (int x, int y)
{
    if (regionGrowCount++ > MAX_REGIONGROW_CALLS)
        return;

    if(y <= 0 || y >= (this->height - 1))
        return;

    if (x <= 0 || x >= (this->width - 1))
        return;

    double *original_ptr_ptr = (double *) FreeImage_GetScanLine(this->original_image_double, y) + x;
    BYTE *processing_image_ptr = FreeImage_GetScanLine(this->processing_image, y) + x;

    *processing_image_ptr = 3;	// Mark as done

    // Top left of 3x3 kernel
    double *kernel_original_ptr = original_ptr_ptr + FIA_GetPitchInPixels(this->original_image_double) - 1;
    BYTE *kernel_processing_ptr = processing_image_ptr + FIA_GetPitchInPixels(this->processing_image) - 1;

    if(kernel_processing_ptr[0] != 3 && CheckToGrowDownHill(original_ptr_ptr[0], kernel_original_ptr[0]))
         RegionGrow (x-1, y+1);

    if(kernel_processing_ptr[1] != 3 && CheckToGrowDownHill(original_ptr_ptr[0], kernel_original_ptr[1]))
         RegionGrow (x, y+1);

    if(kernel_processing_ptr[2] != 3 && CheckToGrowDownHill(original_ptr_ptr[0], kernel_original_ptr[2]))
         RegionGrow (x+1, y+1);

    kernel_original_ptr = original_ptr_ptr - 1;
    kernel_processing_ptr = processing_image_ptr - 1;

    if(kernel_processing_ptr[0] != 3 && CheckToGrowDownHill(original_ptr_ptr[0], kernel_original_ptr[0]))
         RegionGrow (x-1, y);

    if(kernel_processing_ptr[2] != 3 && CheckToGrowDownHill(original_ptr_ptr[0], kernel_original_ptr[2]))
         RegionGrow (x+1, y);

    kernel_original_ptr = original_ptr_ptr - FIA_GetPitchInPixels(this->original_image_double) - 1;
    kernel_processing_ptr = processing_image_ptr - FIA_GetPitchInPixels(this->processing_image) - 1;

    if(kernel_processing_ptr[0] != 3 && CheckToGrowDownHill(original_ptr_ptr[0], kernel_original_ptr[0]))
         RegionGrow (x-1, y-1);

    if(kernel_processing_ptr[1] != 3 && CheckToGrowDownHill(original_ptr_ptr[0], kernel_original_ptr[1]))
         RegionGrow (x, y-1);

    if(kernel_processing_ptr[2] != 3 && CheckToGrowDownHill(original_ptr_ptr[0], kernel_original_ptr[2]))
         RegionGrow (x+1, y-1);
}

void
FindMaxima::PerformRegionGrow ()
{
    register BYTE *processing_ptr;

    for(register int y = 1; y < height - 1; y++)
    {
        processing_ptr = (BYTE *) FreeImage_GetScanLine(this->processing_image, y);

        for(register int x = 1; x < width - 1; x++)
        {
            if (processing_ptr[x] == 2)
            {
                regionGrowCount = 0;
                RegionGrow (x, y);
            }
        }
    }
}

static int COMPAT_WITH_OLD_BROKEN_CODE = 0;

void DLL_CALLCONV
FIA_EnableOldBrokenCodeCompatibility(void)
{
	COMPAT_WITH_OLD_BROKEN_CODE = 1;
}

void
FindMaxima::DrawMaxima (int size)
{
    // A large size allows the peask to merge together if this is desirable

    if (size < 1)
    {
        size = 1;               // Just a check as this has created much confusion
    }

    int half_size = size / 2;

    this->peek_image = FreeImage_Allocate (this->width, this->height, 8, 0, 0, 0);

    FIA_SetGreyLevelPalette (this->peek_image);

    register BYTE *src_ptr, *dst_ptr;
    FIARECT rect;

    //int pitch_in_pixels = FIA_GetPitchInPixels(this->processing_image);
	//FIA_SimpleSaveFIBToFile (this->processing_image, "C:\\NewBeforeDrawMaxima.bmp");

    for(register int y = 0; y < height; y++)
    {
        //src_ptr = this->processing_first_pixel_address_ptr + y * pitch_in_pixels;
        src_ptr = (BYTE *) FIA_GetScanLineFromTop(this->processing_image, y);
        dst_ptr = (BYTE *) FIA_GetScanLineFromTop (this->peek_image, y);

        for(register int x = 0; x < width; x++)
        {
            if (src_ptr[x] == 1)
            {		
                rect.left = x - half_size;
				rect.top = y - half_size;

				if(COMPAT_WITH_OLD_BROKEN_CODE  > 0) {
					rect.left++;
					rect.top++;
				}
        
				// FIA Rect specify left - right
				// If size = 1 we want a width of 1 pixel to we subtract 1 from right
                rect.right = rect.left + size - 1; 
                rect.bottom = rect.top + size - 1;

                // FIBITMAP Bottom starts at zero so we must correct.
                //rect.bottom = this->height - rect.bottom - 1;
                // rect.top = this->height - rect.top - 1;

				if (size==1){
					FIA_SetPixelIndexFromTopLeft (this->peek_image, rect.left, rect.top, 255);
				}
				else {
					if(this->oval_draw || size>1)
						FIA_DrawSolidGreyscaleEllipse (this->peek_image, rect, 255, 0);
					else
						FIA_DrawSolidGreyscaleRect (this->peek_image, rect, 255);
				}
            }
        }
    }
}

static inline int __cdecl
ComparePeaks (const void *element1, const void *element2)
{
    FIAPeak peak1, peak2;

    peak1 = *(FIAPeak *) element1;
    peak2 = *(FIAPeak *) element2;

    if (peak1.value > peak2.value)
    {
        return 1;
    }
    else if (peak2.value > peak1.value)
    {
        return -1;
    }

    return 0;
}

int
FindMaxima::StoreBrightestPeaks (int number, FIAPeak ** peaks_ref)
{
    // Store a number of brightest peaks from peakImage according to brightnessImage
    // if number<=0 then all the peaks are stored
    // the actual number of peaks in the centres list is returned
    PARTICLEINFO *info = NULL;

    if(FIA_ParticleInfo (this->peek_image, &info, 1) == FIA_ERROR)
        return FIA_ERROR;

    int total_blobs = info->number_of_blobs;

    if (number <= 0)
    {
        number = total_blobs;
    }

    if (*peaks_ref == NULL)
    {
        *peaks_ref = (FIAPeak *) malloc (number * sizeof (FIAPeak));
        CheckMemory (*peaks_ref);
    }
    else
    {
        return -1;
    }

    FIAPeak *peaks = *peaks_ref;

    if (peaks == NULL)
    {
        return -1;
    }

    for(int i = 0; i < number; i++)
    {
        BLOBINFO blobinfo = info->blobs[i];

        peaks[i].centre.x = blobinfo.center_x;
        peaks[i].centre.y = blobinfo.center_y;

        FIA_GetPixelValue (this->original_image, peaks[i].centre.x, peaks[i].centre.y,
                           &(peaks[i].value));
    }

    FIA_FreeParticleInfo (info);

    // Sort the peaks
    qsort (peaks, number, sizeof (FIAPeak), ComparePeaks);      // sort into assending order

    return MIN (number, total_blobs);
}

FIBITMAP *
FindMaxima::FindImageMaxima (FIBITMAP * src, FIBITMAP * mask, double threshold,
                             int min_separation, int oval_draw, FIAPeak ** peaks, int number, int *peaks_found)
{
    *peaks_found = 0;

    this->regionGrowCount = 0;
    this->threshold = threshold;
    this->min_separation = min_separation;

    this->original_image = src;
    this->peek_image = NULL;

	this->oval_draw = oval_draw;
    this->width = FreeImage_GetWidth (this->original_image);
    this->height = FreeImage_GetHeight (this->original_image);

    if(this->width == 0 || this->height == 0) {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Error image size is %d x %d", width, height);	
        return NULL;
    }

    this->original_image_double = FIA_ConvertToGreyscaleFloatType(this->original_image, FIT_DOUBLE);
    this->processing_image = FreeImage_Allocate (this->width, this->height, 8, 0, 0, 0);

    this->NonMaxSupression ();

    SetNeigbourPixels ();

    PerformRegionGrow ();

    DrawMaxima (min_separation);

    // allow for masking of the image
    if (mask != NULL)
    {
        FIA_MaskImage (src, mask);
    }

    *peaks_found = StoreBrightestPeaks (number, peaks);

    FreeImage_Unload (this->processing_image);
    FreeImage_Unload (this->original_image_double);

    return this->peek_image;
}

FIBITMAP *DLL_CALLCONV
FIA_FindImageMaxima (FIBITMAP * src, FIBITMAP * mask, double threshold, int min_separation, int oval_draw,
                     FIAPeak ** peaks, int number, int *peaks_found)
{
    FindMaxima maxima;

    // Make sure we have a greyscale image.
    if (!FIA_IsGreyScale(src))
    {
        return NULL;
    }

    return maxima.FindImageMaxima (src, mask, threshold, min_separation, oval_draw, peaks, number,
                                   peaks_found);
}

void DLL_CALLCONV
FIA_FreePeaks (FIAPeak * peaks)
{
    free (peaks);
}

static FIBITMAP *
GetApproximationImage (FIBITMAP * src, double *kernel, int border_size)
{
    FilterKernel kern1, kern2;

    kern1 = FIA_NewKernel (border_size, 0, kernel, 1.0);
    kern2 = FIA_NewKernel (0, border_size, kernel, 1.0);

    FIABITMAP *bordered_image =
        FIA_SetBorder (src, border_size, border_size, BorderType_Mirror, 0.0);

    FIBITMAP *approxImage = FIA_SeparableConvolve (bordered_image, kern1, kern2);

    FIA_Unload (bordered_image);

    return approxImage;
}

// Median Absolute Deviation
static double
GetMADValue (FIBITMAP * src)
{
    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);
    int total = width * height;
    int line_in_bytes = FreeImage_GetLine (src);
    register int x, y;

    assert (FreeImage_GetImageType (src) == FIT_DOUBLE);

    double *data = new double[total];
    BYTE *data_ptr = (BYTE *) data;

    register double *src_ptr;

    for(y = 0; y < height; y++)
    {
        src_ptr = (double *) FreeImage_GetScanLine (src, y);

        memcpy (data_ptr + (y * line_in_bytes), src_ptr, line_in_bytes);
    }

    double median = quick_select_median (data, total);

    for(y = 0; y < height; y++)
    {
        src_ptr = (double *) FreeImage_GetScanLine (src, y);

        for(x = 0; x < width; x++)
        {

            data[y * width + x] = fabs (src_ptr[x] - median);
        }

    }

    double ret = quick_select_median (data, total);

    delete[]data;

    return ret;
}

int DLL_CALLCONV
FIA_ATrousWaveletTransform (FIBITMAP * src, int levels, FIBITMAP ** W)
{
    const int max_resolutions = 4;
    const int number_of_resolutions = levels, k = 3;
    double image_thresholds[max_resolutions];

    if (number_of_resolutions < 1 || number_of_resolutions > max_resolutions)
        return FIA_ERROR;

    double *kernels[max_resolutions];

    FIBITMAP **A = new FIBITMAP *[number_of_resolutions + 1];

    double kernel1[5] = { 1.0 / 16, 1.0 / 4, 3.0 / 8, 1.0 / 4, 1.0 / 16 };
    double kernel2[9] = { 1.0 / 16, 0.0, 1.0 / 4, 0.0, 3.0 / 8, 0.0, 1.0 / 4, 0.0, 1.0 / 16 };
    double kernel3[17] = { 1.0 / 16, 0.0, 0.0, 0.0, 1.0 / 4, 0.0, 0.0, 0.0, 3.0 / 8,
        0.0, 0.0, 0.0, 1.0 / 4, 0.0, 0.0, 0.0, 1.0 / 16
    };

    double kernel4[33] = { 1.0 / 16, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0 / 4,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3.0 / 8,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0 / 4,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0 / 16
    };

    int borders[max_resolutions] = { 2, 4, 8, 16 };
    kernels[0] = kernel1;
    kernels[1] = kernel2;
    kernels[2] = kernel3;
    kernels[3] = kernel4;

    double min, max;

    A[0] = FIA_ConvertToGreyscaleFloatType (src, FIT_DOUBLE);

    for(int i = 1; i <= number_of_resolutions; i++)
    {
        A[i] = GetApproximationImage (A[i - 1], kernels[i - 1], borders[i - 1]);

        W[i - 1] = FreeImage_Clone (A[i - 1]);
        FIA_SubtractGreyLevelImages (W[i - 1], A[i]);

#ifdef VERBOSE_DEBUG
//	    std::ostringstream ss;
		char ss[512];

//		ss << "C:\\Temp\\FIA_Tests\\A_" << i << ".bmp";
//      FIA_SaveFIBToFile (A[i - 1], ss.c_str (), BIT8);
		sprintf(ss, "C:\\Temp\\FIA_Tests\\A_%d.bmp", i);
        FIA_SimpleSaveFIBToFile (A[i - 1], ss);

//        ss << "C:\\Temp\\FIA_Tests\\W_" << i << ".bmp";
//        FIA_SaveFIBToFile (W[i - 1], ss.c_str (), BIT8);
		sprintf(ss, "C:\\Temp\\FIA_Tests\\W_%d.bmp", i);
        FIA_SimpleSaveFIBToFile (W[i - 1], ss);
#endif

        image_thresholds[i - 1] = GetMADValue (W[i - 1]) * k / 0.67;

        FIA_FindMinMax (W[i - 1], &min, &max);

        FIA_InPlaceThreshold (W[i - 1], min, image_thresholds[i - 1], 1.0);

#ifdef VERBOSE_DEBUG
  //      ss << "C:\\Temp\\FIA_Tests\\WThreshold_" << i << ".bmp";
    //    FIA_SaveFIBToFile (test, ss.c_str (), BIT8);
		sprintf(ss, "C:\\Temp\\FIA_Tests\\WThreshold_%d.bmp", i);
		FIA_SetBinaryPalette(W[i - 1]);
		FIA_SimpleSaveFIBToFile(W[i - 1], ss);
#endif
    }

    // Clean up approximation images
    for(int i = 0; i < number_of_resolutions; i++)
    {
        FreeImage_Unload (A[i]);
        A[i] = NULL;
    }

    delete[]A;

    return FIA_SUCCESS;
}

FIBITMAP *DLL_CALLCONV
FIA_MultiscaleProducts (FIBITMAP * src, int start_level, int levels)
{
    // Start level has to be within the number of levels.
    if (start_level > levels)
        return NULL;

    FIBITMAP **W = (FIBITMAP **) malloc (sizeof (FIBITMAP *) * levels);

    if (FIA_ATrousWaveletTransform (src, levels, W) == FIA_ERROR)
        return NULL;

    FIBITMAP *product_image = FreeImage_Clone (W[start_level - 1]);

    for(int i = start_level; i < levels; i++)
        FIA_MultiplyGreyLevelImages (product_image, W[i]);

    // Clean up resolution images
    for(int i = 0; i < levels; i++)
    {
        FreeImage_Unload (W[i]);
        W[i] = NULL;
    }

    free (W);
    W = NULL;

    return product_image;
}
