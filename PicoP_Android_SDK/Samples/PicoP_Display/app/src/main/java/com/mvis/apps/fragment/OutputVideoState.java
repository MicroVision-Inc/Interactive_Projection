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
import static com.mvis.apps.Utils.MSG_GET_OUTPUTVIDEOSTATE;
import static com.mvis.apps.Utils.MSG_SET_OUTPUTVIDEOSTATE;


public class OutputVideoState extends Fragment {
    public static final int MSG_OUTPUTVIDEOSTATE_RESPONSE_GET = 90001;
    public static final int MSG_OUTPUTVIDEOSTATE_RESPONSE_SET = 90002;
    protected static final String TAG = "OutputVideoState";
    private static TextView mTvSetResult, mTextGetOutputState;
    public static Handler outputvideostateHandler = new Handler() {
        public void handleMessage(Message msg) {
            //super.handleMessage(msg);
            int result;
            Bundle b;
            switch (msg.what) {
                case MSG_OUTPUTVIDEOSTATE_RESPONSE_GET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mTextGetOutputState.setText("Current OutputVideoState is : " + (resultData.getOutputstate() == 1 ? "Enabled" : "Disabled"));
                    } else {
                        String STR = b.getString("STR");
                        mTextGetOutputState.setText("SetOutputVideoState failed, error msg = " + STR);
                    }
                    break;
                case MSG_OUTPUTVIDEOSTATE_RESPONSE_SET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mTvSetResult.setText("Call success ");
                    } else {
                        String STR = b.getString("STR");
                        mTvSetResult.setText("GetOutputVideoState failed, error msg = " + STR);
                    }
                    break;
            }
        }
    };
    public RadioButton mEnable, mDisable;
    private View rootView;
    private Button mBtnSet, mBtnGet;
    private CheckBox mChkCommitState;
    private boolean commitState = false;
    private int state = 0;
    public View.OnClickListener onClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.fragment_output_video_state_radio_disabled: {
                    if (mDisable.isChecked()) {
                        state = 0;
                    }
                    break;
                }
                case R.id.fragment_output_video_state_radio_enabled: {
                    if (mEnable.isChecked()) {
                        state = 1;
                    }
                    break;
                }
                case R.id.fragment_set_op_video_state_btn: {
                    Message msg0 = messageHandler.obtainMessage(MSG_SET_OUTPUTVIDEOSTATE);
                    Bundle b = new Bundle();
                    b.putInt("state", state);
                    b.putBoolean("commit", commitState);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_outputvideostate_btn_get_state: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_OUTPUTVIDEOSTATE);
                    Bundle b = new Bundle();
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.checkbox_commit_output_video_state: {
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

    public OutputVideoState() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_outputvideostate, container, false);
        initView();
        return rootView;
    }

    private void initView() {
        mTextGetOutputState = rootView.findViewById(R.id.fragment_outputvideostate_text_get_state);
        mTextGetOutputState.setText("");
        mBtnSet = rootView.findViewById(R.id.fragment_set_op_video_state_btn);
        mBtnSet.setOnClickListener(onClickListener);
        mBtnGet = rootView.findViewById(R.id.fragment_outputvideostate_btn_get_state);
        mBtnGet.setOnClickListener(onClickListener);
        mChkCommitState = rootView.findViewById(R.id.checkbox_commit_output_video_state);
        mChkCommitState.setOnClickListener(onClickListener);

        mEnable = rootView.findViewById(R.id.fragment_output_video_state_radio_enabled);
        mEnable.setOnClickListener(onClickListener);
        mDisable = rootView.findViewById(R.id.fragment_output_video_state_radio_disabled);
        mDisable.setOnClickListener(onClickListener);
        mTvSetResult = rootView.findViewById(R.id.fragment_op_video_state_set_result);

    }

}
