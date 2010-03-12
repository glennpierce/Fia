using System;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace FreeImageAPI
{
    public class FreeImageException : ApplicationException
    {
        public FreeImageException(string message)
            : base(message)
        {
        }
    }

    public class FreeImageAlgorithmsMatrix
    {
        private FIA_Matrix matrix;

        public FreeImageAlgorithmsMatrix()
        {
            this.matrix = FreeImage.MatrixNew();
        }

        ~FreeImageAlgorithmsMatrix() 
        {
            FreeImage.MatrixDestroy(this.matrix);
        }

        public FIA_Matrix Data
        {
            get
            {
                return this.matrix;
            }
        }

        public bool Scale(double x, double y, FIA_MatrixOrder order)
        {
            return FreeImage.MatrixScale(this.matrix, x, y, order);
        }

        public bool Scale(double x, double y)
        {
            return this.Scale(x, y, FIA_MatrixOrder.MatrixOrderPrepend);
        }

        public bool Translate(double x, double y, FIA_MatrixOrder order)
        {
            return FreeImage.MatrixTranslate(this.matrix, x, y, order);
        }

        public bool Translate(double x, double y)
        {
            return this.Translate(x, y, FIA_MatrixOrder.MatrixOrderPrepend);
        }

        public bool Rotate(double a, FIA_MatrixOrder order)
        {
            return FreeImage.MatrixRotate(this.matrix, a, order);
        }

        public bool Rotate(double a)
        {
            return this.Rotate(a, FIA_MatrixOrder.MatrixOrderPrepend);
        }

        public bool Invert()
        {
            return FreeImage.MatrixInvert(this.matrix);
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

        public FIARECT(Point location, Size size)
        {
            this.left = location.X;
            this.top = location.Y;
            this.right = this.left + size.Width - 1;
            this.bottom = this.top + size.Height - 1;
        }

        public FIARECT(Rectangle rect)
        {
            // For win32 rectangles is the sum of X and Width property values is Right.
            // For FIA_RECT it is one less hence the -1
            this.left = rect.Left;
            this.top = rect.Top;
            this.right = rect.Right - 1;    
            this.bottom = rect.Bottom - 1;
        }

        public int Left
        {
            get
            {
                return this.left;
            }
        }

        public int Top
        {
            get
            {
                return this.top;
            }
        }

        public int Right
        {
            get
            {
                return this.right;
            }
        }

        public int Bottom
        {
            get
            {
                return this.bottom;
            }
        }

        public Point Location
        {
            get
            {
                return new Point(this.Left, this.Top);
            }
        }

        public int Width
        {
            get
            {
                return this.right - this.left + 1;
            }
        }

        public int Height
        {
            get
            {
                return this.bottom - this.top + 1;
            }
        }

        public Size Size
        {
            get
            {
                return new Size(this.Width, this.Height);
            }
        }

        public static FIARECT Empty
        {
            get
            {
                return new FIARECT(0, 0, 0, 0);
            }
        }

        public Rectangle ToRectangle()
        {
            return new Rectangle(this.Left, this.Top, this.Width - 1, this.Height - 1);
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

    public class FreeImageAlgorithmsBitmap : FreeImageBitmap
    {
        // We need this to keep a ref so the are not garbage collected.
        // As they are passed to unmanaged code.
        private static OutputMessageFunction errorCallback =
            new OutputMessageFunction(FreeImageAlgorithmsBitmap.FreeImageErrorOccurred);

        static FreeImageAlgorithmsBitmap()
        {
            FreeImage.SetOutputMessage(errorCallback);
        }

        public FreeImageAlgorithmsBitmap() : base () {}

        public FreeImageAlgorithmsBitmap(FreeImageAlgorithmsBitmap fib) : base(fib) { }

        public FreeImageAlgorithmsBitmap(string filename) : base(filename) { }

        public FreeImageAlgorithmsBitmap(FIBITMAP dib) : base(dib) { }

        public FreeImageAlgorithmsBitmap(int width, int height, PixelFormat pixelFormat)
            : base(width, height, pixelFormat) {}

        public FreeImageAlgorithmsBitmap(Bitmap bitmap) : base(bitmap) {}

        public FreeImageAlgorithmsBitmap(Stream stream) : base(stream) { }

        public FreeImageAlgorithmsBitmap(Stream stream, FREE_IMAGE_FORMAT format) : base(stream, format) { }

        public FreeImageAlgorithmsBitmap(int width, int height, int colorDepth)
        {
            FIBITMAP dib = FreeImage.Allocate(width, height, colorDepth,
                FreeImage.FI_RGBA_RED_MASK,
                FreeImage.FI_RGBA_GREEN_MASK,
                FreeImage.FI_RGBA_BLUE_MASK);

            if (dib.IsNull)
            {
                throw new Exception("Unable to create bitmap.");
            }

            this.ReplaceDib(dib);

            this.SetGreyLevelPalette();
        }

        public FreeImageAlgorithmsBitmap(int width, int height, FREE_IMAGE_TYPE type, int colorDepth)
        {
            FIBITMAP dib = FreeImage.AllocateT(type, width, height, colorDepth,
                FreeImage.FI_RGBA_RED_MASK,
                FreeImage.FI_RGBA_GREEN_MASK,
                FreeImage.FI_RGBA_BLUE_MASK);

            if (dib.IsNull)
            {
                throw new Exception("Unable to create bitmap.");
            }

            this.ReplaceDib(dib);
        }

        private static void FreeImageErrorOccurred(FREE_IMAGE_FORMAT format, string msg)
        {
            throw new FreeImageException(msg);
        }

        public void SaveToFile(string filePath)
        {
            if(this.IsGreyScale)
                FreeImage.SaveFIBToFile(this.Dib, filePath, FIA_BITDEPTH.BIT8);
            else
                FreeImage.SaveFIBToFile(this.Dib, filePath, FIA_BITDEPTH.BIT24);
        }

        public void SaveToFile(string filePath, FIA_BITDEPTH depth)
        {
            FreeImage.SaveFIBToFile(this.Dib, filePath, depth);
        }

        ~FreeImageAlgorithmsBitmap()
        {
            // call Dispose with false.  Since we're in the
            // destructor call, the managed resources will be
            // disposed of anyways.
            Dispose(false);	
        }

        public FIARECT BoundingRectangle
        {
            get
            {
                return new FIARECT(0, 0, this.Width - 1, this.Height - 1);
            }
        }

        public Rectangle Bounds
        {
            get
            {
                return new Rectangle(0, 0, this.Width, this.Height);
            }
        }

        /*
        protected virtual void Dispose( bool disposing )
        {
            if (this.IsThumbnail)
            {
                Console.WriteLine("Here");
            }

            base.Dispose(disposing);
        }

        public void Dispose()
        {
            Dispose(true);

            // Tell the GC that the Finalize process no longer needs
            // to be run for this object.
            GC.SuppressFinalize(this);
        }
        */

        public bool IsGreyScale
        {
            get
            {
                if(FreeImage.IsGreyScale(this.Dib))
                    return true;

                return false;
            }
        }

        public void ConvertToStandardType(bool scale)
        {
            FIBITMAP tmp_dib = FreeImage.ConvertToStandardType(this.Dib, scale);

            this.ReplaceDib(tmp_dib);
        }

         public void ConvertToType(FREE_IMAGE_TYPE type)
         {
             FIBITMAP tmp_dib = FreeImage.ConvertToType(this.Dib, type, true);

             this.ReplaceDib(tmp_dib);
        }

        public void ConvertToGreyscale()
        {
            FIBITMAP tmp_dib = FreeImage.ConvertToGreyscale(this.Dib);

            this.ReplaceDib(tmp_dib);
        }

        public void VerticalFlip()
        {
            FreeImage.FlipVertical(this.Dib);
        }

        public void ConvertInt16ToUInt16()
        {
            FIBITMAP tmp_dib = FreeImage.ConvertInt16ToUInt16(this.Dib);

            this.ReplaceDib(tmp_dib);
        }
        

        public void LinearScaleToStandardType(double min, double max)
        {
            double minFound, maxFound;

            FIBITMAP tmp_dib = FreeImage.LinearScaleToStandardType(this.Dib, min, max,
                out minFound, out maxFound);

            ReplaceDib(tmp_dib);

            this.SetGreyLevelPalette();
        }

        public void ConvertTo8Bits()
        {
            FIBITMAP tmp_dib = FreeImage.ConvertTo8Bits(this.Dib);

            this.ReplaceDib(tmp_dib);
        }

        public void ConvertTo24Bits()
        {
            FIBITMAP tmp_dib = FreeImage.ConvertTo24Bits(this.Dib);

            this.ReplaceDib(tmp_dib);
        }
        
        public void StretchImageToType(FREE_IMAGE_TYPE type, double max)
        {
            FIBITMAP tmp_dib = FreeImage.StretchImageToType(this.Dib, type, max);

            this.ReplaceDib(tmp_dib);
        }

        public void Threshold(byte value)
        {
            FIBITMAP tmp_dib = FreeImage.Threshold(this.Dib, value);

            this.ReplaceDib(tmp_dib);
        }

        public void FindMinMaxIntensity(out double min, out double max)
        { 
            FreeImage.FindMinMax(this.Dib, out min, out max);
        }

        public int MemorySizeInBytes
        {
            get
            {
                return (this.ColorDepth * this.Pitch * this.Height) / 8;
            }
        }

        public void SetGreyLevelPalette()
        {
            FreeImage.SetGreyLevelPalette(this.Dib);
        }

        public double MaximumPossibleIntensityValue
        {
            get
            {
                double max = 0.0;

                FreeImage.GetMaxPosibleValueForFib(this.Dib, out max);

                return max;
            }
        }

        public bool GetGreyLevelHistogram(int number_of_bins, out ulong[] hist)
        {
            double max = this.MaximumPossibleIntensityValue;
  
            hist = new ulong[number_of_bins];

            return FreeImage.Histogram(this.Dib, 0, max, number_of_bins, hist);
        }

        public int GetGreyScalePixelValuesAsDoublesForLine(Point pt1, Point pt2, out double[] values)
        {
            FIAPOINT fi_pt1 = new FIAPOINT(pt1.X, pt1.Y);
            FIAPOINT fi_pt2 = new FIAPOINT(pt2.X, pt2.Y);

            return FreeImage.GetGreyScalePixelValuesAsDoublesForLine(this.Dib, fi_pt1, fi_pt2, out values);
        }

        public bool StatisticReport(out StatisticReport report)
        {
            return FreeImage.StatisticReport(this.Dib, out report);
        }

        /*
        public bool GetGreyLevelHistogram(int number_of_bins, out ulong[] hist, out double range_per_bin)
        {            
            double max = 0.0;

            FreeImage.GetMaxPosibleValueForFib(this.Dib, out max);

            FreeImageType type = FreeImage.GetImageType(this.Dib);
	
	        bool isFloat = (type == FreeImageType.Float || type == FreeImageType.Double);

            int number_of_bins = (int) Math.Ceiling(max) + 1;

            range_per_bin = 1.0;
	        if(isFloat || number_of_bins > 255)
	        {
		        number_of_bins = 256;
		        range_per_bin = max / 256.0;   
	        }
	        
            double max = this.MaximumPossibleIntensityValue;
            range_per_bin = max / 256.0;

            hist = new ulong[number_of_bins];

            return FreeImage.Histogram(this.Dib, 0, max, number_of_bins, hist);
        }
        */
        
        /*
        public bool AdjustGamma(double gamma)
        {
            return FreeImage.AdjustGamma(this.Dib, gamma);
        }

        public bool AdjustBrightness(double brightness)
        {
            return FreeImage.AdjustBrightness(this.Dib, brightness);
        }

        public bool AdjustContrast(double contrast)
        {
            return FreeImage.AdjustContrast(this.Dib, contrast);
        }

        public bool Invert()
        {
            return FreeImage.Invert(this.Dib);
        }

        public bool Paste(FreeImageBitmap src, int left, int top, int alpha)
        {
            return FreeImage.Paste(this.Dib, src.dib, left, top, alpha);
        }

        public bool Paste(FreeImageBitmap src, Point location, int alpha)
        {
            return this.Paste(src, location.X, location.Y, alpha);
        }
        */

        public new FreeImageAlgorithmsBitmap Copy(int left, int top, int right, int bottom)
        {
            EnsureNotDisposed();
            FreeImageAlgorithmsBitmap result = null;
            FIBITMAP newDib = FreeImage.Copy(this.Dib, left, top, right, bottom);
            if (!newDib.IsNull)
            {
                result = new FreeImageAlgorithmsBitmap(newDib);
            }
            return result;
        }

        public bool Paste(FreeImageBitmap src, Point location)
        {
            return FreeImage.Paste(this.Dib, src.Dib, location.X, location.Y);
        }

        public bool PasteFromTopLeft(FreeImageBitmap src, Point location)
        {
            return FreeImage.PasteFromTopLeft(this.Dib, src.Dib, location.X, location.Y);
        }

        public bool PasteFromTopLeft(FreeImageBitmap src, int left, int top)
        {
            return FreeImage.PasteFromTopLeft(this.Dib, src.Dib, left, top);
        }
   
        public bool GradientBlendPasteFromTopLeft(FreeImageAlgorithmsBitmap src, Point pt)
        {
            return FreeImage.GradientBlendPasteFromTopLeft(this.Dib, src.Dib, pt.X, pt.Y);
        }

        public bool GradientBlendPasteFromTopLeft(FreeImageAlgorithmsBitmap src, int left, int top)
        {
            return FreeImage.GradientBlendPasteFromTopLeft(this.Dib, src.Dib, left, top);
        }

        public void PasteFromTopLeft(FreeImageAlgorithmsBitmap src, int left, int top, bool blending)
        {
            bool ret;

            if(blending) {
                ret = FreeImage.GradientBlendPasteFromTopLeft(this.Dib, src.Dib, left, top);
            }
            else {
                ret = FreeImage.PasteFromTopLeft(this.Dib, src.Dib, left, top);
            }

            if (ret == false)
            {
                string errorStr = String.Format(
                        "Can not paste freeimage. Dst image bpp {0}, Src image bpp {1}",
                        this.ColorDepth, src.ColorDepth);

                    throw new FormatException(errorStr);
            }
        }

        public void PasteFromTopLeft(FreeImageAlgorithmsBitmap src, Point location, bool blending)
        {
            this.PasteFromTopLeft(src, location.X, location.Y, blending);
        }

        public bool Clear()
        {
            FIARECT fiaRect = new FIARECT(0, 0, this.Width - 1, this.Height - 1);

            if(this.IsGreyScale)
                return FreeImage.DrawSolidRectangle(this.Dib, fiaRect, 0.0f);
            else
                return FreeImage.DrawColourSolidRect(this.Dib, fiaRect, new RGBQUAD(Color.Black));
        }

        public bool DrawColourSolidRect(FIARECT rect, RGBQUAD colour)
        {
            return FreeImage.DrawColourSolidRect(this.Dib, rect, colour);
        }

        public bool DrawColourSolidRect(Rectangle rect, RGBQUAD colour)
        {
            FIARECT fiaRect = new FIARECT(rect);

            return FreeImage.DrawColourSolidRect(this.Dib, fiaRect, colour);
        }
  
        public bool DrawColourRect(FIARECT rect, RGBQUAD colour, int lineWidth)
        {
            return FreeImage.DrawColourRect(this.Dib, rect, colour, lineWidth);
        }

        public bool DrawColourRect(Rectangle rect, RGBQUAD colour, int lineWidth)
        {
            FIARECT fiaRect = new FIARECT(rect);

            return FreeImage.DrawColourRect(this.Dib, fiaRect, colour, lineWidth);
        }

        public bool DrawSolidRectangle(FIARECT rect, double val)
        {
            return FreeImage.DrawSolidRectangle(this.Dib, rect, val);
        }

        public bool DrawSolidRectangle(Rectangle rect, double val)
        {
            FIARECT fiaRect = new FIARECT(rect.Left, rect.Top, rect.Right, rect.Bottom);

            return FreeImage.DrawSolidRectangle(this.Dib, fiaRect, val);
        }

        public bool DrawSolidRectangle(Point location, Size size, double val)
        {
            FIARECT fiaRect = new FIARECT(location, size);

            return FreeImage.DrawSolidRectangle(this.Dib, fiaRect, val);
        }

        public bool DrawSolidRectangle(Point location, uint width, uint height, double val)
        {
            Size size = new Size((int) width, (int) height);
            FIARECT fiaRect = new FIARECT(location, size);

            return FreeImage.DrawSolidRectangle(this.Dib, fiaRect, val);
        }

        public FreeImageAlgorithmsBitmap AffineTransform(int image_dst_width, int image_dst_height,
                                                         FreeImageAlgorithmsMatrix matrix, RGBQUAD colour, int retainBackground)
        {
            FIBITMAP tmp_dib = FreeImage.AffineTransform(this.Dib, image_dst_width, image_dst_height, matrix.Data, colour, retainBackground);

            return new FreeImageAlgorithmsBitmap(tmp_dib);
        }

        public void AffineTransform(FreeImageAlgorithmsMatrix matrix, RGBQUAD colour, int retainBackground)
        {
            FIBITMAP tmp_dib = FreeImage.AffineTransform(this.Dib, (int)this.Width, (int)this.Height, matrix.Data, colour, retainBackground);

            this.ReplaceDib(tmp_dib);
        }

        public void AffineTransform(FreeImageAlgorithmsMatrix matrix, RGBQUAD colour)
        {
            FIBITMAP tmp_dib = FreeImage.AffineTransform(this.Dib, (int) this.Width, (int) this.Height, matrix.Data, colour, 1);

            this.ReplaceDib(tmp_dib);
        }

        public void DrawImage(FreeImageAlgorithmsBitmap dst, FreeImageAlgorithmsMatrix matrix, 
            int dstLeft, int dstTop, int dstWidth, int dstHeight,
            int srcLeft, int srcTop, int srcWidth, int srcHeight,
            RGBQUAD colour)
        {
            FreeImage.DrawImageFromSrcToDst(dst.Dib, this.Dib, matrix.Data,
                dstLeft, dstTop, dstWidth, dstHeight,
                srcLeft, srcTop, srcWidth, srcHeight,
                colour, 1);
        }

        public void DrawImage(FreeImageAlgorithmsBitmap dst, FreeImageAlgorithmsMatrix matrix,
            int dstLeft, int dstTop, int dstWidth, int dstHeight, RGBQUAD colour)
        {
            FreeImage.DrawImageToDst(dst.Dib, this.Dib, matrix.Data,
                dstLeft, dstTop, dstWidth, dstHeight, colour, 1);
        }

        public void DrawImage(FreeImageAlgorithmsBitmap dst, FreeImageAlgorithmsMatrix matrix, Rectangle dstRect, RGBQUAD colour)
        {
            FreeImage.DrawImageToDst(dst.Dib, this.Dib, matrix.Data,
                dstRect.Left, dstRect.Top, dstRect.Width, dstRect.Height, colour, 1);
        }

        public void DrawImage(FreeImageAlgorithmsBitmap dst, Rectangle dstRect, RGBQUAD colour)
        {
            FreeImage.DrawImageToDst(dst.Dib, this.Dib, FIA_Matrix.Zero,
                dstRect.Left, dstRect.Top, dstRect.Width, dstRect.Height, colour, 1);
        }

        public void DrawImage(FreeImageAlgorithmsBitmap dst, Point dstPoint, Size dstSize, RGBQUAD colour)
        {
            FreeImage.DrawImageToDst(dst.Dib, this.Dib, FIA_Matrix.Zero,
                dstPoint.X, dstPoint.Y, dstSize.Width, dstSize.Height, colour, 1);
        }

        public bool KernelCorrelateImageRegions(FreeImageAlgorithmsBitmap src2, FIARECT rect1, FIARECT rect2,
            FIARECT search_area, FIBITMAP mask,
            CorrelationPrefilter prefilter, out FIAPOINT pt, out double max)
        {
            return FreeImage.KernelCorrelateImageRegions(this.Dib, rect1, src2.Dib, rect2, search_area, mask, prefilter, out pt, out max);
        }

        public bool KernelCorrelateImageRegions(FreeImageAlgorithmsBitmap src2, Rectangle rect1, Rectangle rect2,
            Rectangle search_area, FIBITMAP mask,
            CorrelationPrefilter prefilter, out Point pt, out double max)
        {
            FIARECT fiaRect1 = new FIARECT(rect1);
            FIARECT fiaRect2 = new FIARECT(rect2);
            FIARECT searchRect = new FIARECT(search_area);
            FIAPOINT fiaPoint = new FIAPOINT();

            bool ret = FreeImage.KernelCorrelateImageRegions(this.Dib, fiaRect1, src2.Dib, fiaRect2, searchRect, mask, prefilter, out fiaPoint, out max);

            pt = new Point(fiaPoint.x, fiaPoint.y);

            return ret;
        }


        public bool KernelCorrelateImageRegions(FreeImageAlgorithmsBitmap src2, FIARECT rect1, FIARECT rect2,
            FIARECT search_area, CorrelationPrefilter prefilter, out FIAPOINT pt, out double max)
        {
            return FreeImage.KernelCorrelateImageRegions(this.Dib, rect1, src2.Dib, rect2, search_area, FIBITMAP.Zero, prefilter, out pt, out max);
        }

        public bool KernelCorrelateImageRegions(FreeImageAlgorithmsBitmap src2, FIARECT rect1, FIARECT rect2,
            FIBITMAP mask,
            CorrelationPrefilter prefilter, out FIAPOINT pt, out double max)
        {
            return FreeImage.KernelCorrelateImageRegions(this.Dib, rect1, src2.Dib, rect2, FIARECT.Empty, mask, prefilter, out pt, out max);
        }

        public bool KernelCorrelateImageRegions(FreeImageAlgorithmsBitmap src2, FIARECT rect1, FIARECT rect2,
            CorrelationPrefilter prefilter, out FIAPOINT pt, out double max)
        {
            return FreeImage.KernelCorrelateImageRegions(this.Dib, rect1, src2.Dib, rect2, FIARECT.Empty, FIBITMAP.Zero, prefilter, out pt, out max);
        }

        public bool KernelCorrelateImageRegions(FreeImageAlgorithmsBitmap src2, FIARECT rect1, FIARECT rect2,
            out FIAPOINT pt, out double max)
        {
            return FreeImage.KernelCorrelateImageRegions(this.Dib, rect1, src2.Dib, rect2, FIARECT.Empty, FIBITMAP.Zero, null, out pt, out max);
        } 

        public bool KernelCorrelateImageRegions(FreeImageAlgorithmsBitmap src2, Rectangle rect1, Rectangle rect2,
            FIARECT searchRect, FIBITMAP mask,
            CorrelationPrefilter prefilter, out Point pt, out double max)
        {
            FIARECT fiaRect1 = new FIARECT(rect1);
            FIARECT fiaRect2 = new FIARECT(rect2);
            FIAPOINT fiaPoint = new FIAPOINT();

            bool ret = FreeImage.KernelCorrelateImageRegions(this.Dib, fiaRect1, src2.Dib, fiaRect2, searchRect, mask, prefilter, out fiaPoint, out max);

            pt = new Point(fiaPoint.x, fiaPoint.y);

            return ret;
        }

        public bool KernelCorrelateImageRegions(FreeImageAlgorithmsBitmap src2, Rectangle rect1, Rectangle rect2,
            FIARECT searchRect, out Point pt, out double max)
        {
            return this.KernelCorrelateImageRegions(src2, rect1, rect2, searchRect, FIBITMAP.Zero, null, out pt, out max);
        }

        public bool KernelCorrelateImageRegions(FreeImageAlgorithmsBitmap src2, Rectangle rect1, Rectangle rect2,
           Rectangle search_area,
           CorrelationPrefilter prefilter, out Point pt, out double max)
        {
            return this.KernelCorrelateImageRegions(src2, rect1, rect2, search_area, FIBITMAP.Zero, prefilter, out pt, out max);
        }

        public bool KernelCorrelateImageRegions(FreeImageAlgorithmsBitmap src2, Rectangle rect1, Rectangle rect2,
           Rectangle search_area, out Point pt, out double max)
        {
            return this.KernelCorrelateImageRegions(src2, rect1, rect2, search_area, FIBITMAP.Zero, null, out pt, out max);
        }

        public bool KernelCorrelateImageRegions(FreeImageAlgorithmsBitmap src2, Rectangle rect1, Rectangle rect2,
            FIBITMAP mask,
            CorrelationPrefilter prefilter, out Point pt, out double max)
        {
            return this.KernelCorrelateImageRegions(src2, rect1, rect2, FIARECT.Empty, mask, prefilter, out pt, out max);
        }
     
        public bool KernelCorrelateImageRegions(FreeImageAlgorithmsBitmap src2, Rectangle rect1, Rectangle rect2,
            CorrelationPrefilter prefilter, out Point pt, out double max)
        {
            return this.KernelCorrelateImageRegions(src2, rect1, rect2, FIARECT.Empty, FIBITMAP.Zero, prefilter, out pt, out max);
        }

        public bool KernelCorrelateImageRegions(FreeImageAlgorithmsBitmap src2, Rectangle rect1, Rectangle rect2,
            out Point pt, out double max)
        {
            return this.KernelCorrelateImageRegions(src2, rect1, rect2,
                FIARECT.Empty, FIBITMAP.Zero, null, out pt, out max);
        }

        public static FIBITMAP EdgeDetect(FIBITMAP src)
        {
            return FreeImage.EdgeDetect(src);
        }

        public static CorrelationPrefilter EdgeDetectPrefilter
        {
            get
            {
                return new CorrelationPrefilter(FreeImageAlgorithmsBitmap.EdgeDetect);
            }
        }
    }
}
