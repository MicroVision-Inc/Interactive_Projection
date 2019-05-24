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
import android.widget.NumberPicker;
import android.widget.RadioButton;
import android.widget.TextView;

import com.mvis.apps.R;

import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.MessageCenter.resultData;
import static com.mvis.apps.Utils.MSG_GET_GAMMAVAL;
import static com.mvis.apps.Utils.MSG_SET_GAMMAVAL;

public class GammaVal extends Fragment {
    public static final int MSG_GAMMAVAL_RESPONSE_GET = 90001;
    public static final int MSG_GAMMAVAL_RESPONSE_SET = 90002;
    private static final String TAG = "GammaVal";
    private static TextView mTvSetResult, mTvGetGammavalCurrent, mTvGetGammavalStartup, mTvGetGammavalFactory;
    public static Handler gammavalHandler = new Handler() {
        // when handler.message() called, below code will be triggered.
        public void handleMessage(Message msg) {
            //super.handleMessage(msg);
            switch (msg.what) {
                case MSG_GAMMAVAL_RESPONSE_GET:
                    Bundle b = msg.getData();
                    int result = b.getInt("result");
                    int type = b.getInt("type");
                    if (result == 0) {
                        if (type == 0) {
                            mTvGetGammavalCurrent.setText("GammaVal : " + resultData.getGammaval());
                        } else if (type == 1) {
                            mTvGetGammavalStartup.setText("GammaVal : " + resultData.getGammaval());
                        } else if (type == 2) {
                            mTvGetGammavalFactory.setText("GammaVal : " + resultData.getGammaval());
                        }
                    } else {
                        String STR = b.getString("STR");
                        if (type == 0) {
                            mTvGetGammavalCurrent.setText("GetGammaVal failed, error msg = " + STR);
                        } else if (type == 1) {
                            mTvGetGammavalStartup.setText("GetGammaVal failed, error msg = " + STR);
                        } else if (type == 2) {
                            mTvGetGammavalFactory.setText("GetGammaVal failed, error msg = " + STR);
                        }
                    }
                    break;
                case MSG_GAMMAVAL_RESPONSE_SET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mTvSetResult.setText("SetGammaVal success ");
                    } else {
                        String STR = b.getString("STR");
                        mTvSetResult.setText("SetGammaVal failed, error msg = " + STR);
                    }
                    break;
            }
        }
    };
    NumberPicker numberPicker;
    private Button mBtnSet, mBtnGetCurrent, mBtnGetStartup, mBtnGetFactory;
    private RadioButton mRed, mBlue, mGreen;
    private View rootView;
    private int mColor = 0;
    private CheckBox mChkCommit;
    private boolean commit = false;
    private String nums[] = {"1.0", "1.1", "1.2", "1.3", "1.4", "1.5", "1.6", "1.7", "1.8", "1.9", "2.0", "2.1", "2.2", "2.3", "2.4", "2.5", "2.6", "2.7", "2.8", "2.9", "3.0"};
    public View.OnClickListener onClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.checkbox_commit_gamma: {
                    if (mChkCommit.isChecked())
                        commit = true;
                    else
                        commit = false;
                    break;
                }
                case R.id.fragment_gamma_color_red: {
                    if (mRed.isChecked())
                        mColor = 0;
                    break;
                }
                case R.id.fragment_gamma_color_blue: {
                    if (mBlue.isChecked())
                        mColor = 1;
                    break;
                }
                case R.id.fragment_gamma_color_green: {
                    if (mGreen.isChecked())
                        mColor = 2;
                    break;
                }
                case R.id.fragment_gammaval_btn_set: {
                    Message msg0 = messageHandler.obtainMessage(MSG_SET_GAMMAVAL);
                    Bundle b = new Bundle();
                    int index = numberPicker.getValue();
                    String val = nums[index];
                    float selectedFloat = Float.parseFloat(val);
                    b.putBoolean("commit", commit);
                    b.putInt("color", mColor);
                    b.putFloat("gammaval", selectedFloat);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_gammaval_btn_get_current: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_GAMMAVAL);
                    Bundle b = new Bundle();
                    b.putInt("color", mColor);
                    b.putInt("type", 0);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_gammaval_btn_get_startup: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_GAMMAVAL);
                    Bundle b = new Bundle();
                    b.putInt("color", mColor);
                    b.putInt("type", 1);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_gammaval_btn_get_factory: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_GAMMAVAL);
                    Bundle b = new Bundle();
                    b.putInt("color", mColor);
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

    public GammaVal() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_gammaval, container, false);
        initView();
        return rootView;
    }

    private void initView() {

        mBtnSet = rootView.findViewById(R.id.fragment_gammaval_btn_set);
        mBtnSet.setOnClickListener(onClickListener);
        mChkCommit = rootView.findViewById(R.id.checkbox_commit_gamma);
        mChkCommit.setOnClickListener(onClickListener);
        numberPicker = rootView.findViewById(R.id.fragment_gammaval_numberPicker);
        numberPicker.setMaxValue(nums.length - 1);
        numberPicker.setMinValue(0);
        numberPicker.setWrapSelectorWheel(false);
        numberPicker.setDisplayedValues(nums);
        numberPicker.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);

        mRed = rootView.findViewById(R.id.fragment_gamma_color_red);
        mRed.setOnClickListener(onClickListener);
        mBlue = rootView.findViewById(R.id.fragment_gamma_color_blue);
        mBlue.setOnClickListener(onClickListener);
        mGreen = rootView.findViewById(R.id.fragment_gamma_color_green);
        mGreen.setOnClickListener(onClickListener);

        mBtnGetCurrent = rootView.findViewById(R.id.fragment_gammaval_btn_get_current);
        mBtnGetCurrent.setOnClickListener(onClickListener);
        mBtnGetStartup = rootView.findViewById(R.id.fragment_gammaval_btn_get_startup);
        mBtnGetStartup.setOnClickListener(onClickListener);
        mBtnGetFactory = rootView.findViewById(R.id.fragment_gammaval_btn_get_factory);
        mBtnGetFactory.setOnClickListener(onClickListener);

        mTvGetGammavalCurrent = rootView.findViewById(R.id.fragment_gammaval_tv_get_current);
        mTvGetGammavalCurrent.setText("");
        mTvGetGammavalStartup = rootView.findViewById(R.id.fragment_gammaval_tv_get_startup);
        mTvGetGammavalStartup.setText("");
        mTvGetGammavalFactory = rootView.findViewById(R.id.fragment_gammaval_tv_get_factory);
        mTvGetGammavalFactory.setText("");
        mTvSetResult = rootView.findViewById(R.id.fragment_gammaval_set_result);
    }
}