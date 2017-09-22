package xyz.openhh.xcameraview;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements CameraSurfaceView.OptListener {

    // Used to load the 'native-lib' library on application startup.
//    static {
//        System.loadLibrary("native-lib");
//    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
//        TextView tv = (TextView) findViewById(R.id.sample_text);
//        tv.setText(stringFromJNI());


        PermissionsManager.requestAllNeedPermissions(this);

        CameraSurfaceView cameraSurfaceView = (CameraSurfaceView) findViewById(R.id.camera_view);

        cameraSurfaceView.setOptListener(this);
    }

    @Override
    public void onTakePicture(String filePath) {
        PicPreAct.launch(this, filePath);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
//    public native String stringFromJNI();
}
