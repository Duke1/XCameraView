package xyz.openhh.xcameraview;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Build;

import java.util.ArrayList;

/**
 * Created by Duke
 */
public class PermissionsManager {

    public final static String PERMISSION_READ_EXTERNAL_STORAGE = "android.permission.READ_EXTERNAL_STORAGE";
    public final static String PERMISSION_WRITE_EXTERNAL_STORAGE = "android.permission.WRITE_EXTERNAL_STORAGE";
    public final static String PERMISSION_CAMERA = "android.permission.CAMERA";

    public final static String[] ALL_NEED_PERMISSIONS = new String[]{
            PERMISSION_READ_EXTERNAL_STORAGE,
            PERMISSION_CAMERA,
            PERMISSION_WRITE_EXTERNAL_STORAGE
    };


    /**
     * 检查权限
     *
     * @param context
     * @param permission
     * @return true，有权限；false，无权限
     */
    public static boolean checkPermission(Context context, String permission) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            return PackageManager.PERMISSION_GRANTED == context.checkSelfPermission(permission);
        } else return true;
    }


    /**
     * 申请所有需要的权限
     */
    public static void requestAllNeedPermissions(Activity activity) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {

            ArrayList<String> needReReqPermissions = new ArrayList<>(ALL_NEED_PERMISSIONS.length);
            for (String p : ALL_NEED_PERMISSIONS) {
                if (!checkPermission(activity, p))
                    needReReqPermissions.add(p);
            }

            if (needReReqPermissions.size() > 0)
                activity.requestPermissions(needReReqPermissions.toArray(new String[needReReqPermissions.size()]), 0);
        }
    }

    public static void requestNeedPermissions(Activity activity, String permission) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {

            activity.requestPermissions(new String[]{permission}, 0);
        }
    }
}
