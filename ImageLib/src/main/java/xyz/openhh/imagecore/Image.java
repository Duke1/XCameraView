package xyz.openhh.imagecore;

import android.graphics.Bitmap;
import android.view.Surface;

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
        System.loadLibrary("image_kit");
    }

    private static native boolean nativeSave(int format, byte[] data, int offset, int length, int quality, String savePath, long matrixPtr, Surface surface);

    private static native boolean nativeCompress(Bitmap bitmap, int format, int quality, byte[] tempStorage, String savePath);

    public static boolean init() {
        return true;
    }

    public static boolean compress(Bitmap bitmap, CompressFormat format, int quality, String savePath) {
        if (null == bitmap || bitmap.isRecycled()) return false;

        if (quality < 0 || quality > 100) {
            throw new IllegalArgumentException("quality must be 0..100");
        }
        return nativeCompress(bitmap, format.nativeInt, quality, new byte[WORKING_COMPRESS_STORAGE], savePath);
    }

    /**
     * 保存到文件
     *
     * @param data     jpeg数据，不是color数据
     * @param format
     * @param quality
     * @param savePath
     * @return
     */
    public static boolean saveToFile(byte[] data, CompressFormat format, int quality, String savePath, ImageMatrix matrix, Surface surface) {
        if (null == data) return false;
        if (quality < 0 || quality > 100) {
            throw new IllegalArgumentException("quality must be 0..100");
        }
        return nativeSave(format.nativeInt, data, 0, data.length, quality, savePath, null == matrix ? 0 : matrix.native_instance, surface);
    }

    public static boolean saveToFile(byte[] data, CompressFormat format, int quality, String savePath, Surface surface) {
        return saveToFile(data, format, quality, savePath, null);
    }
}
