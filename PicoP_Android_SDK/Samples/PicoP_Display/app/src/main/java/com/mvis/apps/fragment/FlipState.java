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

import picop.interfaces.def.PicoP_FlipStateE;

import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.MessageCenter.resultData;
import static com.mvis.apps.Utils.MSG_GET_FLIPSTATE;
import static com.mvis.apps.Utils.MSG_SET_FLIPSTATE;


public class FlipState extends Fragment {

    public static final int MSG_FLIP_STATE_RESPONSE_SET = 90001;
    public static final int MSG_FLIP_STATE_RESPONSE_GET = 90002;
    private static final String TAG = "FlipState";
    private static TextView mSetFlipStateResult, mTvGetCurrent, mTvGetStartup, mTvGetFactory;
    public static Handler flipStateHandler = new Handler() {
        public void handleMessage(Message msg) {
            //super.handleMessage(msg);
            Bundle b;
            int result;

            switch (msg.what) {
                case MSG_FLIP_STATE_RESPONSE_SET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mSetFlipStateResult.setText("SetFlipState success ");
                    } else {
                        String STR = b.getString("STR");
                        mSetFlipStateResult.setText("SetFlipState failed, error msg = " + STR);
                    }
                    break;
                case MSG_FLIP_STATE_RESPONSE_GET:
                    PicoP_FlipStateE state = PicoP_FlipStateE.eFLIP_NEITHER;
                    b = msg.getData();
                    result = b.getInt("result");
                    int type = b.getInt("type");
                    if (result == 0) {
                        if (type == 0) {
                            mTvGetCurrent.setText("Flip State is: " + state.intToEnum(resultData.getFlipState()));
                        } else if (type == 1) {
                            mTvGetStartup.setText("Flip State is: " + state.intToEnum(resultData.getFlipState()));
                        } else if (type == 2) {
                            mTvGetFactory.setText("Flip State is: " + state.intToEnum(resultData.getFlipState()));
                        }
                    } else {
                        String STR = b.getString("STR");
                        if (type == 0) {
                            mTvGetCurrent.setText("GetFlipState failed, error msg = " + STR);
                        } else if (type == 1) {
                            mTvGetStartup.setText("GetFlipState failed, error msg = " + STR);
                        } else if (type == 2) {
                            mTvGetFactory.setText("GetFlipState failed, error msg = " + STR);
                        }
                    }
                    break;
            }
        }
    };
    public RadioButton mNeither, mHoriz, mVertical, mBoth;
    private View rootView;
    private Button mBtnSetFlipState, mBtnGetCurrent, mBtnGetStartup, mBtnGetFactory;
    private CheckBox mChkCommitFlipState;
    private boolean commitFlipState = false;
    private int flipState = 0;
    /* { button code */
    public View.OnClickListener onClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.checkbox_commit_flip_state: {
                    if (mChkCommitFlipState.isChecked())
                        commitFlipState = true;
                    else
                        commitFlipState = false;
                    break;
                }
                case R.id.fragment_flip_neither: {
                    if (mNeither.isChecked())
                        flipState = 0;
                    break;
                }
                case R.id.fragment_flip_horizontal: {
                    if (mHoriz.isChecked())
                        flipState = 1;
                    break;
                }
                case R.id.fragment_flip_vertical: {
                    if (mVertical.isChecked())
                        flipState = 2;
                    break;
                }
                case R.id.fragment_flip_both: {
                    if (mBoth.isChecked())
                        flipState = 3;
                    break;
                }
                case R.id.fragment_set_flip_state_btn:
                    Message msg1 = messageHandler.obtainMessage(MSG_SET_FLIPSTATE);
                    Bundle b1 = new Bundle();
                    b1.putInt("flipstate", flipState);
                    b1.putBoolean("commit", commitFlipState);
                    msg1.setData(b1);
                    messageHandler.sendMessage(msg1);
                    break;
                case R.id.fragment_flip_state_btn_get_current:
                    Message msg2 = messageHandler.obtainMessage(MSG_GET_FLIPSTATE);
                    Bundle b2 = new Bundle();
                    b2.putInt("type", 0);
                    msg2.setData(b2);
                    messageHandler.sendMessage(msg2);
                    break;
                case R.id.fragment_flip_state_btn_get_startup:
                    Message msg3 = messageHandler.obtainMessage(MSG_GET_FLIPSTATE);
                    Bundle b3 = new Bundle();
                    b3.putInt("type", 1);
                    msg3.setData(b3);
                    messageHandler.sendMessage(msg3);
                    break;
                case R.id.fragment_flip_state_btn_get_factory:
                    Message msg4 = messageHandler.obtainMessage(MSG_GET_FLIPSTATE);
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
        rootView = inflater.inflate(R.layout.fragment_flipstate, container, false);
        initView();
        return rootView;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

    }

    private void initView() {

        mNeither = rootView.findViewById(R.id.fragment_flip_neither);
        mNeither.setOnClickListener(onClickListener);
        mHoriz = rootView.findViewById(R.id.fragment_flip_horizontal);
        mHoriz.setOnClickListener(onClickListener);
        mVertical = rootView.findViewById(R.id.fragment_flip_vertical);
        mVertical.setOnClickListener(onClickListener);
        mBoth = rootView.findViewById(R.id.fragment_flip_both);
        mBoth.setOnClickListener(onClickListener);


        mBtnSetFlipState = rootView.findViewById(R.id.fragment_set_flip_state_btn);
        mBtnSetFlipState.setOnClickListener(onClickListener);
        mBtnGetCurrent = rootView.findViewById(R.id.fragment_flip_state_btn_get_current);
        mBtnGetCurrent.setOnClickListener(onClickListener);
        mBtnGetStartup = rootView.findViewById(R.id.fragment_flip_state_btn_get_startup);
        mBtnGetStartup.setOnClickListener(onClickListener);
        mBtnGetFactory = rootView.findViewById(R.id.fragment_flip_state_btn_get_factory);
        mBtnGetFactory.setOnClickListener(onClickListener);

        mChkCommitFlipState = rootView.findViewById(R.id.checkbox_commit_flip_state);
        mChkCommitFlipState.setOnClickListener(onClickListener);

        mSetFlipStateResult = rootView.findViewById(R.id.fragment_set_flip_state_result);
        mTvGetCurrent = rootView.findViewById(R.id.fragment_flip_state_tv_get_current);
        mTvGetStartup = rootView.findViewById(R.id.fragment_flip_state_tv_get_startup);
        mTvGetFactory = rootView.findViewById(R.id.fragment_flip_state_tv_get_factory);

    }


}
