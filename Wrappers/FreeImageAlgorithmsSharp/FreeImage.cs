using System;
using System.Drawing;
using System.Runtime.InteropServices;

namespace FreeImage
{
    public enum Filter
    {
        Box         = 0,
        BICubic     = 1,
        BILinear    = 2,
        BSpline     = 3,
        CatmullRom  = 4,
        Lanczo      = 5
    }
    
    public enum FreeImageType
    {
        Unknown = 0,
        Bitmap  = 1,
        Uint16  = 2,
        Int16   = 3,
        Uint32  = 4,
        Int32   = 5,
        Float   = 6,
        Double  = 7,
        Complex = 8
    }

    public enum ColourChannel
    {
        Rgb     = 0,
        Red     = 1,
        Green   = 2,
        Blue    = 3,
        Alpha   = 4,
        Black   = 5
    }

    public enum Quantize
    {
        WUQuant = 0,
        NNQunat = 1	
    }


    public class FreeImageException : ApplicationException
    {
        public FreeImageException(string message)
            : base(message)
        {
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct FIARECT
    {
        public int left;
        public int top;
        public int right;
        public int bottom;

        public FIARECT(int left, int top, int right, int bottom)
        {
            this.left = left;
            this.top = top;
            this.right = right;
            this.bottom = bottom;
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct FIAPOINT
    {
        public int x;
        public int y;

        public FIAPOINT(int x, int y)
        {
            this.x = x;
            this.y = y;
        }    
    }

    public class FreeImageBitmap : IDisposable
    {
        private uint dib;
        private bool disposed;

        // We need this to keep a ref so the are not garbage collected.
        // As they are passed to unmanaged code.
        private static FreeImage_OutputMessageFunction errorCallback;
 
        static FreeImageBitmap()
        {
            FreeImageBitmap.errorCallback = FreeImageBitmap.FreeImageErrorOccurred;   
            FreeImageNativeMethods.SetOutputMessage(FreeImageBitmap.errorCallback);
        }

        public FreeImageBitmap() {}

        public FreeImageBitmap(FreeImageType imageType,
                int width, int height, int bitsPerPixel,
                uint red_mask, uint green_mask, uint blue_mask)
        {          
            uint dib = FreeImageNativeMethods.AllocateT(
                imageType, width, height, bitsPerPixel,
                red_mask, green_mask, blue_mask);

            this.LoadFromDib(dib);

            this.SetGreyLevelPalette();
        }

        public FreeImageBitmap(int width, int height, int bitsPerPixel)
            : this(FreeImageType.Bitmap, width, height, bitsPerPixel, 0, 0, 0) {}

        public FreeImageBitmap(string fileName)
        {
            this.LoadFromFile(fileName);
        }

        public FreeImageBitmap(FreeImageBitmap image)
            : this(image.Fib) {}

        public FreeImageBitmap(uint dib)
        {
            this.LoadFromDib(dib);
        }

        private void LoadFromDib(uint dib)
        {
            this.dib = dib;
        }

        private void LoadFromFile(string fileName)
        {
            if (fileName == null)
                return;

            uint dib = FreeImageAlgorithmsNativeMethods.LoadFIBFromFile(fileName);

            this.LoadFromDib(dib);
        }

        private static void FreeImageErrorOccurred(FIF format, string msg)
        {
            throw new FreeImageException(msg);
        }

        public void SaveToFile(string filePath)
        {
            if(this.IsGreyScale)
                FreeImageAlgorithmsNativeMethods.SaveFIBToFile(this.dib, filePath, 0);
            else
                FreeImageAlgorithmsNativeMethods.SaveFIBToFile(this.dib, filePath, 1);
        }

        public void LoadFromBitmap(IntPtr hdc, Bitmap bitmap)
        {
            IntPtr hbitmap = bitmap.GetHbitmap();

            uint dib = FreeImageAlgorithmsNativeMethods.HBitmapToFIB(hdc, hbitmap);

            this.LoadFromDib(dib);

            FreeImageAlgorithmsNativeMethods.FreeHBitmap(hbitmap);
        }

        ~FreeImageBitmap()
        {
            // call Dispose with false.  Since we're in the
            // destructor call, the managed resources will be
            // disposed of anyways.
            Dispose(false);	
        }

        protected virtual void Dispose( bool disposing )
        {
            if(!this.disposed)             {
                //if (disposing)
                //{
                    // We are not in the destructor, OK to ref other objects
                          
                //}

                // Dispose of the unmanaged resources    
                FreeImageNativeMethods.Unload(this.dib);

                this.disposed = true;
            }
        }

        public void Dispose()
        {
            Dispose(true);

            // Tell the GC that the Finalize process no longer needs
            // to be run for this object.
            GC.SuppressFinalize(this);
        }

        public bool IsGreyScale
        {
            get
            {
                if(FreeImageAlgorithmsNativeMethods.IsGreyScale(this.dib))
                    return true;

                return false;
            }
        }

        public void ConvertToStandardType()
        {
            uint tmp_dib = FreeImageNativeMethods.ConvertToStandardType(this.dib, 1);

            FreeImageNativeMethods.Unload(this.dib);

            this.dib = tmp_dib;
        }

        public bool ConvertToType(FreeImageType type)
        {
            uint tmp_dib = FreeImageNativeMethods.ConvertToType(this.dib, type, 1);

            if (tmp_dib == 0)
                return false;

            FreeImageNativeMethods.Unload(this.dib);

            this.dib = tmp_dib;

            return true;
        }

        public void ConvertToGreyscale()
        {
            uint tmp_dib = FreeImageNativeMethods.ConvertToGreyscale(this.dib);

            FreeImageNativeMethods.Unload(this.dib);

            this.dib = tmp_dib;
        }

        public void ConvertInt16ToUInt16()
        {
            uint tmp_dib = FreeImageAlgorithmsNativeMethods.ConvertInt16ToUInt16(this.dib);

            FreeImageNativeMethods.Unload(this.dib);

            this.dib = tmp_dib;
        }

        public void LinearScaleToStandardType(double min, double max)
        {
            double minFound, maxFound;

            uint tmp_dib = FreeImageAlgorithmsNativeMethods.LinearScaleToStandardType(this.dib, min, max,
                out minFound, out maxFound);

            FreeImageNativeMethods.Unload(this.dib);

            this.dib = tmp_dib;
        }

        public void ConvertTo8Bits()
        {
            uint tmp_dib = FreeImageNativeMethods.ConvertTo8Bits(this.dib);

            FreeImageNativeMethods.Unload(this.dib);

            this.dib = tmp_dib;
        }

        public void ConvertTo24Bits()
        {
            uint tmp_dib = FreeImageNativeMethods.ConvertTo24Bits(this.dib);

            FreeImageNativeMethods.Unload(this.dib);

            this.dib = tmp_dib;
        }

        public IntPtr ToHBitmap(IntPtr hdc)
        {
            return FreeImageAlgorithmsNativeMethods.GetDibSection(
                this.dib, hdc, 0, 0, this.Width, this.Height);
        }

        public void StretchImageToType(FreeImageType type, double max)
        {
            uint tmp_dib = FreeImageAlgorithmsNativeMethods.StretchImageToType(this.dib, type, max);

            FreeImageNativeMethods.Unload(this.dib);

            this.dib = tmp_dib;
        }

        public void Threshold(uint value)
        {
            uint tmp_dib = FreeImageNativeMethods.Threshold(this.dib, value);

            FreeImageNativeMethods.Unload(this.dib);

            this.dib = tmp_dib;
        }

        public IntPtr ToHBitmap()
        {
            return FreeImageAlgorithmsNativeMethods.ToHBitmap(this.dib);
        }

        public Bitmap ToBitmap()
        {
            IntPtr hbitmap = this.ToHBitmap();

            try
            {
                return System.Drawing.Image.FromHbitmap(hbitmap);
            }
            finally
            {
                FreeImageAlgorithmsNativeMethods.DeleteObject(hbitmap);
            }
        }


        public Bitmap ToBitmap(int width, int height)
        {
            IntPtr hbitmap = IntPtr.Zero;
            Bitmap bitmap = null, thumbnail;
            Graphics graphics = null;

            try
            {
                hbitmap = this.ToHBitmap();
                bitmap = System.Drawing.Image.FromHbitmap(hbitmap);
                thumbnail = new Bitmap(width, height);
                graphics = Graphics.FromImage(thumbnail);

                graphics.DrawImage(bitmap,
                    new Rectangle(0, 0, width, height),
                    new Rectangle(0, 0, bitmap.Width, bitmap.Height), GraphicsUnit.Pixel);

                return thumbnail;
            }
            finally
            {
                if (hbitmap != IntPtr.Zero)
                    FreeImageAlgorithmsNativeMethods.DeleteObject(hbitmap);

                if (bitmap != null)
                    bitmap.Dispose();

                if (graphics != null)
                    graphics.Dispose();
            }
        }

        public void FindMinMaxIntensity(out double min, out double max)
        { 
            FreeImageAlgorithmsNativeMethods.FindMinMax(this.dib, out min, out max);
        }

        public int MemorySizeInBytes
        {
            get
            {
                return (this.BitsPerPixel * this.Pitch * this.Height) / 8;
            }
        }

        public void SetGreyLevelPalette()
        {
            FreeImageAlgorithmsNativeMethods.SetGreyLevelPalette(this.dib);
        }

        public void FlipVertical()
        {
            FreeImageNativeMethods.FlipVertical(this.dib);
        }

        public void FlipHorizontal()
        {
            FreeImageNativeMethods.FlipHorizontal(this.dib);
        }

        public void Rescale(int width, int height, Filter filter)
        {
            uint tmp_dib = FreeImageNativeMethods.Rescale(this.dib, width, height, filter);

            FreeImageNativeMethods.Unload(this.dib);

            this.dib = tmp_dib;
        }

        public FreeImageBitmap RescaleToFreeImageBitmap(int width, int height, Filter filter)
        {
            uint dib = FreeImageNativeMethods.Rescale(this.dib, width, height, filter);

            return new FreeImageBitmap(dib);
        }

        public int Width
        {
            get
            {
                return FreeImageNativeMethods.GetWidth(dib);
            }
        }

        public int Height
        {
            get
            {
                return FreeImageNativeMethods.GetHeight(dib);
            }
        }

        public int Pitch
        {
            get
            {
                return FreeImageNativeMethods.GetPitch(this.dib);
            }
        }

        public uint Line
        {
            get
            {
                return FreeImageNativeMethods.GetLine(this.dib);
            }
        }

        public Size Size
        {
            get
            {
                return new Size(this.Width, this.Height);
            }
        }

        public int BitsPerPixel
        {
            get
            {
                return FreeImageNativeMethods.GetBPP(this.dib);
            }
        }

        public bool Is12Bit
        {
            get
            {
                if (this.BitsPerPixel != 16)
                    return false;

                return FreeImageAlgorithmsNativeMethods.Is16BitReally12BitImage(this.dib);
            }
        }

        public FreeImageType FreeImageType
        {
            get
            {
                return FreeImageNativeMethods.GetImageType(this.dib);	
            }
        }


        public uint Fib
        {
            get
            {
                return this.dib;
            }
        }

        public int MaximumPossibleIntensityValue
        {
            get
            {
                return (int) Math.Pow(2.0, this.BitsPerPixel);
            }
        }

        public int[] GetHistogram(ColourChannel channel)
        {
            int[] hist = new int[256];

            if(FreeImageNativeMethods.GetHistogram(this.dib, hist, channel))
                return hist;

            return null;
        }

        public bool AdjustGamma(double gamma)
        {
            return FreeImageNativeMethods.AdjustGamma(this.dib, gamma);
        }

        public bool AdjustBrightness(double brightness)
        {
            return FreeImageNativeMethods.AdjustBrightness(this.dib, brightness);
        }

        public bool AdjustContrast(double contrast)
        {
            return FreeImageNativeMethods.AdjustContrast(this.dib, contrast);
        }

        public bool Invert()
        {
            return FreeImageNativeMethods.Invert(this.dib);
        }

        public bool Paste(FreeImageBitmap src, int left, int top, int alpha)
        {
            return FreeImageNativeMethods.Paste(this.dib, src.dib, left, top, alpha);
        }

        public bool Paste(FreeImageBitmap src, Point location, int alpha)
        {
            return this.Paste(src, location.X, location.Y, alpha);
        }

        public bool SimplePaste(FreeImageBitmap src, Point location)
        {
            return FreeImageAlgorithmsNativeMethods.SimplePaste(this.dib, src.dib, location.X, location.Y);
        }

        public void EqualizeHistogram()
        {
            uint tmp_dib = FreeImageAlgorithmsNativeMethods.FreeImage_HistEq(this.dib);

            FreeImageNativeMethods.Unload(this.dib);

            this.dib = tmp_dib;
        }

        public FIAPOINT Correlate(FIARECT rect1, FreeImageBitmap src2, FIARECT rect2, out double max)
        {
            FIAPOINT pt = new FIAPOINT();

            FreeImageAlgorithmsNativeMethods.CorrelateImageRegions(this.dib, rect1,
                src2.dib, rect2, out pt, out max);

            return pt;
        }
    }
}
