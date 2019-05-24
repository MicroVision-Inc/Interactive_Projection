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

import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.MessageCenter.resultData;
import static com.mvis.apps.Utils.MSG_GET_SENSINGSTATE;
import static com.mvis.apps.Utils.MSG_SET_SENSINGSTATE;


public class SensingState extends Fragment {

    public static final int MSG_SENSING_STATE_RESPONSE_GET = 90001;
    public static final int MSG_SENSING_STATE_RESPONSE_SET = 90002;
    private static final String TAG = "SensingState";
    public static TextView mStateGet, mSetResult;
    public static Handler inputSensingStateHandler = new Handler() {
        public void handleMessage(Message msg) {
            //super.handleMessage(msg);
            Bundle b;
            int result;
            switch (msg.what) {
                case MSG_SENSING_STATE_RESPONSE_GET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mStateGet.setText("Sensing state is " + (resultData.getSensingstate() == 0 ? "OFF" : "ON"));
                    } else {
                        String STR = b.getString("STR");
                        mStateGet.setText("Run failed, errno = " + STR);
                    }
                    break;
                case MSG_SENSING_STATE_RESPONSE_SET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mSetResult.setText("Call success ");
                    } else {
                        String STR = b.getString("STR");
                        mSetResult.setText("Run failed, errno = " + STR);
                    }
                    break;
            }
        }
    };
    public Button mBtnGetState, mBtnSetState;
    public CheckBox mChkCommitState;
    public RadioButton mEnable, mDisable;
    private View rootView;
    private boolean commitState = false;
    private int state = 0;
    public View.OnClickListener onClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.fragment_sensing_state_radio_disabled: {
                    if (mDisable.isChecked()) {
                        state = 0;
                    }
                    break;
                }
                case R.id.fragment_sensing_state_radio_enabled: {
                    if (mEnable.isChecked()) {
                        state = 1;
                    }
                    break;
                }
                case R.id.fragment_sensing_state_btn_get_state: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_SENSINGSTATE);
                    Bundle b = new Bundle();
                    b.putInt("type", 0);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.checkbox_commit_sensing_state_channel: {
                    if (mChkCommitState.isChecked())
                        commitState = true;
                    else
                        commitState = false;
                    Log.i(TAG, "checkbox_commit_sensing_state_channel: commit " + commitState);
                    break;
                }
                case R.id.fragment_set_sensing_btn:
                    Message msg0 = messageHandler.obtainMessage(MSG_SET_SENSINGSTATE);
                    Bundle b = new Bundle();
                    b.putInt("state", state);
                    b.putBoolean("commit", commitState);
                    msg0.setData(b);
                    Log.i(TAG, "Enable Sensing ");
                    messageHandler.sendMessage(msg0);
                    break;
                default: {
                    break;
                }
            }
        }
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_sensingstate, container, false);
        mStateGet = rootView.findViewById(R.id.fragment_sensing_state_get_state);
        mStateGet.setText("click button to get sensing state");
        mEnable = rootView.findViewById(R.id.fragment_sensing_state_radio_enabled);
        mEnable.setOnClickListener(onClickListener);
        mDisable = rootView.findViewById(R.id.fragment_sensing_state_radio_disabled);
        mDisable.setOnClickListener(onClickListener);
        mBtnGetState = rootView.findViewById(R.id.fragment_sensing_state_btn_get_state);
        mBtnGetState.setOnClickListener(onClickListener);
        mChkCommitState = rootView.findViewById(R.id.checkbox_commit_sensing_state_channel);
        mChkCommitState.setOnClickListener(onClickListener);
        mBtnSetState = rootView.findViewById(R.id.fragment_set_sensing_btn);
        mBtnSetState.setOnClickListener(onClickListener);
        mSetResult = rootView.findViewById(R.id.fragment_sensing_set);
        return rootView;
    }


}