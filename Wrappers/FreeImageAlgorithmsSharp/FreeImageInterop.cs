using System;
using System.Drawing;
using System.Runtime.InteropServices;

namespace FreeImage
{
    [StructLayout(LayoutKind.Sequential)]
    internal struct RGBQUAD
    { 
        public byte rgbBlue;
        public byte rgbGreen;
        public byte rgbRed;
        public byte rgbReserved;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct BITMAPINFOHEADER
    {
        public uint size;
        public int width; 
        public int height; 
        public ushort biPlanes; 
        public ushort biBitCount;
        public uint biCompression; 
        public uint biSizeImage; 
        public int biXPelsPerMeter; 
        public int biYPelsPerMeter; 
        public uint biClrUsed; 
        public uint biClrImportant;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct BITMAPINFO
    {
        public BITMAPINFOHEADER bmiHeader; 
        public RGBQUAD bmiColors;
    }

    internal enum FIF
    {
        FIF_UNKNOWN = -1,
        FIF_BMP     = 0,
        FIF_ICO     = 1,
        FIF_JPEG    = 2,
        FIF_JNG     = 3,
        FIF_KOALA   = 4,
        FIF_LBM     = 5,
        FIF_IFF     = FIF_LBM,
        FIF_MNG     = 6,
        FIF_PBM     = 7,
        FIF_PBMRAW  = 8,
        FIF_PCD     = 9,
        FIF_PCX     = 10,
        FIF_PGM     = 11,
        FIF_PGMRAW  = 12,
        FIF_PNG     = 13,
        FIF_PPM     = 14,
        FIF_PPMRAW  = 15,
        FIF_RAS     = 16,
        FIF_TARGA   = 17,
        FIF_TIFF    = 18,
        FIF_WBMP    = 19,
        FIF_PSD     = 20,
        FIF_CUT     = 21,
        FIF_XBM     = 22,
        FIF_XPM     = 23,
        FIF_DDS     = 24,
        FIF_GIF     = 25
    }
    
    internal enum FI_DITHER
    {
        FID_FS              = 0,
        FID_BAYER4x4        = 1,
        FID_BAYER8x8        = 2,
        FID_CLUSTER6x6      = 3,
        FID_CLUSTER8x8      = 4,
        FID_CLUSTER16x16    = 5
    }
    
    internal delegate void FreeImage_OutputMessageFunction(FIF format, string msg);
    
    public sealed class FreeImageNativeMethods
    {
        private FreeImageNativeMethods() {}
        
        // Init/Error routines ----------------------------------------
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_Initialise")]
        internal static extern void Initialise(
            [MarshalAs(UnmanagedType.Bool)] bool loadLocalPluginsOnly);
        
        // alias for Americans :)
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_Initialise")]
        internal static extern void Initialize(
            [MarshalAs(UnmanagedType.Bool)] bool loadLocalPluginsOnly);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_DeInitialise")]
        internal static extern void DeInitialise();
        
        // alias for Americians :)
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_DeInitialise")]
        internal static extern void DeInitialize();
            
        // Version routines -------------------------------------------
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetVersion")]
        internal static extern string GetVersion();
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetCopyrightMessage")]
        internal static extern string GetCopyrightMessage();
    
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_SetOutputMessage")]
        internal static extern void SetOutputMessage(FreeImage_OutputMessageFunction omf);
            
        // Allocate/Clone/Unload routines -----------------------------
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_Allocate")]
        internal static extern uint Allocate(int width, int height, 
            int bpp, uint red_mask, uint green_mask, uint blue_mask);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_AllocateT")]
        internal static extern uint AllocateT(FreeImageType ftype, int width, 
                int height, int bpp, uint red_mask, uint green_mask, 
                uint blue_mask);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_Clone")]
        internal static extern uint Clone(uint dib);

        [DllImport("FreeImage.dll", EntryPoint="FreeImage_Unload")]
        internal static extern void Unload(uint dib);
    
        // Load/Save routines -----------------------------------------
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_Load")]
        internal static extern uint Load(FIF format, string filename, int flags);

        [DllImport("FreeImage.dll", EntryPoint="FreeImage_Save")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool Save(FIF format, uint dib, string filename, int flags);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetFIFCount")]
        internal static extern int GetFIFCount();
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_SetPluginEnabled")]
        internal static extern int SetPluginEnabled(FIF format,
            [MarshalAs(UnmanagedType.Bool)] bool enabled);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_IsPluginEnabled")]
        internal static extern int IsPluginEnabled(FIF format);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetFIFFromFormat")]
        internal static extern FIF GetFIFFromFormat(string format);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetFIFFromMime")]
        internal static extern FIF GetFIFFromMime(string mime);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetFormatFromFIF")]
        internal static extern string GetFormatFromFIF(FIF format);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetFIFExtensionList")]
        internal static extern string GetFIFExtensionList(FIF format);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetFIFDescription")]
        internal static extern string GetFIFDescription(FIF format);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetFIFRegExpr")]
        internal static extern string GetFIFRegExpr(FIF format);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetFIFFromFilename")]
        internal static extern FIF GetFIFFromFilename(string filename);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_FIFSupportsReading")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool FIFSupportsReading(FIF format);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_FIFSupportsWriting")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool FIFSupportsWriting(FIF format);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_FIFSupportsExportBPP")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool FIFSupportsExportBPP(FIF format, int bpp);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_FIFSupportsExportType")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool FIFSupportsExportType(FIF format, FreeImageType ftype);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_FIFSupportsICCProfiles")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool FIFSupportsICCProfiles(FIF format);

        // Multipage interface ---------------------------------------
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_OpenMultiBitmap")]
        internal static extern uint OpenMultiBitmap(
            FIF format,
            string fileName,
            [MarshalAs(UnmanagedType.Bool)] bool createNew,
            [MarshalAs(UnmanagedType.Bool)] bool readOnly,
            [MarshalAs(UnmanagedType.Bool)] bool keepCacheInMemory,
            int flags);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_CloseMultiBitmap")]
        internal static extern long CloseMultiBitmap(uint bitmap, int flags);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetPageCount")]
        internal static extern int GetPageCount(uint bitmap);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_AppendPage")]
        internal static extern void AppendPage(uint bitmap, uint data);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_InsertPage")]
        internal static extern void InsertPage(uint bitmap, int page, uint data);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_DeletePage")]
        internal static extern void DeletePage(uint bitmap, int page);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_LockPage")]
        internal static extern uint LockPage(uint bitmap, int page);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_UnlockPage")]
        internal static extern void UnlockPage(uint bitmap, int page,
            [MarshalAs(UnmanagedType.Bool)] bool changed);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_MovePage")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool MovePage(uint bitmap, int target, int source);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetLockedPageNumbers")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool GetLockedPageNumbers(uint bitmap, IntPtr pages, IntPtr count);
    
