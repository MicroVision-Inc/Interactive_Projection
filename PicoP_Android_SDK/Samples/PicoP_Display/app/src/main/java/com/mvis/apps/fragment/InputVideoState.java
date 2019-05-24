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

import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.MessageCenter.resultData;
import static com.mvis.apps.Utils.MSG_GET_INPUTVIDEOSTATE;
import static com.mvis.apps.Utils.MSG_SET_INPUTVIDEOSTATE;

public class InputVideoState extends Fragment {
    public static final int MSG_INPUTVIDEOSTATE_RESPONSE_GET = 90001;
    public static final int MSG_INPUTVIDEOSTATE_RESPONSE_SET = 90002;
    protected static final String TAG = "InputVideoState";
    private static TextView mTvGet, mTvSetResult;
    public static Handler inputvideostateHandler = new Handler() {
        public void handleMessage(Message msg) {
            int result;
            Bundle b;
            //super.handleMessage(msg);
            switch (msg.what) {
                case MSG_INPUTVIDEOSTATE_RESPONSE_GET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mTvGet.setText("Current InputVideoState is : " + (resultData.getInputstate() == 1 ? "Enabled" : "Disabled"));
                    } else {
                        String STR = b.getString("STR");
                        mTvGet.setText("GetInputVideoState failed, error msg = " + STR);
                    }
                    break;
                case MSG_INPUTVIDEOSTATE_RESPONSE_SET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mTvSetResult.setText("Call success ");
                    } else {
                        String STR = b.getString("STR");
                        mTvSetResult.setText("SetInputVideoState failed, error msg = " + STR);
                    }
                    break;
            }

        }
    };
    public RadioButton mEnable, mDisable;
    private View rootView;
    private Button mBtnSet, mBtnDisable, mBtnGet;
    private CheckBox mChkCommitState;
    private boolean commitState = false;
    private int state = 0;
    public View.OnClickListener onClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.fragment_input_video_state_radio_disabled: {
                    if (mDisable.isChecked()) {
                        state = 0;
                    }
                    break;
                }
                case R.id.fragment_input_video_state_radio_enabled: {
                    if (mEnable.isChecked()) {
                        state = 1;
                    }
                    break;
                }
                case R.id.fragment_set_ip_video_state_btn: {
                    Message msg0 = messageHandler.obtainMessage(MSG_SET_INPUTVIDEOSTATE);
                    Bundle b = new Bundle();
                    b.putInt("state", state);
                    b.putBoolean("commit", commitState);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_inputvideostate_btn_get_state: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_INPUTVIDEOSTATE);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.checkbox_commit_video_state: {
                    if (mChkCommitState.isChecked())
                        commitState = true;
                    else
                        commitState = false;
                    break;
                }
                default: {
                    break;
                }
            }
        }
    };

    public InputVideoState() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_inputvideostate, container, false);
        mTvGet = rootView.findViewById(R.id.fragment_inputvideostate_tv_get_state);
        mTvGet.setText("Click button to get input video state");
        mBtnSet = rootView.findViewById(R.id.fragment_set_ip_video_state_btn);
        mBtnSet.setOnClickListener(onClickListener);
        mBtnGet = rootView.findViewById(R.id.fragment_inputvideostate_btn_get_state);
        mBtnGet.setOnClickListener(onClickListener);
        mChkCommitState = rootView.findViewById(R.id.checkbox_commit_video_state);
        mChkCommitState.setOnClickListener(onClickListener);

        mEnable = rootView.findViewById(R.id.fragment_input_video_state_radio_enabled);
        mEnable.setOnClickListener(onClickListener);
        mDisable = rootView.findViewById(R.id.fragment_input_video_state_radio_disabled);
        mDisable.setOnClickListener(onClickListener);
        mTvSetResult = rootView.findViewById(R.id.fragment_ip_video_state_set_result);
        return rootView;
    }

}
