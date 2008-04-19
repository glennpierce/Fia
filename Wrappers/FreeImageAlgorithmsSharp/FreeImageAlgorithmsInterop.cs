using System;
using System.Drawing;
using System.Runtime.InteropServices;

namespace FreeImage
{
    internal sealed class FreeImageAlgorithmsNativeMethods
    {
        private FreeImageAlgorithmsNativeMethods() {}

        enum FREEIMAGE_ALGORITHMS_SAVE_BITDEPTH {BIT8, BIT24};

        internal delegate void FreeImageAlgorithms_OutputMessageFunction(string msg);

        [DllImport("gdi32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool DeleteObject(IntPtr hObject);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FreeImageAlgorithms_IsGreyScale")]	
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool IsGreyScale(uint dib);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FreeImageAlgorithms_SetOutputMessage")]
        internal static extern void SetOutputMessage(FreeImageAlgorithms_OutputMessageFunction omf);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FreeImageAlgorithms_LoadFIBFromFile")]
        internal static extern uint LoadFIBFromFile(string filepath);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FreeImageAlgorithms_SaveFIBToFile")]
        internal static extern int SaveFIBToFile(uint dib,  string filepath, int bitDepth);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FreeImageAlgorithms_HistEq")]
        internal static extern uint FreeImage_HistEq(uint dib);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FreeImageAlgorithms_GetDibSection")]
        internal static extern IntPtr GetDibSection(uint dib, IntPtr hdc, int left, int top, int right, int bottom);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FreeImageAlgorithms_FibToHBitmap")]
        internal static extern IntPtr ToHBitmap(uint dib);  

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FreeImageAlgorithms_HBitmapToFIB")]
        internal static extern uint HBitmapToFIB(IntPtr hdc, IntPtr hbitmap);  

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FreeImageAlgorithms_FreeHBitmap")]
        internal static extern void FreeHBitmap(IntPtr hbitmap);  

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FreeImageAlgorithms_SetGreyLevelPalette")]
        internal static extern int SetGreyLevelPalette(uint dib);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FreeImageAlgorithms_FindMinMax")]
        internal static extern void FindMinMax(uint dib, out double min, out double max);
        
        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FreeImageAlgorithms_ConvertInt16ToUInt16")]
        internal static extern uint ConvertInt16ToUInt16(uint dib);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FreeImageAlgorithms_LinearScaleToStandardType")]
        internal static extern uint LinearScaleToStandardType(uint dib, double min, double max, out double min_within_image, out double max_within_image);
    
        [DllImport("FreeImageAlgorithms.dll", EntryPoint="FreeImageAlgorithms_SimplePaste")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool SimplePaste(uint dst, uint src, int left, int top);

        [DllImport("FreeImageAlgorithms.dll", EntryPoint = "FreeImageAlgorithms_StretchImageToType")]
        internal static extern uint StretchImageToType(uint src, FreeImageType type, double max);
    }
}
