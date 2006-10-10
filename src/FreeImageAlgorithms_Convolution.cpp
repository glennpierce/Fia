#include "FreeImageAlgorithms_Convolution.h"


template<class Tsrc>
class CONVOLUTION
{
public:
	FIBITMAP* Convolve(FIABITMAP src, int kernel_x_radius, int kernel_y_radius, int **kernel);
private:
	inline void SumRow(int row, int size_of_maxblocks, int reminder);
	Tsrc *src_row_ptr;
	Tsrc *tmp_ptr;
	Tsrc *dst_ptr;

};

CONVOLUTION<unsigned char>		convolveUCharImage;
CONVOLUTION<unsigned short>		convolveUShortImage;
CONVOLUTION<short>				convolveShortImage;
CONVOLUTION<unsigned long>		convolveULongImage;
CONVOLUTION<long>				convolveLongImage;
CONVOLUTION<float>				convolveFloatImage;
CONVOLUTION<double>				convolveDoubleImage;
									

template<class Tsrc> 
inline void CONVOLUTION<Tsrc>::SumRow(int row, int size_of_maxblocks, int reminder)
{
	for(int col=0; col < size_of_maxblocks; col+=8)
		*dst_ptr += *(tmp_ptr + col) * array[row][col] + *(tmp_ptr+1+col) * array[row][1+col] + \
		*(tmp_ptr+2+col) * array[row][2+col] + *(tmp_ptr+3+col) * array[row][3+col] + *(tmp_ptr+4+col) * array[row][4+col] + \
		*(tmp_ptr+5+col) * array[row][5+col] + *(tmp_ptr+6+col) * array[row][6+col] + *(tmp_ptr+7+col) * array[row][7+col];
	
	switch(reminder) {
		case 7: 
			*dst_ptr += *(tmp_ptr + x_max_block_size + 6) + *(tmp_ptr + x_max_block_size + 5) + *(tmp_ptr + x_max_block_size + 4) + \
				*(tmp_ptr + x_max_block_size + 3) + *(tmp_ptr + x_max_block_size + 2) + \
				*(tmp_ptr + x_max_block_size + 1) + *(tmp_ptr + x_max_block_size); 
			break;
		
		case 6:
			*dst_ptr += *(tmp_ptr + x_max_block_size + 5) + *(tmp_ptr + x_max_block_size + 4) + *(tmp_ptr + x_max_block_size + 3) + \
				*(tmp_ptr + x_max_block_size + 2) + *(tmp_ptr + x_max_block_size + 1) + *(tmp_ptr + x_max_block_size);
			break;
		
		case 5:
			*dst_ptr += *(tmp_ptr + x_max_block_size + 4) + *(tmp_ptr + x_max_block_size + 3) + *(tmp_ptr + x_max_block_size + 2) + \
				*(tmp_ptr + x_max_block_size + 1) + *(tmp_ptr + x_max_block_size);
			break;

		case 4:
			*dst_ptr += *(tmp_ptr + x_max_block_size + 3) + *(tmp_ptr + x_max_block_size + 2) + *(tmp_ptr + x_max_block_size + 1) + \
				*(tmp_ptr + x_max_block_size);
			break;
		
		case 3:
			*dst_ptr += *(tmp_ptr + x_max_block_size + 2) + *(tmp_ptr + x_max_block_size + 1) + *(tmp_ptr + x_max_block_size);
			break;

		case 2:
			*dst_ptr += *(tmp_ptr + x_max_block_size + 1) + *(tmp_ptr + x_max_block_size);
			break; 

		case 1:
			*dst_ptr += *(tmp_ptr + x_max_block_size); 
	}
}

