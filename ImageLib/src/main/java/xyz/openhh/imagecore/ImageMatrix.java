package xyz.openhh.imagecore;

/**
 * Created by Duke
 */

public class ImageMatrix {
    long native_instance;

    private static native long nCreate(long native_src_or_zero);

    private static native void nPostRotate(long nObject, float degrees);

    private static native void nPostScale(long nObject, float sx, float sy);

    public ImageMatrix() {
        native_instance = nCreate(0);
    }

    public boolean postRotate(float degrees) {
        nPostRotate(native_instance, degrees);
        return true;
    }

    public boolean postScale(float sx, float sy) {
        nPostScale(native_instance, sx, sy);
        return true;
    }
}
