/* 
 * Copyright 2007-2010 Glenn Pierce, Paul Barber,
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef __FREEIMAGE_ALGORITHMS_UTILS__
#define __FREEIMAGE_ALGORITHMS_UTILS__

#include "FreeImageAlgorithms.h"



void CheckMemory(void *ptr);

/// Max function
template <class T> inline T
MAX(T a, T b)
{
	return (a > b) ? a: b;
}

/// Min function
template <class T> inline T
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
template <class T> inline void 
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
template <class T> inline long 
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
template <class T> inline void
INPLACESWAP(T& a, T& b)
{
	a ^= b; b ^= a; a ^= b;
}

// In place swap doesn't work for float point types
template <class T> inline void
SWAP(T& a, T& b)
{
	register T tmp = b;
	
	b = a;
	a = tmp;
}

template <class T> inline double 
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
		return FIA_ERROR;

	long mid_element = (size / 2) - 1;

	for (int i=0; i <= mid_element ; i++)
		SWAP(array[i], array[size - 1 - i]);

	return FIA_SUCCESS;
}


/*
* This Quickselect routine is based on the algorithm described in
* "Numerical recipes in C", Second Edition,
* Cambridge University Press, 1992, Section 8.5, ISBN 0-521-43108-5
* This code by Nicolas Devillard - 1998. Public domain.
*/
template<typename Tsrc>
Tsrc quick_select_median(Tsrc arr[], int n)
{
	int low, high;
	int median;
	int middle, ll, hh;

	low = 0 ; high = n-1 ; median = (low + high) / 2;

	for (;;) {

		if (high <= low) /* One element only */
			return arr[median] ;

		if (high == low + 1) { /* Two elements only */
		
			if (arr[low] > arr[high])
				SWAP(arr[low], arr[high]) ;
	

			return arr[median] ;
		}

		/* Find median of low, middle and high items; swap into position low */
		middle = (low + high) / 2;
		
		if (arr[middle] > arr[high])
			SWAP(arr[middle], arr[high]) ;

		if (arr[low] > arr[high])
			SWAP(arr[low], arr[high]) ;

		if (arr[middle] > arr[low])
			SWAP(arr[middle], arr[low]) ;

		/* Swap low item (now in position middle) into position (low+1) */
		SWAP(arr[middle], arr[low+1]) ;

		/* Nibble from each end towards middle, swapping items when stuck */
		ll = low + 1;
		hh = high;

		for (;;) {

			do ll++; while (arr[low] > arr[ll]) ;

			do hh--; while (arr[hh] > arr[low]) ;

			if (hh < ll)
				break;

			SWAP(arr[ll], arr[hh]) ;
		}

		/* Swap middle item (in position low) back into correct position */
		SWAP(arr[low], arr[hh]) ;

		/* Re-set active partition */
		if (hh <= median)
			low = ll;

		if (hh >= median)
			high = hh - 1;
	}
}

int IntersectingRect(FIARECT r1, FIARECT r2, FIARECT *r3);

#endif
