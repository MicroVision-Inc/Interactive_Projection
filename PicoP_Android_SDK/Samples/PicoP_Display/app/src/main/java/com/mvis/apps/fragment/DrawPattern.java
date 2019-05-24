package com.mvis.apps.fragment;

import android.graphics.Color;
import android.os.Bundle;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.RadioGroup;
import android.widget.TextView;

import com.mvis.apps.R;

import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.Utils.MSG_GET_DRAWTESTPATTERN;

import com.madrapps.pikolo.HSLColorPicker;
import com.madrapps.pikolo.listeners.SimpleColorSelectionListener;

import picop.interfaces.def.PicoP_TestPatternE;


public class DrawPattern extends Fragment {

    public DrawPattern() {
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        View rootView = inflater.inflate(R.layout.fragment_draw_pattern, container, false);

        final RadioGroup radioGroup = rootView.findViewById(R.id.radio_group_pattern_picker);
        radioGroup.check(R.id.radio_btn_pattern_off); // pre-select the first one

        final Button button = (Button) rootView.findViewById(R.id.btn_draw_test_pattern);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                PicoP_TestPatternE pattern;

                Message msg = messageHandler.obtainMessage(MSG_GET_DRAWTESTPATTERN);
                Bundle b = new Bundle();
                switch (radioGroup.getCheckedRadioButtonId()) {
                    case R.id.radio_btn_pattern_off:
                        b.putInt("foregroundcolor", 0xff000000);
                        b.putInt("backgroundcolor", 0xff000000);
                        pattern = PicoP_TestPatternE.eTEST_PATTERN_OFF;
                        break;
                    case R.id.radio_btn_checker_pattern:
                        b.putInt("foregroundcolor", 0x000000ff);
                        b.putInt("backgroundcolor", 0x00ff0000);
                        pattern = PicoP_TestPatternE.eCHECKER_BOARD_PATTERN;
                        break;
                    case R.id.radio_btn_grid_pattern:
                        b.putInt("foregroundcolor", 0x00ffffff);
                        b.putInt("backgroundcolor", 0);
                        pattern = PicoP_TestPatternE.eGRID_PATTERN;
                        break;
                    case R.id.radio_btn_cross_hair:
                        b.putInt("foregroundcolor", 0x00ffffff);
                        b.putInt("backgroundcolor", 0);
                        pattern = PicoP_TestPatternE.eCROSS_HAIR_PATTERN;
                        break;
                    case R.id.radio_btn_const_color:
                        b.putInt("foregroundcolor", 0xff00ffff);
                        b.putInt("backgroundcolor", 0xff000000);
                        pattern = PicoP_TestPatternE.eCONSTANT_COLOR_PATTERN;
                        break;
                    default:
                        pattern = PicoP_TestPatternE.eTEST_PATTERN_OFF;
                }

                // send message

                b.putInt("pattern", pattern.ordinal());

                msg.setData(b);
                messageHandler.sendMessage(msg);
            }
        });


        return rootView;
    }
}