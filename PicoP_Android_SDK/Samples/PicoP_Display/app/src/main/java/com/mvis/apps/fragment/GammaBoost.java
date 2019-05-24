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
import static com.mvis.apps.Utils.MSG_SET_GAMMABOOSTMODE;
import static com.mvis.apps.Utils.MSG_GET_GAMMABOOSTMODE;


public class GammaBoost extends Fragment {
    public static final int MSG_GAMMABOOST_RESPONSE_GET = 90001;
    public static final int MSG_GAMMABOOST_RESPONSE_SET = 90002;
    private static final String TAG = "GammaBoost";
    private static TextView mTvSetResult, mTvGetGammaBoostCurrent, mTvGetGammaBoostStartup, mTvGetGammaBoostFactory;
    public static Handler gammaBoostHandler = new Handler() {
        // when handler.message() called, below code will be triggered.
        public void handleMessage(Message msg) {
            //super.handleMessage(msg);
            switch (msg.what) {
                case MSG_GAMMABOOST_RESPONSE_GET:
                    Bundle b = msg.getData();
                    int result = b.getInt("result");
                    int type = b.getInt("type");
                    if (result == 0) {
                        if (type == 0) {
                            mTvGetGammaBoostCurrent.setText("" + modeToString(resultData.getGammaBoostMode()));
                        } else if (type == 1) {
                            mTvGetGammaBoostStartup.setText("" + modeToString(resultData.getGammaBoostMode()));
                        } else if (type == 2) {
                            mTvGetGammaBoostFactory.setText("" + modeToString(resultData.getGammaBoostMode()));
                        }
                    } else {
                        String STR = b.getString("STR");
                        if (type == 0) {
                            mTvGetGammaBoostCurrent.setText("GetGammaBoost failed, error msg = " + STR);
                        } else if (type == 1) {
                            mTvGetGammaBoostStartup.setText("GetGammaBoost failed, error msg = " + STR);
                        } else if (type == 2) {
                            mTvGetGammaBoostFactory.setText("GetGammaBoost failed, error msg = " + STR);
                        }
                    }
                    break;
                case MSG_GAMMABOOST_RESPONSE_SET:
                    b = msg.getData();
                    result = b.getInt("result");
                    if (result == 0) {
                        mTvSetResult.setText("Call success ");
                    } else {
                        String STR = b.getString("STR");
                        mTvSetResult.setText("SetGammaBoost failed, error msg = " + STR);
                    }
                    break;
            }
        }
    };
    private Button mBtnSet, mBtnGetCurrent, mBtnGetStartup, mBtnGetFactory;
    private int mSetMode = 0, mColor = 0;
    private View rootView;
    private RadioButton mRed, mBlue, mGreen, mModeNone, mMode30;
    private CheckBox mChkCommit;
    private boolean commit = false;
    public View.OnClickListener onClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.checkbox_commit_gammaboost: {
                    if (mChkCommit.isChecked())
                        commit = true;
                    else
                        commit = false;
                    break;
                }
                case R.id.fragment_gammaboost_color_red: {
                    if (mRed.isChecked())
                        mColor = 0;
                    break;
                }
                case R.id.fragment_gammaboost_color_blue: {
                    if (mBlue.isChecked())
                        mColor = 1;
                    break;
                }
                case R.id.fragment_gammaboost_color_green: {
                    if (mGreen.isChecked())
                        mColor = 2;
                    break;
                }
                case R.id.fragment_gammaboost_mode1: {
                    if (mModeNone.isChecked())
                        mSetMode = 0;
                    break;
                }
                case R.id.fragment_gammaboost_mode2: {
                    if (mMode30.isChecked())
                        mSetMode = 1;
                    break;
                }
                case R.id.fragment_gammaboost_set: {
                    Message msg0 = messageHandler.obtainMessage(MSG_SET_GAMMABOOSTMODE);
                    Bundle b = new Bundle();
                    b.putBoolean("commit", commit);
                    b.putInt("color", mColor);
                    b.putInt("mode", mSetMode);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_gammaboost_btn_get_current: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_GAMMABOOSTMODE);
                    Bundle b = new Bundle();
                    b.putInt("color", mColor);
                    b.putInt("type", 0);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_gammaboost_btn_get_startup: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_GAMMABOOSTMODE);
                    Bundle b = new Bundle();
                    b.putInt("color", mColor);
                    b.putInt("type", 1);
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_gammaboost_btn_get_factory: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_GAMMABOOSTMODE);
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

    public GammaBoost() {
    }

    private static String modeToString(int mode) {
        String ret = "";
        switch (mode) {
            case 0:
                ret = "BOOST_MODE_NONE";
                break;
            case 1:
                ret = "BOOST_MODE_VIRTUAL_LUMENS_30PCT";
                break;
            default:
                Log.w(TAG, "Wrong enum value");
                break;
        }
        return ret;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_gammaboostmode, container, false);
        initView();
        return rootView;
    }

    private void initView() {

        mBtnSet = rootView.findViewById(R.id.fragment_gammaboost_set);
        mBtnSet.setOnClickListener(onClickListener);

        mRed = rootView.findViewById(R.id.fragment_gammaboost_color_red);
        mRed.setOnClickListener(onClickListener);
        mBlue = rootView.findViewById(R.id.fragment_gammaboost_color_blue);
        mBlue.setOnClickListener(onClickListener);
        mGreen = rootView.findViewById(R.id.fragment_gammaboost_color_green);
        mGreen.setOnClickListener(onClickListener);
        mModeNone = rootView.findViewById(R.id.fragment_gammaboost_mode1);
        mModeNone.setOnClickListener(onClickListener);
        mMode30 = rootView.findViewById(R.id.fragment_gammaboost_mode2);
        mMode30.setOnClickListener(onClickListener);
        mChkCommit = rootView.findViewById(R.id.checkbox_commit_gammaboost);
        mChkCommit.setOnClickListener(onClickListener);

        mBtnGetCurrent = rootView.findViewById(R.id.fragment_gammaboost_btn_get_current);
        mBtnGetCurrent.setOnClickListener(onClickListener);
        mBtnGetStartup = rootView.findViewById(R.id.fragment_gammaboost_btn_get_startup);
        mBtnGetStartup.setOnClickListener(onClickListener);
        mBtnGetFactory = rootView.findViewById(R.id.fragment_gammaboost_btn_get_factory);
        mBtnGetFactory.setOnClickListener(onClickListener);


        mTvGetGammaBoostCurrent = rootView.findViewById(R.id.fragment_gammaboost_tv_get_current);
        mTvGetGammaBoostCurrent.setText("");
        mTvGetGammaBoostStartup = rootView.findViewById(R.id.fragment_gammaboost_tv_get_startup);
        mTvGetGammaBoostStartup.setText("");
        mTvGetGammaBoostFactory = rootView.findViewById(R.id.fragment_gammaboost_tv_get_factory);
        mTvGetGammaBoostFactory.setText("");
        mTvSetResult = rootView.findViewById(R.id.fragment_gammaboost_set_result);
    }
}
