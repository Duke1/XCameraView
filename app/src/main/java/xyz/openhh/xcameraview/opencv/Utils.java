package xyz.openhh.xcameraview.opencv;

import android.graphics.Bitmap;


public class Utils {


    public static void bitmapToMat(Bitmap bmp, Mat mat, boolean unPremultiplyAlpha) {
        if (bmp == null)
            throw new java.lang.IllegalArgumentException("bmp == null");
        if (mat == null)
            throw new java.lang.IllegalArgumentException("mat == null");
        nBitmapToMat2(bmp, mat.nativeObj, unPremultiplyAlpha);
    }

    public static void bitmapToMat(Bitmap bmp, Mat mat) {
        bitmapToMat(bmp, mat, false);
    }

    public static void matToBitmap(Mat mat, Bitmap bmp, boolean premultiplyAlpha) {
        if (mat == null)
            throw new java.lang.IllegalArgumentException("mat == null");
        if (bmp == null)
            throw new java.lang.IllegalArgumentException("bmp == null");
        nMatToBitmap2(mat.nativeObj, bmp, premultiplyAlpha);
    }

    public static void matToBitmap(Mat mat, Bitmap bmp) {
        matToBitmap(mat, bmp, false);
    }

    public static void cvtColor(Mat src, Mat dst, int code, int dstCn) {
        if (src == null)
            throw new java.lang.IllegalArgumentException("src mat == null");
        if (dst == null)
            throw new java.lang.IllegalArgumentException("dst mat == null");
        cvtColor(src.nativeObj, dst.nativeObj, code, dstCn);
    }

    private static native void nBitmapToMat2(Bitmap b, long m_addr, boolean unPremultiplyAlpha);

    private static native void nMatToBitmap2(long m_addr, Bitmap b, boolean premultiplyAlpha);

    private static native void cvtColor(long src_addr, long dst_addr, int code, int dstCn);
}
