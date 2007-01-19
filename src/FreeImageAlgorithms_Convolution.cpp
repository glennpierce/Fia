#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#include <math.h>

#define BLOCKSIZE 8

struct ImageData
{
	int kernel_width;
	int kernel_height;
	int x_reminder;
	int y_reminder;
	int x_max_block_size;
	int y_max_block_size;
	int src_pitch_in_pixels;
	double sum;
};

static inline void SumRow(double *ptr, double *kernel, int kernel_index, ImageData *data)
{
	register double sum = 0.0;

	int x_max_block_size = data->x_max_block_size;
	register double *tmp;
	register int kernel_tmp;

	for(register int col=0; col < x_max_block_size; col+=BLOCKSIZE){
		 
		tmp = ptr + col;
		kernel_tmp = kernel_index + col;

		data->sum += tmp[0] * kernel[kernel_tmp] + tmp[1] * kernel[kernel_tmp + 1] + 
			   tmp[2] * kernel[kernel_tmp + 2] + tmp[3] * kernel[kernel_tmp + 3] + 
			   tmp[4] * kernel[kernel_tmp + 4] + tmp[5] * kernel[kernel_tmp + 5] + 
			   tmp[6] * kernel[kernel_tmp + 6] + tmp[7] * kernel[kernel_tmp + 7];
	}

	tmp = ptr + x_max_block_size;
	kernel_tmp = kernel_index + x_max_block_size;

	switch(data->x_reminder) {
		case 7: 
			data->sum += tmp[6] * kernel[kernel_tmp + 6] + 
				   tmp[5] * kernel[kernel_tmp + 5] + 
				   tmp[4] * kernel[kernel_tmp + 4] +
				   tmp[3] * kernel[kernel_tmp + 3] +
				   tmp[2] * kernel[kernel_tmp + 2] +
				   tmp[1] * kernel[kernel_tmp + 1] +
				   tmp[0] * kernel[kernel_tmp]; 
			break;
		
		case 6:
			data->sum += tmp[5] * kernel[kernel_tmp + 5] + 
				   tmp[4] * kernel[kernel_tmp + 4] + 
				   tmp[3] * kernel[kernel_tmp + 3] +
				   tmp[2] * kernel[kernel_tmp + 2] +
				   tmp[1] * kernel[kernel_tmp + 1] + 
				   tmp[0] * kernel[kernel_tmp];
			break;
		
		case 5:
			data->sum += tmp[4] * kernel[kernel_tmp + 4] + 
				   tmp[3] * kernel[kernel_tmp + 3] + 
				   tmp[2] * kernel[kernel_tmp + 2] + 
				   tmp[1] * kernel[kernel_tmp + 1] + 
				   tmp[0] * kernel[kernel_tmp];
			break;

		case 4:
			data->sum += tmp[3] * kernel[kernel_tmp + 3] + 
				   tmp[2] * kernel[kernel_tmp + 2] + 
				   tmp[1] * kernel[kernel_tmp + 1] + 
				   tmp[0] * kernel[kernel_tmp];
			break;
		
		case 3:
			data->sum += tmp[2] * kernel[kernel_tmp + 2] +
				   tmp[1] * kernel[kernel_tmp + 1] + 
				   tmp[0] * kernel[kernel_tmp];
			break;

		case 2:
			data->sum += tmp[1] * kernel[kernel_tmp + 1] +
				   tmp[0] * kernel[kernel_tmp];
			break; 

		case 1:
			data->sum += tmp[0] * kernel[kernel_tmp]; 
	}
}

