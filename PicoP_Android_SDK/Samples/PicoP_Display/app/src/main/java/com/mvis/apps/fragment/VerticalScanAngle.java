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
import android.widget.TextView;

import com.mvis.apps.R;

import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.MessageCenter.resultData;
import static com.mvis.apps.Utils.MSG_GET_VERTICAL_SCAN_ANGLE;
import static com.mvis.apps.Utils.MSG_SET_VERTICAL_SCAN_ANGLE;

public class VerticalScanAngle extends Fragment {
    public static final int MSG_VERTICAL_SCAN_ANGLE_RESPONSE_GET = 90001;
    public static final int MSG_VERTICAL_SCAN_ANGLE_RESPONSE_SET = 90002;
    private static final String TAG = "VerticalScanAngle";
    private static TextView mTvSetResult, mTvGetCurrent, mTvGetStartup, mTvGetFactory;
    public static Handler verticalScanAngleHandler = new Handler() {
        // when handler.message() called, below code will be triggered.
        public void handleMessage(Message msg) {
            //super.handleMessage(msg);
            switch (msg.what) {
                case MSG_VERTICAL_SCAN_ANGLE_RESPONSE_GET:
                    Bundle b = msg.getData();
                    int result = b.getInt("result");
                    int type = b.getInt("type");
                    if (result == 0) {
                        if (type == 0) {
                            mTvGetCurrent.setText("VerticalScanAngle : " + resultData.getVerticalScanAngle());
                        } else if (type == 1) {
                            mTvGetStartup.setText("VerticalScanAngle : " + resultData.getVerticalScanAngle());
                        } else if (type == 2) {
                            mTvGetFactory.setText("VerticalScanAngle : " + resultData.getVerticalScanAngle());
                        }
                    } else {
                        String STR = b.getString("STR");
                        if (type == 0) {
                            mTvGetCurrent.setText("GetVerticalScanAngle failed, error msg = " + STR);
                        } else if (type == 1) {
                            mTvGetStartup.setText("GetVerticalScanAngle failed, error msg = " + STR);
                        } else if (type == 2) {
                            mTvGetFactory.setText("GetVerticalScanAngle failed, error msg = " + STR);
                        }
                    }
                    break;
                case MSG_VERTICAL_SCAN_ANGLE_RESPONSE_SET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mTvSetResult.setText("SetVerticalScanAngle success ");
                    } else {
                        String STR = b.getString("STR");
                        mTvSetResult.setText("SetVerticalScanAngle failed, error msg = " + STR);
                    }
                    break;
            }
        }
    };
    NumberPicker numberPicker1, numberPicker2;
    private Button mBtnSet, mBtnGetCurrent, mBtnGetStartup, mBtnGetFactory;
    private View rootView;
    private CheckBox mChkCommit;
    private boolean commit = false;
    public View.OnClickListener onClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.checkbox_commit_verticalscanangle: {
                    if (mChkCommit.isChecked())
                        commit = true;
                    else
                        commit = false;
                    break;
                }
                case R.id.fragment_verticalscanangle_btn_set: {
                    Message msg0 = messageHandler.obtainMessage(MSG_SET_VERTICAL_SCAN_ANGLE);
                    Bundle b = new Bundle();
                    int val1 = numberPicker1.getValue();
                    int val2 = numberPicker2.getValue();
                    String val = "" + val1 + "." + val2 + "f";
                    float selectedFloat = Float.parseFloat(val);
                    b.putBoolean("commit", commit);
                    b.putFloat("scanangle", selectedFloat);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_verticalscanangle_btn_get_current: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_VERTICAL_SCAN_ANGLE);
                    Bundle b = new Bundle();
                    b.putInt("type", 0);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_verticalscanangle_btn_get_startup: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_VERTICAL_SCAN_ANGLE);
                    Bundle b = new Bundle();
                    b.putInt("type", 1);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_verticalscanangle_btn_get_factory: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_VERTICAL_SCAN_ANGLE);
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

    public VerticalScanAngle() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_verticalscanangle, container, false);
        initView();
        return rootView;
    }

    private void initView() {

        mBtnSet = rootView.findViewById(R.id.fragment_verticalscanangle_btn_set);
        mBtnSet.setOnClickListener(onClickListener);
        mChkCommit = rootView.findViewById(R.id.checkbox_commit_verticalscanangle);
        mChkCommit.setOnClickListener(onClickListener);
        numberPicker1 = rootView.findViewById(R.id.fragment_verticalscanangle_numberPicker1);
        numberPicker1.setMaxValue(100);
        numberPicker1.setMinValue(0);
        numberPicker1.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);

        numberPicker2 = rootView.findViewById(R.id.fragment_verticalscanangle_numberPicker2);
        numberPicker2.setMaxValue(9);
        numberPicker2.setMinValue(0);
        numberPicker2.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);


        mBtnGetCurrent = rootView.findViewById(R.id.fragment_verticalscanangle_btn_get_current);
        mBtnGetCurrent.setOnClickListener(onClickListener);
        mBtnGetStartup = rootView.findViewById(R.id.fragment_verticalscanangle_btn_get_startup);
        mBtnGetStartup.setOnClickListener(onClickListener);
        mBtnGetFactory = rootView.findViewById(R.id.fragment_verticalscanangle_btn_get_factory);
        mBtnGetFactory.setOnClickListener(onClickListener);

        mTvGetCurrent = rootView.findViewById(R.id.fragment_verticalscanangle_tv_get_current);
        mTvGetCurrent.setText("");
        mTvGetStartup = rootView.findViewById(R.id.fragment_verticalscanangle_tv_get_startup);
        mTvGetStartup.setText("");
        mTvGetFactory = rootView.findViewById(R.id.fragment_verticalscanangle_tv_get_factory);
        mTvGetFactory.setText("");
        mTvSetResult = rootView.findViewById(R.id.fragment_verticalscanangle_set_result);
        numberPicker1.setValue(100);
    }
}