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

#ifndef __FREEIMAGE_ALGORITHMS_CONVOLUTION_PRIVATE__
#define __FREEIMAGE_ALGORITHMS_CONVOLUTION_PRIVATE__

#include "FreeImageAlgorithms.h"

#define BLOCKSIZE 8

template <typename Tsrc>
class Kernel; 

template <typename Tsrc>
class KernelIterator
{
	public:

		KernelIterator(Kernel<Tsrc> *kernel)
		{
			this->kernel = kernel;
			this->current_kernel_ptr = const_cast<Tsrc*>(kernel->KernelValues());
			this->current_image_ptr = kernel->KernelFirstValuePtr();
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

		inline Kernel<Tsrc>* GetKernel() { return this->kernel; }

	private:

		Tsrc *current_kernel_ptr;
		Tsrc *current_image_ptr;
		Kernel<Tsrc> *kernel;
};


template <typename Tsrc>
class Kernel
{
	public:
		Kernel(FIABITMAP* src, int x_radius, int y_radius, const Tsrc *values, double divider);
		
		inline void Move(int x, int y)
		{ 
			this->current_src_ptr = (const_cast<Tsrc*>(this->src_first_pixel_address_ptr) 
				+ (y + this->y_amount_to_image) * this->src_pitch_in_pixels + x_amount_to_image + x);
			
			this->current_src_center_ptr = this->current_src_ptr
				+ (y_radius * this->src_pitch_in_pixels) - x_radius;
		}
		
		inline void MoveUpRow()
		{	this->current_src_ptr += this->src_pitch_in_pixels;
			this->current_src_center_ptr += this->src_pitch_in_pixels;
		}

		inline void Increment()
		{ 
			++(this->current_src_ptr);
			++(this->current_src_center_ptr);
		}
		
		inline int ImageWidth() { return this->src_image_width; }
		inline int ImageHeight() { return this->src_image_height; }
		inline int KernelWidth() { return this->kernel_width; }
		inline int ImagePitchInPixels() { return this->src_pitch_in_pixels; }
		inline const Tsrc* KernelValues() { return this->values; }
		inline Tsrc GetNumberOfBlocksOfEightInKernelRows() { return this->x_max_block_size; }
		inline Tsrc GetNumberOfBlocksOfEightInKernelColoumns() { return this->y_max_block_size; }
		inline Tsrc GetRemainderAfterBlocksInRows() { return this->x_reminder; }
		inline Tsrc GetRemainderAfterBlocksInColoumns() { return this->y_reminder; }
		inline Tsrc* ImageStartPtr() { return this->src_first_pixel_address_ptr; }
		inline Tsrc* KernelFirstValuePtr() { return this->current_src_ptr; }
		inline Tsrc KernelFirstValue() { return *(this->current_src_ptr); }
		inline Tsrc* KernelCenterValuePtr() { return this->current_src_center_ptr; }
		inline Tsrc KernelCenterValue() { return *(this->current_src_center_ptr); }
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
		const Tsrc *values;	
		int x_amount_to_image;
		int y_amount_to_image;

		Tsrc *src_first_pixel_address_ptr;

		double sum;
		
		Tsrc *current_src_center_ptr;
		Tsrc *current_src_ptr;	
};

// The following code does a lot of loop unrolling for performance.
template<typename Tsrc>
Kernel<Tsrc>::Kernel(FIABITMAP* src, int x_radius, int y_radius, const Tsrc *values, double divider)
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
	src_image_type(FreeImage_GetImageType(src->fib)),
	values(values)
{
	// Border must be large enough to account for kernel radius
	if(xborder < x_radius || yborder < y_radius)
		return;

	this->dib = src->fib;

	this->src_first_pixel_address_ptr = (Tsrc*) FreeImage_GetBits(this->dib);
	this->current_src_ptr = const_cast<Tsrc*>(this->src_first_pixel_address_ptr);

	// Amount we need to move in x to get pass the border and onto the image.
	this->x_amount_to_image = this->xborder - this->x_radius; 
	this->y_amount_to_image = this->yborder - this->y_radius; 

	this->Move(0, 0);
}

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

#endif
