#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Particle.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"

typedef struct _blob blob;

struct _blob
{
	int left;
	int bottom;
	int right;
	int top;
	int area;
	int sum_x;
	int sum_y;

	int rank;
	blob *parent;
};

typedef struct _run run;

struct _run
{
	int x;
	int y;
	int end_x;
	int sum_x;

	blob *blob;
};

typedef struct
{
    blob* blobpool_start_ptr;  // Start of memory allocated for blobpool
    blob* blobpool_ptr;		   // The current pointer into the blobpool
    int blobpool_blobcount;	   // Number of blobs in the pool including merged ones.
						 	   // We don't want to delete merged blobs until the end for performance.
    int real_blobcount;		   // This is the real blob count that takes account of merging.

} BLOBPOOL;

static BLOBPOOL* UnionFindInit(int size)
{
	BLOBPOOL *pool = (BLOBPOOL*) malloc(sizeof(BLOBPOOL));

	CheckMemory(pool);

	pool->blobpool_start_ptr = (blob*) malloc(size * sizeof(blob));
 
	CheckMemory(pool->blobpool_start_ptr);

	pool->blobpool_ptr = pool->blobpool_start_ptr;
	pool->blobpool_blobcount = 0;
	pool->real_blobcount = 0;

	return pool;
}

static void UnionFindFree(BLOBPOOL* pool)
{
	free(pool->blobpool_start_ptr);
	pool->blobpool_start_ptr = NULL;
	pool->blobpool_ptr = NULL;
	free(pool);
}

// New blob references itself as parent.
static inline blob* NewBlob(BLOBPOOL *pool, unsigned int top_row, run *run)
{
	blob *b = pool->blobpool_ptr;

	b->parent = b;
	b->rank = 0;
	b->left = run->x;
	b->bottom = run->y;
	b->right = run->end_x;
	b->top = b->bottom;

	b->area = run->end_x - run->x + 1;
	b->sum_x = run->sum_x;
	b->sum_y = (top_row - run->y) * b->area;		// Current row times number in run

	run->blob = b;

	pool->blobpool_ptr++;
	pool->blobpool_blobcount++;
	pool->real_blobcount++;

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
static inline blob* MergeBlobs(BLOBPOOL *pool, blob *blob1, blob *blob2)
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
	b1->parent->right = max(b1->right, b2->right);
	b1->parent->top = max(b1->top, b2->top);
	b1->parent->area = b1->area + b2->area;
	b1->parent->sum_x = b1->sum_x + b2->sum_x;
	b1->parent->sum_y = b1->sum_y + b2->sum_y;

	pool->real_blobcount--;

	return b1->parent;
}


