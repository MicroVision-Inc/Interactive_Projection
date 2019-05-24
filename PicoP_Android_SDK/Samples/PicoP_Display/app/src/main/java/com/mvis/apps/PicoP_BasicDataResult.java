package com.mvis.apps;

public class PicoP_BasicDataResult {
    public static int brightness;
    public static int colormode;
    public static int keyStoneCorrectionValue;
    public static float offset;
    public static int phaseValue;
    public static float gammaval;
    public static int inputstate;
    public static int outputstate;
    public static float frameRate;
    public static int lines;
    public static int horizontalPixels;
    public static int microweaveMode;
    public static int flipState;
    public static int coefficient;
    public static int videoMode;
    public static int renderTarget;
    public static int splashScreenTimeout;
    public static int gammaMode;
    public static int horizontalBrightnessCompensationMode;
    public static float horizScanAngle;
    public static float verticalScanAngle;
    public static int horizontalRes;
    public static int verticalRes;
    public static float outputFrameRate;
    public static int frameratemode;
    public static int vsmode;
    public static int hsmode;
    public static int sensingstate;

    public PicoP_BasicDataResult() {
        brightness = 0;
        colormode = 0;
        keyStoneCorrectionValue = 0;
        offset = 0;
        phaseValue = 0;
        gammaval = 0;
        inputstate = 0;
        outputstate = 0;
        frameRate = 0;
        lines = 0;
        horizontalPixels = 0;
        microweaveMode = 0;
        flipState = 0;
        coefficient = 0;
        videoMode = 3;
        renderTarget = 0;
        splashScreenTimeout = 0;
        gammaMode = 0;
        horizontalBrightnessCompensationMode = 0;
        horizScanAngle = 0.0f;
        verticalScanAngle = 0.0f;
        frameratemode = 0;
        vsmode = 0;
        hsmode = 0;
        sensingstate = 0;


    }

    public static int getMicroweaveMode() {
        return microweaveMode;
    }

    public static void setMicroweaveMode(int _microweaveMode) {
        microweaveMode = _microweaveMode;
    }

    public static int getFlipState() {
        return flipState;
    }

    public static void setFlipState(int _flipState) {
        flipState = _flipState;
    }

    public static int getCoefficient() {
        return coefficient;
    }

    public static void setCoefficient(int _coefficient) {
        coefficient = _coefficient;
    }

    public static int getFrameRateMode() {
        return frameratemode;
    }

    public static int getVSMode() {
        return vsmode;
    }

    public static int getHSmode() {
        return hsmode;
    }

    public static int getBrightness() {
        return brightness;
    }

    public static void setBrightness(int _brightness) {
        brightness = _brightness;
    }

    public static int getColormode() {
        return colormode;
    }

    public static void setColormode(int _colormode) {
        colormode = _colormode;
    }

    public static float getOffset() {
        return offset;
    }

    public static void setOffset(float _offset) {
        offset = _offset;
    }

    public static int getPhaseValue() {
        return phaseValue;
    }

    public static void setPhaseValue(int _phaseValue) {
        phaseValue = _phaseValue;
    }

    public static float getGammaval() {
        return gammaval;
    }

    public static void setGammaval(float _gammaval) {
        gammaval = _gammaval;
    }

    public static int getInputstate() {
        return inputstate;
    }

    public static void setInputstate(int _state) {
        inputstate = _state;
    }

    public static int getOutputstate() {
        return outputstate;
    }

    public static void setOutputstate(int _state) {
        outputstate = _state;
    }

    public static float getFrameRate() {
        return frameRate;
    }

    public static int getLines() {
        return lines;
    }

    public static int gethorizontalPixels() {
        return horizontalPixels;
    }

    public static float getOutputFrameRate() {
        return outputFrameRate;
    }

    public static int getHorizontalRes() {
        return horizontalRes;
    }

    public static int getVerticalRes() {
        return verticalRes;
    }

    public static int getGammaBoostMode() {
        return gammaMode;
    }

    public static void setGammaBoostMode(int _mode) {
        gammaMode = _mode;
    }

    public static int getHorizontalBrightnessCompensationMode() {
        return horizontalBrightnessCompensationMode;
    }

    public static void setHorizontalBrightnessCompensationMode(int _mode) {
        horizontalBrightnessCompensationMode = _mode;
    }

    public static float getVerticalScanAngle() {
        return verticalScanAngle;
    }

    public static void setVerticalScanAngle(float _angle) {
        verticalScanAngle = _angle;
    }

    public static float getHorizontalScanAngle() {
        return horizScanAngle;
    }

    public static void setHorizontalScanAngle(float _angle) {
        horizScanAngle = _angle;
    }

    public static int getSensingstate() {
        return sensingstate;
    }

    public static void setSensingstate(int _state) {
        sensingstate = _state;
    }

}
