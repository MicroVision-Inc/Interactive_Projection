// ****************************************************************************
// main.cpp
//
// Simple Linux console app demonstrating how to use display APIs
//
// Copyright (C) 2018-2019 MicroVision, Inc.
// This source code is subject to the Microvision Source Code License.
//
// THIS CODE IS FOR GUIDANCE ONLY. IT IS INTENDED AS AN EDUCATIONAL SAMPLE DEMONSTRATING
// SIMPLIFIED USE OF THE MICROVISION PRODUCT. THE CODE AND INFORMATION ARE PROVIDED "AS IS"
// WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "picop_api.h"
#include "picop_def.h"

using std::cout;
using std::endl;
using std::make_shared;
using std::shared_ptr;
using std::setw;

static PicoP_HANDLE library_handle_ = NULL; 
static PicoP_HANDLE connection_handle_ = NULL;


enum class ControlModeE {
    BRIGHTNESS_CONTROL_MODE,
    COLOR_CONTROL_MODE,
    FLIP_CONTROL_MODE,
    TEST_CONTROL_PATTERN_MODE,
    EVENT_LOG_MODE,
};

void usage()
{
    cout << "USAGE:" << endl;
    cout << "console_demo" << endl;
}

void print_key_processing()
{
    cout << endl << "****************************** Main Menu ******************************" << endl;
    cout << endl;
    cout << " This application demonstrates a few PSE SDK calls" << endl;
    cout << " Select one of the following modes using the keypad" << endl;
    cout << " All modes except system status (s) allow value iteration using the up/down keys" << endl;
    cout << " System Status (s) just reports the system status" << endl;
    cout << endl;

    cout << "  q - quit" << endl;
    cout << "  b - place in brightness mode" << endl;
    cout << "  c - place in color mode" << endl;
    cout << "  e - event log" << endl;
    cout << "  f - place in flip display mode" << endl;
    cout << "  s - system status" << endl;
    cout << "  t - place in test pattern mode" << endl;
    cout << "  <up key> - increment mode setting" << endl;
    cout << "  <down key> - decrement mode setting" << endl;
    cout << "  H - Print this menu" << endl;
    cout << endl;
    cout << "*********************************************************************"  << endl;
}

