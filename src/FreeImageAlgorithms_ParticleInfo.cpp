#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Convolution_Private.h"
#include "FreeImageAlgorithms_Morphology.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utils.h"

typedef struct _blob blob;

struct _blob
{
	int left;
	int bottom;
	int right;
	int top;
	int rank;
	blob *parent;
};

typedef struct _run run;

struct _run
{
	int x;
	int y;
	int end_x;
	blob *blob;
};

static blob* blobpool = NULL;
static blob* blobpool_ptr = NULL;
static size_t blobpool_size = 1;
static int blobcount = 0;

static void UnionFindInit(int initial_size)
{
	if(blobpool == NULL) {
		blobpool_size = initial_size;
		blobpool = (blob*) malloc(blobpool_size * sizeof(blob));
		blobpool_ptr = blobpool;
	}
}

static void UnionFindFree()
{
	free(blobpool);
	blobpool = NULL;
	blobpool_ptr = NULL;
}

// New blob references itself as parent.
static inline blob* NewBlob(run *run)
{
	blob *b = blobpool_ptr;

	b->parent = b;
	b->rank = 0;
	b->left = run->x;
	b->bottom = run->y;
	b->right = run->end_x;
	b->top = b->bottom;

	run->blob = b;

	blobpool_ptr++;
	blobcount++;

	return b;
}

static inline blob* FindBlob(blob *blob)
{
	// Path Compression
	// Since, anyway, we travel the path to the root during a find operation,
	// we might as well hung all the nodes on the path directly on the root.
	if (blob != blob->parent)
		blob->parent = FindBlob(blob->parent);

	return blob->parent;
}

// Return toplevel blob
static inline void MergeBlobs(blob *blob1, blob *blob2)
{
	// Union by rank, Always hanges the smaller tree off the larger
	blob *b1 = FindBlob(blob1);
	blob *b2 = FindBlob(blob2);

	if(b1->rank > b2->rank) {
		b2->parent = b1;
	}
	else {
	
		b1->parent = b2;

		if (b1->rank == b2->rank) 
			b2->rank++;
	}
	
	// Set new width height etc
	b1->parent->left = min(b1->left, b2->left);
	b1->parent->bottom = min(b1->bottom, b2->bottom);
	b1->parent->right= max(b1->right, b2->right);
	b1->parent->top = max(b1->top, b2->top);
}


int DLL_CALLCONV
FreeImageAlgorithms_ParticleInfo(FIBITMAP* src, PARTICLEINFO** info, unsigned char white_on_black)
{
	const int width = FreeImage_GetWidth(src);
	const int height = FreeImage_GetHeight(src);
	
	unsigned char *src_ptr;

	register int i = 0;
	register int x = 0;

	unsigned char bg_val = 0;

	if(!white_on_black)
		bg_val = 1;

	run* current_runs = (run *) malloc(sizeof(run) * width / 2);		// Array of all runs
	run* last_runs = (run *) malloc(sizeof(run) * width / 2);
	run* last_runs_ptr = last_runs, *current_runs_ptr = current_runs;

	UnionFindInit(width * height / 2);

	int last_row_run_count = 0, current_run_count = 0;

	// Do the first line
	src_ptr = (unsigned char*) FreeImage_GetScanLine(src, 0);

	// Pass through first row - Skip background pixels
	// All runs are new so no merging takes place.
	for(x=0; x < width; x++) {

		// If bg pixel
		if(src_ptr[x] == bg_val)
			continue;

		// new blob
		last_runs_ptr[last_row_run_count].x = x;
		last_runs_ptr[last_row_run_count].y = 0;
		
		// While fg pixel increment.
		while(src_ptr[x++] != bg_val && x < width) ;

		last_runs_ptr[last_row_run_count].end_x = x;			

		// This is the first line so all new runs are new blobs update the blob info
		NewBlob(&last_runs_ptr[last_row_run_count]);

		last_row_run_count++;
	}

	// Do the rest of the rows
	for(register int y=1; y < height; y++)
	{
		current_run_count = 0;

		src_ptr = (unsigned char*) FreeImage_GetScanLine(src, y);

		run tmp_run;

		// Pass through next row - Skip background pixels
		for(x=0; x < width; x++) {

			// Skip bg pixels
			if(src_ptr[x] == bg_val)
				continue;

			tmp_run.x = x;
			tmp_run.y = y;
			tmp_run.blob = NULL;

			// While fg pixel increment.
			while(src_ptr[x++] != bg_val && x < width) ;

			tmp_run.end_x = x;
					
			// Check whether run is overlapping with previous runs
			for(i=0; i < last_row_run_count; i++) {

				run *last_run = &last_runs_ptr[i];

				// Are runs overlapping ?
				if(tmp_run.end_x >= last_run->x && tmp_run.x <= last_run->end_x) {

					// Point to same blob as overlapped run if the blobs are the same
					// Or if its the first overlapping run in the previous line.
					if(tmp_run.blob == NULL || tmp_run.blob == last_run->blob) {

						tmp_run.blob = FindBlob(last_run->blob);

						// Set new width height etc
						if(last_run->x < tmp_run.blob->left)
							tmp_run.blob->left = last_run->x;	

						if(last_run->end_x > tmp_run.blob->right)
							tmp_run.blob->right = last_run->end_x;

						tmp_run.blob->top = y;
					}
					else {

						// We have more than one previous overlapping run.
						// The blobs are not the same so we merge them.
						MergeBlobs(tmp_run.blob, last_run->blob);
					}	
				}
			}

			// We have no connected runs create a new blob
			if(tmp_run.blob == NULL)
				NewBlob(&tmp_run);	
		
			// Add run to current run array
			current_runs_ptr[current_run_count].x  = tmp_run.x;
			current_runs_ptr[current_run_count].y  = tmp_run.y;
			current_runs_ptr[current_run_count].end_x  = tmp_run.end_x;
			current_runs_ptr[current_run_count].blob  = tmp_run.blob;

			current_run_count++;
		}

		// Switch pointer to current runs to last runs a reloop
		SWAP(last_runs_ptr, current_runs_ptr);

		last_row_run_count = current_run_count;
	}

	int number_of_blobs = 0;

	// Get blob count
	for(int i=0; i < blobcount; i++) {

		blob* ptr = &blobpool[i]; 

		if(ptr != ptr->parent)
			continue;
	
		number_of_blobs++;
	}
		
	 // Create PARTICLEINFO/BLOBINFO array
	*info = (PARTICLEINFO*) malloc (sizeof(PARTICLEINFO));
	(*info)->number_of_blobs = number_of_blobs;
	(*info)->blobs = (BLOBINFO*) malloc (sizeof(BLOBINFO) * number_of_blobs);

	// Get blob count
	for(int i=0, j=0; i < blobcount; i++) {

		blob* ptr = &blobpool[i]; 

		if(ptr != ptr->parent)
			continue;
	
		(*info)->blobs[j].left = ptr->left;
		(*info)->blobs[j].top = ptr->top;
		(*info)->blobs[j].right = ptr->right;
		(*info)->blobs[j].bottom = ptr->bottom;

		j++;
	}
	
	UnionFindFree();
	free(current_runs);
	free(last_runs);

	return FREEIMAGE_ALGORITHMS_SUCCESS;
};

void DLL_CALLCONV
FreeImageAlgorithms_FreeParticleInfo(PARTICLEINFO* info)
{
	free(info->blobs);
	info->blobs = NULL;

	free(info);
}