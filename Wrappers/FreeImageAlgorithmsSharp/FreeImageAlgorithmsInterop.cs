using System;
using System.Drawing;
using System.Runtime.InteropServices;

namespace FreeImageAPI
{
    public enum FIA_BITDEPTH
    {
        BIT8,
        BIT16,
        BIT24,
        BIT32
    }

    public partial class FreeImage
    {
        //enum FREEIMAGE_ALGORITHMS_SAVE_BITDEPTH {BIT8, BIT24};

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_IsGreyScale")]	
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool IsGreyScale(FIBITMAP dib);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_LoadFIBFromFile")]
        internal static extern uint LoadFIBFromFile(string filepath);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_SaveFIBToFile")]
        internal static extern int SaveFIBToFile(FIBITMAP dib, string filepath, FIA_BITDEPTH bitDepth);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_HistEq")]
        internal static extern uint FreeImage_HistEq(FIBITMAP dib);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_GetDibSection")]
        internal static extern IntPtr GetDibSection(FIBITMAP dib, IntPtr hdc, int left, int top, int right, int bottom);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_FibToHBitmap")]
        internal static extern IntPtr ToHBitmap(FIBITMAP dib);  

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_HBitmapToFIB")]
        internal static extern uint HBitmapToFIB(IntPtr hdc, IntPtr hbitmap);  

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_FreeHBitmap")]
        internal static extern void FreeHBitmap(IntPtr hbitmap);  

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_SetGreyLevelPalette")]
        internal static extern int SetGreyLevelPalette(FIBITMAP dib);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_FindMinMax")]
        internal static extern void FindMinMax(FIBITMAP dib, out double min, out double max);
        
        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_ConvertInt16ToUInt16")]
        internal static extern FIBITMAP ConvertInt16ToUInt16(FIBITMAP dib);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_GetMaxPosibleValueForFib")]
        internal static extern void GetMaxPosibleValueForFib(FIBITMAP dib, out double max);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_GetMinPosibleValueForFib")]
        internal static extern void GetMinPosibleValueForFib(FIBITMAP dib, out double min);

        [DllImport("FreeImageAlgorithms_d.dll", EntryPoint="FIA_Histogram")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool Histogram(FIBITMAP dib, double min, double max, int number_of_bins,
            [In, Out] ulong[] values);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_LinearScaleToStandardType")]
        internal static extern FIBITMAP LinearScaleToStandardType(FIBITMAP dib, double min, double max,
            out double min_within_image, out double max_within_image);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_GetGreyScalePixelValuesForLine")]
        internal static extern int GetGreyScalePixelValuesForLine(FIBITMAP dib, FIAPOINT pt1, FIAPOINT pt2,
            out short[] values);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_GetGreyScalePixelValuesAsDoublesForLine")]
        internal static extern int GetGreyScalePixelValuesAsDoublesForLine(FIBITMAP dib,
            FIAPOINT pt1, FIAPOINT pt2, out double[] values);    

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_SimplePaste")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool SimplePaste(FIBITMAP dst, FIBITMAP src, int left, int bottom);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FIA_SimplePasteFromTopLeft")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool SimplePasteFromTopLeft(FIBITMAP dst, FIBITMAP src, int left, int top);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_StretchImageToType")]
        internal static extern FIBITMAP StretchImageToType(FIBITMAP src, FREE_IMAGE_TYPE type, double max);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_Is16BitReally12BitImage")]
        internal static extern bool Is16BitReally12BitImage(FIBITMAP src);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_CorrelateImageRegions")]
        internal static extern bool CorrelateImageRegions(FIBITMAP src1, FIARECT rect1, FIBITMAP src2, FIARECT rect2, out FIAPOINT pt, out double max);
    
        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_CorrelateImagesAlongRightEdge")]
        internal static extern bool CorrelateImagesAlongRightEdge(FIBITMAP src1, FIBITMAP src2, uint thickness, out FIAPOINT pt, out double max);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_CorrelateImagesAlongBottomEdge")]
        internal static extern bool CorrelateImagesAlongBottomEdge(FIBITMAP src1, FIBITMAP src2, uint thickness, out FIAPOINT pt, out double max);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_GetGradientBlendAlphaImage")]
        internal static extern FIBITMAP GetGradientBlendAlphaImage(FIBITMAP src2, FIARECT rect1, FIARECT rect2, out FIARECT intersect_rect);
    
        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_GradientBlendPasteFromTopLeft")]
        internal static extern bool GradientBlendPasteFromTopLeft(FIBITMAP dst, FIBITMAP src, int left, int top, FIBITMAP mask);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_GradientBlendedIntersectionImage")]
        internal static extern FIBITMAP GradientBlendedIntersectionImage(FIBITMAP src1, FIARECT rect1, FIBITMAP src2, FIARECT rect2, FIBITMAP mask, out FIARECT intersect_rect);
   
        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FIA_DrawSolidGreyscaleRect")]
        internal static extern bool DrawSolidRectangle(FIBITMAP src, FIARECT rect, double value);
    }
}