static void process_key_entry()
{
    int ch;
    struct termios t;
    PicoP_ReturnStatus picop_rc;

    PicoP_ColorModeE color_mode = eCOLOR_MODE_STANDARD;
    PicoP_FlipStateE flip_state = eFLIP_NEITHER;
    ControlModeE control_mode = ControlModeE::FLIP_CONTROL_MODE;

    uint8_t brightness = 100;
    uint8_t num_event_requests = 1;

    int test_pattern_info = eTEST_PATTERN_OFF;
    
    print_key_processing();

    if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_GetBrightness(connection_handle_, &brightness, eCURRENT_VALUE))) {
        cout << "PicoP_GetBrightness() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
        return;
    }

    if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_GetColorMode(connection_handle_, &color_mode, eCURRENT_VALUE))) {
        cout << "PicoP_GetColorMode() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
        return;
    }

    if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_GetFlipState(connection_handle_, &flip_state, eCURRENT_VALUE))) {
        cout << "PicoP_GetFlipState() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
        return;
    }

    // disable buffering
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    t.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

    while ('q' != (ch = getchar())) {

        switch (ch) {
            case 'H':
                print_key_processing();
                break;
            case 'b':
                cout << "placed in brightness mode" << endl;
                control_mode = ControlModeE::BRIGHTNESS_CONTROL_MODE;
                break;
            case 'c':
                cout << "placed in color mode" << endl;
                control_mode = ControlModeE::COLOR_CONTROL_MODE;
                break;
            case 'e':
                cout << "event log mode" << endl;
                control_mode = ControlModeE::EVENT_LOG_MODE;
                break;
            case 'f':
                cout << "placed in flip mode" << endl;
                control_mode = ControlModeE::FLIP_CONTROL_MODE;
                break;
            case 's':
                PicoP_SystemStatusS system_status;
                if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetSystemStatus(connection_handle_, &system_status))) {

                    cout << "system status:" << endl;
                    switch(system_status.systemState) {
                        case eON:
                            cout << "eON" << endl;
                            break;
                        case eSTARTING:
                            cout << "eSTARTING" << endl;
                            break;
                        case eCALIBRATION:
                            cout << "eCALIBRATION" << endl;
                            break;
                        case eFAULT:
                            cout << "eFAULT" << endl;
                            break;
                        default:
                            cout << "invalid state" << endl;
                            break;
                    }
                    cout << "systemFault: " << system_status.systemFault << endl;
                    cout << "temperature: " << system_status.temperature << endl;
                } else {
                    cout << "PicoP_GetSystemStatus() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
                }
                break;
            case 't':
                cout << "placed in test pattern mode" << endl;
                control_mode = ControlModeE::TEST_CONTROL_PATTERN_MODE;
                break;
            case 27:
                // up arrow
                if (91 == getchar()) {
                    bool advance_mode;
                    int key_val = getchar();
                    if (65 == key_val) {
                        advance_mode = true;
                    } else if (66 == key_val) {
                        advance_mode = false;
                    } else {
                        break;
                    }

                    switch(control_mode) {

                        case ControlModeE::BRIGHTNESS_CONTROL_MODE:
                            if (advance_mode) {
                                if (brightness < 100) brightness++;
                            } else {
                                if (brightness > 0) brightness--;
                            }
                            if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetBrightness(connection_handle_, brightness, false))) {
                                cout << "brightness = " << unsigned(brightness) << endl;

                            } else {
                                cout << "PicoP_SetBrightness() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
                            }
                            break;

                        case ControlModeE::COLOR_CONTROL_MODE:
                            {
                                switch (color_mode) {
                                    case eCOLOR_MODE_BRILLIANT:
                                        color_mode = (advance_mode) ? eCOLOR_MODE_STANDARD : eCOLOR_MODE_STANDARD;
                                        break;
                                    case eCOLOR_MODE_STANDARD:
                                        color_mode = (advance_mode) ? eCOLOR_MODE_RICH : eCOLOR_MODE_BRILLIANT;
                                        break;
                                    // not currently supported
                                    case eCOLOR_MODE_INVERTED:
                                        color_mode = (advance_mode) ? eCOLOR_MODE_RICH : eCOLOR_MODE_STANDARD;
                                        break;
                                    case eCOLOR_MODE_RICH:
                                        color_mode = (advance_mode) ? eCOLOR_MODE_BRILLIANT : eCOLOR_MODE_STANDARD;
                                        break;
                                    case eCOLOR_MODE_CUSTOM:
                                        color_mode = (advance_mode) ? eCOLOR_MODE_BRILLIANT : eCOLOR_MODE_RICH;
                                        break;
                                }

                                cout << "color mode = ";
                                switch (color_mode) {
                                    case eCOLOR_MODE_BRILLIANT:
                                        cout << "brilliant" << endl;
                                        break;
                                    case eCOLOR_MODE_STANDARD:
                                        cout << "standard" << endl;
                                        break;
                                    case eCOLOR_MODE_INVERTED:
                                        cout << "inverted" << endl;
                                        break;
                                    case eCOLOR_MODE_RICH:
                                        cout << "rich" << endl;
                                        break;
                                    case eCOLOR_MODE_CUSTOM:
                                         cout << "custom" << endl;
                                         break;
                                }

                                if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_SetColorMode(connection_handle_, color_mode, false))) {
                                    cout << "PicoP_SetColorMode() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
                                }
                            }
                            break;

                        case ControlModeE::FLIP_CONTROL_MODE:
                            {
                                switch (flip_state) {
                                    case eFLIP_NEITHER:
                                        flip_state = (advance_mode) ? eFLIP_HORIZONTAL : eFLIP_BOTH;
                                        break;
                                    case eFLIP_HORIZONTAL:
                                        flip_state = (advance_mode) ? eFLIP_VERTICAL : eFLIP_NEITHER;
                                        break;
                                    case eFLIP_VERTICAL:
                                        flip_state = (advance_mode) ? eFLIP_BOTH : eFLIP_HORIZONTAL;
                                        break;
                                    case eFLIP_BOTH:
                                        flip_state = (advance_mode) ? eFLIP_NEITHER : eFLIP_VERTICAL;
                                        break;
                                }
                                
                                if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetFlipState(connection_handle_, flip_state, false))) {

                                    cout << "flipped ";
                                    switch (flip_state) {
                                        case eFLIP_NEITHER:
                                            cout << "neither" << endl;
                                            break;
                                        case eFLIP_HORIZONTAL:
                                            cout << "horizontally" << endl;
                                            break;
                                        case eFLIP_VERTICAL:
                                            cout << "vertically" << endl;
                                            break;
                                        case eFLIP_BOTH:
                                            cout << "horzontally and vertically" << endl;
                                            break;
                                    }

                                } else {
                                    cout << "PicoP_SetFlipState() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
                                }
                            }
                            break;

                        case ControlModeE::TEST_CONTROL_PATTERN_MODE:
                            {
                                test_pattern_info = (advance_mode) ? (test_pattern_info+1) : (test_pattern_info-1);
                                if (test_pattern_info < eTEST_PATTERN_OFF) test_pattern_info = eCONSTANT_COLOR_PATTERN;
                                if (test_pattern_info > eCONSTANT_COLOR_PATTERN) test_pattern_info = eTEST_PATTERN_OFF;

                                if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_DrawTestPattern(connection_handle_, 
                                        static_cast<PicoP_TestPatternE>(test_pattern_info),
                                        ((100u << 24) | (100u << 16) | (100u << 8) | 0),
                                        ((0u << 24) | (0u << 16) | (0u << 8) | 0) ))) {

                                    cout << "test pattern : ";
                                    switch (test_pattern_info) {
                                        case eTEST_PATTERN_OFF:
                                            cout << "no test pattern" << endl;
                                            break;
                                        case eCHECKERBOARD_PATTERN:
                                            cout << "checker board" << endl;
                                            break;
                                        case eCROSSHAIR_PATTERN:
                                            cout << "cross hair" << endl;
                                            break;
                                        case eGRID_PATTERN:
                                            cout << "grid" << endl;
                                            break;
                                        case eCONSTANT_COLOR_PATTERN:
                                            cout << "constant color" << endl;
                                            break;
                                    }

                                } else {
                                    cout << "PicoP_DrawTestPattern() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
                                }
                            }
                            break;

                        case ControlModeE::EVENT_LOG_MODE: 
                            {
                                if (advance_mode) {
                                    if (200 == num_event_requests) num_event_requests = 0;
                                    else num_event_requests += 1;
                                } else {
                                    if (0 == num_event_requests) num_event_requests = 200;
                                    else num_event_requests -= 1;
                                }

                                auto shared_event_array = make_shared<PicoP_EventS*>(new PicoP_EventS[num_event_requests]);

                                if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetEventLog(connection_handle_, num_event_requests, *shared_event_array))) {

                                    cout << endl << unsigned(num_event_requests)  << " events" << endl;

                                    for (int i = 0; i < num_event_requests; i++) {

                                        UINT16 session = (*shared_event_array)[i].session;
                                        UINT16 eventId = (*shared_event_array)[i].eventId;
                                        UINT32 time = (*shared_event_array)[i].time;
                                        UINT32 data = (*shared_event_array)[i].data;
                                        UINT16 cid = (*shared_event_array)[i].cid;
                                        UINT16 line = (*shared_event_array)[i].line;

                                        cout << "session : " << setw(4) << unsigned(session);
                                        cout << ",   eventId : " << setw(6) << unsigned(eventId);
                                        cout << ",   time : " << setw(10) << time;
                                        cout << ",   data : " << setw(10) << data;
                                        cout << ",   cid : " << setw(4) << cid;
                                        cout << ",   line : " << setw(8) << line << endl;
                                    }

                                } else {
                                    cout << "PicoP_GetEventLog() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
                                }

                            }
                            break;

                        default:
                            break;
                    }
                }
                break;
            default:
                break;
        }
    }

    // enable buffering
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ICANON;
    t.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

    cout << "exit" << endl;
}

