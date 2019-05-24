package com.mvis.apps.fragment;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.SeekBar;
import android.widget.TextView;

import com.mvis.apps.R;


import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.MessageCenter.resultData;
import static com.mvis.apps.Utils.MSG_GET_COLORCONVERTER;
import static com.mvis.apps.Utils.MSG_SET_COLORCONVERTER;

public class ColorConvertor extends Fragment {

    public static final int MSG_COLORCONVERTOR_RESPONSE_GET = 90001;
    public static final int MSG_COLORCONVERTOR_RESPONSE_SET = 90002;
    private static final String TAG = "ColorConvertor";
    public static Handler colorconvertorHandler = new Handler() {
        // when handler.message() called, below code will be triggered.
        public void handleMessage(Message msg) {
            Bundle b;
            int result;
            //super.handleMessage(msg);
            switch (msg.what) {
                case MSG_COLORCONVERTOR_RESPONSE_SET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mTvSetResult.setText("Call success ");
                    } else {
                        String STR = b.getString("STR");
                        mTvSetResult.setText("SetColorConvertor failed, error msg = " + STR);
                    }
                    break;
                case MSG_COLORCONVERTOR_RESPONSE_GET:
                    b = msg.getData();
                    result = b.getInt("result");
                    int type = b.getInt("type");
                    if (result == 0) {
                        if (type == 0) {
                            mTvGetColorConvertorCurrent.setText("coefficient:" + resultData.getCoefficient());
                        } else if (type == 1) {
                            mTvGetColorConvertorStartup.setText("coefficient:" + resultData.getCoefficient());
                        } else if (type == 2) {
                            mTvGetColorConvertorFactory.setText("coefficient:" + resultData.getCoefficient());
                        }
                    } else {
                        String STR = b.getString("STR");
                        if (type == 0) {
                            mTvGetColorConvertorCurrent.setText("GetColorConvertor failed, error msg = " + STR);
                        } else if (type == 1) {
                            mTvGetColorConvertorStartup.setText("GetColorConvertor failed, error msg = " + STR);
                        } else if (type == 2) {
                            mTvGetColorConvertorFactory.setText("GetColorConvertor failed, error msg = " + STR);
                        }
                    }
                    break;
            }
        }
    };
    private static TextView mTvSetColorConvertorCurrent, mTvGetColorConvertorCurrent, mTvGetColorConvertorStartup, mTvGetColorConvertorFactory, mTvSetResult;
    private SeekBar mSeekBarColorConvertor = null;
    private Button mBtnSet, mBtnGetCurrent, mBtnGetStartup, mBtnGetFactory;
    private RadioButton mRed, mBlue, mGreen, mGreentToRed, mBlueToRed, mRedToGreen, mBlueToGreen, mRedToBlue, mGreenToBlue;
    private RadioGroup g1, g2, g3, g4, g5;
    private CheckBox mChkCommit;
    private boolean commit = false;
    private int color = 0;
    private int coefficient = 0;
    /* { button code */
    public View.OnClickListener onClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.checkbox_commit_colorconvertor: {
                    if (mChkCommit.isChecked())
                        commit = true;
                    else
                        commit = false;
                    break;
                }
                case R.id.fragment_colorconvertorcolor_red: {
                    if (mRed.isChecked()) {
                        g2.clearCheck();
                        g3.clearCheck();
                        g4.clearCheck();
                        g5.clearCheck();
                        color = 0;
                    }
                    break;
                }
                case R.id.fragment_colorconvertorcolor_green_to_red: {
                    if (mGreentToRed.isChecked()) {
                        g1.clearCheck();
                        g3.clearCheck();
                        g4.clearCheck();
                        g5.clearCheck();
                        color = 1;
                    }
                    break;
                }
                case R.id.fragment_colorconvertorcolor_blue_to_red: {
                    if (mBlueToRed.isChecked()) {
                        g1.clearCheck();
                        g2.clearCheck();
                        g3.clearCheck();
                        g4.clearCheck();
                        color = 2;
                    }
                    break;
                }
                case R.id.fragment_colorconvertorcolor_red_to_green: {
                    if (mRedToGreen.isChecked()) {
                        g1.clearCheck();
                        g5.clearCheck();
                        g3.clearCheck();
                        g4.clearCheck();
                        color = 3;
                    }
                    break;
                }
                case R.id.fragment_colorconvertorcolor_green: {
                    if (mGreen.isChecked()) {
                        g2.clearCheck();
                        g5.clearCheck();
                        g3.clearCheck();
                        g4.clearCheck();
                        color = 4;
                    }
                    break;
                }
                case R.id.fragment_colorconvertorcolor_blue_to_green: {
                    if (mBlueToGreen.isChecked()) {
                        g2.clearCheck();
                        g5.clearCheck();
                        g3.clearCheck();
                        g1.clearCheck();
                        color = 5;
                    }
                    break;
                }
                case R.id.fragment_colorconvertorcolor_red_to_blue: {
                    if (mRedToBlue.isChecked()) {
                        g2.clearCheck();
                        g5.clearCheck();
                        g4.clearCheck();
                        g1.clearCheck();
                        color = 6;
                    }
                    break;
                }
                case R.id.fragment_colorconvertorcolor_green_to_blue: {
                    if (mGreenToBlue.isChecked()) {
                        g2.clearCheck();
                        g5.clearCheck();
                        g4.clearCheck();
                        g1.clearCheck();
                        color = 7;
                    }
                    break;
                }
                case R.id.fragment_colorconvertorcolor_blue: {
                    if (mBlue.isChecked()) {
                        g2.clearCheck();
                        g5.clearCheck();
                        g3.clearCheck();
                        g1.clearCheck();
                        color = 8;
                    }
                    break;
                }
                case R.id.fragment_colorconvertorbtn_set: {
                    Message msg0 = messageHandler.obtainMessage(MSG_SET_COLORCONVERTER);
                    Bundle b = new Bundle();
                    b.putBoolean("commit", commit);
                    b.putInt("color", color);
                    b.putInt("coefficient", coefficient);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_colorconvertorbtn_get_current: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_COLORCONVERTER);
                    Bundle b = new Bundle();
                    b.putInt("type", 0);
                    b.putInt("color", color);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_colorconvertorbtn_get_startup: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_COLORCONVERTER);
                    Bundle b = new Bundle();
                    b.putInt("type", 1);
                    b.putInt("color", color);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_colorconvertorbtn_get_factory: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_COLORCONVERTER);
                    Bundle b = new Bundle();
                    b.putInt("type", 2);
                    b.putInt("color", color);
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
    private View rootView;

    public ColorConvertor() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_colorconvertor, container, false);
        initView();
        return rootView;
    }

    private void initView() {
        /* Set Brightnes */
        //Textview should init before seekbar, otherwise seekbar changelistener will crash.
        mTvSetColorConvertorCurrent = rootView.findViewById(R.id.fragment_colorconvertortv_set_curr_value);
        mTvSetColorConvertorCurrent.setText("0");

        mSeekBarColorConvertor = rootView.findViewById(R.id.fragment_colorconvertorseekbar_colorconvertor);
        mSeekBarColorConvertor.setOnSeekBarChangeListener(new ColorConvertor.OnSeekBarChangeListenerImp());
        mSeekBarColorConvertor.setEnabled(true);
        mSeekBarColorConvertor.setMax(100);
        mSeekBarColorConvertor.setProgress(50);

        mBtnSet = rootView.findViewById(R.id.fragment_colorconvertorbtn_set);
        mBtnSet.setOnClickListener(onClickListener);

        g1 = rootView.findViewById(R.id.group_one);
        g2 = rootView.findViewById(R.id.group_two);
        g3 = rootView.findViewById(R.id.group_three);
        g4 = rootView.findViewById(R.id.group_four);
        g5 = rootView.findViewById(R.id.group_five);

        mRed = rootView.findViewById(R.id.fragment_colorconvertorcolor_red);
        mRed.setOnClickListener(onClickListener);
        mBlue = rootView.findViewById(R.id.fragment_colorconvertorcolor_blue);
        mBlue.setOnClickListener(onClickListener);
        mGreen = rootView.findViewById(R.id.fragment_colorconvertorcolor_green);
        mGreen.setOnClickListener(onClickListener);
        mGreentToRed = rootView.findViewById(R.id.fragment_colorconvertorcolor_green_to_red);
        mGreentToRed.setOnClickListener(onClickListener);
        mBlueToRed = rootView.findViewById(R.id.fragment_colorconvertorcolor_blue_to_red);
        mBlueToRed.setOnClickListener(onClickListener);
        mRedToGreen = rootView.findViewById(R.id.fragment_colorconvertorcolor_red_to_green);
        mRedToGreen.setOnClickListener(onClickListener);
        mBlueToGreen = rootView.findViewById(R.id.fragment_colorconvertorcolor_blue_to_green);
        mBlueToGreen.setOnClickListener(onClickListener);
        mRedToBlue = rootView.findViewById(R.id.fragment_colorconvertorcolor_red_to_blue);
        mRedToBlue.setOnClickListener(onClickListener);
        mGreenToBlue = rootView.findViewById(R.id.fragment_colorconvertorcolor_green_to_blue);
        mGreenToBlue.setOnClickListener(onClickListener);
        mChkCommit = rootView.findViewById(R.id.checkbox_commit_colorconvertor);
        mChkCommit.setOnClickListener(onClickListener);
        mTvSetResult = rootView.findViewById(R.id.fragment_colorconvertorset_result);

        mBtnGetCurrent = rootView.findViewById(R.id.fragment_colorconvertorbtn_get_current);
        mBtnGetCurrent.setOnClickListener(onClickListener);
        mBtnGetStartup = rootView.findViewById(R.id.fragment_colorconvertorbtn_get_startup);
        mBtnGetStartup.setOnClickListener(onClickListener);
        mBtnGetFactory = rootView.findViewById(R.id.fragment_colorconvertorbtn_get_factory);
        mBtnGetFactory.setOnClickListener(onClickListener);

        mTvGetColorConvertorCurrent = rootView.findViewById(R.id.fragment_colorconvertortv_get_current);
        mTvGetColorConvertorCurrent.setText("");
        mTvGetColorConvertorStartup = rootView.findViewById(R.id.fragment_colorconvertortv_get_startup);
        mTvGetColorConvertorStartup.setText("");
        mTvGetColorConvertorFactory = rootView.findViewById(R.id.fragment_colorconvertortv_get_factory);
        mTvGetColorConvertorFactory.setText("");

    }

    private class OnSeekBarChangeListenerImp implements
            SeekBar.OnSeekBarChangeListener {
        // progress
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            //setMoveTextLayout();
            coefficient = (int) (progress * 1310.71 - 65536);
            mTvSetColorConvertorCurrent.setText("" + coefficient);
        }

        // start
        public void onStartTrackingTouch(SeekBar seekBar) {
        }

        // stop
        public void onStopTrackingTouch(SeekBar seekBar) {

            coefficient = (int) (seekBar.getProgress() * 1310.71 - 65536);
        }
    }
}