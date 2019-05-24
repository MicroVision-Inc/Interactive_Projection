package com.mvis.apps;

import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.util.Log;


import picop.interfaces.def.*;
import picop.interfaces.def.PicoP_Api.*;


import static com.mvis.apps.MainActivity.SERIAL_PORT_OPENED;
import static com.mvis.apps.MainActivity.connectionInfo;
import static com.mvis.apps.MainActivity.mPicoPHandler;
import static com.mvis.apps.Utils.MSG_CLOSE_CONNECTION;
import static com.mvis.apps.Utils.MSG_DO_INIT;
import static com.mvis.apps.Utils.MSG_GET_BRIGHTNESS;
import static com.mvis.apps.Utils.MSG_GET_COLORALIGNMENT;
import static com.mvis.apps.Utils.MSG_GET_COLORCONVERTER;
import static com.mvis.apps.Utils.MSG_GET_COLOR_MODE;
import static com.mvis.apps.Utils.MSG_GET_DRAWTESTPATTERN;
import static com.mvis.apps.Utils.MSG_GET_EVENTLOG;
import static com.mvis.apps.Utils.MSG_GET_FLIPSTATE;
import static com.mvis.apps.Utils.MSG_GET_FRAMERATE_MODE;
import static com.mvis.apps.Utils.MSG_GET_GAMMAVAL;
import static com.mvis.apps.Utils.MSG_GET_HORIZONTAL_SCAN_ANGLE;
import static com.mvis.apps.Utils.MSG_GET_INPUTVIDEOPROPERTIES;
import static com.mvis.apps.Utils.MSG_GET_INPUTVIDEOSTATE;
import static com.mvis.apps.Utils.MSG_GET_MICROWEAVE_MODE;
import static com.mvis.apps.Utils.MSG_GET_OUTPUTVIDEOPROPERTIES;
import static com.mvis.apps.Utils.MSG_GET_OUTPUTVIDEOSTATE;
import static com.mvis.apps.Utils.MSG_GET_PHASE;
import static com.mvis.apps.Utils.MSG_GET_SENSINGSTATE;
import static com.mvis.apps.Utils.MSG_GET_SYSTEMINFO;
import static com.mvis.apps.Utils.MSG_GET_SYSTEMSTATUS;
import static com.mvis.apps.Utils.MSG_GET_VERTICAL_SCAN_ANGLE;
import static com.mvis.apps.Utils.MSG_SET_BRIGHTNESS;
import static com.mvis.apps.Utils.MSG_SET_COLORALIGNMENT;
import static com.mvis.apps.Utils.MSG_SET_COLORCONVERTER;
import static com.mvis.apps.Utils.MSG_SET_COLOR_MODE;
import static com.mvis.apps.Utils.MSG_SET_FLIPSTATE;
import static com.mvis.apps.Utils.MSG_SET_FRAMERATE_MODE;
import static com.mvis.apps.Utils.MSG_SET_GAMMAVAL;
import static com.mvis.apps.Utils.MSG_SET_HORIZONTAL_SCAN_ANGLE;
import static com.mvis.apps.Utils.MSG_SET_INPUTVIDEOSTATE;
import static com.mvis.apps.Utils.MSG_SET_MICROWEAVE_MODE;
import static com.mvis.apps.Utils.MSG_SET_OUTPUTVIDEOSTATE;
import static com.mvis.apps.Utils.MSG_SET_PHASE;

import static com.mvis.apps.Utils.MSG_SET_SENSINGSTATE;
import static com.mvis.apps.Utils.MSG_SET_VERTICAL_SCAN_ANGLE;
import static com.mvis.apps.fragment.Brightness.MSG_BRIGHTNESS_RESPONSE_GET;
import static com.mvis.apps.fragment.Brightness.MSG_BRIGHTNESS_RESPONSE_SET;
import static com.mvis.apps.fragment.Brightness.brightnessHandler;
import static com.mvis.apps.fragment.ColorAlignment.MSG_COLORALIGNMENT_RESPONSE_GET;
import static com.mvis.apps.fragment.ColorAlignment.MSG_COLORALIGNMENT_RESPONSE_SET;
import static com.mvis.apps.fragment.ColorAlignment.coloralignmentHandler;
import static com.mvis.apps.fragment.ColorConvertor.MSG_COLORCONVERTOR_RESPONSE_GET;
import static com.mvis.apps.fragment.ColorConvertor.MSG_COLORCONVERTOR_RESPONSE_SET;
import static com.mvis.apps.fragment.ColorConvertor.colorconvertorHandler;
import static com.mvis.apps.fragment.ColorMode.MSG_COLORMODE_RESPONSE_GET;
import static com.mvis.apps.fragment.ColorMode.MSG_COLORMODE_RESPONSE_SET;
import static com.mvis.apps.fragment.ColorMode.colorModeHandler;
import static com.mvis.apps.fragment.EventLog.MSG_EVENTLOG_RESPONSE_GET;
import static com.mvis.apps.fragment.FrameRate.MSG_FRAMERATE_RESPONSE_GET;
import static com.mvis.apps.fragment.FrameRate.MSG_FRAMERATE_RESPONSE_SET;
import static com.mvis.apps.fragment.FrameRate.frameRateHandler;
import static com.mvis.apps.fragment.GammaBoost.MSG_GAMMABOOST_RESPONSE_GET;
import static com.mvis.apps.fragment.GammaBoost.MSG_GAMMABOOST_RESPONSE_SET;
import static com.mvis.apps.fragment.GammaBoost.gammaBoostHandler;
import static com.mvis.apps.fragment.GammaVal.MSG_GAMMAVAL_RESPONSE_GET;
import static com.mvis.apps.fragment.GammaVal.MSG_GAMMAVAL_RESPONSE_SET;
import static com.mvis.apps.fragment.GammaVal.gammavalHandler;
import static com.mvis.apps.fragment.HorizontalBrightnessCompensationMode.MSG_HORIZONTAL_BRIGHTNESS_MODE_RESPONSE_GET;
import static com.mvis.apps.fragment.HorizontalBrightnessCompensationMode.MSG_HORIZONTAL_BRIGHTNESS_MODE_RESPONSE_SET;
import static com.mvis.apps.fragment.HorizontalBrightnessCompensationMode.horizontalbrightnesscompensationmodeHandler;
import static com.mvis.apps.fragment.HorizontalScanAngle.MSG_HORIZ_SCAN_ANGLE_RESPONSE_GET;
import static com.mvis.apps.fragment.HorizontalScanAngle.MSG_HORIZ_SCAN_ANGLE_RESPONSE_SET;
import static com.mvis.apps.fragment.HorizontalScanAngle.horizontalScanAngleHandler;
import static com.mvis.apps.fragment.InputVideoProperties.MSG_GET_INPUTVIDEOPROPERTIES_RESPONSE_GET;
import static com.mvis.apps.fragment.InputVideoProperties.inputvideopropertiesHandler;
import static com.mvis.apps.fragment.InputVideoState.MSG_INPUTVIDEOSTATE_RESPONSE_GET;
import static com.mvis.apps.fragment.InputVideoState.MSG_INPUTVIDEOSTATE_RESPONSE_SET;
import static com.mvis.apps.fragment.InputVideoState.inputvideostateHandler;
import static com.mvis.apps.fragment.MicroweaveMode.MSG_MICROWEAVE_MODE_RESPONSE_GET;
import static com.mvis.apps.fragment.MicroweaveMode.MSG_MICROWEAVE_MODE_RESPONSE_SET;
import static com.mvis.apps.fragment.MicroweaveMode.microweaveModeHandler;
import static com.mvis.apps.fragment.OutputVideoProperties.MSG_GET_OUTPUTVIDEOPROPERTIES_RESPONSE_GET;
import static com.mvis.apps.fragment.OutputVideoProperties.outputvideopropertiesHandler;
import static com.mvis.apps.fragment.OutputVideoState.MSG_OUTPUTVIDEOSTATE_RESPONSE_GET;
import static com.mvis.apps.fragment.OutputVideoState.MSG_OUTPUTVIDEOSTATE_RESPONSE_SET;
import static com.mvis.apps.fragment.OutputVideoState.outputvideostateHandler;
import static com.mvis.apps.fragment.Phase.MSG_PHASE_RESPONSE_GET;
import static com.mvis.apps.fragment.Phase.MSG_PHASE_RESPONSE_SET;
import static com.mvis.apps.fragment.Phase.phaseHandler;