template<class Tsrc> 
FIBITMAP* CONVOLUTION<Tsrc>::Convolve(FIABITMAP src, int kernel_x_radius, int kernel_y_radius, int **kernel)
{
	// Border must be large enough to account for kernel radius
	if(src.border < MAX(kernel_x_radius, kernel_y_radius))
		return NULL;

	const int image_width = FreeImage_GetWidth(src);
	const int image_height = FreeImage_GetWidth(src);
	const int blocksize = 8;
	const int kernel_width = (kernel_x_radius * 2) + 1;
	const int kernel_height = (kernel_y_radius * 2) + 1;
	const int x_blocks = kernel_width / blocksize;  
	const int x_reminder = kernel_width % blocksize;
	const int y_blocks = kernel_height / blocksize;  
	const int x_max_block_size = (kernel_width / blocksize) * blocksize; 
	const int y_max_block_size = (kernel_height / blocksize) * blocksize;  
	const int y_reminder = kernel_height - y_max_block_size; 	
	const int x_range = image_width - kernel_width;
	const int y_range = image_height - kernel_height;

	int tmp_row;

	FIBITMAP *dst = FreeImageAlgorithms_CloneImageType(src, width - (2 * src.border), 
		height - (2 * src.border));

	for (int y=0; y < y_range; y++)
	{		
		this->src_row_ptr = FreeImage_GetScanLine(src, y);
		this->dst_ptr = FreeImage_GetScanLine(dst, y);
	
		for (int x=0; x < x_range; x++) 
		{
			pIn=pInS;
			*pOut = 0.0; 
		
			for(int row=0; row < x_max_block_size; row+=8)
			{  
				tmp_row = row;
				
				SumRow(tmp_row, x_max_block_size, x_reminder);
				pIn   += inInfo.rawPixels; tmp_row++; 
				
				BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
				pIn   += inInfo.rawPixels; tmp_row++; 

				BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)    
				pIn   += inInfo.rawPixels; tmp_row++; 
				
				BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)   
				pIn   += inInfo.rawPixels; tmp_row++;
				
				BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
				pIn   += inInfo.rawPixels; tmp_row++; 
				
				BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
				pIn   += inInfo.rawPixels; tmp_row++; 
				
				BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
				pIn   += inInfo.rawPixels; tmp_row++;   
				
				BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
				pIn   += inInfo.rawPixels;
			} 
			
			switch(y_reminder) { \
				case 7:
					BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
					pIn   += inInfo.rawPixels; tmp_row++; 	
				case 6:
					BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
					pIn   += inInfo.rawPixels; tmp_row++; 
				case 5:
					BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
					pIn   += inInfo.rawPixels; tmp_row++; 
				case 4:
					BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
					pIn   += inInfo.rawPixels; tmp_row++; 
				case 3:
					BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
					pIn   += inInfo.rawPixels; tmp_row++; 
				case 2:
					BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
					pIn   += inInfo.rawPixels; tmp_row++; 
				case 1:
					BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
			}

			pInS++;
			pOut++;
		}
	}




}


/*
void smooth7x7 (IPIImageRef in, IPIImageRef out, int border_size, int x_radius, int y_radius)
{
	IPIImageInfo inInfo, outInfo;
	float *pInS, *pIn=NULL, *pOut;
	register int row, col, x, y;
	int x_blocks, x_reminder, y_blocks, blocksize = 8, y_reminder;
	int tmp_row=0;
	int x_max_block_size, y_max_block_size;
	int kernel_width = (x_radius * 2) + 1;
	int kernel_height = (y_radius * 2) + 1;
	int x_range, y_range;
	float middle_kernel_value;
	
	IPI_Cast(in, IPI_PIXEL_SGL);
	IPI_Cast(out, IPI_PIXEL_SGL);
	
	IPI_GetImageInfo (in, &inInfo);
	IPI_GetImageInfo (out, &outInfo);
	


	x_blocks = kernel_width / blocksize;  
	x_reminder = kernel_width % blocksize;
	y_blocks = kernel_height / blocksize;  
	
	x_max_block_size = (kernel_width / blocksize) * blocksize; 
	y_max_block_size = (kernel_height / blocksize) * blocksize;  
	
	y_reminder = kernel_height - y_max_block_size; 	
	
	x_range = inInfo.width - kernel_width;
	y_range = inInfo.height - kernel_height;
	
	for (y=0; y < y_range; y++)
	{		
		pInS  =  (inInfo.firstPixelAddress.PixFloat_Ptr + (y - border_size) * inInfo.rawPixels) - border_size;
		pOut = (outInfo.firstPixelAddress.PixFloat_Ptr + y * outInfo.rawPixels);
	
		for (x=0; x < x_range; x++) 
		{
			pIn=pInS;
			*pOut = 0.0; 
		
			for(row=0; row < x_max_block_size; row+=8)
			{  
				tmp_row = row;
					
				BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
				pIn   += inInfo.rawPixels; tmp_row++; 
				
				BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
				pIn   += inInfo.rawPixels; tmp_row++; 

				BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)    
				pIn   += inInfo.rawPixels; tmp_row++; 
				
				BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)   
				pIn   += inInfo.rawPixels; tmp_row++;
				
				BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
				pIn   += inInfo.rawPixels; tmp_row++; 
				
				BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
				pIn   += inInfo.rawPixels; tmp_row++; 
				
				BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
				pIn   += inInfo.rawPixels; tmp_row++;   
				
				BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
				pIn   += inInfo.rawPixels;
			} 
			
			switch(y_reminder) { \
				case 7:
					BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
					pIn   += inInfo.rawPixels; tmp_row++; 	
				case 6:
					BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
					pIn   += inInfo.rawPixels; tmp_row++; 
				case 5:
					BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
					pIn   += inInfo.rawPixels; tmp_row++; 
				case 4:
					BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
					pIn   += inInfo.rawPixels; tmp_row++; 
				case 3:
					BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
					pIn   += inInfo.rawPixels; tmp_row++; 
				case 2:
					BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
					pIn   += inInfo.rawPixels; tmp_row++; 
				case 1:
					BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder)
			}

			pInS++;
			pOut++;
		}
	}

	
	

	
	
}
*/
