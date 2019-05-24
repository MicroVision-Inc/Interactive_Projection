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

import picop.interfaces.def.PicoP_MicroWeaveE;

import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.MessageCenter.resultData;
import static com.mvis.apps.Utils.MSG_GET_FLIPSTATE;
import static com.mvis.apps.Utils.MSG_GET_MICROWEAVE_MODE;
import static com.mvis.apps.Utils.MSG_SET_FLIPSTATE;
import static com.mvis.apps.Utils.MSG_SET_MICROWEAVE_MODE;


public class MicroweaveMode extends Fragment {

    public static final int MSG_MICROWEAVE_MODE_RESPONSE_SET = 90001;
    public static final int MSG_MICROWEAVE_MODE_RESPONSE_GET = 90002;
    private static final String TAG = "MicroweaveMode";
    private static TextView mSetModeResult, mTvGetCurrent, mTvGetStartup, mTvGetFactory;
    public static Handler microweaveModeHandler = new Handler() {
        public void handleMessage(Message msg) {
            //super.handleMessage(msg);
            Bundle b;
            int result;

            switch (msg.what) {
                case MSG_MICROWEAVE_MODE_RESPONSE_SET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mSetModeResult.setText("Set Microweave Mode: Success ");
                    } else {
                        String STR = b.getString("STR");
                        mSetModeResult.setText("Set Microweave Mode failed, error msg = " + STR);
                    }
                    break;
                case MSG_MICROWEAVE_MODE_RESPONSE_GET:
                    PicoP_MicroWeaveE state = PicoP_MicroWeaveE.eMICROWEAVE_OFF;
                    b = msg.getData();
                    result = b.getInt("result");
                    int type = b.getInt("type");
                    if (result == 0) {
                        if (type == 0) {
                            mTvGetCurrent.setText("Microweave Mode  is: " + state.intToEnum(resultData.getMicroweaveMode()));
                        } else if (type == 1) {
                            mTvGetStartup.setText("Microweave Mode  is: " + state.intToEnum(resultData.getMicroweaveMode()));
                        } else if (type == 2) {
                            mTvGetFactory.setText("Microweave Mode  is: " + state.intToEnum(resultData.getMicroweaveMode()));
                        }
                    } else {
                        String STR = b.getString("STR");
                        if (type == 0) {
                            mTvGetCurrent.setText("GetMicroweaveMode  failed, error msg = " + STR);
                        } else if (type == 1) {
                            mTvGetStartup.setText("GetMicroweaveMode failed, error msg = " + STR);
                        } else if (type == 2) {
                            mTvGetFactory.setText("GetMicroweaveMode failed, error msg = " + STR);
                        }
                    }
                    break;
            }
        }
    };
    public RadioButton mOff, mOn;
    private View rootView;
    private Button mBtnSetMode, mBtnGetCurrent, mBtnGetStartup, mBtnGetFactory;
    private CheckBox mChkCommit;
    private boolean commitMicroweaveMode = false;
    private int microweaveMode = 0;
    /* { button code */
    public View.OnClickListener onClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.checkbox_commit_microweave: {
                    if (mChkCommit.isChecked())
                        commitMicroweaveMode = true;
                    else
                        commitMicroweaveMode = false;
                    break;
                }
                case R.id.fragment_microweave_off: {
                    if (mOff.isChecked())
                        microweaveMode = 0;
                    break;
                }
                case R.id.fragment_microweave_on: {
                    if (mOn.isChecked())
                        microweaveMode = 1;
                    break;
                }
                case R.id.fragment_set_microweave_btn:
                    Message msg1 = messageHandler.obtainMessage(MSG_SET_MICROWEAVE_MODE);
                    Bundle b1 = new Bundle();
                    b1.putInt("mode", microweaveMode);
                    b1.putBoolean("commit", commitMicroweaveMode);
                    msg1.setData(b1);
                    messageHandler.sendMessage(msg1);
                    break;
                case R.id.fragment_microweave_btn_get_current:
                    Message msg2 = messageHandler.obtainMessage(MSG_GET_MICROWEAVE_MODE);
                    Bundle b2 = new Bundle();
                    b2.putInt("type", 0);
                    msg2.setData(b2);
                    messageHandler.sendMessage(msg2);
                    break;
                case R.id.fragment_microweave_btn_get_startup:
                    Message msg3 = messageHandler.obtainMessage(MSG_GET_MICROWEAVE_MODE);
                    Bundle b3 = new Bundle();
                    b3.putInt("type", 1);
                    msg3.setData(b3);
                    messageHandler.sendMessage(msg3);
                    break;
                case R.id.fragment_microweave_btn_get_factory:
                    Message msg4 = messageHandler.obtainMessage(MSG_GET_MICROWEAVE_MODE);
                    Bundle b4 = new Bundle();
                    b4.putInt("type", 2);
                    msg4.setData(b4);
                    messageHandler.sendMessage(msg4);
                    break;
                default:
                    Log.i(TAG, "why you reach here?");
                    break;
            }
        }
    };

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        rootView = inflater.inflate(R.layout.fragment_microweave, container, false);
        initView();
        return rootView;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

    }

    private void initView() {

        mOff = rootView.findViewById(R.id.fragment_microweave_off);
        mOff.setOnClickListener(onClickListener);
        mOn = rootView.findViewById(R.id.fragment_microweave_on);
        mOn.setOnClickListener(onClickListener);


        mBtnSetMode = rootView.findViewById(R.id.fragment_set_microweave_btn);
        mBtnSetMode.setOnClickListener(onClickListener);
        mBtnGetCurrent = rootView.findViewById(R.id.fragment_microweave_btn_get_current);
        mBtnGetCurrent.setOnClickListener(onClickListener);
        mBtnGetStartup = rootView.findViewById(R.id.fragment_microweave_btn_get_startup);
        mBtnGetStartup.setOnClickListener(onClickListener);
        mBtnGetFactory = rootView.findViewById(R.id.fragment_microweave_btn_get_factory);
        mBtnGetFactory.setOnClickListener(onClickListener);

        mChkCommit = rootView.findViewById(R.id.checkbox_commit_microweave);
        mChkCommit.setOnClickListener(onClickListener);

        mSetModeResult = rootView.findViewById(R.id.fragment_set_microweave_result);
        mTvGetCurrent = rootView.findViewById(R.id.fragment_microweave_tv_get_current);
        mTvGetStartup = rootView.findViewById(R.id.fragment_microweave_tv_get_startup);
        mTvGetFactory = rootView.findViewById(R.id.fragment_microweave_tv_get_factory);

    }


}
