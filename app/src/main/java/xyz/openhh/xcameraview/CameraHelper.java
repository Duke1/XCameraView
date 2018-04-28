package xyz.openhh.xcameraview;

import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceHolder;

import java.io.File;
import java.io.IOException;

import xyz.openhh.imagecore.Image;
import xyz.openhh.imagecore.ImageMatrix;

/**
 * Created by Duke
 */

public class CameraHelper {
    private volatile static CameraHelper _instance;

    private CameraHelper() {
    }

    public static CameraHelper getInstance() {
        if (null == _instance) {
            synchronized (CameraHelper.class) {
                if (null == _instance) {
                    _instance = new CameraHelper();
                }
            }
        }
        return _instance;
    }


    private Camera mCamera;
    private ICameraView cameraView;

    public void setCameraView(ICameraView cameraView) {
        this.cameraView = cameraView;
    }

    public void openCamera(SurfaceHolder holder) {
        if (mCamera == null) {
            mCamera = Camera.open();//开启相机
            try {
                mCamera.setPreviewDisplay(holder);//摄像头画面显示在Surface上
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

    }

    public void startPreview() {

        mCamera.startPreview();
    }

    public void setPreviewTexture(SurfaceTexture surfaceTexture) {

        try {
            mCamera.setPreviewTexture(surfaceTexture);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void stopCamera() {
        if (null != mCamera) {
            mCamera.stopPreview();//停止预览
            mCamera.release();//释放相机资源
            mCamera = null;
        }
    }


    public void takePicture() {
        mCamera.takePicture(null, null, JPEG);
    }

    public Camera getCamera() {
        return mCamera;
    }

    //创建jpeg图片回调数据对象
    private Camera.PictureCallback JPEG = new Camera.PictureCallback() {

        @Override
        public void onPictureTaken(byte[] data, Camera Camera) {
            String filePath =
                    new StringBuilder()
                            .append(getImageStoreDirectory())
                            .append(File.separator)
                            .append(System.currentTimeMillis())
                            .append(".jpg").toString();//照片保存路径
            try {
                long exTime = System.currentTimeMillis();

                ImageMatrix matrix = null;

                Camera.CameraInfo info = new Camera.CameraInfo();
                Camera.getCameraInfo(android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK, info);
                matrix = new ImageMatrix();
                matrix.postRotate(info.orientation);
                matrix.postScale(0.5f, 0.5f);

                Image.saveToFile(data, Image.CompressFormat.JPEG, 100, filePath, matrix, cameraView.getSurface());

                exTime = System.currentTimeMillis() - exTime;
                Log.e("", "excute time :  " + (exTime / 1000f));
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                stopCamera();
                if (null != optListener) optListener.onTakePicture(filePath);
            }

        }
    };

    public static final String getImageStoreDirectory() {
        StringBuilder sBuilder = new StringBuilder()
                .append(Environment.getExternalStorageDirectory())
                .append(File.separator).append("ACamera");

        String path = sBuilder.toString();

        File filePath = new File(path);
        if (!filePath.isDirectory()) {
            if (!filePath.mkdir()) {
                filePath = new File(DCIM_PATH);
                if (!filePath.isDirectory())
                    filePath.mkdir();
                return DCIM_PATH;
            }
        }
        return path;
    }

    public static String DCIM_PATH = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM).getAbsolutePath()
            + File.separator + "Camera";


    private OptListener optListener;

    public void setOptListener(OptListener optListener) {
        this.optListener = optListener;
    }


}
