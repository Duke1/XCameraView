package xyz.openhh.xcameraview;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.RelativeLayout;

import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;

public class PicPreAct extends AppCompatActivity {

    public static void launch(Activity act, String imgUri) {


        Intent intent = new Intent();
        intent.setClass(act, PicPreAct.class);
        intent.putExtra("img_uri", imgUri);
        act.startActivity(intent);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pic_pre);

        String imgUri = getIntent().getStringExtra("img_uri");

        ImageView imageView = (ImageView) findViewById(R.id.pre_image_view);
        FileInputStream f = null;
        try {
            f = new FileInputStream(imgUri);

            Bitmap bm = null;
            BitmapFactory.Options options = new BitmapFactory.Options();
            options.inSampleSize = 1;
            BufferedInputStream bis = new BufferedInputStream(f);
            bm = BitmapFactory.decodeStream(bis, null, options);
            imageView.setImageBitmap(bm);
            imageView.setLayoutParams(new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

    }
}
