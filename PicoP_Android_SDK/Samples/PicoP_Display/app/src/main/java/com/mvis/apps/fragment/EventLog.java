package com.mvis.apps.fragment;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.mvis.apps.R;

import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.MessageCenter.pEvent;
import static com.mvis.apps.Utils.MSG_GET_EVENTLOG;


public class EventLog extends Fragment {

    public static final int MSG_EVENTLOG_RESPONSE_GET = 90001;
    protected static final String TAG = "GetEventLog";
    private static short eventNumber = 0;
    public static Handler eventlogHandler = new Handler() {
        // when handler.message() called, below code will be triggered.
        public void handleMessage(Message msg) {
            //super.handleMessage(msg);
            switch (msg.what) {
                case MSG_EVENTLOG_RESPONSE_GET:
                    Bundle b = msg.getData();
                    int result = b.getInt("result");
                    String eventLog = "";
                    if (result == 0) {
                        for (int i = 0; i < eventNumber; i++) {
                            eventLog = eventLog + "Event session is: " + pEvent[i].getSession()
                                    + "   id is: " + pEvent[i].getEventId()
                                    + "   time is: " + pEvent[i].getTime()
                                    + "   data is: " + pEvent[i].getData()
                                    + "   cid is: " + pEvent[i].getCid()
                                    + "   line is: " + pEvent[i].getLine() + "\n";
                        }
                        mTvEventLog.setText(eventLog);
                    } else {
                        String STR = b.getString("STR");
                        mTvEventLog.setText("Run failed, errno = " + STR);
                    }
                    break;
            }
        }
    };
    private static TextView mTvEventLogNumber, mTvEventLog;
    private View rootView;
    private Button button;
    private EditText eventText;
    public View.OnClickListener btnClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.fragment_getsystemlog: {
                    Message msg0 = messageHandler.obtainMessage(MSG_GET_EVENTLOG);
                    Bundle b = new Bundle();
                    if (!eventText.getText().toString().isEmpty()) {
                        String value = eventText.getText().toString();
                        eventNumber = Short.parseShort(value);
                        if (eventNumber > 200)
                            eventNumber = 200;
                    }
                    b.putShort("eventlognumber", eventNumber);
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
    private short mProgress = 1;

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        rootView = inflater.inflate(R.layout.fragment_geteventlog, container, false);
        button = rootView.findViewById(R.id.fragment_getsystemlog);
        button.setOnClickListener(btnClickListener);

        eventText = rootView.findViewById(R.id.fragment_eventlog_cnt);
        mTvEventLog = rootView.findViewById(R.id.fragment_getsystemlog_tv);
        mTvEventLog.setText("");

        return rootView;
    }
}