import static com.mvis.apps.fragment.FlipState.flipStateHandler;
import static com.mvis.apps.fragment.FlipState.MSG_FLIP_STATE_RESPONSE_SET;
import static com.mvis.apps.fragment.FlipState.MSG_FLIP_STATE_RESPONSE_GET;

import static com.mvis.apps.Utils.MSG_SET_GAMMABOOSTMODE;
import static com.mvis.apps.Utils.MSG_GET_GAMMABOOSTMODE;
import static com.mvis.apps.Utils.MSG_SET_HORIZONTAL_BRIGHTNESS_COMPENSATION_MODE;
import static com.mvis.apps.Utils.MSG_GET_HORIZONTAL_BRIGHTNESS_COMPENSATION_MODE;
import static com.mvis.apps.fragment.SensingState.MSG_SENSING_STATE_RESPONSE_GET;
import static com.mvis.apps.fragment.SensingState.MSG_SENSING_STATE_RESPONSE_SET;
import static com.mvis.apps.fragment.SensingState.inputSensingStateHandler;
import static com.mvis.apps.fragment.System.MSG_GET_SYSTEMINFO_RESPONSE_GET;
import static com.mvis.apps.fragment.System.MSG_GET_SYSTEMSTATUS_RESPONSE_GET;
import static com.mvis.apps.fragment.System.systemstatusHandler;
import static com.mvis.apps.fragment.VerticalScanAngle.MSG_VERTICAL_SCAN_ANGLE_RESPONSE_GET;
import static com.mvis.apps.fragment.VerticalScanAngle.MSG_VERTICAL_SCAN_ANGLE_RESPONSE_SET;
import static com.mvis.apps.fragment.VerticalScanAngle.verticalScanAngleHandler;
import static com.mvis.apps.fragment.EventLog.eventlogHandler;
import static picop.interfaces.def.PicoP_PpcpUtils.MAX_EVENT_LOG;


public class MessageCenter {


    private static final String TAG = "MessageCenter";
    public static PicoP_BasicDataResult resultData = new PicoP_BasicDataResult();
    public static PicoP_SystemStatusS systemStatus = new PicoP_SystemStatusS();
    public static PicoP_SystemInfoS systemInfo = new PicoP_SystemInfoS();
    public static PicoP_RC RC_FOR_SEND = PicoP_RC.eSUCCESS;
    public static PicoP_RC RC_FOR_GET = PicoP_RC.eSUCCESS;
    public static PicoP_Api picoPApi = new PicoP_Api();
    public static PicoP_EventS[] pEvent = new PicoP_EventS[MAX_EVENT_LOG];

    /**
     * Callback function to get response from JAR lib.
     */
    static PicoP_Callback callback = new PicoP_Callback() {

        public void colorModeCallback(int colorMode) {
            PicoP_BasicDataResult.setColormode(colorMode);
        }

        public void colorAligmentCallback(float offset) {
            PicoP_BasicDataResult.setOffset(offset);
        }

        public void getPhaseCallback(int phaseValue) {
            PicoP_BasicDataResult.setPhaseValue(phaseValue);
        }

        public void gammavalCallback(float gammaVal) {
            PicoP_BasicDataResult.setGammaval(gammaVal);
        }

        public void inputStateCallback(int state) {
            PicoP_BasicDataResult.setInputstate(state);
        }

        public void outputStateCallback(int state) {
            PicoP_BasicDataResult.setOutputstate(state);
        }

        public void flipStateCallback(int flipState) {
            PicoP_BasicDataResult.setFlipState(flipState);
        }

        public void colorConverterCallback(int coefficient) {
            PicoP_BasicDataResult.setCoefficient(coefficient);
        }

        public void videoGammaBoostModeCallback(int modeType) {
            PicoP_BasicDataResult.setGammaBoostMode(modeType);
        }

        public void horizontalBrightnessCompensationMode(int modeType) {
            PicoP_BasicDataResult.setHorizontalBrightnessCompensationMode(modeType);
        }

        public void brightnessCallback(int brightness) {
            PicoP_BasicDataResult.setBrightness(brightness);
        }

        public void verticalScanAngleCallback(float angle) {
            PicoP_BasicDataResult.setVerticalScanAngle(angle);
        }

        public void horizScanAngleCallback(float angle) {
            PicoP_BasicDataResult.setHorizontalScanAngle(angle);
        }

        public void microWeaveModeCallback(int mode) {
            PicoP_BasicDataResult.setMicroweaveMode(mode);
        }

        public void frameRateCallback(int frameRate, int verticalScaling, int horizontalScaling) {
            PicoP_BasicDataResult.frameratemode = frameRate;
            PicoP_BasicDataResult.vsmode = verticalScaling;
            PicoP_BasicDataResult.hsmode = horizontalScaling;
        }

        public void inputPropertiesCallback(int horizontalPixels, int verticalLines, float frameRate) {
            PicoP_BasicDataResult.horizontalPixels = horizontalPixels;
            PicoP_BasicDataResult.lines = verticalLines;
            PicoP_BasicDataResult.frameRate = frameRate;
        }

        public void outputPropertiesCallback(int horizontalResolution, int verticalResolution, float frameRate) {
            PicoP_BasicDataResult.horizontalRes = horizontalResolution;
            PicoP_BasicDataResult.verticalRes = verticalResolution;
            PicoP_BasicDataResult.outputFrameRate = frameRate;
        }

        public void sensingStateCallback(int state) {
            PicoP_BasicDataResult.setSensingstate(state);
        }

    };

