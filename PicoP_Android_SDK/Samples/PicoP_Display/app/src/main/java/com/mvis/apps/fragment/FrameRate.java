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
import android.widget.RadioButton;
import android.widget.TextView;

import com.mvis.apps.R;

import picop.interfaces.def.PicoP_DisplayHorizontalScalingModeE;
import picop.interfaces.def.PicoP_DisplayVerticalScalingModeE;
import picop.interfaces.def.PicoP_FrameRateE;

import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.MessageCenter.resultData;
import static com.mvis.apps.Utils.MSG_GET_FRAMERATE_MODE;
import static com.mvis.apps.Utils.MSG_GET_GAMMABOOSTMODE;
import static com.mvis.apps.Utils.MSG_SET_FRAMERATE_MODE;
import static com.mvis.apps.Utils.MSG_SET_GAMMABOOSTMODE;


public class FrameRate extends Fragment {
    public static final int MSG_FRAMERATE_RESPONSE_GET = 90001;
    public static final int MSG_FRAMERATE_RESPONSE_SET = 90002;
    private static final String TAG = "FrameRate";
    private static TextView mTvSetResult, mTvGetFrameRateModeCurrent, mTvGetFrameRateModeStartup, mTvGetFrameRateModeFactory;
    public static Handler frameRateHandler = new Handler() {
        // when handler.message() called, below code will be triggered.
        public void handleMessage(Message msg) {
            //super.handleMessage(msg);
            switch (msg.what) {
                case MSG_FRAMERATE_RESPONSE_GET:
                    Bundle b = msg.getData();
                    int result = b.getInt("result");
                    int type = b.getInt("type");
                    if (result == 0) {
                        if (type == 0) {
                            mTvGetFrameRateModeCurrent.setText("Frame Rate:" + resultData.getFrameRateMode() +
                                    ", Vertical Scaling:" + resultData.getVSMode() +
                                    ", Horiz Scaling : " + resultData.getHSmode());
                        } else if (type == 1) {
                            mTvGetFrameRateModeStartup.setText("Frame Rate:" + resultData.getFrameRateMode() +
                                    ", Vertical Scaling:" + resultData.getVSMode() +
                                    ", Horiz Scaling : " + resultData.getHSmode());
                        } else if (type == 2) {
                            mTvGetFrameRateModeFactory.setText("Frame Rate:" + resultData.getFrameRateMode() +
                                    ", Vertical Scaling:" + resultData.getVSMode() +
                                    ", Horiz Scaling : " + resultData.getHSmode());
                        }
                    } else {
                        String STR = b.getString("STR");
                        if (type == 0) {
                            mTvGetFrameRateModeCurrent.setText("GetFrame Rate failed, error msg = " + STR);
                        } else if (type == 1) {
                            mTvGetFrameRateModeStartup.setText("GetFrame Rate failed, error msg = " + STR);
                        } else if (type == 2) {
                            mTvGetFrameRateModeFactory.setText("GetFrame Rate failed, error msg = " + STR);
                        }
                    }
                    break;
                case MSG_FRAMERATE_RESPONSE_SET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mTvSetResult.setText("Call success ");
                    } else {
                        String STR = b.getString("STR");
                        mTvSetResult.setText("SetFrame Rate failed, error msg = " + STR);
                    }
                    break;
            }
        }
    };
    private Button mBtnSet, mBtnGetCurrent, mBtnGetStartup, mBtnGetFactory;
    private int mSetFrameRateMode = 0, mVSMode = 0, mHSMode = 0;
    private View rootView;
    private RadioButton m720p, m600p, m640p, mHSNone, mHSDown, mVSBottom, mVSDown, mVSEven;
    private CheckBox mChkCommit;
    private boolean commit = false;
    public View.OnClickListener onClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.checkbox_commit_framerate: {
                    if (mChkCommit.isChecked())
                        commit = true;
                    else
                        commit = false;
                    break;
                }
                case R.id.fragment_framerate_mode1: {
                    if (m720p.isChecked())
                        mSetFrameRateMode = 0;
                    break;
                }
                case R.id.fragment_framerate_mode2: {

                    if (m640p.isChecked())
                        mSetFrameRateMode = 1;
                    break;
                }
                case R.id.fragment_framerate_mode3: {
                    if (m600p.isChecked())
                        mSetFrameRateMode = 2;
                    break;
                }
                case R.id.fragment_framerate_vs_down: {
                    if (mVSDown.isChecked())
                        mVSMode = 0;
                    break;
                }
                case R.id.fragment_framerate_vs_bottom: {

                    if (mVSBottom.isChecked())
                        mVSMode = 1;
                    break;
                }
                case R.id.fragment_framerate_vs_even: {
                    if (mVSEven.isChecked())
                        mVSMode = 2;
                    break;
                }
                case R.id.fragment_framerate_hs_down: {
                    if (mHSDown.isChecked())
                        mHSMode = 0;
                    break;
                }
                case R.id.fragment_framerate_hs_none: {
                    if (mHSNone.isChecked())
                        mHSMode = 1;
                    break;
                }
                case R.id.fragment_framerate_set: {
                    Message msg0 = messageHandler.obtainMessage(MSG_SET_FRAMERATE_MODE);
                    Bundle b = new Bundle();
                    b.putBoolean("commit", commit);
                    b.putInt("vsmode", mVSMode);
                    b.putInt("hsmode", mHSMode);
                    b.putInt("frameratemode", mSetFrameRateMode);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_framerate_btn_get_current: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_FRAMERATE_MODE);
                    Bundle b = new Bundle();
                    b.putInt("type", 0);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_framerate_btn_get_startup: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_FRAMERATE_MODE);
                    Bundle b = new Bundle();
                    b.putInt("type", 1);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_framerate_btn_get_factory: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_FRAMERATE_MODE);
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

    public FrameRate() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_frameratemode, container, false);
        initView();
        return rootView;
    }

    private void initView() {
        m720p = rootView.findViewById(R.id.fragment_framerate_mode1);
        m720p.setOnClickListener(onClickListener);
        m640p = rootView.findViewById(R.id.fragment_framerate_mode2);
        m640p.setOnClickListener(onClickListener);
        m600p = rootView.findViewById(R.id.fragment_framerate_mode3);
        m600p.setOnClickListener(onClickListener);

        mHSDown = rootView.findViewById(R.id.fragment_framerate_hs_down);
        mHSDown.setOnClickListener(onClickListener);
        mHSNone = rootView.findViewById(R.id.fragment_framerate_hs_none);
        mHSNone.setOnClickListener(onClickListener);

        mVSDown = rootView.findViewById(R.id.fragment_framerate_vs_down);
        mVSDown.setOnClickListener(onClickListener);
        mVSBottom = rootView.findViewById(R.id.fragment_framerate_vs_bottom);
        mVSBottom.setOnClickListener(onClickListener);
        mVSEven = rootView.findViewById(R.id.fragment_framerate_vs_even);
        mVSEven.setOnClickListener(onClickListener);

        mBtnSet = rootView.findViewById(R.id.fragment_framerate_set);
        mBtnSet.setOnClickListener(onClickListener);

        mChkCommit = rootView.findViewById(R.id.checkbox_commit_framerate);
        mChkCommit.setOnClickListener(onClickListener);

        mBtnGetCurrent = rootView.findViewById(R.id.fragment_framerate_btn_get_current);
        mBtnGetCurrent.setOnClickListener(onClickListener);
        mBtnGetStartup = rootView.findViewById(R.id.fragment_framerate_btn_get_startup);
        mBtnGetStartup.setOnClickListener(onClickListener);
        mBtnGetFactory = rootView.findViewById(R.id.fragment_framerate_btn_get_factory);
        mBtnGetFactory.setOnClickListener(onClickListener);


        mTvGetFrameRateModeCurrent = rootView.findViewById(R.id.fragment_framerate_tv_get_current);
        mTvGetFrameRateModeCurrent.setText("");
        mTvGetFrameRateModeStartup = rootView.findViewById(R.id.fragment_framerate_tv_get_startup);
        mTvGetFrameRateModeStartup.setText("");
        mTvGetFrameRateModeFactory = rootView.findViewById(R.id.fragment_framerate_tv_get_factory);
        mTvGetFrameRateModeFactory.setText("");
        mTvSetResult = rootView.findViewById(R.id.fragment_framerate_set_result);
    }
}
