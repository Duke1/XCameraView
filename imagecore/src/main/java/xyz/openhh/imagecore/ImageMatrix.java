package xyz.openhh.imagecore;

/**
 * Created by Duke
 */

public class ImageMatrix {
    public long native_instance;

    private static native long nCreate(long native_src_or_zero);
    private static native void nPostRotate(long nObject, float degrees);

    public ImageMatrix() {
        native_instance = nCreate(0);
    }

    public boolean postRotate(float degrees) {
        nPostRotate(native_instance, degrees);
        return true;
    }
}
