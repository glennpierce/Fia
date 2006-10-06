static IPIError IPI_ImageBorderSize (IPIImageRef source_image, long Operation, long *border_size)
{
// By P Barber, 05.06.03
// IPI_ImageBorderSize is in the IMAQ vision .fp library file but never existed as a function
// but here is my version of it.

	IPIError error;
	IPIImageInfo info;
	IPIImageRef temp=0;
	
	if (source_image<=0) return (IPI_ERR_BADIMAGEREF);
	if (border_size==NULL) return (IPI_ERR_NULLPTR);

	error = IPI_GetImageInfo (source_image, &info);  if (error!=IPI_ERR_NOERROR) return (error);

	if (Operation==0)  // get border size
	{
		*border_size = info.border;
	}
	else			   // set border size
	{
		if (*border_size<0) return (IPI_ERR_BADBORDER);
		if (*border_size == info.border) return (IPI_ERR_NOERROR);  // border is already the correct size

		error = IPI_Create (&temp, info.pixelType, *border_size);                     if (error!=IPI_ERR_NOERROR) return (error);
		error = IPI_SetImageSize (temp, info.width, info.height);                     if (error!=IPI_ERR_NOERROR) return (error);
		error = IPI_ImageToImage (source_image, temp, 0, 0);			      if (error!=IPI_ERR_NOERROR) return (error);
		error = IPI_SetImageOffset (temp, info.xOffset, info.yOffset);	              if (error!=IPI_ERR_NOERROR) return (error);
		error = IPI_SetImageCalibration (temp, info.unit, info.xCalib, info.yCalib);  if (error!=IPI_ERR_NOERROR) return (error);
		error = IPI_Copy (temp, source_image);								          if (error!=IPI_ERR_NOERROR) return (error);
		error = IPI_Dispose (temp);								                      if (error!=IPI_ERR_NOERROR) return (error);
	}
	return(IPI_ERR_NOERROR);
}


static float array[21][21] = {{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 			      {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 			      {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 			      {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 			      {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 			 	  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 			  	  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
							  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
							  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
							  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
							  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 			      {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 			      {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 			      {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 			      {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 				  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 				  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
							  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
							  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
							  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
							  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}
};


/*
static float array[7][7] = {{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 			    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 			    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 			    {1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0},
			 			    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 				{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			 				{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}};
*/

/*
static float array[3][3] = {{1.0, 1.0, 1.0},
			 			    {1.0, 1.0, 1.0},
			 			    {1.0, 1.0, 1.0}};
							
*/
							
#define BLOCK8_COLSUM(x_blocks, x_max_block_size, kernel_width, x_reminder) \
	for(col=0; col < x_max_block_size; col+=8) { \
		*pOut += *(pIn + col) * array[tmp_row][col] + *(pIn+1+col) * array[tmp_row][1+col] + *(pIn+2+col) * array[tmp_row][2+col] + *(pIn+3+col) * array[tmp_row][3+col] + *(pIn+4+col) * array[tmp_row][4+col] + *(pIn+5+col) * array[tmp_row][5+col] + *(pIn+6+col) * array[tmp_row][6+col] + *(pIn+7+col) * array[tmp_row][7+col]; } \
	switch(x_reminder) { \
		case 7: *pOut += *(pIn + x_max_block_size + 6) + *(pIn + x_max_block_size + 5) + *(pIn + x_max_block_size + 4) + *(pIn + x_max_block_size + 3) + *(pIn + x_max_block_size + 2) + *(pIn + x_max_block_size + 1) + *(pIn + x_max_block_size); break; \
		case 6: *pOut += *(pIn + x_max_block_size + 5) + *(pIn + x_max_block_size + 4) + *(pIn + x_max_block_size + 3) + *(pIn + x_max_block_size + 2) + *(pIn + x_max_block_size + 1) + *(pIn + x_max_block_size); break; \
		case 5: *pOut += *(pIn + x_max_block_size + 4) + *(pIn + x_max_block_size + 3) + *(pIn + x_max_block_size + 2) + *(pIn + x_max_block_size + 1) + *(pIn + x_max_block_size); break; \
		case 4: *pOut += *(pIn + x_max_block_size + 3) + *(pIn + x_max_block_size + 2) + *(pIn + x_max_block_size + 1) + *(pIn + x_max_block_size); break; \
		case 3: *pOut += *(pIn + x_max_block_size + 2) + *(pIn + x_max_block_size + 1) + *(pIn + x_max_block_size); break; \
		case 2: *pOut += *(pIn + x_max_block_size + 1) + *(pIn + x_max_block_size); break; \
		case 1: *pOut += *(pIn + x_max_block_size); \
	}
																																																									   
																																																									   
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
	
	PROFILE_START("smooth7x7");  

	// Set the middle kernel val to 0 so it does not have an effect.
	middle_kernel_value = array[x_radius][y_radius];
	array[x_radius][y_radius] = 0.0;
	
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

	
	// Correct for middle value of kernel
	for (y=0; y < y_range; y++)
	{	
		pInS  =  (inInfo.firstPixelAddress.PixFloat_Ptr + (y - border_size) * inInfo.rawPixels) - border_size;
		pOut = (outInfo.firstPixelAddress.PixFloat_Ptr + y * outInfo.rawPixels);
	
		for (x=0; x < x_range; x++) 
		{
			pIn = pInS + (inInfo.rawPixels * y_radius) + x_radius;
			
			*pOut = (*pOut + (*pIn * middle_kernel_value)) / 440.0f;
			pInS++; 
			pOut++;  
		}
	}

	
	PROFILE_STOP("smooth7x7"); 
}
