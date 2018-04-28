package xyz.openhh.xcameraview;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import xyz.openhh.imagecore.Image;

public class MainActivity extends Activity implements OptListener {
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);


        setContentView(R.layout.activity_main);

        Image.init();


        PermissionsManager.requestAllNeedPermissions(this);

        final CameraSurfaceView cameraSurfaceView = (CameraSurfaceView) findViewById(R.id.camera_view);

        CameraHelper.getInstance().setOptListener(this);

        findViewById(R.id.take_pic_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                cameraSurfaceView.takePicture();
            }
        });
    }

    @Override
    public void onTakePicture(String filePath) {
        PicPreAct.launch(this, filePath);
    }

}
