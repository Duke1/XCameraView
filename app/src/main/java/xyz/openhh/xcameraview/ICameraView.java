package xyz.openhh.xcameraview;

import android.view.Surface;

/**
 * Created by Duke
 */

public interface ICameraView {
    static final String TAG = "CameraSurfaceView";

    Surface getSurface();
}