static inline void SumKernel(double *src_row_ptr, double* kernel, ImageData *data)
{
	register int kernel_index = 0;
	register double *tmp_ptr = src_row_ptr;
		
	data->sum = 0.0;

	for(register int row=0; row < data->y_max_block_size; row+=BLOCKSIZE)
	{  
		SumRow(tmp_ptr, kernel, kernel_index, data);
		tmp_ptr += data->src_pitch_in_pixels;
		kernel_index += data->kernel_width; 
				
		SumRow(tmp_ptr, kernel, kernel_index, data);
		tmp_ptr += data->src_pitch_in_pixels;
		kernel_index += data->kernel_width; 

		SumRow(tmp_ptr, kernel, kernel_index, data);
		tmp_ptr += data->src_pitch_in_pixels;
		kernel_index += data->kernel_width; 
				
		SumRow(tmp_ptr, kernel, kernel_index, data);
		tmp_ptr += data->src_pitch_in_pixels;
		kernel_index += data->kernel_width; 
				
		SumRow(tmp_ptr, kernel, kernel_index, data);
		tmp_ptr += data->src_pitch_in_pixels;
		kernel_index += data->kernel_width; 
				
		SumRow(tmp_ptr, kernel, kernel_index, data);
		tmp_ptr += data->src_pitch_in_pixels;
		kernel_index += data->kernel_width; 
				
		SumRow(tmp_ptr, kernel, kernel_index, data);
		tmp_ptr += data->src_pitch_in_pixels;
		kernel_index += data->kernel_width; 
				
		SumRow(tmp_ptr, kernel, kernel_index, data);
		tmp_ptr += data->src_pitch_in_pixels;
		kernel_index += data->kernel_width; 
	} 
		
	switch(data->y_reminder)
	{
		case 7:
			SumRow(tmp_ptr, kernel, kernel_index, data);
			tmp_ptr += data->src_pitch_in_pixels;
			kernel_index += data->kernel_width; 
		case 6:
			SumRow(tmp_ptr, kernel, kernel_index, data);
			tmp_ptr += data->src_pitch_in_pixels;
			kernel_index += data->kernel_width; 
		case 5:
			SumRow(tmp_ptr, kernel, kernel_index, data);
			tmp_ptr += data->src_pitch_in_pixels;
			kernel_index += data->kernel_width; 
		case 4:
			SumRow(tmp_ptr, kernel, kernel_index, data);
			tmp_ptr += data->src_pitch_in_pixels;
			kernel_index += data->kernel_width; 
		case 3:
			SumRow(tmp_ptr, kernel, kernel_index, data);
			tmp_ptr += data->src_pitch_in_pixels;
			kernel_index += data->kernel_width; 
		case 2:
			SumRow(tmp_ptr, kernel, kernel_index, data);
			tmp_ptr += data->src_pitch_in_pixels;
			kernel_index += data->kernel_width; 
		case 1:
			SumRow(tmp_ptr, kernel, kernel_index, data);
	}
}

