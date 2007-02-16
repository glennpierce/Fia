#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Convolution_Private.h"
#include "FreeImageAlgorithms_Morphology.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utils.h"

#include <vector>

class BINARY_FLOODFILL
{
public:
	
	FIBITMAP* FloodFill(FIBITMAP* src, int seed_x, int seed_y, unsigned char fill_colour);

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
	unsigned char fill_colour;
	FIBITMAP *dst;
	unsigned char *first_pixel_ptr;
};

/*
    Finds the furthermost left and right boundaries of the fill area
    on a given y coordinate, starting from a given x coordinate, filling as it goes.
    Adds the resulting horizontal range to the queue of floodfill ranges,
    to be processed in the main loop.
*/
void BINARY_FLOODFILL::LinearFill(int x, int y)
{
	// Find Left Edge of bg or fg area
	int lFillLoc = x; //the location to check/fill on the left
	unsigned char* idx = this->first_pixel_ptr + this->src_pitch_in_pixels * y + x;

	while (1)
    {
		// Fill with the color
		*idx = this->fill_colour;

        lFillLoc--;     // Move left
        idx--; 
        
		// Exit loop if we're at edge of bitmap or fill color area
		if (lFillLoc <= 0  || *idx == this->fill_colour)
			break;
	}

	lFillLoc++;

    // Find Right Edge of Color Area
	int rFillLoc = x; //the location to check/fill on the left
	idx = this->first_pixel_ptr + this->src_pitch_in_pixels * y + x;

	while (1)
	{
		// Fill with the color
		*idx = this->fill_colour;

		rFillLoc++;     // Move right
		idx++; 
            
		// Exit loop if we're at edge of bitmap or fill color area
		if (rFillLoc >= this->width  || *idx == this->fill_colour)
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


FIBITMAP* BINARY_FLOODFILL::FloodFill(FIBITMAP* src, int seed_x, int seed_y, unsigned char fill_colour)
{
	this->width = FreeImage_GetWidth(src);
	this->height = FreeImage_GetHeight(src);
	this->ranges.reserve(width * height);
	this->fill_colour = fill_colour;
	this->src_pitch_in_pixels = FreeImage_GetPitch(src) / sizeof(unsigned char);

	this->dst = FreeImage_Clone(src);
	this->first_pixel_ptr = (unsigned char*) FreeImage_GetBits(dst);

	// First look around the 1 pixel edge of the image and mark any white pixels as 
	// a value of 2 if we are excluding border particles
	this->LinearFill(seed_x, seed_y);

	FloodFillRange range;

	while (!this->ranges.empty())
	{
		// Get Next Range Off the Queue
		range = ranges.back();
		ranges.pop_back();

		// Check Above and Below Each Pixel in the Floodfill Range
                
		unsigned char* down_idx = this->first_pixel_ptr +
		this->src_pitch_in_pixels * (range.line - 1) + range.left;

		unsigned char* up_idx = this->first_pixel_ptr +
		this->src_pitch_in_pixels * (range.line + 1) + range.left;

		for (int i = range.left; i <= range.right; i++)
		{
			// Start Fill Upwards
			if (range.line < (this->height - 1) && *up_idx != this->fill_colour)
				LinearFill(i, range.line + 1);
                    
			// Start Fill Downwards
			// If we're not below the bottom of the bitmap and the pixel below this one is within the color tolerance
			if (range.line > 0 && *down_idx != this->fill_colour)    
				LinearFill(i, range.line - 1);
					
			up_idx++;
			down_idx++;
		}
    }

	return dst;
};


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_FloodFill(FIBITMAP* src, int seed_x, int seed_y, unsigned char fill_colour)
{
	BINARY_FLOODFILL filler;

	return filler.FloodFill(src, seed_x, seed_y, fill_colour);
};