int DLL_CALLCONV
FreeImageAlgorithms_ParticleInfo(FIBITMAP* src, PARTICLEINFO** info, unsigned char white_on_black)
{
	if(src == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(FreeImage_GetBPP(src) != 8 && FreeImage_GetImageType(src) != FIT_BITMAP)
		return FREEIMAGE_ALGORITHMS_ERROR;

	const int width = FreeImage_GetWidth(src);
	const int height = FreeImage_GetHeight(src);
	
	unsigned int top_row = height - 1;
	unsigned char *src_ptr;

	register int i = 0;
	register int x = 0;

	unsigned char bg_val = 0;

	if(!white_on_black)
		bg_val = 1;

	run* current_runs = (run *) malloc(sizeof(run) * width / 2);		// Array of all runs
	CheckMemory(current_runs);

	run* last_runs = (run *) malloc(sizeof(run) * width / 2);
	CheckMemory(last_runs);

	run* last_runs_ptr = last_runs, *current_runs_ptr = current_runs;

	BLOBPOOL *pool = UnionFindInit(width * height / 2);

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
		last_runs_ptr[last_row_run_count].sum_x = 0;

		// While fg pixel increment.
		while(src_ptr[x] != bg_val && x < width) {

			last_runs_ptr[last_row_run_count].sum_x += x;		
			x++;
		} 
 
		last_runs_ptr[last_row_run_count].end_x = x - 1;			

		// This is the first line so all new runs are new blobs update the blob info
		NewBlob(pool, top_row, &last_runs_ptr[last_row_run_count]);

		last_row_run_count++;
	}

	blob *last_blob = NULL;

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
			tmp_run.sum_x = 0;
			tmp_run.blob = NULL;

			// While fg pixel increment.
			while(src_ptr[x] != bg_val && x < width) {
				tmp_run.sum_x += x;
				x++;
			}

			tmp_run.end_x = x - 1;
			
			// Check whether run is overlapping with previous runs
			for(i=0; i < last_row_run_count; i++) {

				run *last_run = &last_runs_ptr[i];
				last_blob = FindBlob(last_run->blob);

				// Are runs overlapping ?
				// The -1 are to allow pixels connected on the diagonal
				if(tmp_run.end_x >= last_run->x - 1 && tmp_run.x <= last_run->end_x + 1) {

					// Point to same blob as overlapped run if the blobs are the same
					// Or if its the first overlapping run in the previous line.
					if(tmp_run.blob == NULL) {

						// Set new width height etc
						tmp_run.blob = last_blob;

						if(tmp_run.x < last_blob->left)
							last_blob->left = tmp_run.x;	

						if(tmp_run.end_x > last_blob->right)
							last_blob->right = tmp_run.end_x;

						tmp_run.blob->top = y;

						tmp_run.blob->area = tmp_run.end_x - tmp_run.x + 1 + last_blob->area;
						tmp_run.blob->sum_x = tmp_run.sum_x + last_blob->sum_x;
						tmp_run.blob->sum_y += ((top_row - y) * (tmp_run.end_x - tmp_run.x + 1));
						
					}
					else if(tmp_run.blob != last_blob) {

						// We have more than one previous overlapping run.
						// The blobs are not the same so we merge them.
						tmp_run.blob = MergeBlobs(pool, tmp_run.blob, last_blob);
					}	
				}
			}

			// We have no connected runs create a new blob
			if(tmp_run.blob == NULL)
				NewBlob(pool, top_row, &tmp_run);	
		
			// Add run to current run array
			current_runs_ptr[current_run_count].x  = tmp_run.x;
			current_runs_ptr[current_run_count].y  = tmp_run.y;
			current_runs_ptr[current_run_count].end_x  = tmp_run.end_x;
			current_runs_ptr[current_run_count].sum_x  = tmp_run.sum_x;
			current_runs_ptr[current_run_count].blob  = tmp_run.blob;

			current_run_count++;
		}

		// Switch pointer to current runs to last runs a reloop
		SWAP(last_runs_ptr, current_runs_ptr);

		last_row_run_count = current_run_count;
	}	
	
	 // Create PARTICLEINFO/BLOBINFO array
	*info = (PARTICLEINFO*) malloc (sizeof(PARTICLEINFO));
	CheckMemory(*info);

	(*info)->number_of_blobs = pool->real_blobcount;
	(*info)->blobs = (BLOBINFO*) malloc (sizeof(BLOBINFO) * pool->real_blobcount);
	CheckMemory((*info)->blobs);
	
	// Get blobs
	for(int i=0, j=0; i < pool->blobpool_blobcount; i++) {

		blob* ptr = &(pool->blobpool_start_ptr[i]); 
 
		if(ptr != ptr->parent)
			continue;
	
		(*info)->blobs[j].rect.left = ptr->left;
		(*info)->blobs[j].rect.top = top_row - ptr->top;  
		(*info)->blobs[j].rect.right = ptr->right;
		(*info)->blobs[j].rect.bottom = top_row - ptr->bottom;
		(*info)->blobs[j].area = ptr->area;
		(*info)->blobs[j].center_x = ptr->sum_x / ptr->area; 
		(*info)->blobs[j].center_y = ptr->sum_y / ptr->area;

		j++;
	}

	UnionFindFree(pool);
	free(current_runs);
	current_runs = NULL;
	free(last_runs);
	last_runs = NULL;

	return FREEIMAGE_ALGORITHMS_SUCCESS;
};

void DLL_CALLCONV
FreeImageAlgorithms_FreeParticleInfo(PARTICLEINFO* info)
{
	free(info->blobs);
	info->blobs = NULL;

	free(info);
}