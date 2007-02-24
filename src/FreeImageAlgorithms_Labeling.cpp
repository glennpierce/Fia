#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Convolution_Private.h"
#include "FreeImageAlgorithms_Morphology.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utils.h"

#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_IO.h"

#include <list>
#include <vector>
#include <iostream>

class BINARY_LABEL
{
public:
	
	FIBITMAP* Label(FIBITMAP* src, unsigned char bg_colour);
};


typedef struct _run run;

struct _run
{
	int id;
	int x;
	int y;
	int end_x;
};


typedef struct _blobinfo blobinfo;

struct _blobinfo
{
	int x;
	int y;
	int end_x;
	int height;
	blobinfo *parent;
	int rank;
};


static blobinfo* blobpool = NULL;
static blobinfo* blobpool_ptr = NULL;

static void UnionFindInit(int entries)
{
	if(blobpool == NULL) {
		blobpool = (blobinfo*) malloc(sizeof(blobinfo) * entries);
		blobpool_ptr = blobpool;
	}
}

// New blob references itself as parent.
static blob* NewBlob()
{
	blob *b = blobpool_ptr;

	b->parent = b;
	b->rank = 0;
	blobpool_ptr++;

	return b;
}

static blob* FindBlob(blob *blob)
{
	// Path Compression
	// Since, anyway, we travel the path to the root during a find operation,
	// we might as well hung all the nodes on the path directly on the root.
	if (blob != blob->parent)
		blob->parent = FindBlob(blob->parent);


	return blob->parent;
}

static void MergeBlobs(blob *blob1, blob *blob2)
{
	// Union by rank, Always hanges the smaller tree off the larger
}



FIBITMAP* BINARY_LABEL::Label(FIBITMAP* src, unsigned char bg_colour)
{
	const int width = FreeImage_GetWidth(src);
	const int height = FreeImage_GetHeight(src);
	
	unsigned char *src_ptr;

	register int i = 0;
	register int x = 0;

	//Create pool of blobs.
	
	size_t pool_size = sizeof(run) * width * height / 2;	// Max possible number of blobs/runs ie every other pixel is on
	run* runs = (run *) malloc(pool_size);					// Array of all runs
	blobinfo* blobinfo_array = (blobinfo *) malloc(pool_size);		

	run* last_row_runs; // = (run*) malloc(sizeof(run*) * width / 2);		// Pointers to runs of last two rows

	int last_row_run_count = 0, row_run_count = 0, tmp_row_run_count = 0, total_run_count = 0, total_id = 0, runs_overlap = 0;

	// Do the first line
	src_ptr = (unsigned char*) FreeImage_GetScanLine(src, 0);

	// Pass through first row - Skip background pixels
	// All runs are new so no merging takes place.
	for(x=0; x < width; x++) {

		// If fg pixel
		if(!src_ptr[x])
			continue;

		// new blob
		runs[total_run_count].x = x;
		runs[total_run_count].y = 0;
		
		// While fg pixel increment.
		while(src_ptr[x++]) ;

		runs[total_run_count].end_x = x;			
		runs[total_run_count].id = total_id;	

		// Update point to last row runs
		//last_row_runs[last_row_run_count++] = &runs[total_run_count];

		// This is the first line so all new runs are new blobs update the blob info
		blobinfo_array[total_id].x = runs[total_run_count].x;
		blobinfo_array[total_id].y = 0;
		blobinfo_array[total_id].end_x = x;
		blobinfo_array[total_id].height = 1;

		total_id++;
		total_run_count++;
	}

	last_row_runs = runs;
	last_row_run_count = total_run_count;

	// Do the rest of the rows
	for(register int y=1; y < height; y++)
	{
	//	if(y >= 19)
	//		y=y;

		row_run_count = 0;

		src_ptr = (unsigned char*) FreeImage_GetScanLine(src, y);

		run tmp_run;
		tmp_run.id = -1;

		// Pass through next row - Skip background pixels
		for(x=0; x < width; x++) {

			// Skip bg pixels
			if(!src_ptr[x])
				continue;

			
			tmp_run.x = x;
			tmp_run.y = y;

			// While fg pixel increment.
			while(src_ptr[x++]) ;

			row_run_count++;
			tmp_run.end_x = x;
			
			if(tmp_run.x < 40)
				y=y;

			// Check whether run is overlapping with previous runs
			for(i=0; i < last_row_run_count; i++) {

				run *last_run = &last_row_runs[i];

				// No more overlapping blobs from last row
				//if(last_run->x > tmp_run.end_x)
				//	break;

				//int over = 1;

				// blobs overlap
				if(tmp_run.end_x >= last_run->x && tmp_run.x <= last_run->end_x) {
						
					// Does the previous run from last line have a different id - Assign old id to new
					if(last_run->id != runs[total_run_count - 1].id)
						

					if(tmp_run.id < 0)
						tmp_run.id = last_run->id;

					

					

					//over++;
				}
			}

			if(tmp_run.id < 0) { // We have no connected runs create a new blob
					
				tmp_run.id = total_id;	

				blobinfo_array[tmp_run.id].x = tmp_run.x;
				blobinfo_array[tmp_run.id].y = tmp_run.y;
				blobinfo_array[tmp_run.id].end_x = tmp_run.end_x;
				blobinfo_array[tmp_run.id].height = 1;

				total_id++;
			}else
			{
				// Merge blob details with last row overlapping blob
		
					if(tmp_run.x < blobinfo_array[tmp_run.id].x)
						blobinfo_array[tmp_run.id].x = tmp_run.x;

					// Does run have a greater end point than the previous max
					if(tmp_run.end_x > blobinfo_array[tmp_run.id].end_x)
						blobinfo_array[tmp_run.id].end_x = tmp_run.end_x;

					// Increase height of blob
				//	std::cout << "id " << tmp_run.id << "  height  " <<  blobinfo_array[tmp_run.id].height << std::endl; 
					blobinfo_array[tmp_run.id].height = y - blobinfo_array[tmp_run.id].y;
			}
				
			// Add the run
			runs[total_run_count].x  = tmp_run.x;
			runs[total_run_count].y  = tmp_run.y;
			runs[total_run_count].end_x  = tmp_run.end_x;
			runs[total_run_count].id = tmp_run.id;

			

			total_run_count++;
			//last_row_run_count++;	
		}

		last_row_runs = &runs[total_run_count - row_run_count];
		last_row_run_count = row_run_count;
	}



	FIBITMAP *dst = FreeImage_ConvertTo24Bits(src);

	for(int i=0; i < total_id; i++)
	{
		RECT rect;
		rect.left = blobinfo_array[i].x;
		rect.bottom = FreeImage_GetHeight(dst) - blobinfo_array[i].y;
		rect.right = blobinfo_array[i].end_x;
		rect.top = rect.bottom - blobinfo_array[i].height;

		FreeImageAlgorithms_DrawColourRect (dst, rect, RGB(255,0,0), 2);
	}
	


	FreeImageAlgorithms_SaveFIBToFile(dst, "C:\\Documents and Settings\\Pierce\\Desktop\\particle_rect.jpg", BIT24);

//	std::cout << "number of blobs: " << total_id - 1<< std::endl;	

	return NULL;
};

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Label(FIBITMAP* src, unsigned char bg_colour)
{
	BINARY_LABEL labeler;

	return labeler.Label(src, bg_colour);
};
