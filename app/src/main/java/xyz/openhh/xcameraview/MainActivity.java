package xyz.openhh.xcameraview;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import xyz.openhh.imagecore.Image;

public class MainActivity extends AppCompatActivity implements OptListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Image.init();


        PermissionsManager.requestAllNeedPermissions(this);

        final CameraGLSurfaceView cameraSurfaceView = (CameraGLSurfaceView) findViewById(R.id.camera_view);

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
