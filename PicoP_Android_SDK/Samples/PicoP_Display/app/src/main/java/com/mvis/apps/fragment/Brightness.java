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
import android.widget.SeekBar;
import android.widget.TextView;

import com.mvis.apps.R;

import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.MessageCenter.resultData;
import static com.mvis.apps.Utils.MSG_GET_BRIGHTNESS;
import static com.mvis.apps.Utils.MSG_SET_BRIGHTNESS;


public class Brightness extends Fragment {

    public static final int MSG_BRIGHTNESS_RESPONSE_GET = 90001;
    public static final int MSG_BRIGHTNESS_RESPONSE_SET = 90002;
    private static final String TAG = "Brightness";
    public static Handler brightnessHandler = new Handler() {
        // when handler.message() called, below code will be triggered.
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MSG_BRIGHTNESS_RESPONSE_GET:
                    Bundle b = msg.getData();
                    int result = b.getInt("result");
                    int type = b.getInt("type");
                    if (result == 0) {
                        if (type == 0) {
                            mTvGetBrightnessCurrent.setText("" + resultData.getBrightness());
                        } else if (type == 1) {
                            mTvGetBrightnessStartup.setText("" + resultData.getBrightness());
                        } else if (type == 2) {
                            mTvGetBrightnessFactory.setText("" + resultData.getBrightness());
                        }
                    } else {
                        String STR = b.getString("STR");
                        if (type == 0) {
                            mTvGetBrightnessCurrent.setText("GetBrightness failed, error msg  = " + STR);
                        } else if (type == 1) {
                            mTvGetBrightnessStartup.setText("GetBrightness failed, error msg = " + STR);
                        } else if (type == 2) {
                            mTvGetBrightnessFactory.setText("GetBrightness failed, error msg  = " + STR);
                        }
                    }
                    break;
                case MSG_BRIGHTNESS_RESPONSE_SET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mTvSetResult.setText("Call success ");
                    } else {
                        String STR = b.getString("STR");
                        mTvSetResult.setText("SetMode Call failed, error msg = " + STR);
                    }
                    break;
            }
        }
    };
    private static TextView mTvMaxValue, mTvSetBrightnessCurrent, mTvGetBrightnessCurrent, mTvGetBrightnessStartup, mTvGetBrightnessFactory, mTvSetResult;
    private SeekBar mSeekBarBrightness = null;
    private View rootView;
    private Button mBtnSet, mBtnGetCurrent, mBtnGetStartup, mBtnGetFactory;
    private int mProgress = 100;
    private CheckBox mChkCommit;
    private boolean commit = false;
    public View.OnClickListener onClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.checkbox_commit_brightness: {
                    if (mChkCommit.isChecked())
                        commit = true;
                    else
                        commit = false;
                    break;
                }
                case R.id.fragment_brightness_btn_set: {
                    Message msg0 = messageHandler.obtainMessage(MSG_SET_BRIGHTNESS);
                    Bundle b = new Bundle();
                    b.putBoolean("commit", commit);
                    b.putInt("brightness", mProgress);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_brightness_btn_get_current: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_BRIGHTNESS);
                    Bundle b = new Bundle();
                    b.putInt("type", 0);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_brightness_btn_get_startup: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_BRIGHTNESS);
                    Bundle b = new Bundle();
                    b.putInt("type", 1);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_brightness_btn_get_factory: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_BRIGHTNESS);
                    Bundle b = new Bundle();
                    b.putInt("type", 2);
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

    public Brightness() {
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_brightness, container, false);
        initView();
        return rootView;
    }

    private void initView() {
        //Textview should init before seekbar, otherwise seekbar changelistener will crash.
        mTvSetBrightnessCurrent = rootView.findViewById(R.id.fragment_brightness_tv_set_curr_value);
        mTvSetBrightnessCurrent.setText("1.0");

        mSeekBarBrightness = rootView.findViewById(R.id.fragment_brightness_seekbar_brightness);
        mSeekBarBrightness.setOnSeekBarChangeListener(new OnSeekBarChangeListenerImp());
        mSeekBarBrightness.setEnabled(true);
        mSeekBarBrightness.setMax(100);
        mSeekBarBrightness.setProgress(100);

        mTvMaxValue = rootView.findViewById(R.id.fragment_brightness_tv_max);
        mTvMaxValue.setText("100");
        mBtnSet = rootView.findViewById(R.id.fragment_brightness_btn_set);
        mBtnSet.setOnClickListener(onClickListener);

        mBtnGetCurrent = rootView.findViewById(R.id.fragment_brightness_btn_get_current);
        mBtnGetCurrent.setOnClickListener(onClickListener);
        mBtnGetStartup = rootView.findViewById(R.id.fragment_brightness_btn_get_startup);
        mBtnGetStartup.setOnClickListener(onClickListener);
        mBtnGetFactory = rootView.findViewById(R.id.fragment_brightness_btn_get_factory);
        mBtnGetFactory.setOnClickListener(onClickListener);

        mTvGetBrightnessCurrent = rootView.findViewById(R.id.fragment_brightness_tv_get_current);
        mTvGetBrightnessCurrent.setText("");
        mTvGetBrightnessStartup = rootView.findViewById(R.id.fragment_brightness_tv_get_startup);
        mTvGetBrightnessStartup.setText("");
        mTvGetBrightnessFactory = rootView.findViewById(R.id.fragment_brightness_tv_get_factory);
        mTvGetBrightnessFactory.setText("");

        mTvSetResult = rootView.findViewById(R.id.fragment_set_brightness_result);


        mChkCommit = rootView.findViewById(R.id.checkbox_commit_brightness);
        mChkCommit.setOnClickListener(onClickListener);

    }

    private class OnSeekBarChangeListenerImp implements
            SeekBar.OnSeekBarChangeListener {
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            mTvSetBrightnessCurrent.setText("" + progress);
        }

        // start
        public void onStartTrackingTouch(SeekBar seekBar) {
        }

        // stop
        public void onStopTrackingTouch(SeekBar seekBar) {

            mProgress = seekBar.getProgress();
        }
    }

}