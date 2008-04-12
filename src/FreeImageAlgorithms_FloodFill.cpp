/*
 * Copyright 2007 Glenn Pierce
 *
 * This file is part of FreeImageAlgorithms.
 *
 * FreeImageAlgorithms is free software: you can redistribute it and/or modify
 * it under the terms of the Lesser GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FreeImageAlgorithms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser GNU General Public License for more details.
 * 
 * You should have received a copy of the Lesser GNU General Public License
 * along with FreeImageAlgorithms.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"

#include <vector>

template <typename IntType>
class GS_FLOODFILL
{
public:
	
	int FloodFill(FIBITMAP* src, int seed_x, int seed_y, unsigned char fill_colour);

private:

	// Represents a linear range to be filled and branched from.
    struct FloodFillRange
    {
        int left;
        int right;
        int line;
    };

	std::vector<FloodFillRange> ranges;

	void LinearFill(int x, int y);

	int width;
	int height;
	int src_pitch_in_pixels;
	IntType fill_colour;
	IntType seed_colour;
	IntType *first_pixel_ptr;
	FIBITMAP *dst;
};

/*
    Finds the furthermost left and right boundaries of the fill area
    on a given y coordinate, starting from a given x coordinate, filling as it goes.
    Adds the resulting horizontal range to the queue of floodfill ranges,
    to be processed in the main loop.
*/
template <typename IntType>
void GS_FLOODFILL<IntType>::LinearFill(int x, int y)
{
	// Find Left Edge of bg or fg area
	int lFillLoc = x; //the location to check/fill on the left
	IntType* idx = this->first_pixel_ptr + this->src_pitch_in_pixels * y + x;

	while (1)
    {
		// Fill with the color
		*idx = this->fill_colour;

        lFillLoc--;     // Move left
        idx--; 
        
		// Exit loop if we're at edge of bitmap or fill color area
		if (*idx == this->fill_colour || *idx != this->seed_colour || lFillLoc <= 0)
			break;

		
	}

	lFillLoc++;

    // Find Right Edge of Color Area
	int rFillLoc = x; //the location to check/fill on the left
	idx = this->first_pixel_ptr + this->src_pitch_in_pixels * y + x + 1;

	while (1)
	{
		// Fill with the color
		*idx = this->fill_colour;

		rFillLoc++;     // Move right
		idx++; 
            
		// Exit loop if we're at edge of bitmap or fill color area
		if (*idx == this->fill_colour || *idx != this->seed_colour || rFillLoc >= this->width)
			break;
	}

	rFillLoc--;

	//add range to queue
	FloodFillRange r;
		   
	r.left = lFillLoc;
	r.right = rFillLoc;
	r.line = y;
		  
	ranges.push_back(r);
}

template <typename IntType>
int GS_FLOODFILL<IntType>::FloodFill(FIBITMAP* src, int seed_x, int seed_y, unsigned char fill_colour)
{
	this->width = FreeImage_GetWidth(src);
	this->height = FreeImage_GetHeight(src);
	this->ranges.reserve(width * height);
	this->fill_colour = fill_colour;
	this->src_pitch_in_pixels = FreeImage_GetPitch(src) / sizeof(IntType);

	this->first_pixel_ptr = (IntType*) FreeImage_GetBits(src);

	this->seed_colour = *(this->first_pixel_ptr + this->src_pitch_in_pixels * seed_y + seed_x);
	this->LinearFill(seed_x, seed_y);

	FloodFillRange range;

	while (!this->ranges.empty())
	{
		// Get Next Range Off the Queue
		range = ranges.back();
		ranges.pop_back();

		// Check Above and Below Each Pixel in the Floodfill Range
                
		IntType* down_idx = this->first_pixel_ptr +
		this->src_pitch_in_pixels * (range.line - 1) + range.left;

		IntType* up_idx = this->first_pixel_ptr +
		this->src_pitch_in_pixels * (range.line + 1) + range.left;

		for (int i = range.left; i <= range.right; i++)
		{
			// Start Fill Upwards
			if (range.line < (this->height - 1) && *up_idx == this->seed_colour)
				LinearFill(i, range.line + 1);
                    
			// Start Fill Downwards
			// If we're not below the bottom of the bitmap and the pixel below this one is within the color tolerance
			if (range.line > 0 && *down_idx == this->seed_colour)    
				LinearFill(i, range.line - 1);
					
			up_idx++;
			down_idx++;
		}
    }

	return FREEIMAGE_ALGORITHMS_SUCCESS;
};


GS_FLOODFILL<unsigned char>		floodfillUCharImage;
GS_FLOODFILL<unsigned short>	floodfillUShortImage;
GS_FLOODFILL<short>				floodfillShortImage;
GS_FLOODFILL<unsigned long>		floodfillULongImage;
GS_FLOODFILL<long>				floodfillLongImage;


FIBITMAP* DLL_CALLCONV
FIA_FloodFill(FIBITMAP* src, int seed_x, int seed_y, int fill_colour)
{
	if(!src) return
		NULL;

	FIBITMAP* dst = FreeImage_Clone(src);

	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src);
	int err = 0;

	switch(type) {
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				err = floodfillUCharImage.FloodFill(dst, seed_x, seed_y, fill_colour);
			break;
		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
				err = floodfillUShortImage.FloodFill(dst, seed_x, seed_y, fill_colour);
			break;
		case FIT_INT16:		// array of short: signed 16-bit
				err = floodfillShortImage.FloodFill(dst, seed_x, seed_y, fill_colour);
			break;
		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
				err = floodfillULongImage.FloodFill(dst, seed_x, seed_y, fill_colour);
			break;
		case FIT_INT32:		// array of long: signed 32-bit
				err = floodfillLongImage.FloodFill(dst, seed_x, seed_y, fill_colour);
			break;
		default:
			break;
	}

	if(err == 0) {
		FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.", type, FIT_BITMAP);
		return NULL;
	}

	return dst;
};


int DLL_CALLCONV
FIA_InPlaceFloodFill(FIBITMAP* src, int seed_x, int seed_y, int fill_colour)
{
	if(!src)
		return FREEIMAGE_ALGORITHMS_ERROR;

	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src);
	int err = 0;

	switch(type) {
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				err = floodfillUCharImage.FloodFill(src, seed_x, seed_y, fill_colour);
			break;
		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
				err = floodfillUShortImage.FloodFill(src, seed_x, seed_y, fill_colour);
			break;
		case FIT_INT16:		// array of short: signed 16-bit
				err = floodfillShortImage.FloodFill(src, seed_x, seed_y, fill_colour);
			break;
		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
				err = floodfillULongImage.FloodFill(src, seed_x, seed_y, fill_colour);
			break;
		case FIT_INT32:		// array of long: signed 32-bit
				err = floodfillLongImage.FloodFill(src, seed_x, seed_y, fill_colour);
			break;
		default:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	if(err == 0) {
		FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.", type, FIT_BITMAP);
		return FREEIMAGE_ALGORITHMS_ERROR;
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;
};

