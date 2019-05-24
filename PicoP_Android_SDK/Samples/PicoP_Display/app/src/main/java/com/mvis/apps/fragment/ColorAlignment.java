package com.mvis.apps.fragment;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.NumberPicker;
import android.widget.RadioButton;
import android.widget.TextView;

import com.mvis.apps.R;

import static com.mvis.apps.MessageCenter.intToColor;
import static com.mvis.apps.MessageCenter.intToDirection;
import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.MessageCenter.resultData;
import static com.mvis.apps.Utils.MSG_GET_COLORALIGNMENT;
import static com.mvis.apps.Utils.MSG_SET_COLORALIGNMENT;

public class ColorAlignment extends Fragment {

    public static final int MSG_COLORALIGNMENT_RESPONSE_GET = 90001;
    public static final int MSG_COLORALIGNMENT_RESPONSE_SET = 90002;
    private static final String TAG = "ColorAlignment";
    private static TextView mTvGetColoralignmentCurrent, mTvGetColoralignmentStartup, mTvGetColoralignmentFactory, mTvSetResult;
    public static Handler coloralignmentHandler = new Handler() {
        // when handler.message() called, below code will be triggered.
        public void handleMessage(Message msg) {
            Bundle b;
            int result;
            //super.handleMessage(msg);
            switch (msg.what) {
                case MSG_COLORALIGNMENT_RESPONSE_SET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mTvSetResult.setText("Call success ");
                    } else {
                        String STR = b.getString("STR");
                        mTvSetResult.setText("SetColorAlignment failed, error msg = " + STR);
                    }
                    break;
                case MSG_COLORALIGNMENT_RESPONSE_GET:
                    b = msg.getData();
                    result = b.getInt("result");
                    int type = b.getInt("type");
                    if (result == 0) {
                        if (type == 0) {
                            mTvGetColoralignmentCurrent.setText( "Color offset : " + resultData.getOffset());
                        } else if (type == 1) {
                            mTvGetColoralignmentStartup.setText( "Color offset : " + resultData.getOffset());;
                        } else if (type == 2) {
                            mTvGetColoralignmentFactory.setText( "Color offset : " + resultData.getOffset());
                        }
                    } else {
                        String STR = b.getString("STR");
                        if (type == 0) {
                            mTvGetColoralignmentCurrent.setText("GetColorAlignment failed, error msg = " + STR);
                        } else if (type == 1) {
                            mTvGetColoralignmentStartup.setText("GetColorAlignment failed, error msg = " + STR);
                        } else if (type == 2) {
                            mTvGetColoralignmentFactory.setText("GetColorAlignment failed, error msg = " + STR);
                        }
                    }
                    break;
            }
        }
    };
    final int minValue = -32;
    final int maxValue = 32;
    NumberPicker numberPicker1, numberPicker2;
    private Button mBtnSet, mBtnGetCurrent, mBtnGetStartup, mBtnGetFactory;
    private RadioButton mRed, mBlue, mGreen, mRed2, mGreen2, directionH, directionV;
    private CheckBox mChkCommit;
    private boolean commit = false;
    private int direction = 0, color = 0;
    /* { button code */
    public View.OnClickListener onClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.checkbox_commit_coloralignment: {
                    if (mChkCommit.isChecked())
                        commit = true;
                    else
                        commit = false;
                    break;
                }
                case R.id.fragment_coloralignment_color_red: {
                    if (mRed.isChecked())
                        color = 0;
                    break;
                }
                case R.id.fragment_coloralignment_color_green: {
                    if (mGreen.isChecked())
                        color = 1;
                    break;
                }
                case R.id.fragment_coloralignment_color_blue: {
                    if (mBlue.isChecked())
                        color = 2;
                    break;
                }
                case R.id.fragment_coloralignment_color_red_2: {
                    if (mRed2.isChecked())
                        color = 3;
                    break;
                }
                case R.id.fragment_coloralignment_color_green_2: {
                    if (mGreen2.isChecked())
                        color = 4;
                    break;
                }
                case R.id.fragment_coloralignment_dir_h: {
                    if (mGreen.isChecked())
                        direction = 0;
                    break;
                }
                case R.id.fragment_coloralignment_dir_v: {
                    if (mBlue.isChecked())
                        direction = 1;
                    break;
                }
                case R.id.fragment_coloralignment_btn_set: {
                    Message msg0 = messageHandler.obtainMessage(MSG_SET_COLORALIGNMENT);
                    Bundle b = new Bundle();
                    b.putBoolean("commit", commit);
                    int val1 = numberPicker1.getValue() + minValue;
                    int val2 = numberPicker2.getValue();
                    String val = "" + val1 + "." + val2 + "f";
                    float selectedFloat = Float.parseFloat(val);
                    b.putFloat("coloroffset", selectedFloat);
                    b.putInt("color", color);
                    b.putInt("direction", direction);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_coloralignment_btn_get_current: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_COLORALIGNMENT);
                    Bundle b = new Bundle();
                    b.putInt("type", 0);
                    b.putInt("color", color);
                    b.putInt("direction", direction);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_coloralignment_btn_get_startup: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_COLORALIGNMENT);
                    Bundle b = new Bundle();
                    b.putInt("type", 1);
                    b.putInt("color", color);
                    b.putInt("direction", direction);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_coloralignment_btn_get_factory: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_COLORALIGNMENT);
                    Bundle b = new Bundle();
                    b.putInt("type", 2);
                    b.putInt("color", color);
                    b.putInt("direction", direction);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                default: {
                    break;
                }
            }
        }
    };
    private int colorOffset = 0;
    private View rootView;

    public ColorAlignment() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_coloralignment, container, false);
        initView();
        return rootView;
    }

    private void initView() {

        numberPicker1 = rootView.findViewById(R.id.fragment_coloralignment_numberPicker1);
        numberPicker1.setMinValue(0);
        numberPicker1.setMaxValue(maxValue - minValue);
        numberPicker1.setFormatter(new NumberPicker.Formatter() {
            @Override
            public String format(int index) {
                return Integer.toString(index + minValue);
            }
        });
        numberPicker1.setValue(0);
        numberPicker1.setWrapSelectorWheel(false);
        numberPicker1.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);


        numberPicker2 = rootView.findViewById(R.id.fragment_coloralignment_numberPicker2);
        numberPicker2.setMaxValue(9);
        numberPicker2.setMinValue(0);
        numberPicker1.setValue(0);
        numberPicker2.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);

        mBtnSet = rootView.findViewById(R.id.fragment_coloralignment_btn_set);
        mBtnSet.setOnClickListener(onClickListener);

        mRed = rootView.findViewById(R.id.fragment_coloralignment_color_red);
        mRed.setOnClickListener(onClickListener);
        mBlue = rootView.findViewById(R.id.fragment_coloralignment_color_blue);
        mBlue.setOnClickListener(onClickListener);
        mGreen = rootView.findViewById(R.id.fragment_coloralignment_color_green);
        mGreen.setOnClickListener(onClickListener);
        mRed2 = rootView.findViewById(R.id.fragment_coloralignment_color_red_2);
        mRed2.setOnClickListener(onClickListener);
        mGreen2 = rootView.findViewById(R.id.fragment_coloralignment_color_green_2);
        mGreen2.setOnClickListener(onClickListener);
        directionH = rootView.findViewById(R.id.fragment_coloralignment_dir_h);
        directionH.setOnClickListener(onClickListener);
        directionV = rootView.findViewById(R.id.fragment_coloralignment_dir_v);
        directionV.setOnClickListener(onClickListener);
        mChkCommit = rootView.findViewById(R.id.checkbox_commit_coloralignment);
        mChkCommit.setOnClickListener(onClickListener);
        mTvSetResult = rootView.findViewById(R.id.fragment_coloralignment_set_result);

        mBtnGetCurrent = rootView.findViewById(R.id.fragment_coloralignment_btn_get_current);
        mBtnGetCurrent.setOnClickListener(onClickListener);
        mBtnGetStartup = rootView.findViewById(R.id.fragment_coloralignment_btn_get_startup);
        mBtnGetStartup.setOnClickListener(onClickListener);
        mBtnGetFactory = rootView.findViewById(R.id.fragment_coloralignment_btn_get_factory);
        mBtnGetFactory.setOnClickListener(onClickListener);

        mTvGetColoralignmentCurrent = rootView.findViewById(R.id.fragment_coloralignment_tv_get_current);
        mTvGetColoralignmentCurrent.setText("");
        mTvGetColoralignmentStartup = rootView.findViewById(R.id.fragment_coloralignment_tv_get_startup);
        mTvGetColoralignmentStartup.setText("");
        mTvGetColoralignmentFactory = rootView.findViewById(R.id.fragment_coloralignment_tv_get_factory);
        mTvGetColoralignmentFactory.setText("");

        color = 1;
        direction = 1;
    }
}