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
import android.widget.TextView;


import com.mvis.apps.R;

import picop.interfaces.def.PicoP_ColorModeE;

import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.MessageCenter.resultData;
import static com.mvis.apps.Utils.MSG_GET_COLOR_MODE;
import static com.mvis.apps.Utils.MSG_SET_COLOR_MODE;

public class ColorMode extends Fragment {

    public static final int MSG_COLORMODE_RESPONSE_GET = 90001;
    public static final int MSG_COLORMODE_RESPONSE_SET = 90002;
    private static TextView mTvSetResult, mTvGetColormodeCurrent, mTvGetColormodeStartup, mTvGetColormodeFactory;
    public static Handler colorModeHandler = new Handler() {
        // when handler.message() called, below code will be triggered.
        public void handleMessage(Message msg) {
            Bundle b;
            int result;
            //super.handleMessage(msg);
            switch (msg.what) {
                case MSG_COLORMODE_RESPONSE_GET:
                    b = msg.getData();
                    result = b.getInt("result");
                    int type = b.getInt("type");
                    PicoP_ColorModeE colorModeE = PicoP_ColorModeE.eCOLOR_MODE_STANDARD;
                    if (result == 0) {
                        if (type == 0) {
                            mTvGetColormodeCurrent.setText("" + colorModeE.intToEnum(resultData.getColormode()));
                        } else if (type == 1) {
                            mTvGetColormodeStartup.setText("" + colorModeE.intToEnum(resultData.getColormode()));
                        } else if (type == 2) {
                            mTvGetColormodeFactory.setText("" + colorModeE.intToEnum(resultData.getColormode()));
                        }
                    } else {
                        String STR = b.getString("STR");
                        if (type == 0) {
                            mTvGetColormodeCurrent.setText("GetColorMode failed, error msg = " + STR);
                        } else if (type == 1) {
                            mTvGetColormodeStartup.setText("GetColorMode failed, error msg = " + STR);
                        } else if (type == 2) {
                            mTvGetColormodeFactory.setText("GetColorMode failed, error msg = " + STR);
                        }
                    }
                    break;
                case MSG_COLORMODE_RESPONSE_SET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mTvSetResult.setText("Call success ");
                    } else {
                        String STR = b.getString("STR");
                        mTvSetResult.setText("SetColorMode failed, error msg = " + STR);
                    }
                    break;
            }
        }
    };
    public RadioButton mBrilliant, mInverted, mStandard, mRich, mCustom;
    private Button mBtnSet, mBtnGetCurrent, mBtnGetStartup, mBtnGetFactory;
    private int mSetMode = 0;
    private View rootView;
    private CheckBox mChkCommit;
    private boolean commit = false;
    /* { button code */
    public View.OnClickListener onClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.checkbox_commit_color_mode: {
                    if (mChkCommit.isChecked())
                        commit = true;
                    else
                        commit = false;
                    break;
                }
                case R.id.fragment_colormode_standard: {
                    if (mStandard.isChecked())
                        mSetMode = 0;
                    break;
                }
                case R.id.fragment_colormode_brilliant: {
                    if (mBrilliant.isChecked())
                        mSetMode = 1;
                    break;
                }
                case R.id.fragment_colormode_rich: {
                    if (mRich.isChecked())
                        mSetMode = 2;
                    break;
                }
                case R.id.fragment_colormode_inverted: {
                    if (mInverted.isChecked())
                        mSetMode = 3;
                    break;
                }
                case R.id.fragment_colormode_custom: {
                    if (mCustom.isChecked())
                        mSetMode = 4;
                    break;
                }
                case R.id.fragment_colormode_btn_set_commit: {
                    Message msg0 = messageHandler.obtainMessage(MSG_SET_COLOR_MODE);
                    Bundle b = new Bundle();
                    b.putBoolean("commit", commit);
                    b.putInt("mode", mSetMode);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_colormode_btn_get_current: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_COLOR_MODE);
                    Bundle b = new Bundle();
                    b.putInt("type", 0);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_colormode_btn_get_startup: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_COLOR_MODE);
                    Bundle b = new Bundle();
                    b.putInt("type", 1);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_colormode_btn_get_factory: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_COLOR_MODE);
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

    public ColorMode() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_colormode, container, false);
        initView();
        return rootView;
    }

    private void initView() {

        mBtnSet = rootView.findViewById(R.id.fragment_colormode_btn_set_commit);
        mBtnSet.setOnClickListener(onClickListener);

        mBrilliant = rootView.findViewById(R.id.fragment_colormode_brilliant);
        mBrilliant.setOnClickListener(onClickListener);
        mInverted = rootView.findViewById(R.id.fragment_colormode_inverted);
        mInverted.setOnClickListener(onClickListener);
        mStandard = rootView.findViewById(R.id.fragment_colormode_standard);
        mStandard.setOnClickListener(onClickListener);
        mRich = rootView.findViewById(R.id.fragment_colormode_rich);
        mRich.setOnClickListener(onClickListener);
        mCustom = rootView.findViewById(R.id.fragment_colormode_custom);
        mCustom.setOnClickListener(onClickListener);
        mChkCommit = rootView.findViewById(R.id.checkbox_commit_color_mode);
        mChkCommit.setOnClickListener(onClickListener);

        mBtnGetCurrent = rootView.findViewById(R.id.fragment_colormode_btn_get_current);
        mBtnGetCurrent.setOnClickListener(onClickListener);
        mBtnGetStartup = rootView.findViewById(R.id.fragment_colormode_btn_get_startup);
        mBtnGetStartup.setOnClickListener(onClickListener);
        mBtnGetFactory = rootView.findViewById(R.id.fragment_colormode_btn_get_factory);
        mBtnGetFactory.setOnClickListener(onClickListener);


        mTvGetColormodeCurrent = rootView.findViewById(R.id.fragment_colormode_tv_get_current);
        mTvGetColormodeCurrent.setText("");
        mTvGetColormodeStartup = rootView.findViewById(R.id.fragment_colormode_tv_get_startup);
        mTvGetColormodeStartup.setText("");
        mTvGetColormodeFactory = rootView.findViewById(R.id.fragment_colormode_tv_get_factory);
        mTvGetColormodeFactory.setText("");
        mTvSetResult = rootView.findViewById(R.id.fragment_set_color_mode_result);

    }
}