        // File type request routines ---------------------------------
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetFileType")]
        internal static extern FIF GetFileType(string filename, int size);

        // Image type request routines --------------------------------
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetImageType")]
        internal static extern FreeImageType GetImageType(uint dib);
        
        // Info functions ---------------------------------------------
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_IsLittleEndian")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool IsLittleEndian();
        
        // Pixel access functions -------------------------------------
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetBits")]
        internal static extern IntPtr GetBits(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetScanLine")]
        internal static extern byte[] GetScanLine(uint dib, int scanline);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetPixelIndex")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool GetPixelIndex(uint dib, uint x, uint y, out byte value);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetColorsUsed")]
        internal static extern int GetColorsUsed(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetBPP")]
        internal static extern int GetBPP(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetWidth")]
        internal static extern int GetWidth(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetHeight")]
        internal static extern int GetHeight(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetLine")]
        internal static extern uint GetLine(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetPitch")]
        internal static extern int GetPitch(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetDIBSize")]
        internal static extern uint GetDIBSize(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetPalette")]
        internal static extern RGBQUAD[] GetPalette(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetDotsPerMeterX")]
        internal static extern uint GetDotsPerMeterX(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetDotsPerMeterY")]
        internal static extern uint GetDotsPerMeterY(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetInfoHeader")]
        [return: MarshalAs(UnmanagedType.LPStruct)]
        internal static extern BITMAPINFOHEADER GetInfoHeader(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetInfo")]
        [return: MarshalAs(UnmanagedType.LPStruct)]
        internal static extern BITMAPINFO GetInfo(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetColorType")]
        internal static extern int GetColorType(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetRedMask")]
        internal static extern uint GetRedMask(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetGreenMask")]
        internal static extern uint GetGreenMask(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetBlueMask")]
        internal static extern uint GetBlueMask(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetTransparencyCount")]
        internal static extern uint GetTransparencyCount(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetTransparencyTable")]
        internal static extern IntPtr GetTransparencyTable(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_SetTransparent")]
        internal static extern void SetTransparent(uint dib,
            [MarshalAs(UnmanagedType.Bool)] bool enabled);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_IsTransparent")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool IsTransparent(uint dib);

        [DllImport("FreeImage.dll", EntryPoint="FreeImage_ConvertTo8Bits")]
        internal static extern uint ConvertTo8Bits(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_ConvertTo16Bits555")]
        internal static extern uint ConvertTo16Bits555(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_ConvertTo16Bits565")]
        internal static extern uint ConvertTo16Bits565(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_ConvertTo24Bits")]
        internal static extern uint ConvertTo24Bits(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_ConvertTo32Bits")]
        internal static extern uint ConvertTo32Bits(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_ConvertToStandardType")]
        internal static extern uint ConvertToStandardType(uint dib, int scale);

        [DllImport("FreeImage.dll", EntryPoint = "FreeImage_ConvertToGreyscale")]
        internal static extern uint ConvertToGreyscale(uint dib);

        [DllImport("FreeImage.dll", EntryPoint = "FreeImage_ConvertToType")]
        internal static extern uint ConvertToType(uint dib, FreeImageType type, int scale);

        [DllImport("FreeImage.dll", EntryPoint="FreeImage_ColorQuantize")]
        internal static extern uint ColorQuantize(uint dib, Quantize quantize);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_Threshold")]
        internal static extern uint Threshold(uint dib, uint t);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_Dither")]
        internal static extern uint Dither(uint dib, FI_DITHER algorithm);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_ConvertFromRawBits")]
        internal static extern uint ConvertFromRawBits(byte[] bits, int width, int height,
            int pitch, uint bpp, uint redMask, uint greenMask, uint blueMask,
            [MarshalAs(UnmanagedType.Bool)] bool topDown);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_ConvertToRawBits")]
        internal static extern void ConvertToRawBits(IntPtr bits, uint dib, int pitch,
            uint bpp, uint redMask, uint greenMask, uint blueMask,
            [MarshalAs(UnmanagedType.Bool)] bool topDown);

        [DllImport("FreeImage.dll", EntryPoint="FreeImage_RotateClassic")]
        internal static extern uint RotateClassic(uint dib, Double angle);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_RotateEx")]
        internal static extern uint RotateEx(uint dib, Double angle,
            Double xShift, Double yShift, Double xOrigin, Double yOrigin,
            [MarshalAs(UnmanagedType.Bool)] bool useMask);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_FlipHorizontal")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool FlipHorizontal(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_FlipVertical")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool FlipVertical(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_Rescale")]
        internal static extern uint Rescale(uint dib, int dst_width, int dst_height, Filter filter);
            
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_AdjustCurve")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool AdjustCurve(uint dib, byte[] lut, ColourChannel channel);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_AdjustGamma")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool AdjustGamma(uint dib, Double gamma);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_AdjustBrightness")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool AdjustBrightness(uint dib, Double percentage);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_AdjustContrast")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool AdjustContrast(uint dib, Double percentage);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_Invert")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool Invert(uint dib);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetHistogram")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool GetHistogram(uint dib, [In, Out] int[] histo, ColourChannel channel);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_GetChannel")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool GetChannel(uint dib, ColourChannel channel);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_SetChannel")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool SetChannel(uint dib, uint dib8, ColourChannel channel);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_Copy")]
        internal static extern uint Copy(uint dib, int left, int top, int right, int bottom);
        
        [DllImport("FreeImage.dll", EntryPoint="FreeImage_Paste")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool Paste(uint dst, uint src, int left, int top, int alpha);
    }
}

