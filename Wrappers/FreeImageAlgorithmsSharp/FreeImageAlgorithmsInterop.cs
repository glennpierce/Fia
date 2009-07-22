using System;
using System.Drawing;
using System.Runtime.InteropServices;

namespace FreeImage
{
	public enum FIA_BITDEPTH
	{
		BIT8,
		BIT16,
		BIT24
	}
	
    internal sealed class FreeImageAlgorithmsNativeMethods
    {
        private FreeImageAlgorithmsNativeMethods() {}

        enum FREEIMAGE_ALGORITHMS_SAVE_BITDEPTH {BIT8, BIT24};

        [DllImport("gdi32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool DeleteObject(IntPtr hObject);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_IsGreyScale")]	
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool IsGreyScale(uint dib);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_LoadFIBFromFile")]
        internal static extern uint LoadFIBFromFile(string filepath);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_SaveFIBToFile")]
        internal static extern int SaveFIBToFile(uint dib, string filepath, FIA_BITDEPTH bitDepth);
		
        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_HistEq")]
        internal static extern uint FreeImage_HistEq(uint dib);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_GetDibSection")]
        internal static extern IntPtr GetDibSection(uint dib, IntPtr hdc, int left, int top, int right, int bottom);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_FibToHBitmap")]
        internal static extern IntPtr ToHBitmap(uint dib);  

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_HBitmapToFIB")]
        internal static extern uint HBitmapToFIB(IntPtr hdc, IntPtr hbitmap);  

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_FreeHBitmap")]
        internal static extern void FreeHBitmap(IntPtr hbitmap);  

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_SetGreyLevelPalette")]
        internal static extern int SetGreyLevelPalette(uint dib);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_FindMinMax")]
        internal static extern void FindMinMax(uint dib, out double min, out double max);
        
        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_ConvertInt16ToUInt16")]
        internal static extern uint ConvertInt16ToUInt16(uint dib);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_LinearScaleToStandardType")]
        internal static extern uint LinearScaleToStandardType(uint dib, double min, double max, out double min_within_image, out double max_within_image);
    
        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_SimplePaste")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool SimplePaste(uint dst, uint src, int left, int bottom);
		
		[DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_SimplePasteFromTopLeft")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool SimplePasteFromTopLeft(uint dst, uint src, int left, int top);
		
        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_StretchImageToType")]
        internal static extern uint StretchImageToType(uint src, FreeImageType type, double max);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_Is16BitReally12BitImage")]
        internal static extern bool Is16BitReally12BitImage(uint src);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_CorrelateImageRegions")]
        internal static extern bool CorrelateImageRegions(uint src1, FIARECT rect1, uint src2, FIARECT rect2, out FIAPOINT pt, out double max);
    
        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_CorrelateImagesAlongRightEdge")]
        internal static extern bool CorrelateImagesAlongRightEdge(uint src1, uint src2, uint thickness,  out FIAPOINT pt, out double max);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_CorrelateImagesAlongBottomEdge")]
        internal static extern bool CorrelateImagesAlongBottomEdge(uint src1, uint src2, uint thickness, out FIAPOINT pt, out double max);
    }
}
