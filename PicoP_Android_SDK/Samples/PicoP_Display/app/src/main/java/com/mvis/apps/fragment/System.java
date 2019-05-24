package com.mvis.apps.fragment;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.mvis.apps.R;

import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.MessageCenter.systemInfo;
import static com.mvis.apps.MessageCenter.systemStatus;
import static com.mvis.apps.Utils.MSG_GET_SYSTEMINFO;
import static com.mvis.apps.Utils.MSG_GET_SYSTEMSTATUS;

public class System extends Fragment {
    public static final int MSG_GET_SYSTEMINFO_RESPONSE_GET = 90001;
    public static final int MSG_GET_SYSTEMSTATUS_RESPONSE_GET = 90002;
    protected static final String TAG = "System";
    private static TextView mTvGetSystemStatus, mTvGetSystemState, mTvGetTemperature,
            mGetSeriesNumber, mGetFirmwareVersion, mGetElectronicsVersion;
    public static Handler systemstatusHandler = new Handler() {
        // when handler.message() called, below code will be triggered.
        public void handleMessage(Message msg) {
            //super.handleMessage(msg);
            switch (msg.what) {
                case MSG_GET_SYSTEMSTATUS_RESPONSE_GET:
                    Bundle b = msg.getData();
                    int result = b.getInt("result");
                    if (mTvGetSystemStatus != null) {
                        if (result == 0) {
                            mTvGetSystemStatus.setText("System status : " + systemStatus.getSystemState());
                            mTvGetSystemState.setText("System state : " + Integer.toString(systemStatus.getSystemFault()));
                            mTvGetTemperature.setText("Temperature : " + Float.toString(systemStatus.getTemperature()));
                        } else {
                            String STR = b.getString("STR");
                            mTvGetSystemStatus.setText("GetSystemStatus failed, error msg = " + STR);
                            mTvGetSystemState.setText("GetSystemState   failed, error msg = " + STR);
                            mTvGetTemperature.setText("GetSystemTemprature failed, error msg = " + STR);
                        }
                    }
                    break;
                case MSG_GET_SYSTEMINFO_RESPONSE_GET:
                    if (msg.getData().getInt("result") == 0) {
                        mGetSeriesNumber.setText("Serial Number : " + systemInfo.getSerialNumber());
                        mGetFirmwareVersion.setText("Firmware Version : " + systemInfo.getFirmwareVersion());
                        mGetElectronicsVersion.setText("Electronics version : " + systemInfo.getElectronicsVersion());
                    } else {
                        String STR = msg.getData().getString("STR");
                        mGetSeriesNumber.setText("Get Serial number failed, error msg = " + STR);
                        mGetFirmwareVersion.setText("Get Firmware version failed, error msg = " + STR);
                        mGetElectronicsVersion.setText("Get Electronics version failed, error msg = " + STR);
                    }
                    break;
            }
        }
    };
    public View.OnClickListener btnClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.fragment_systemstatus: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_SYSTEMSTATUS);
                    Bundle b = new Bundle();
                    msg0.setData(b);
                    messageHandler.sendMessage(msg0);
                    break;
                }
                case R.id.fragment_systemsinfo: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_SYSTEMINFO);
                    Bundle b = new Bundle();
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
    private Button mBtnSysStatus, mBtnSysInfo;

    public System() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_system, container, false);
        mBtnSysStatus = rootView.findViewById(R.id.fragment_systemstatus);
        mBtnSysStatus.setOnClickListener(btnClickListener);
        mTvGetSystemStatus = rootView.findViewById(R.id.fragment_tv_systemstatus);
        mTvGetSystemStatus.setText("");
        mTvGetSystemState = rootView.findViewById(R.id.fragment_tv_systemstate);
        mTvGetSystemState.setText("");
        mTvGetTemperature = rootView.findViewById(R.id.fragment_tv_temperature);
        mTvGetTemperature.setText("");
        mBtnSysInfo = rootView.findViewById(R.id.fragment_systemsinfo);
        mBtnSysInfo.setOnClickListener(btnClickListener);
        mGetSeriesNumber = rootView.findViewById(R.id.fragment_tv_systemsinfo_serialNumber);
        mGetSeriesNumber.setText("");
        mGetFirmwareVersion = rootView.findViewById(R.id.fragment_tv_systemsinfo_softwareVersion);
        mGetFirmwareVersion.setText("");
        mGetElectronicsVersion = rootView.findViewById(R.id.fragment_tv_systemsinfo_fpgaVersion);
        mGetElectronicsVersion.setText("");
        return rootView;
    }

}