int main(int argc, __attribute__ ((unused)) char *argv[])
{
    cout << "GIT VERSION : " << VERSION << endl;

    if (argc != 1) {
        usage();
        return 0;
    }

    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_OpenLibrary(&library_handle_))) {

        PicoP_ConnectionInfoS connection_information;
        connection_information.connectionType = eUSB;
        connection_information.usbInfo.productID = 0x0004;
        connection_information.usbInfo.serialNumber = "serial";

        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_OpenConnection(library_handle_, connection_information, &connection_handle_))) {

            PicoP_LibraryInfoS picop_library_info;
            if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetLibraryInfo(connection_handle_, &picop_library_info))) {

                // qualified the connection and library version ... continue with commands
                cout << "\n\n\nLibrary Information :" << endl;
                cout << "  Major - " << std::to_string(picop_library_info.majorVersion) << endl;
                cout << "  Minor - " << std::to_string(picop_library_info.minorVersion) << endl;
                cout << "  Patch - " << std::to_string(picop_library_info.patchVersion) << endl;
                cout << "  Capabilities - " << picop_library_info.capabilityFlags << endl;

                PicoP_SystemInfoS picop_system_info;
                if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetSystemInfo(connection_handle_, &picop_system_info))) {

                    cout << "\nSystem Information :" << endl;
                    cout << "  System Serial number - " << picop_system_info.serialNumber << endl;
                    cout << "  FW Version - " << picop_system_info.firmwareVersion << endl;
                    cout << "  Electronics Version - " << picop_system_info.electronicsVersion << endl;

                    process_key_entry();

                    if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_CloseConnection(connection_handle_))) {
                        cout << "PicoP_CloseConnection() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
                    }
                    if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_CloseLibrary(library_handle_))) {
                        cout << "PicoP_CloseConnection() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
                    }

                } else {
                    cout << "PicoP_GetSystemInfo() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
                }
            } else {
                cout << "PicoP_GetLibraryInfo() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
            }
        } else {
            cout << "PicoP_OpenConnection() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
            if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_CloseLibrary(library_handle_))) {
                cout << "PicoP_CloseLibrary() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
            }
        }
    } else {
        cout << "PicoP_OpenLibrary() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
    }

    return picop_rc;
}
