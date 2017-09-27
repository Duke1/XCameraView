package xyz.openhh.imagecore;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import java.io.OutputStream;

/**
 * Created by Duke
 */

public class Image {
    private final static int WORKING_COMPRESS_STORAGE = 4096;

    public enum CompressFormat {
        JPEG(0),
        PNG(1),
        WEBP(2);

        CompressFormat(int nativeInt) {
            this.nativeInt = nativeInt;
        }

        final int nativeInt;
    }

    static {
        System.loadLibrary("imageengine");
    }

    public native static void compressToJpeg(byte[] data);

    public native static Bitmap nativeDecodeByteArray(byte[] data, int offset, int length, BitmapFactory.Options opts);

    private static native boolean nativeCompress(Bitmap bitmap, int format, int quality, OutputStream stream, byte[] tempStorage);


    public static boolean compress(Bitmap bitmap, CompressFormat format, int quality, OutputStream stream) {
        if (stream == null) {
            throw new NullPointerException();
        }
        if (quality < 0 || quality > 100) {
            throw new IllegalArgumentException("quality must be 0..100");
        }
        boolean result = nativeCompress(bitmap, format.nativeInt,
                quality, stream, new byte[WORKING_COMPRESS_STORAGE]);
        return result;
    }
}
