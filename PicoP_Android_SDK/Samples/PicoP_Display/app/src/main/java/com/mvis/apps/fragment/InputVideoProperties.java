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
import static com.mvis.apps.Utils.MSG_GET_INPUTVIDEOPROPERTIES;

public class InputVideoProperties extends Fragment {
    public static final int MSG_GET_INPUTVIDEOPROPERTIES_RESPONSE_GET = 90001;
    protected static final String TAG = "InputVideoProperties";
    private static TextView mTvGetPropertiesFrameRate, mTvGetPropertiesLines, mTvGetPropertieshorizontalPixels;
    public static Handler inputvideopropertiesHandler = new Handler() {
        // when handler.message() called, below code will be triggered.
        public void handleMessage(Message msg) {
            //super.handleMessage(msg);
            switch (msg.what) {
                case MSG_GET_INPUTVIDEOPROPERTIES_RESPONSE_GET:
                    Bundle b = msg.getData();
                    int result = b.getInt("result");
                    if (result == 0) {
                        mTvGetPropertiesFrameRate.setText("FrameRate:" + resultData.getFrameRate());
                        mTvGetPropertiesLines.setText("Vertical Lines:" + resultData.getLines());
                        mTvGetPropertieshorizontalPixels.setText("Horizontal Pixels:" + resultData.gethorizontalPixels());
                    } else {
                        String STR = b.getString("STR");
                        mTvGetPropertiesFrameRate.setText("GetInputVideoProperties failed, error msg = " + STR);

                    }
                    break;
            }
        }
    };
    public View.OnClickListener btnClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.fragment_properties: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_INPUTVIDEOPROPERTIES);
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

    public InputVideoProperties() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_inputvideoproperties, container, false);
        button1 = rootView.findViewById(R.id.fragment_properties);
        button1.setOnClickListener(btnClickListener);

        mTvGetPropertiesFrameRate = rootView.findViewById(R.id.fragment_properties_tv_framerate);
        mTvGetPropertiesFrameRate.setText("");
        mTvGetPropertiesLines = rootView.findViewById(R.id.fragment_properties_tv_lines);
        mTvGetPropertiesLines.setText("");
        mTvGetPropertieshorizontalPixels = rootView.findViewById(R.id.mTvGetPropertieshorizontalPixels);
        mTvGetPropertieshorizontalPixels.setText("");
        return rootView;
    }

}