    private static HandlerThread handlerThread = new HandlerThread("MessageHandlerThread");

    static {
        handlerThread.start();
    }

    public static Handler messageHandler = new Handler(handlerThread.getLooper(),
            new Handler.Callback() {
                @Override
                public boolean handleMessage(Message msg) {
                    PicoP_Ulog.d(TAG, "Handler is handeling a message");
                    //super.handleMessage(msg);
                    switch (msg.what) {
                        case MSG_SET_COLOR_MODE: {
                            Bundle b = msg.getData();
                            int mode = b.getInt("mode");
                            boolean commit = b.getBoolean("commit");
                            Log.i(TAG, "received message to set color mode, mode = " + mode + ", commit = " + commit);
                            doSetColorMode(mode, commit);
                            break;
                        }
                        case MSG_GET_COLOR_MODE: {
                            Bundle b = msg.getData();
                            int type = b.getInt("type");
                            Log.i(TAG, "received message to get color mode, type = " + type);
                            doGetColorMode(type);
                            break;
                        }
                        case MSG_SET_PHASE: {
                            Bundle b = msg.getData();
                            int phase = b.getInt("phase");
                            boolean commit = b.getBoolean("commit");
                            Log.i(TAG, "received message to set phase, phase = " + phase + ", commit = " + commit);
                            doSetphase(phase, commit);
                            break;
                        }
                        case MSG_GET_PHASE: {
                            Bundle b = msg.getData();
                            int type = b.getInt("type");
                            Log.i(TAG, "received message to get keystone, type = " + type);
                            doGetphase(type);
                            break;
                        }
                        case MSG_SET_GAMMAVAL: {
                            Bundle b = msg.getData();
                            boolean commit = b.getBoolean("commit");
                            float gammaval = b.getFloat("gammaval");
                            int color = b.getInt("color");
                            Log.i(TAG, "received message to set gammaval, gammaval = " + gammaval + ", commit = " + commit);
                            doSetGammaval(gammaval, color, commit);
                            break;
                        }
                        case MSG_GET_GAMMAVAL: {
                            Bundle b = msg.getData();
                            int type = b.getInt("type");
                            int color = b.getInt("color");
                            Log.i(TAG, "received message to get gammaval, type = " + type + ", color = " + color);
                            doGetGammaval(type, color);
                            break;
                        }
                        case MSG_SET_GAMMABOOSTMODE: {
                            Bundle b = msg.getData();
                            boolean commit = b.getBoolean("commit");
                            int mode = b.getInt("mode");
                            int color = b.getInt("color");
                            Log.i(TAG, "received message to set gamma boost mode, color = " + color + " ,mode = " + mode + ", commit = " + commit);
                            doSetGammaBoostMode(color, mode, commit);
                            break;
                        }
                        case MSG_GET_GAMMABOOSTMODE: {
                            Bundle b = msg.getData();
                            int type = b.getInt("type");
                            int color = b.getInt("color");
                            Log.i(TAG, "received message to get boost mode, type = " + type + ", color = " + color);
                            doGetGammaBoostMode(type, color);
                            break;
                        }
                        case MSG_SET_OUTPUTVIDEOSTATE: {
                            Bundle b = msg.getData();
                            int state = b.getInt("state");
                            boolean commit = b.getBoolean("commit");
                            Log.i(TAG, "received message to set output video state, state = " + state + ", commit = " + commit);
                            doSetOutputVideoState(state, commit);
                            break;
                        }
                        case MSG_GET_OUTPUTVIDEOSTATE: {
                            Bundle b = msg.getData();
                            int state = b.getInt("state");
                            int storageType = b.getInt("type");
                            Log.i(TAG, "received message to get output video state, state = " + state + " Type = " + storageType);
                            doGetOutputVideoState(state, storageType);
                            break;
                        }
                        case MSG_SET_INPUTVIDEOSTATE: {
                            Bundle b = msg.getData();
                            int state = b.getInt("state");
                            boolean commit = b.getBoolean("commit");
                            Log.i(TAG, "received message to set output video state, state = " + state);
                            doSetInputVideoState(state, commit);
                            break;
                        }
                        case MSG_GET_INPUTVIDEOSTATE: {
                            Bundle b = msg.getData();
                            int state = b.getInt("state");
                            Log.i(TAG, "received message to get output video state, state = " + state);
                            doGetInputVideoState(state);
                            break;
                        }
                        case MSG_SET_FLIPSTATE: {
                            Bundle b = msg.getData();
                            int flipState = b.getInt("flipstate");
                            boolean commit = b.getBoolean("commit");
                            Log.i(TAG, "received message to set flip state, state = " + flipState);
                            doSetFlipState(flipState, commit);
                            break;
                        }
                        case MSG_GET_FLIPSTATE: {
                            Bundle b = msg.getData();
                            int type = b.getInt("type");
                            doGetFlipState(type);
                            Log.i(TAG, "received message to get flipState");
                            break;
                        }
                        case MSG_SET_COLORCONVERTER: {
                            Bundle b = msg.getData();
                            int color = b.getInt("color");
                            int coefficient = b.getInt("coefficient");
                            boolean commit = b.getBoolean("commit");
                            doSetColorConverter(color, coefficient, commit);
                            Log.i(TAG, "received message to set colorConverter");
                            break;
                        }
                        case MSG_GET_COLORCONVERTER: {
                            Bundle b = msg.getData();
                            int color = b.getInt("color");
                            int type = b.getInt("type");
                            doGetColorConverter(color, type);
                            Log.i(TAG, "received message to get colorConverter");
                            break;
                        }
                        case MSG_CLOSE_CONNECTION: {
                            doClose();
                            Log.i(TAG, "received message to close connection");
                            break;
                        }
                        case MSG_GET_SYSTEMSTATUS: {
                            doGetSystemStatus();
                            Log.i(TAG, "received message to get systemStatus");
                            break;
                        }
                        case MSG_GET_SYSTEMINFO: {
                            doGetSystemInfo();
                            Log.i(TAG, "received message to get systemInfo");
                            break;
                        }
                        case MSG_DO_INIT: {
                            doInit();
                            Log.i(TAG, "received message to get systemStatus");
                            break;
                        }
                        case MSG_SET_BRIGHTNESS: {
                            Bundle b = msg.getData();
                            int brightness = b.getInt("brightness", 0);
                            boolean commit = b.getBoolean("commit");
                            Log.i(TAG, "received message to set brightness, brightness = " + brightness + ", commit = " + commit);
                            doSetBrightness(brightness, commit);
                            break;
                        }
                        case MSG_GET_BRIGHTNESS: {
                            Bundle b = msg.getData();
                            int type = b.getInt("type");
                            Log.i(TAG, "received message to get brightness, type = " + type);
                            doGetBrightness(type);
                            break;
                        }
                        case MSG_GET_DRAWTESTPATTERN: {
                            int patternInt = msg.getData().getInt("pattern");
                            PicoP_TestPatternE pattern = PicoP_TestPatternE.values()[patternInt];
                            int foreground = msg.getData().getInt("foregroundcolor");
                            int background = msg.getData().getInt("backgroundcolor");
                            Log.i(TAG, "received message to drawTestPattern foregroundcolor = " + Integer.toHexString(foreground) + "  backgroundcolor:" + Integer.toHexString(background));
                            doGetDrawTestPattern(pattern, foreground, background);

                            break;
                        }
                        case MSG_SET_COLORALIGNMENT: {
                            Bundle b = msg.getData();
                            boolean commit = b.getBoolean("commit");
                            int color = b.getInt("color");
                            int direction = b.getInt("direction");
                            float offset = b.getFloat("coloroffset");
                            Log.i(TAG, "received message to set color alignment, color = " + color + ", direction = " + direction + ", offset = " + offset);
                            doSetColorAlignment(color, direction, offset, commit);
                            break;
                        }
                        case MSG_GET_COLORALIGNMENT: {
                            Bundle b = msg.getData();
                            int type = b.getInt("type");
                            int color = b.getInt("color");
                            int direction = b.getInt("direction");
                            Log.i(TAG, "received meessage to get color alignment, color = " + color + ", direction = " + direction +"type = " + type);
                            doGetColorAlignment(color, direction, type);
                            break;
                        }
                        case MSG_SET_MICROWEAVE_MODE: {
                            Bundle b = msg.getData();
                            boolean commit = b.getBoolean("commit");
                            int mode = b.getInt("mode");
                            Log.i(TAG, "received message to set microweave mode, mode = " + mode);
                            doSetMicroweaveMode(mode, commit);
                            break;
                        }
                        case MSG_GET_MICROWEAVE_MODE: {
                            Bundle b = msg.getData();
                            int type = b.getInt("type");
                            Log.i(TAG, "received meessage to get microweave mode, type = " + type);
                            doGetMicroweaveMode(type);
                            break;
                        }
                        case MSG_SET_FRAMERATE_MODE: {
                            Bundle b = msg.getData();
                            boolean commit = b.getBoolean("commit");
                            int vsmode = b.getInt("vsmode");
                            int hsmode = b.getInt("hsmode");
                            int frameratemode = b.getInt("frameratemode");
                            Log.i(TAG, "received message to set frame rate, vsmode = " + vsmode + ", hsmode = " + hsmode + ", frameratemode = " + frameratemode);
                            doSetFrameRateMode(vsmode, hsmode, frameratemode, commit);
                            break;
                        }
                        case MSG_GET_FRAMERATE_MODE: {
                            Bundle b = msg.getData();
                            int type = b.getInt("type");
                            Log.i(TAG, "received meessage to get frame rate, type = " + type);
                            doGetFrameRateMode(type);
                            break;
                        }
                        case MSG_GET_INPUTVIDEOPROPERTIES: {
                            Log.i(TAG, "received meessage to get input video properties");
                            doGetInputVideoProperties();
                            break;
                        }
                        case MSG_GET_OUTPUTVIDEOPROPERTIES: {
                            Log.i(TAG, "received meessage to get input video properties");
                            doGetOutputVideoProperties();
                            break;
                        }
                        case MSG_GET_EVENTLOG: {
                            Bundle b = msg.getData();
                            short number = b.getShort("eventlognumber");
                            Log.i(TAG, "received message to get event log, number = " + number);
                            doGetEventLog(number);
                            break;
                        }
                        case MSG_SET_SENSINGSTATE: {
                            Bundle b = msg.getData();
                            int state = b.getInt("state");
                            boolean commit = b.getBoolean("commit");
                            Log.i(TAG, "received message to set sensing state, state = " + state + ", commit = " + commit);
                            doSetSensingState(state, commit);
                            break;
                        }
                        case MSG_GET_SENSINGSTATE: {
                            Bundle b = msg.getData();
                            int storageType = b.getInt("type");
                            Log.i(TAG, "received message to get sensing state, storage type = " + storageType);
                            doGetSensingState(storageType);
                        }
                    }

                    return true;
                }
            });