FIBITMAP* ConvolveOld(FIABITMAP* src, int kernel_x_radius, int kernel_y_radius, double *kernel, double divider)
{
	// Border must be large enough to account for kernel radius
	if(src->xborder < kernel_x_radius || src->yborder < kernel_y_radius)
		return NULL;

	const int src_image_width = FreeImage_GetWidth(src->fib);
	const int src_image_height = FreeImage_GetHeight(src->fib);
 
	const int dst_width = src_image_width - (2 * src->xborder);
	const int dst_height = src_image_height - (2 * src->yborder);

	FIBITMAP *dst = FreeImageAlgorithms_CloneImageType(src->fib, dst_width, dst_height);
	
	const int dst_pitch_in_pixels = FreeImage_GetPitch(dst) / sizeof(double);

	ImageData data;

	data.kernel_width = (kernel_x_radius * 2) + 1;
	data.kernel_height = (kernel_y_radius * 2) + 1;
	data.x_reminder = data.kernel_width % BLOCKSIZE;
	data.y_reminder = data.kernel_height % BLOCKSIZE;
	data.x_max_block_size = (data.kernel_width / BLOCKSIZE) * BLOCKSIZE;
	data.y_max_block_size = (data.kernel_height / BLOCKSIZE) * BLOCKSIZE;
	data.src_pitch_in_pixels = FreeImage_GetPitch(src->fib) / sizeof(double);

	double *src_first_pixel_address_ptr = (double*) FreeImage_GetBits(src->fib);
	double *dst_first_pixel_address_ptr = (double*) FreeImage_GetBits(dst);
	
	register double *dst_ptr;
	register double *src_row_ptr;

	for (register int y=0; y < dst_height; y++)
	{		
		src_row_ptr = (src_first_pixel_address_ptr + y * data.src_pitch_in_pixels);
		dst_ptr = (dst_first_pixel_address_ptr + y * dst_pitch_in_pixels);

		for (register int x=0; x < dst_width; x++) 
		{
			SumKernel(src_row_ptr, kernel, &data);
			*dst_ptr++ =  data.sum / divider;
			src_row_ptr++;
		}
	}

	return dst;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Convolve_Old(FIABITMAP* src, int kernel_x_radius, int kernel_y_radius, double *kernel, double divider)
{
	FIBITMAP *dst = NULL;

	if(!src->fib)
		return NULL;

	dst = ConvolveOld(src, kernel_x_radius, kernel_y_radius, kernel, divider);

	if(dst == NULL)
		FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to convolve with type.");

	return dst;
}


FilterKernel DLL_CALLCONV
FreeImageAlgorithms_NewKernel(int x_radius, int y_radius,
							  double *values, double divider)
{
	FilterKernel kernel;

	kernel.x_radius = x_radius;
	kernel.y_radius = y_radius;
	kernel.values = values;
	kernel.divider = divider;

	return kernel;
}


// The following code does a lot of loop unrolling for performance.
template<typename Tsrc>
Kernel<Tsrc>::Kernel(FIABITMAP* src, int x_radius, int y_radius, Tsrc *values, double divider)
:
	xborder(src->xborder),
	yborder(src->yborder),
	x_radius(x_radius),
	y_radius(y_radius),
	divider(divider),
	src_image_width(FreeImage_GetWidth(src->fib)),
	src_image_height(FreeImage_GetHeight(src->fib)),
	kernel_width(x_radius * 2 + 1),
	kernel_height(y_radius * 2 + 1),
	x_reminder(kernel_width % BLOCKSIZE),
	y_reminder(kernel_height % BLOCKSIZE),
	x_max_block_size((kernel_width / BLOCKSIZE) * BLOCKSIZE),
	y_max_block_size((kernel_height / BLOCKSIZE) * BLOCKSIZE),
	src_pitch_in_pixels(FreeImage_GetPitch(src->fib) / sizeof(Tsrc)),
	src_image_type(FreeImage_GetImageType(src->fib))
{
	// Border must be large enough to account for kernel radius
	if(xborder < x_radius || yborder < y_radius)
		return;

	this->dib = src->fib;
	this->values = values;	

	this->src_first_pixel_address_ptr = (Tsrc*) FreeImage_GetBits(this->dib);
	this->current_src_ptr = this->src_first_pixel_address_ptr;

	this->current_src_center_ptr = this->src_first_pixel_address_ptr
		+ (y_radius * this->src_pitch_in_pixels) - x_radius;
}

template<typename Tsrc>
Kernel<Tsrc>::Kernel(FIABITMAP* src, int x_radius, int y_radius, Tsrc *values)
: this(src, x_radius, y_radius, values, 1.0)
{}

template<typename Tsrc>
inline void Kernel<Tsrc>::ConvolveKernelRow(KernelIterator<Tsrc> &iterator)
{
	register double *tmp, *kernel_ptr;

	int x_max_block_size = this->x_max_block_size;
	int x_reminder = this->x_reminder;

	for(register int col=0; col < x_max_block_size; col+=BLOCKSIZE){
		 
		tmp = iterator.GetImagePtrValue() + col;
		kernel_ptr = iterator.GetKernelPtrValue() + col;

		sum += tmp[0] * kernel_ptr[0] + tmp[1] * kernel_ptr[1] + 
			   tmp[2] * kernel_ptr[2] + tmp[3] * kernel_ptr[3] + 
			   tmp[4] * kernel_ptr[4] + tmp[5] * kernel_ptr[5] + 
			   tmp[6] * kernel_ptr[6] + tmp[7] * kernel_ptr[7];
	}

	if(x_reminder) {
		tmp = iterator.GetImagePtrValue() + x_max_block_size;
		kernel_ptr = iterator.GetKernelPtrValue() + x_max_block_size;
	}

	switch(x_reminder) {

		case 7: 
			sum += tmp[6] * kernel_ptr[6] + 
				   tmp[5] * kernel_ptr[5] + 
				   tmp[4] * kernel_ptr[4] +
				   tmp[3] * kernel_ptr[3] +
				   tmp[2] * kernel_ptr[2] +
				   tmp[1] * kernel_ptr[1] +
				   tmp[0] * kernel_ptr[0]; 
			break;
		
		case 6:
			sum += tmp[5] * kernel_ptr[5] + 
				   tmp[4] * kernel_ptr[4] + 
				   tmp[3] * kernel_ptr[3] +
				   tmp[2] * kernel_ptr[2] +
				   tmp[1] * kernel_ptr[1] + 
				   tmp[0] *	kernel_ptr[0];
			break;
		
		case 5:
			sum += tmp[4] * kernel_ptr[4] + 
				   tmp[3] * kernel_ptr[3] + 
				   tmp[2] * kernel_ptr[2] + 
				   tmp[1] * kernel_ptr[1] + 
				   tmp[0] * kernel_ptr[0];
			break;

		case 4:
			sum += tmp[3] * kernel_ptr[3] + 
				   tmp[2] * kernel_ptr[2] + 
				   tmp[1] * kernel_ptr[1] + 
				   tmp[0] * kernel_ptr[0];
			break;
		
		case 3:
			sum += tmp[2] * kernel_ptr[2] +
				   tmp[1] * kernel_ptr[1] + 
				   tmp[0] * kernel_ptr[0];
			break;

		case 2:
			sum += tmp[1] * kernel_ptr[1] +
				   tmp[0] * kernel_ptr[0];
			break; 

		case 1:
			sum += tmp[0] * kernel_ptr[0]; 
	}
}

template<typename Tsrc>
inline void Kernel<Tsrc>::ConvolveKernel()
{
	this->sum = 0.0f;

	KernelIterator<Tsrc> iterator = this->Begin();

	for(register int row=0; row < this->y_max_block_size; row+=BLOCKSIZE)
	{  
		ConvolveKernelRow(iterator);
		iterator.IncrementByRow();
		
		ConvolveKernelRow(iterator);
		iterator.IncrementByRow();

		ConvolveKernelRow(iterator);
		iterator.IncrementByRow();

		ConvolveKernelRow(iterator);
		iterator.IncrementByRow();
			
		ConvolveKernelRow(iterator);
		iterator.IncrementByRow();
		
		ConvolveKernelRow(iterator);
		iterator.IncrementByRow();
		
		ConvolveKernelRow(iterator);
		iterator.IncrementByRow();
		
		ConvolveKernelRow(iterator);
		iterator.IncrementByRow(); 
	} 
		
	switch(this->y_reminder)
	{
		case 7:
			ConvolveKernelRow(iterator);
			iterator.IncrementByRow();

		case 6:
			ConvolveKernelRow(iterator);
			iterator.IncrementByRow();
		case 5:
			ConvolveKernelRow(iterator);
			iterator.IncrementByRow();
		case 4:
			ConvolveKernelRow(iterator);
			iterator.IncrementByRow();
		case 3:
			ConvolveKernelRow(iterator);
			iterator.IncrementByRow();
		case 2:
			ConvolveKernelRow(iterator);
			iterator.IncrementByRow();
		case 1:
			ConvolveKernelRow(iterator);
	}
}

template<typename Tsrc>
FIBITMAP* Kernel<Tsrc>::Convolve()
{
	const int dst_width = src_image_width - (2 * this->xborder);
	const int dst_height = src_image_height - (2 * this->yborder);

	FIBITMAP *dst = FreeImageAlgorithms_CloneImageType(this->dib, dst_width, dst_height);

	const int dst_pitch_in_pixels = FreeImage_GetPitch(dst) / sizeof(Tsrc);

	register double *dst_ptr;

	double *dst_first_pixel_address_ptr = (Tsrc*) FreeImage_GetBits(dst);
	
	for (register int y=0; y < dst_height; y++)
	{		
		this->Move(0,y);
		dst_ptr = (dst_first_pixel_address_ptr + y * dst_pitch_in_pixels);

		for (register int x=0; x < dst_width; x++) 
		{
			this->ConvolveKernel();
			*dst_ptr++ =  this->sum / this->divider;
			this->Increment();
		}
	}

	return dst;
};


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Convolve(FIABITMAP *src, FilterKernel kernel)
{
	FIBITMAP *dst = NULL;
	FIABITMAP *tmp = NULL;
	FIABITMAP border_tmp;

	if(!src)
		return NULL;

	tmp = src;

	// convert from src_type to FIT_BITMAP
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);

	switch(src_type) {
		
		case FIT_FLOAT:		// array of float: 32-bit
		case FIT_DOUBLE:	// array of double: 64-bit
			break;

		case FIT_INT16:		// array of short: signed 16-bit
		case FIT_UINT16:    // array of unsigned short: unsigned 16-bit
		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
		case FIT_INT32:		// array of long: signed 32-bit
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit	
		{
			// Converting to float. Seems to be much faster.		
			border_tmp.fib = FreeImageAlgorithms_ConvertToGreyscaleFloatType(src->fib, FIT_DOUBLE);
			border_tmp.xborder = src->xborder;
			border_tmp.yborder = src->yborder;
			
			tmp = &border_tmp;

			break;
		}

		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	Kernel<double> *kern = new Kernel<double>(tmp, kernel.x_radius,
					kernel.y_radius, kernel.values, kernel.divider);

	dst = kern->Convolve();

	FreeImage_Unload(border_tmp.fib);

	if(NULL == dst) {
		FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.", src_type, FIT_BITMAP);
	}

	delete kern;

	return dst;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_SeparableConvolve(FIABITMAP *src, FilterKernel kernel1, FilterKernel kernel2)
{
	FIBITMAP *tmp_dst = NULL, *dst = NULL;
	FIABITMAP *tmp = NULL, *tmp_border = NULL;
	FIABITMAP border_tmp;

	if(!src)
		return NULL;

	tmp = src;

	// convert from src_type to FIT_BITMAP
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);

	switch(src_type) {
		
		case FIT_FLOAT:		// array of float: 32-bit
		case FIT_DOUBLE:	// array of double: 64-bit
			break;

		case FIT_INT16:		// array of short: signed 16-bit
		case FIT_UINT16:    // array of unsigned short: unsigned 16-bit
		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
		case FIT_INT32:		// array of long: signed 32-bit
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit	
		{
			// Converting to float. Seems to be much faster.		
			border_tmp.fib = FreeImageAlgorithms_ConvertToGreyscaleFloatType(src->fib, FIT_DOUBLE);
			border_tmp.xborder = kernel1.x_radius;
			border_tmp.yborder = kernel1.y_radius;
			
			tmp = &border_tmp;

			break;
		}

		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	Kernel<double> *kern1 = new Kernel<double>(tmp, kernel1.x_radius,
								kernel1.y_radius, kernel1.values, kernel1.divider);

	tmp_dst = kern1->Convolve();
		
	tmp_border = FreeImageAlgorithms_SetBorder(tmp_dst, kernel2.x_radius, kernel2.y_radius);

	Kernel<double> *kern2 = new Kernel<double>(tmp_border, kernel2.x_radius,
				kernel2.y_radius, kernel2.values, kernel2.divider);

	dst = kern2->Convolve();

	FreeImageAlgorithms_Unload(tmp_border);
	FreeImage_Unload(tmp_dst);

	if(NULL == dst) {
		FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.", src_type, FIT_BITMAP);
	}

	return dst;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Sobel(FIBITMAP *src)
{
	double sobel_left_kernel[] = {-1.0, 0.0, 1.0,
								  -2.0, 0.0, 2.0,
								  -1.0, 0.0, 1.0};

	double sobel_top_kernel[] = { 1.0, 2.0, 1.0,
								  0.0, 0.0, 0.0,
								 -1.0, -2.0, -1.0};

	PROFILE_START("Adding Border"); 

	FIABITMAP *src_bordered = FreeImageAlgorithms_SetBorder(src, 10, 10);

	PROFILE_STOP("Adding Border");

	PROFILE_START("Making Kernels"); 

	FilterKernel convolve_kernel_left = FreeImageAlgorithms_NewKernel(1, 1,
		sobel_left_kernel, 1.0);

	FilterKernel convolve_kernel_top = FreeImageAlgorithms_NewKernel(1, 1,
		sobel_top_kernel, 1.0);

	PROFILE_STOP("Making Kernels"); 


	PROFILE_START("Sobel Left Edge"); 

	FIBITMAP* dib1 = FreeImageAlgorithms_Convolve(src_bordered, convolve_kernel_left);

	PROFILE_STOP("Sobel Left Edge"); 

	PROFILE_START("Sobel Top Edge"); 

	FIBITMAP* dib2 = FreeImageAlgorithms_Convolve(src_bordered, convolve_kernel_top);

	PROFILE_STOP("Sobel Top Edge"); 

	int dst_width = FreeImage_GetWidth(dib1);
	int dst_height = FreeImage_GetHeight(dib1);

	FIBITMAP *dst = FreeImage_AllocateT(FIT_DOUBLE, dst_width, dst_height,
										8, 0, 0, 0);

	const int dst_pitch_in_pixels = FreeImage_GetPitch(dst) / sizeof(double);

	register double *dst_ptr, *dib1_ptr, *dib2_ptr;

	double *dst_first_pixel_address_ptr = (double*) FreeImage_GetBits(dst);
	double *dib1_first_pixel_address_ptr = (double*) FreeImage_GetBits(dib1);
	double *dib2_first_pixel_address_ptr = (double*) FreeImage_GetBits(dib2);

	for (register int y=0; y < dst_height; y++)
	{		
		dst_ptr = (dst_first_pixel_address_ptr + y * dst_pitch_in_pixels);
		dib1_ptr = (dib1_first_pixel_address_ptr + y * dst_pitch_in_pixels);
		dib2_ptr = (dib2_first_pixel_address_ptr + y * dst_pitch_in_pixels);

		for (register int x=0; x < dst_width; x++) 
		{
			*dst_ptr++ = sqrt((*dib1_ptr * *dib1_ptr) + (*dib2_ptr * *dib2_ptr));
			++dib1_ptr;
			++dib2_ptr;
		}
	}

	FreeImageAlgorithms_Unload(src_bordered);
	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);

	return dst;
}

// Test
// Seems slow but maybe not for large kernels.
/*
FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_SeparableSobel(FIBITMAP *src)
{
	double sobel_horz_values[] = {-1.0, 0.0, 1.0};

	double sobel_vert_values[] = {1.0, 2.0, 1.0};

	FilterKernel sobel_horz_kernel = FreeImageAlgorithms_NewKernel(1, 0,
		sobel_horz_values, 1.0);

	FilterKernel sobel_vert_kernel = FreeImageAlgorithms_NewKernel(0, 1,
		sobel_vert_values, 1.0);

	FIBITMAP* dst = FreeImageAlgorithms_SeparableConvolve(src, sobel_horz_kernel, sobel_vert_kernel);

	return dst;
}
*/