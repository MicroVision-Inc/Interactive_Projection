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
import android.widget.SeekBar;
import android.widget.TextView;

import com.mvis.apps.R;

import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.MessageCenter.resultData;
import static com.mvis.apps.Utils.MSG_GET_PHASE;
import static com.mvis.apps.Utils.MSG_SET_PHASE;

public class Phase extends Fragment {

    public static final int MSG_PHASE_RESPONSE_GET = 90001;
    public static final int MSG_PHASE_RESPONSE_SET = 90002;
    public static Handler phaseHandler = new Handler() {
        // when handler.message() called, below code will be triggered.
        public void handleMessage(Message msg) {
            //super.handleMessage(msg);
            switch (msg.what) {
                case MSG_PHASE_RESPONSE_GET:
                    Bundle b = msg.getData();
                    int result = b.getInt("result");
                    int type = b.getInt("type");
                    if (result == 0) {
                        if (type == 0) {
                            mTvGetPhaseCurrent.setText("" + resultData.getPhaseValue());
                        } else if (type == 1) {
                            mTvGetPhaseStartup.setText("" + resultData.getPhaseValue());
                        } else if (type == 2) {
                            mTvGetPhaseFactory.setText("" + resultData.getPhaseValue());
                        }
                    } else {
                        String STR = b.getString("STR");
                        if (type == 0) {
                            mTvGetPhaseCurrent.setText("GetBiPhase failed, error msg = " + STR);
                        } else if (type == 1) {
                            mTvGetPhaseStartup.setText("GetBiPhase failed, error msg = " + STR);
                        } else if (type == 2) {
                            mTvGetPhaseFactory.setText("GetBiPhase failed, error msg = " + STR);
                        }
                    }
                    break;
                case MSG_PHASE_RESPONSE_SET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mTvSetResult.setText("Call success ");
                    } else {
                        String STR = b.getString("STR");
                        mTvSetResult.setText("SetBiPhase failed, error msg = " + STR);
                    }
                    break;
            }
        }
    };
    private static TextView mTvSetPhaseCurrent, mTvGetPhaseCurrent, mTvGetPhaseStartup, mTvGetPhaseFactory, mTvSetResult;
    private SeekBar mSeekBarPhase = null;
    private Button mBtnSet, mBtnGetCurrent, mBtnGetStartup, mBtnGetFactory;
    private int mSetPhase = 0;
    private CheckBox mChkCommit;
    private boolean commit = false;
    public View.OnClickListener onClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.checkbox_commit_phase: {
                    if (mChkCommit.isChecked())
                        commit = true;
                    else
                        commit = false;
                    break;
                }
                case R.id.fragment_phase_btn_set: {
                    Message msg0 = messageHandler.obtainMessage(MSG_SET_PHASE);
                    Bundle b = new Bundle();
                    b.putBoolean("commit", commit);
                    b.putInt("phase", mSetPhase);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_phase_btn_get_current: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_PHASE);
                    Bundle b = new Bundle();
                    b.putInt("type", 0);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_phase_btn_get_startup: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_PHASE);
                    Bundle b = new Bundle();
                    b.putInt("type", 1);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_phase_btn_get_factory: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_PHASE);
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

    public Phase() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_phase, container, false);
        initView();
        return rootView;
    }

    private void initView() {
        //Textview should init before seekbar, otherwise seekbar changelistener will crash.
        mTvSetPhaseCurrent = rootView.findViewById(R.id.fragment_phase_tv_set_curr_value);
        mTvSetPhaseCurrent.setText("0");

        mSeekBarPhase = (SeekBar) rootView.findViewById(R.id.fragment_phase_seekbar_phase);
        mSeekBarPhase.setOnSeekBarChangeListener(new Phase.OnSeekBarChangeListenerImp());
        mSeekBarPhase.setEnabled(true);
        mSeekBarPhase.setMax(200);
        mSeekBarPhase.setProgress(100);

        mBtnSet = rootView.findViewById(R.id.fragment_phase_btn_set);
        mBtnSet.setOnClickListener(onClickListener);
        mChkCommit = rootView.findViewById(R.id.checkbox_commit_phase);
        mChkCommit.setOnClickListener(onClickListener);

        mTvSetResult = rootView.findViewById(R.id.fragment_set_phase_result);

        mBtnGetCurrent = rootView.findViewById(R.id.fragment_phase_btn_get_current);
        mBtnGetCurrent.setOnClickListener(onClickListener);
        mBtnGetStartup = rootView.findViewById(R.id.fragment_phase_btn_get_startup);
        mBtnGetStartup.setOnClickListener(onClickListener);
        mBtnGetFactory = rootView.findViewById(R.id.fragment_phase_btn_get_factory);
        mBtnGetFactory.setOnClickListener(onClickListener);

        mTvGetPhaseCurrent = rootView.findViewById(R.id.fragment_phase_tv_get_current);
        mTvGetPhaseCurrent.setText("");
        mTvGetPhaseStartup = rootView.findViewById(R.id.fragment_phase_tv_get_startup);
        mTvGetPhaseStartup.setText("");
        mTvGetPhaseFactory = rootView.findViewById(R.id.fragment_phase_tv_get_factory);
        mTvGetPhaseFactory.setText("");

    }

    private class OnSeekBarChangeListenerImp implements
            SeekBar.OnSeekBarChangeListener {
        // progress
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            mTvSetPhaseCurrent.setText("" + (progress - 100));
        }

        // start
        public void onStartTrackingTouch(SeekBar seekBar) {
        }

        // stop
        public void onStopTrackingTouch(SeekBar seekBar) {

            mSetPhase = (seekBar.getProgress() - 100);
        }
    }
}