    private static boolean openLib() {
        PicoP_RC ret = picoPApi.PicoP_OpenLibrary(mPicoPHandler);
        if (PicoP_RC.eSUCCESS != ret) {
            Log.e(TAG, "open library failed, it should not happened " + ret);
            return false;
        }
        return true;
    }

    private static boolean doInit() {
        if (openLib() != true) {
            Log.e(TAG, "open library failed, it should not happened ");
            return false;
        }


        // For USB port
        PicoP_RC ret = picoPApi.PicoP_OpenConnection(mPicoPHandler, PicoP_ConnectionTypeE.eUSB, connectionInfo);

        if (PicoP_RC.eSUCCESS != ret) {
            SERIAL_PORT_OPENED = false;
            Log.e(TAG, "open conn failed, it should not happened " + ret);
            return false;
        }

        SERIAL_PORT_OPENED = true;
        return true;
    }

    private static boolean doClose() {
        PicoP_RC ret = picoPApi.PicoP_CloseConnection(mPicoPHandler, PicoP_ConnectionTypeE.eUSB);

        SERIAL_PORT_OPENED = false;
        return true;
    }

    private static void doGetSystemStatus() {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetSystemStatus(mPicoPHandler, systemStatus);
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetSystemStatus(mPicoPHandler, systemStatus);
            }
        }
        Message msg = systemstatusHandler.obtainMessage(MSG_GET_SYSTEMSTATUS_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        msg.setData(b);
        systemstatusHandler.sendMessage(msg);
    }

    private static void doSetColorMode(int mode, boolean commit) {
        RC_FOR_SEND = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_SEND = picoPApi.PicoP_SetColorMode(mPicoPHandler, intToColorMode(mode), commit);
        } else {
            if (doInit()) {
                RC_FOR_SEND = picoPApi.PicoP_SetColorMode(mPicoPHandler, intToColorMode(mode), commit);
            }
        }
        Message msg = colorModeHandler.obtainMessage(MSG_COLORMODE_RESPONSE_SET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_SEND.enumtoInt(RC_FOR_SEND));
        b.putString("STR", RC_FOR_SEND.toString());
        msg.setData(b);
        colorModeHandler.sendMessage(msg);
    }

    private static void doGetColorMode(int type) {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetColorMode(mPicoPHandler, callback, intToStorageTye(type));
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetColorMode(mPicoPHandler, callback, intToStorageTye(type));
            }
        }
        Message msg = colorModeHandler.obtainMessage(MSG_COLORMODE_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        b.putInt("type", type);
        msg.setData(b);
        colorModeHandler.sendMessage(msg);

    }

    private static void doSetphase(int phase, boolean commit) {
        RC_FOR_SEND = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_SEND = picoPApi.PicoP_SetBiPhase(mPicoPHandler, (short) phase, commit);
        } else {
            if (doInit()) {
                RC_FOR_SEND = picoPApi.PicoP_SetBiPhase(mPicoPHandler, (short) phase, commit);
            }
        }
        Message msg = phaseHandler.obtainMessage(MSG_PHASE_RESPONSE_SET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_SEND.enumtoInt(RC_FOR_SEND));
        b.putString("STR", RC_FOR_SEND.toString());
        msg.setData(b);
        phaseHandler.sendMessage(msg);
    }

    private static void doGetphase(int type) {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetBiPhase(mPicoPHandler, callback, intToStorageTye(type));
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetBiPhase(mPicoPHandler, callback, intToStorageTye(type));
            }
        }
        Message msg = phaseHandler.obtainMessage(MSG_PHASE_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        b.putInt("type", type);
        msg.setData(b);
        phaseHandler.sendMessage(msg);
    }


    private static void doSetGammaBoostMode(int color, int mode, boolean commit) {
        RC_FOR_SEND = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_SEND = picoPApi.PicoP_SetVideoGammaBoostMode(mPicoPHandler, intToColor(color), intToGammaBoostMode(mode), commit);
        } else {
            if (doInit()) {
                RC_FOR_SEND = picoPApi.PicoP_SetVideoGammaBoostMode(mPicoPHandler, intToColor(color), intToGammaBoostMode(mode), commit);
            }
        }
        Message msg = gammaBoostHandler.obtainMessage(MSG_GAMMABOOST_RESPONSE_SET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_SEND.enumtoInt(RC_FOR_SEND));
        b.putString("STR", RC_FOR_SEND.toString());
        msg.setData(b);
        gammaBoostHandler.sendMessage(msg);
    }

    private static void doGetGammaBoostMode(int type, int color) {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetVideoGammaBoostMode(mPicoPHandler, callback, intToColor(color), intToStorageTye(type));
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetVideoGammaBoostMode(mPicoPHandler, callback, intToColor(color), intToStorageTye(type));
            }
        }
        Message msg = gammaBoostHandler.obtainMessage(MSG_GAMMABOOST_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        b.putInt("type", type);
        ;
        msg.setData(b);
        gammaBoostHandler.sendMessage(msg);
    }

    private static void doSetGammaval(float gammaVal, int color, boolean commit) {
        RC_FOR_SEND = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_SEND = picoPApi.PicoP_SetGamma(mPicoPHandler, intToColor(color), gammaVal, commit);
        } else {
            if (doInit()) {
                RC_FOR_SEND = picoPApi.PicoP_SetGamma(mPicoPHandler, intToColor(color), gammaVal, commit);
            }
        }
        Message msg = gammavalHandler.obtainMessage(MSG_GAMMAVAL_RESPONSE_SET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_SEND.enumtoInt(RC_FOR_SEND));
        b.putString("STR", RC_FOR_SEND.toString());
        msg.setData(b);
        gammavalHandler.sendMessage(msg);
    }

    private static void doGetGammaval(int type, int color) {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetGamma(mPicoPHandler, callback, intToColor(color), intToStorageTye(type));
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetGamma(mPicoPHandler, callback, intToColor(color), intToStorageTye(type));
            }
        }
        Message msg = gammavalHandler.obtainMessage(MSG_GAMMAVAL_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        b.putInt("type", type);
        b.putInt("color", color);
        msg.setData(b);
        gammavalHandler.sendMessage(msg);
    }

    private static void doSetOutputVideoState(int state, boolean commit) {
        RC_FOR_SEND = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_SEND = picoPApi.PicoP_SetOutputVideoState(mPicoPHandler, intToOutputVideoState(state), commit);
        } else {
            if (doInit()) {
                RC_FOR_SEND = picoPApi.PicoP_SetOutputVideoState(mPicoPHandler, intToOutputVideoState(state), commit);
            }
        }
        Message msg = outputvideostateHandler.obtainMessage(MSG_OUTPUTVIDEOSTATE_RESPONSE_SET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_SEND.enumtoInt(RC_FOR_SEND));
        b.putString("STR", RC_FOR_SEND.toString());
        msg.setData(b);
        outputvideostateHandler.sendMessage(msg);
    }

    private static void doGetOutputVideoState(int state, int storageType) {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetOutputVideoState(mPicoPHandler, callback, intToStorageTye(storageType));
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetOutputVideoState(mPicoPHandler, callback, intToStorageTye(storageType));
            }
        }
        Message msg = outputvideostateHandler.obtainMessage(MSG_OUTPUTVIDEOSTATE_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        msg.setData(b);
        outputvideostateHandler.sendMessage(msg);
    }

    private static void doSetInputVideoState(int state, boolean bCommit) {
        RC_FOR_SEND = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_SEND = picoPApi.PicoP_SetInputVideoState(mPicoPHandler, intToInputVideoState(state), bCommit);
        } else {
            if (doInit()) {
                RC_FOR_SEND = picoPApi.PicoP_SetInputVideoState(mPicoPHandler, intToInputVideoState(state), bCommit);
            }
        }
        Message msg = inputvideostateHandler.obtainMessage(MSG_INPUTVIDEOSTATE_RESPONSE_SET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_SEND.enumtoInt(RC_FOR_SEND));
        b.putString("STR", RC_FOR_SEND.toString());
        msg.setData(b);
        inputvideostateHandler.sendMessage(msg);
    }

    private static void doGetInputVideoState(int state) {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetInputVideoState(mPicoPHandler, callback, intToStorageTye(0));
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetInputVideoState(mPicoPHandler, callback, intToStorageTye(0));
            }
        }
        Message msg = inputvideostateHandler.obtainMessage(MSG_INPUTVIDEOSTATE_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        b.putInt("state", state);
        msg.setData(b);
        inputvideostateHandler.sendMessage(msg);
    }

    private static void doSetFlipState(int state, boolean bCommit) {
        RC_FOR_SEND = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_SEND = picoPApi.PicoP_SetFlipState(mPicoPHandler, intToFlipStateType(state), bCommit);
        } else {
            if (doInit()) {
                RC_FOR_SEND = picoPApi.PicoP_SetFlipState(mPicoPHandler, intToFlipStateType(state), bCommit);
            }
        }
        Message msg = flipStateHandler.obtainMessage(MSG_FLIP_STATE_RESPONSE_SET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_SEND.enumtoInt(RC_FOR_SEND));
        b.putString("STR", RC_FOR_SEND.toString());
        msg.setData(b);
        flipStateHandler.sendMessage(msg);
    }

    private static void doGetFlipState(int type) {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetFlipState(mPicoPHandler, callback, intToStorageTye(type));
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetFlipState(mPicoPHandler, callback, intToStorageTye(type));
            }
        }
        Message msg = flipStateHandler.obtainMessage(MSG_FLIP_STATE_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        b.putInt("type", type);
        msg.setData(b);
        flipStateHandler.sendMessage(msg);
    }

    private static void doSetColorConverter(int color, int coefficient, boolean bCommit) {
        RC_FOR_SEND = PicoP_RC.eFAILURE;
        PicoP_ColorConvertE temp = PicoP_ColorConvertE.eBLUE_TO_GREEN;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_SEND = picoPApi.PicoP_SetColorConverter(mPicoPHandler, temp.intToEnum(color), coefficient, bCommit);
        } else {
            if (doInit()) {
                RC_FOR_SEND = picoPApi.PicoP_SetColorConverter(mPicoPHandler, temp.intToEnum(color), coefficient, bCommit);
            }
        }
        Message msg = colorconvertorHandler.obtainMessage(MSG_COLORCONVERTOR_RESPONSE_SET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_SEND.enumtoInt(RC_FOR_SEND));
        b.putString("STR", RC_FOR_SEND.toString());
        msg.setData(b);
        colorconvertorHandler.sendMessage(msg);
    }

    private static void doGetColorConverter(int color, int type) {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        PicoP_ColorConvertE temp = PicoP_ColorConvertE.eBLUE_TO_GREEN;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetColorConverter(mPicoPHandler, callback, temp.intToEnum(color), intToStorageTye(type));
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetColorConverter(mPicoPHandler, callback, temp.intToEnum(color), intToStorageTye(type));
            }
        }
        Message msg = colorconvertorHandler.obtainMessage(MSG_COLORCONVERTOR_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        b.putInt("type", type);
        b.putInt("color", color);
        msg.setData(b);
        colorconvertorHandler.sendMessage(msg);
    }

    private static void doGetSystemInfo() {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetSystemInfo(mPicoPHandler, systemInfo);
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetSystemInfo(mPicoPHandler, systemInfo);
            }
        }
        Message msg = systemstatusHandler.obtainMessage(MSG_GET_SYSTEMINFO_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        msg.setData(b);
        systemstatusHandler.sendMessage(msg);
    }

    private static void doSetBrightness(int brightness, boolean commit) {
        RC_FOR_SEND = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_SEND = picoPApi.PicoP_SetBrightness(mPicoPHandler, brightness, commit);
        } else {
            if (doInit()) {
                RC_FOR_SEND = picoPApi.PicoP_SetBrightness(mPicoPHandler, brightness, commit);
            }
        }
        Message msg = brightnessHandler.obtainMessage(MSG_BRIGHTNESS_RESPONSE_SET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_SEND.enumtoInt(RC_FOR_SEND));
        b.putString("STR", RC_FOR_SEND.toString());
        msg.setData(b);
        brightnessHandler.sendMessage(msg);
    }

    private static void doGetBrightness(int type) {
        RC_FOR_GET = PicoP_RC.eFAILURE;

        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetBrightness(mPicoPHandler, callback, intToStorageTye(type));
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetBrightness(mPicoPHandler, callback, intToStorageTye(type));
            }
        }
        Message msg = brightnessHandler.obtainMessage(MSG_BRIGHTNESS_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        b.putInt("type", type);
        msg.setData(b);
        brightnessHandler.sendMessage(msg);
    }

    private static void doGetDrawTestPattern(PicoP_TestPatternE pattern, int foregroundColor, int backgroundColor) {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_DrawTestPattern(mPicoPHandler, pattern, foregroundColor, backgroundColor);

        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_DrawTestPattern(mPicoPHandler, pattern, foregroundColor, backgroundColor);
            }
        }
    }

    private static void doSetMicroweaveMode(int mode, boolean commit) {
        RC_FOR_SEND = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_SEND = picoPApi.PicoP_SetMicroWeaveMode(mPicoPHandler, intToMicroweaveModeE(mode), commit);
        } else {
            if (doInit()) {
                RC_FOR_SEND = picoPApi.PicoP_SetMicroWeaveMode(mPicoPHandler, intToMicroweaveModeE(mode), commit);
            }
        }
        Message msg = microweaveModeHandler.obtainMessage(MSG_MICROWEAVE_MODE_RESPONSE_SET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_SEND.enumtoInt(RC_FOR_SEND));
        b.putString("STR", RC_FOR_SEND.toString());
        msg.setData(b);
        microweaveModeHandler.sendMessage(msg);
    }

    private static void doGetMicroweaveMode(int type) {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetMicroWeaveMode(mPicoPHandler, callback, intToStorageTye(type));
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetMicroWeaveMode(mPicoPHandler, callback, intToStorageTye(type));
            }
        }
        Message msg = microweaveModeHandler.obtainMessage(MSG_MICROWEAVE_MODE_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        b.putInt("type", type);
        msg.setData(b);
        microweaveModeHandler.sendMessage(msg);
    }

    private static void doSetColorAlignment(int color, int direction, float offset, boolean commit) {
        RC_FOR_SEND = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_SEND = picoPApi.PicoP_SetColorAlignment(mPicoPHandler, intToDirection(direction), intToLaser(color), offset, commit);
        } else {
            if (doInit()) {
                RC_FOR_SEND = picoPApi.PicoP_SetColorAlignment(mPicoPHandler, intToDirection(direction), intToLaser(color), offset, commit);
            }
        }
        Message msg = coloralignmentHandler.obtainMessage(MSG_COLORALIGNMENT_RESPONSE_SET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_SEND.enumtoInt(RC_FOR_SEND));
        b.putString("STR", RC_FOR_SEND.toString());
        msg.setData(b);
        coloralignmentHandler.sendMessage(msg);
    }

    private static void doGetColorAlignment(int color, int direction, int type) {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetColorAlignment(mPicoPHandler, intToDirection(direction), intToLaser(color), callback, intToStorageTye(type));
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetColorAlignment(mPicoPHandler, intToDirection(direction), intToLaser(color), callback, intToStorageTye(type));
            }
        }
        Message msg = coloralignmentHandler.obtainMessage(MSG_COLORALIGNMENT_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        b.putInt("type", type);
        msg.setData(b);
        coloralignmentHandler.sendMessage(msg);
    }

    private static void doSetFrameRateMode(int vsmode, int hsmode, int frameratemode, boolean commit) {
        RC_FOR_SEND = PicoP_RC.eFAILURE;
        PicoP_FrameRateModeS mode = new PicoP_FrameRateModeS();
        mode.setPicoP_FrameRateModeS(PicoP_FrameRateE.values()[frameratemode], PicoP_DisplayVerticalScalingModeE.values()[vsmode],
                PicoP_DisplayHorizontalScalingModeE.values()[hsmode]);
        if (SERIAL_PORT_OPENED) {
            RC_FOR_SEND = picoPApi.PicoP_SetFrameRateMode(mPicoPHandler, mode, commit);
        } else {
            if (doInit()) {
                RC_FOR_SEND = picoPApi.PicoP_SetFrameRateMode(mPicoPHandler, mode, commit);
            }
        }
        Message msg = frameRateHandler.obtainMessage(MSG_FRAMERATE_RESPONSE_SET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_SEND.enumtoInt(RC_FOR_SEND));
        b.putString("STR", RC_FOR_SEND.toString());
        msg.setData(b);
        frameRateHandler.sendMessage(msg);
    }

    private static void doGetFrameRateMode(int type) {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetFrameRateMode(mPicoPHandler, callback, intToStorageTye(type));
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetFrameRateMode(mPicoPHandler, callback, intToStorageTye(type));
            }
        }
        Message msg = frameRateHandler.obtainMessage(MSG_FRAMERATE_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        b.putInt("type", type);
        msg.setData(b);
        frameRateHandler.sendMessage(msg);
    }

    private static void doGetInputVideoProperties() {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetInputVideoProperties(mPicoPHandler, callback);
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetInputVideoProperties(mPicoPHandler, callback);
            }
        }
        Message msg = inputvideopropertiesHandler.obtainMessage(MSG_GET_INPUTVIDEOPROPERTIES_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        msg.setData(b);
        inputvideopropertiesHandler.sendMessage(msg);
    }

    private static void doGetOutputVideoProperties() {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetOutputVideoProperties(mPicoPHandler, callback);
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetOutputVideoProperties(mPicoPHandler, callback);
            }
        }
        Message msg = outputvideopropertiesHandler.obtainMessage(MSG_GET_OUTPUTVIDEOPROPERTIES_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        msg.setData(b);
        outputvideopropertiesHandler.sendMessage(msg);
    }

    private static void doGetEventLog(short eventLogNumber) {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetEventLog(mPicoPHandler, eventLogNumber, pEvent);
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetEventLog(mPicoPHandler, eventLogNumber, pEvent);
            }
        }
        Message msg = eventlogHandler.obtainMessage(MSG_EVENTLOG_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        b.putInt("eventLogNumber", eventLogNumber);
        msg.setData(b);
        eventlogHandler.sendMessage(msg);
    }

    private static void doSetSensingState(int state, boolean commit) {
        RC_FOR_SEND = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_SEND = picoPApi.PicoP_SetSensingState(mPicoPHandler, intToSensingState(state), commit);
        } else {
            if (doInit()) {
                RC_FOR_SEND = picoPApi.PicoP_SetSensingState(mPicoPHandler, intToSensingState(state), commit);
            }
        }
        Message msg = inputSensingStateHandler.obtainMessage(MSG_SENSING_STATE_RESPONSE_SET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_SEND.enumtoInt(RC_FOR_SEND));
        b.putString("STR", RC_FOR_SEND.toString());
        msg.setData(b);
        inputSensingStateHandler.sendMessage(msg);
    }

    private static void doGetSensingState(int storageType) {
        RC_FOR_GET = PicoP_RC.eFAILURE;
        if (SERIAL_PORT_OPENED) {
            RC_FOR_GET = picoPApi.PicoP_GetSensingState(mPicoPHandler, callback, intToStorageType(storageType));
        } else {
            if (doInit()) {
                RC_FOR_GET = picoPApi.PicoP_GetSensingState(mPicoPHandler, callback, intToStorageType(storageType));
            }
        }
        Message msg = inputSensingStateHandler.obtainMessage(MSG_SENSING_STATE_RESPONSE_GET);
        Bundle b = new Bundle();
        b.putInt("result", RC_FOR_GET.enumtoInt(RC_FOR_GET));
        b.putString("STR", RC_FOR_GET.toString());
        msg.setData(b);
        inputSensingStateHandler.sendMessage(msg);
    }


    private static int PicoP_RC_To_Int(PicoP_RC ret) {
        int value = 0;
        return value;
    }

    public static PicoP_ValueStorageTypeE intToStorageTye(int type) {
        switch (type) {
            case 0:
                return PicoP_ValueStorageTypeE.eCURRENT_VALUE;
            case 1:
                return PicoP_ValueStorageTypeE.eVALUE_ON_STARTUP;
            case 2:
                return PicoP_ValueStorageTypeE.eFACTORY_VALUE;
            default:
                return PicoP_ValueStorageTypeE.eFACTORY_VALUE;
        }
    }

    public static PicoP_DirectionE intToDirection(int direction) {
        switch (direction) {
            case 0:
                return PicoP_DirectionE.eHORIZONTAL;
            case 1:
                return PicoP_DirectionE.eVERTICAL;
            default:
                return PicoP_DirectionE.eHORIZONTAL;
        }
    }

    public static PicoP_ColorE intToColor(int color) {
        switch (color) {
            case 0:
                return PicoP_ColorE.eRED;
            case 1:
                return PicoP_ColorE.eGREEN;
            case 2:
                return PicoP_ColorE.eBLUE;
            default:
                return PicoP_ColorE.eRED;
        }
    }

    public static PicoP_LaserE intToLaser(int color) {
        switch (color) {
            case 0:
                return PicoP_LaserE.eLASER_RED_1;
            case 1:
                return PicoP_LaserE.eLASER_GREEN_1;
            case 2:
                return PicoP_LaserE.eLASER_BLUE_1;
            case 3:
                return PicoP_LaserE.eLASER_RED_2;
            case 4:
                return PicoP_LaserE.eLASER_GREEN_2;
            default:
                return PicoP_LaserE.eLASER_RED_1;
        }
    }

    public static PicoP_OutputVideoStateE intToOutputVideoState(int state) {
        switch (state) {
            case 0:
                return PicoP_OutputVideoStateE.eOUTPUT_VIDEO_DISABLED;
            case 1:
                return PicoP_OutputVideoStateE.eOUTPUT_VIDEO_ENABLED;
            default:
                return PicoP_OutputVideoStateE.eOUTPUT_VIDEO_DISABLED;
        }

    }

    public static PicoP_InputVideoStateE intToInputVideoState(int state) {
        switch (state) {
            case 0:
                return PicoP_InputVideoStateE.eINPUT_VIDEO_DISABLED;
            case 1:
                return PicoP_InputVideoStateE.eINPUT_VIDEO_ENABLED;
            default:
                return PicoP_InputVideoStateE.eINPUT_VIDEO_DISABLED;
        }
    }

    public static PicoP_SensingStateE intToSensingState(int state) {
        switch (state) {
            case 0:
                return PicoP_SensingStateE.eSENSING_DISABLED;
            case 1:
                return PicoP_SensingStateE.eSENSING_ENABLED;
            default:
                return PicoP_SensingStateE.eSENSING_DISABLED;
        }
    }

    public static PicoP_ValueStorageTypeE intToStorageType(int type) {
        switch (type) {
            case 0:
                return PicoP_ValueStorageTypeE.eCURRENT_VALUE;
            case 1:
                return PicoP_ValueStorageTypeE.eFACTORY_VALUE;
            default:
                return PicoP_ValueStorageTypeE.eVALUE_ON_STARTUP;
        }

    }

    public static PicoP_GammaBoostModeE intToGammaBoostMode(int type) {
        switch (type) {
            case 0:
                return PicoP_GammaBoostModeE.eGAMMA_BOOST_MODE_NONE;
            case 1:
                return PicoP_GammaBoostModeE.eGAMMA_BOOST_MODE_VIRTUAL_LUMENS_30PCT;
            default:
                return PicoP_GammaBoostModeE.eGAMMA_BOOST_MODE_NONE;
        }

    }

    public static PicoP_FlipStateE intToFlipStateType(int type) {
        switch (type) {
            case 0:
                return PicoP_FlipStateE.eFLIP_NEITHER;
            case 1:
                return PicoP_FlipStateE.eFLIP_HORIZONTAL;
            case 2:
                return PicoP_FlipStateE.eFLIP_VERTICAL;
            case 3:
                return PicoP_FlipStateE.eFLIP_BOTH;
            default:
                return PicoP_FlipStateE.eFLIP_NEITHER;
        }

    }

    public static PicoP_ColorModeE intToColorMode(int type) {
        switch (type) {
            case 0:
                return PicoP_ColorModeE.eCOLOR_MODE_STANDARD;
            case 1:
                return PicoP_ColorModeE.eCOLOR_MODE_BRILLIANT;
            case 2:
                return PicoP_ColorModeE.eCOLOR_MODE_RICH;
            case 3:
                return PicoP_ColorModeE.eCOLOR_MODE_INVERTED;
            case 4:
                return PicoP_ColorModeE.eCOLOR_MODE_CUSTOM;
            default:
                return PicoP_ColorModeE.eCOLOR_MODE_STANDARD;
        }

    }

    public static PicoP_HorizontalBrightnessCompensationModeE intToBrightnessCompensationModeE(int type) {
        switch (type) {
            case 0:
                return PicoP_HorizontalBrightnessCompensationModeE.eHBC_OFF;
            case 1:
                return PicoP_HorizontalBrightnessCompensationModeE.eHBC_MEDIUM;
            case 2:
                return PicoP_HorizontalBrightnessCompensationModeE.eHBC_UNIFORM_BRIGHTNESS;
            default:
                return PicoP_HorizontalBrightnessCompensationModeE.eHBC_OFF;
        }

    }

    public static PicoP_MicroWeaveE intToMicroweaveModeE(int type) {
        switch (type) {
            case 0:
                return PicoP_MicroWeaveE.eMICROWEAVE_OFF;
            case 1:
                return PicoP_MicroWeaveE.eMICROWEAVE_ON;
            default:
                return PicoP_MicroWeaveE.eMICROWEAVE_OFF;
        }

    }

}
