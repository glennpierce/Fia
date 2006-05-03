#ifndef __FREEIMAGE_ALGORITHMS_UTILS__
#define __FREEIMAGE_ALGORITHMS_UTILS__

#include "FreeImageAlgorithms.h"

/// Max function
template <class T> T
MAX(T a, T b)
{
	return (a > b) ? a: b;
}

/// Min function
template <class T> T
MIN(T a, T b)
{
	return (a < b) ? a: b;
}

/** This procedure computes minimum min and maximum max
 of n numbers using only (3n/2) - 2 comparisons.
 min = L[i1] and max = L[i2].
 ref: Aho A.V., Hopcroft J.E., Ullman J.D., 
 The design and analysis of computer algorithms, 
 Addison-Wesley, Reading, 1974.
*/
template <class T> void 
MAXMIN(const T* L, long n, T& max, T& min)
{
	long i1, i2, i, j;
	T x1, x2;
	long k1, k2;

	i1 = 0; i2 = 0; min = L[0]; max = L[0]; j = 0;
	if((n % 2) != 0)  j = 1;
	for(i = j; i < n; i+= 2) {
		k1 = i; k2 = i+1;
		x1 = L[k1]; x2 = L[k2];
		if(x1 > x2)	{
			k1 = k2;  k2 = i;
			x1 = x2;  x2 = L[k2];
		}
		if(x1 < min) {
			min = x1;  i1 = k1;
		}
		if(x2 > max) {
			max = x2;  i2 = k2;
		}
	}
}


// Finds the max element in an array returns the index and the value in the parameter
template <class T> long 
FINDMAX(const T* L, long n, T& max)
{
	long i, max_index = 0;
	T temp_max;

	if (n < 1) 
		return 0;

	temp_max = *L;

	for (i=1; i<n; i++) {

		if (*++L > temp_max) {
			temp_max = *L;
			max_index = i;
		}
	}

	max = temp_max;

	return max_index;
}

/// INPLACESWAP adopted from codeguru.com 
template <class T> void
INPLACESWAP(T& a, T& b)
{
	a ^= b; b ^= a; a ^= b;
}

// In place swap doesn't work for float point types
template <class T> void
SWAP(T& a, T& b)
{
	T tmp = b;
	
	b = a;
	a = tmp;
}

template <class T> double 
MeanAverage(const T* L, long n)
{
	double total = 0.0;

	for( int i = 0; i < n; i++)
		total += *L++; 	
			
	return total / n;
}



template <class T> int
ArrayReverse(T *array, long size)
{
	if(array == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	long mid_element = (size / 2) - 1;

	for (int i=0; i <= mid_element ; i++)
		SWAP(array[i], array[size - 1 - i]);

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

#endif