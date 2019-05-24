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

import picop.interfaces.def.PicoP_HorizontalBrightnessCompensationModeE;

import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.MessageCenter.resultData;
import static com.mvis.apps.Utils.MSG_SET_HORIZONTAL_BRIGHTNESS_COMPENSATION_MODE;
import static com.mvis.apps.Utils.MSG_GET_HORIZONTAL_BRIGHTNESS_COMPENSATION_MODE;


public class HorizontalBrightnessCompensationMode extends Fragment {

    public static final int MSG_HORIZONTAL_BRIGHTNESS_MODE_RESPONSE_GET = 90001;
    public static final int MSG_HORIZONTAL_BRIGHTNESS_MODE_RESPONSE_SET = 90002;
    private static TextView mTvSetResult, mTvGetCurrent, mTvGetStartup, mTvGetFactory;
    public static Handler horizontalbrightnesscompensationmodeHandler = new Handler() {
        // when handler.message() called, below code will be triggered.
        public void handleMessage(Message msg) {
            //super.handleMessage(msg);
            Bundle b;
            int result;
            switch (msg.what) {
                case MSG_HORIZONTAL_BRIGHTNESS_MODE_RESPONSE_GET:
                    b = msg.getData();
                    result = b.getInt("result");
                    int type = b.getInt("type");
                    PicoP_HorizontalBrightnessCompensationModeE modeE = PicoP_HorizontalBrightnessCompensationModeE.eHBC_OFF;
                    if (result == 0) {
                        if (type == 0) {
                            mTvGetCurrent.setText("" + modeE.intToEnum(resultData.getHorizontalBrightnessCompensationMode()));
                        } else if (type == 1) {
                            mTvGetStartup.setText("" + modeE.intToEnum(resultData.getHorizontalBrightnessCompensationMode()));
                        } else if (type == 2) {
                            mTvGetFactory.setText("" + modeE.intToEnum(resultData.getHorizontalBrightnessCompensationMode()));
                        }
                    } else {
                        String STR = b.getString("STR");
                        if (type == 0) {
                            mTvGetCurrent.setText("GetMode failed, error msg = " + STR);
                        } else if (type == 1) {
                            mTvGetStartup.setText("GetMode failed, error msg = " + STR);
                        } else if (type == 2) {
                            mTvGetFactory.setText("GetMode failed, error msg = " + STR);
                        }
                    }
                    break;
                case MSG_HORIZONTAL_BRIGHTNESS_MODE_RESPONSE_SET:
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
    private Button mBtnSet, mBtnGetCurrent, mBtnGetStartup, mBtnGetFactory;
    private RadioButton mOff, mMedium, mUni;
    private CheckBox mChkCommit;
    private boolean commit = false;
    private int mSetMode = 0;
    /* { button code */
    public View.OnClickListener onClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.checkbox_commit_comp_mode: {
                    if (mChkCommit.isChecked())
                        commit = true;
                    else
                        commit = false;
                    break;
                }
                case R.id.fragment_comp_off: {
                    if (mOff.isChecked())
                        mSetMode = 0;
                    break;
                }
                case R.id.fragment_comp_med: {
                    if (mMedium.isChecked())
                        mSetMode = 1;
                    break;
                }
                case R.id.fragment_comp_uni: {
                    if (mUni.isChecked())
                        mSetMode = 2;
                    break;
                }
                case R.id.fragment_comp_mode_btn_set: {
                    Message msg0 = messageHandler.obtainMessage(MSG_SET_HORIZONTAL_BRIGHTNESS_COMPENSATION_MODE);
                    Bundle b = new Bundle();
                    b.putBoolean("commit", commit);
                    b.putInt("comp_mode", mSetMode);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_comp_mode_btn_get_current: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_HORIZONTAL_BRIGHTNESS_COMPENSATION_MODE);
                    Bundle b = new Bundle();
                    b.putInt("type", 0);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_comp_mode_btn_get_startup: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_HORIZONTAL_BRIGHTNESS_COMPENSATION_MODE);
                    Bundle b = new Bundle();
                    b.putInt("type", 1);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_comp_mode_btn_get_factory: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_HORIZONTAL_BRIGHTNESS_COMPENSATION_MODE);
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
    private View rootView;

    public HorizontalBrightnessCompensationMode() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_horizontalbrightnesscompensationmode, container, false);
        initView();
        return rootView;
    }

    private void initView() {

        mBtnSet = rootView.findViewById(R.id.fragment_comp_mode_btn_set);
        mBtnSet.setOnClickListener(onClickListener);

        mChkCommit = rootView.findViewById(R.id.checkbox_commit_comp_mode);
        mChkCommit.setOnClickListener(onClickListener);

        mOff = rootView.findViewById(R.id.fragment_comp_off);
        mOff.setOnClickListener(onClickListener);
        mMedium = rootView.findViewById(R.id.fragment_comp_med);
        mMedium.setOnClickListener(onClickListener);
        mUni = rootView.findViewById(R.id.fragment_comp_uni);
        mUni.setOnClickListener(onClickListener);

        mBtnGetCurrent = rootView.findViewById(R.id.fragment_comp_mode_btn_get_current);
        mBtnGetCurrent.setOnClickListener(onClickListener);
        mBtnGetStartup = rootView.findViewById(R.id.fragment_comp_mode_btn_get_startup);
        mBtnGetStartup.setOnClickListener(onClickListener);
        mBtnGetFactory = rootView.findViewById(R.id.fragment_comp_mode_btn_get_factory);
        mBtnGetFactory.setOnClickListener(onClickListener);

        mTvGetCurrent = rootView.findViewById(R.id.fragment_comp_mode_tv_get_current);
        mTvGetCurrent.setText("");
        mTvGetStartup = rootView.findViewById(R.id.fragment_comp_mode_tv_get_startup);
        mTvGetStartup.setText("");
        mTvGetFactory = rootView.findViewById(R.id.fragment_comp_mode_tv_get_factory);
        mTvGetFactory.setText("");
        mTvSetResult = rootView.findViewById(R.id.fragment_set_comp_mode_result);

    }
}