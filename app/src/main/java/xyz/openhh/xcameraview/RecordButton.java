package xyz.openhh.xcameraview;

import android.animation.TypeEvaluator;
import android.animation.ValueAnimator;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.util.Log;
import android.util.TypedValue;
import android.view.View;


public class RecordButton extends View implements View.OnClickListener {


    private static final float C = 0.551915024494f;


    private Paint mPaint;
    private int mCenterX, mCenterY;

    private float mCircleRadius, mCircleRadius1;
    private float mDifference,//内层图
            mDifference1;        // 圆形的控制点与数据点的差值
    private float dc;

    private float[] mData = new float[8];
    private float[] mCtrl = new float[16];

    //外层圆
    private float[] mData1 = new float[8];
    private float[] mCtrl1 = new float[16];


    private int width, height;

    private float padding;

    public RecordButton(Context context) {
        this(context, null);
    }

    public RecordButton(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public RecordButton(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);


        mPaint = new Paint();
        mPaint.setColor(Color.BLACK);
        mPaint.setStrokeWidth(8);
        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setTextSize(60);
        mPaint.setAntiAlias(true);

        padding = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 2, context.getResources().getDisplayMetrics());


        initPoint();


        setOnClickListener(this);
    }

    private void initCircle() {

        mCircleRadius = width / 6f;
        mDifference = mCircleRadius * C;        // 圆形的控制点与数据点的差值
        dc = mCircleRadius - mDifference;

        mCircleRadius1 = (width - padding) / 2f;
        mDifference1 = mCircleRadius1 * C;
    }

    private void initPoint() {
        // 初始化数据点

        mData[0] = 0;
        mData[1] = mCircleRadius;

        mData[2] = mCircleRadius;
        mData[3] = 0;

        mData[4] = 0;
        mData[5] = -mCircleRadius;

        mData[6] = -mCircleRadius;
        mData[7] = 0;

        // 初始化控制点

        mCtrl[0] = mData[0] + mDifference;
        mCtrl[1] = mData[1];

        mCtrl[2] = mData[2];
        mCtrl[3] = mData[3] + mDifference;

        mCtrl[4] = mData[2];
        mCtrl[5] = mData[3] - mDifference;

        mCtrl[6] = mData[4] + mDifference;
        mCtrl[7] = mData[5];

        mCtrl[8] = mData[4] - mDifference;
        mCtrl[9] = mData[5];

        mCtrl[10] = mData[6];
        mCtrl[11] = mData[7] - mDifference;

        mCtrl[12] = mData[6];
        mCtrl[13] = mData[7] + mDifference;

        mCtrl[14] = mData[0] - mDifference;
        mCtrl[15] = mData[1];


        //外层圆
        mData1[0] = 0;
        mData1[1] = mCircleRadius1;

        mData1[2] = mCircleRadius1;
        mData1[3] = 0;

        mData1[4] = 0;
        mData1[5] = -mCircleRadius1;

        mData1[6] = -mCircleRadius1;
        mData1[7] = 0;


        mCtrl1[0] = mData1[0] + mDifference1;
        mCtrl1[1] = mData1[1];

        mCtrl1[2] = mData1[2];
        mCtrl1[3] = mData1[3] + mDifference1;

        mCtrl1[4] = mData1[2];
        mCtrl1[5] = mData1[3] - mDifference1;

        mCtrl1[6] = mData1[4] + mDifference1;
        mCtrl1[7] = mData1[5];

        mCtrl1[8] = mData1[4] - mDifference1;
        mCtrl1[9] = mData1[5];

        mCtrl1[10] = mData1[6];
        mCtrl1[11] = mData1[7] - mDifference1;

        mCtrl1[12] = mData1[6];
        mCtrl1[13] = mData1[7] + mDifference1;

        mCtrl1[14] = mData1[0] - mDifference1;
        mCtrl1[15] = mData1[1];
    }


    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        this.width = w;
        this.height = h;

        initCircle();
        initPoint();

        mCenterX = w / 2;
        mCenterY = h / 2;

        invalidate();
    }


    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        drawCoordinateSystem(canvas);       // 绘制坐标系

        canvas.translate(mCenterX, mCenterY); // 将坐标系移动到画布中央
        canvas.scale(1, -1);                 // 翻转Y轴

        drawAuxiliaryLine(canvas);


        //外层图
        mPaint.setColor(Color.RED);
        mPaint.setStrokeWidth(8);
        mPaint.setStyle(Paint.Style.FILL);

        Path path = new Path();
        path.moveTo(mData1[0], mData1[1]);

        path.cubicTo(mCtrl1[0], mCtrl1[1], mCtrl1[2], mCtrl1[3], mData1[2], mData1[3]);
        path.cubicTo(mCtrl1[4], mCtrl1[5], mCtrl1[6], mCtrl1[7], mData1[4], mData1[5]);
        path.cubicTo(mCtrl1[8], mCtrl1[9], mCtrl1[10], mCtrl1[11], mData1[6], mData1[7]);
        path.cubicTo(mCtrl1[12], mCtrl1[13], mCtrl1[14], mCtrl1[15], mData1[0], mData1[1]);

        canvas.drawPath(path, mPaint);

        // 绘制贝塞尔曲线
        mPaint.setColor(Color.WHITE);
        mPaint.setStrokeWidth(8);
        mPaint.setStyle(Paint.Style.FILL);

        Path pathFlag = new Path();
        pathFlag.moveTo(mData[0], mData[1]);

        pathFlag.cubicTo(mCtrl[0], mCtrl[1], mCtrl[2], mCtrl[3], mData[2], mData[3]);
        pathFlag.cubicTo(mCtrl[4], mCtrl[5], mCtrl[6], mCtrl[7], mData[4], mData[5]);
        pathFlag.cubicTo(mCtrl[8], mCtrl[9], mCtrl[10], mCtrl[11], mData[6], mData[7]);
        pathFlag.cubicTo(mCtrl[12], mCtrl[13], mCtrl[14], mCtrl[15], mData[0], mData[1]);

        canvas.drawPath(pathFlag, mPaint);

    }

    // 绘制辅助线
    private void drawAuxiliaryLine(Canvas canvas) {
        // 绘制数据点和控制点
        mPaint.setColor(Color.GRAY);
        mPaint.setStrokeWidth(20);

        for (int i = 0; i < 8; i += 2) {
            canvas.drawPoint(mData[i], mData[i + 1], mPaint);
        }

        for (int i = 0; i < 16; i += 2) {
            canvas.drawPoint(mCtrl[i], mCtrl[i + 1], mPaint);
        }


        // 绘制辅助线
        mPaint.setStrokeWidth(4);

        for (int i = 2, j = 2; i < 8; i += 2, j += 4) {
            canvas.drawLine(mData[i], mData[i + 1], mCtrl[j], mCtrl[j + 1], mPaint);
            canvas.drawLine(mData[i], mData[i + 1], mCtrl[j + 2], mCtrl[j + 3], mPaint);
        }
        canvas.drawLine(mData[0], mData[1], mCtrl[0], mCtrl[1], mPaint);
        canvas.drawLine(mData[0], mData[1], mCtrl[14], mCtrl[15], mPaint);
    }

    // 绘制坐标系
    private void drawCoordinateSystem(Canvas canvas) {
        canvas.save();                      // 绘制做坐标系

        canvas.translate(mCenterX, mCenterY); // 将坐标系移动到画布中央
        canvas.scale(1, -1);                 // 翻转Y轴

        Paint fuzhuPaint = new Paint();
        fuzhuPaint.setColor(Color.parseColor("#3304EF72"));
        fuzhuPaint.setStrokeWidth(5);
        fuzhuPaint.setStyle(Paint.Style.STROKE);

        canvas.drawLine(0, -width, 0, width, fuzhuPaint);
        canvas.drawLine(-height, 0, height, 0, fuzhuPaint);

        canvas.restore();
    }

    private volatile boolean mRecording;

    public synchronized void record() {
        mRecording = true;
        animat(false);
    }

    private void animat(boolean reverse) {
        ValueAnimator valueAnimator = ValueAnimator.ofObject(new TypeEvaluator<Float>() {
            @Override
            public Float evaluate(float fraction, Float startValue, Float endValue) {
                Log.e("", "evaluate===" + fraction);
                return fraction * endValue;
            }
        }, 0f, dc);
        valueAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animation) {
                float dcPiece = (float) animation.getAnimatedValue();
                mCtrl[0] = mData[0] + mDifference + dcPiece;

                mCtrl[3] = mData[3] + mDifference + dcPiece;

                mCtrl[5] = mData[3] - mDifference - dcPiece;

                mCtrl[6] = mData[4] + mDifference + dcPiece;

                mCtrl[8] = mData[4] - mDifference - dcPiece;

                mCtrl[11] = mData[7] - mDifference - dcPiece;

                mCtrl[13] = mData[7] + mDifference + dcPiece;

                mCtrl[14] = mData[0] - mDifference - dcPiece;

                postInvalidate();
            }
        });
        valueAnimator.setDuration(100);

        if (reverse) valueAnimator.reverse();
        else valueAnimator.start();
    }

    public synchronized void stop() {
        mRecording = false;
        animat(true);
    }

    @Override
    public void onClick(View v) {
        if (mRecording) stop();
        else record();
    }
}
