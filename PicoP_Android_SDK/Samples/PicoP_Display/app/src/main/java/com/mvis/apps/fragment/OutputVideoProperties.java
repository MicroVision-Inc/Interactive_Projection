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
import static com.mvis.apps.MessageCenter.resultData;
import static com.mvis.apps.Utils.MSG_GET_OUTPUTVIDEOPROPERTIES;

public class OutputVideoProperties extends Fragment {
    public static final int MSG_GET_OUTPUTVIDEOPROPERTIES_RESPONSE_GET = 90001;
    protected static final String TAG = "OutputVideoProperties";
    private static TextView mTvGetPropertiesFrameRate, mTvGetVerticalResolution, mTvGetHorizontalResolution;
    public static Handler outputvideopropertiesHandler = new Handler() {
        // when handler.message() called, below code will be triggered.
        public void handleMessage(Message msg) {
            //super.handleMessage(msg);
            switch (msg.what) {
                case MSG_GET_OUTPUTVIDEOPROPERTIES_RESPONSE_GET:
                    Bundle b = msg.getData();
                    int result = b.getInt("result");
                    if (result == 0) {
                        mTvGetPropertiesFrameRate.setText("FrameRate:" + resultData.getOutputFrameRate());
                        mTvGetVerticalResolution.setText("Vertical Resolution:" + resultData.getVerticalRes());
                        mTvGetHorizontalResolution.setText("Horizontal Resolution:" + resultData.getHorizontalRes());
                    } else {
                        String STR = b.getString("STR");
                        mTvGetPropertiesFrameRate.setText("GetOutputVideoProperties failed, error msg = " + STR);

                    }
                    break;
            }
        }
    };
    public View.OnClickListener btnClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.fragment_properties: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_OUTPUTVIDEOPROPERTIES);
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
    private Button button1;

    public OutputVideoProperties() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_outputvideoproperties, container, false);
        button1 = rootView.findViewById(R.id.fragment_properties);
        button1.setOnClickListener(btnClickListener);

        mTvGetPropertiesFrameRate = rootView.findViewById(R.id.fragment_op_properties_tv_framerate);
        mTvGetPropertiesFrameRate.setText("");
        mTvGetVerticalResolution = rootView.findViewById(R.id.fragment_op_properties_tv_vertical);
        mTvGetVerticalResolution.setText("");
        mTvGetHorizontalResolution = rootView.findViewById(R.id.fragment_op_properties_tv_horizontal);
        mTvGetHorizontalResolution.setText("");
        return rootView;
    }

}
