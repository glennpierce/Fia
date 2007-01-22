#ifndef __FREEIMAGE_ALGORITHMS_CONVOLUTION__
#define __FREEIMAGE_ALGORITHMS_CONVOLUTION__

#include "FreeImageAlgorithms.h"

template <typename Tsrc>
class Kernel; 

template <typename Tsrc>
class KernelIterator
{
	public:

		KernelIterator(Kernel<Tsrc> *kernel)
		{
			this->kernel = kernel;
			this->current_kernel_ptr = kernel->KernelValues();
			this->current_image_ptr = kernel->CenterValuePtr();
		}
		
		inline void Increment()
		{
			this->current_kernel_ptr++;
			this->current_image_ptr++;
		}

		inline void IncrementByRow()
		{
			this->current_kernel_ptr += kernel->KernelWidth(); 
			this->current_image_ptr += kernel->ImagePitchInPixels();
		}

		inline Tsrc GetKernelValue()
		{
			return *(this->current_kernel_ptr);
		}

		inline Tsrc GetImageValue()
		{
			return *(this->current_image_ptr);
		}

		inline Tsrc* GetKernelPtrValue()
		{
			return this->current_kernel_ptr;
		}

		inline Tsrc* GetImagePtrValue()
		{
			return this->current_image_ptr;
		}

	private:

		Tsrc *current_kernel_ptr;
		Tsrc *current_image_ptr;
		Kernel<Tsrc> *kernel;
};


template <typename Tsrc>
class Kernel
{
	public:
		Kernel(FIABITMAP* src, int x_radius, int y_radius, Tsrc *values, double divider);
		
		inline void Move(int x, int y) { this->current_src_ptr = (this->src_first_pixel_address_ptr + (y * this->src_pitch_in_pixels) + x); }
		inline void MoveUpRow() { this->current_src_ptr += this->src_pitch_in_pixels; }
		inline void Increment() { this->current_src_ptr++; }
		inline int KernelWidth() { return this->kernel_width; }
		inline int ImagePitchInPixels() { return this->src_pitch_in_pixels; }
		inline Tsrc* KernelValues() { return this->values; }
		inline Tsrc* ImageStartPtr() { return this->src_first_pixel_address_ptr; }
		inline Tsrc* CenterValuePtr() { return this->current_src_ptr; }
		inline KernelIterator<Tsrc> Begin() { return KernelIterator<Tsrc>(this); }

		FIBITMAP* Convolve();	

	private:

		inline void ConvolveKernel();
		inline void ConvolveKernelRow(KernelIterator<Tsrc> &iterator);

		FIBITMAP *dib;
		const int xborder;
		const int yborder;
		const int x_radius;
		const int y_radius;
		const double divider;

		const int src_image_width;
		const int src_image_height;
		const int kernel_width;
		const int kernel_height;
		const int x_reminder;
		const int y_reminder;
		const int x_max_block_size;
		const int y_max_block_size;
		const int src_pitch_in_pixels;
		const FREE_IMAGE_TYPE src_image_type;

		double sum;
		
		Tsrc *values;	
		Tsrc *src_first_pixel_address_ptr;
		Tsrc *current_src_ptr;	
};


#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
*	Provides a convolution function.
*/

/** \brief Convolve and image with a kernel.
 *
 *  \param src FIBITMAP bitmap to perform the convolution on.
 *  \param kernel_x_radius for a kernel of width 3 the x radius would be 1.
 *  \param kernel_y_radius for a kernel of height 3 the y radius would be 1.
 *  \param kernel array representing the values of the kernel.
 *  \param divider The divider of the kernel);
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Convolve_Old(FIABITMAP* src, int kernel_x_radius, int kernel_y_radius, double *kernel, double divider);


typedef struct
{
	int x_radius;
	int y_radius;
	double *values;
	double divider;

} FilterKernel;

DLL_API FilterKernel DLL_CALLCONV
FreeImageAlgorithms_NewKernel(int x_radius, int y_radius, 
							  double *values, double divider);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Convolve(FIABITMAP *src, const FilterKernel kernel);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_SeparableConvolve(FIABITMAP *src, FilterKernel kernel1, FilterKernel kernel2);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Sobel(FIBITMAP *src);

#ifdef __cplusplus
}
#endif

#endif