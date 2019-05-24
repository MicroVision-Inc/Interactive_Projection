// ****************************************************************************
// main.cpp
//
// Simple Linux GUI app demonstrating how to use display APIs
//
// Copyright (C) 2018-2019 MicroVision, Inc.
// This source code is subject to the Microvision Source Code License.
//
// THIS CODE IS FOR GUIDANCE ONLY. IT IS INTENDED AS AN EDUCATIONAL SAMPLE DEMONSTRATING
// SIMPLIFIED USE OF THE MICROVISION PRODUCT. THE CODE AND INFORMATION ARE PROVIDED "AS IS"
// WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#include <gtkmm.h>
#include <gtkmm/application.h>
#include <mutex>
#include <stdbool.h>
#include <sstream>

#include "picop_api.h"
#include "picop_def.h"

#if defined(NDEBUG) 
#define LOG(FMT, ...)
#define ENTER()
#define EXIT()
#else
#define LOG(FMT, ...) printf("[%d:%s:%s]:" FMT, __LINE__, __FILE__, __FUNCTION__, ##__VA_ARGS__)
#define ENTER() printf("[%d:%s:%s]: enter\n", __LINE__, __FILE__, __FUNCTION__)
#define EXIT() printf("[%d:%s:%s]: exit\n", __LINE__, __FILE__, __FUNCTION__)
#endif
#define LOG_E(FMT, ...) printf("********* ERROR [%d:%s:%s]:" FMT, __LINE__, __FILE__, __FUNCTION__, ##__VA_ARGS__)

static Gtk::Window* main_window_ = nullptr;

using namespace std;

static PicoP_HANDLE library_handle_ = NULL;
static PicoP_HANDLE connection_handle_ = NULL;

enum CommandE {
    COMMAND_READ_LIBRARY_INFO,
    COMMAND_READ_SYSTEM_INFO,
    COMMAND_SET_GET_INPUT_VIDEO_STATE,
    COMMAND_GET_INPUT_VIDEO_PROPERTIES,
    COMMAND_SET_GET_OUTPUT_VIDEO_STATE,
    COMMAND_GET_OUTPUT_VIDEO_PROPERTIES,
    COMMAND_GET_SET_FRAME_RATE_MODE,
    COMMAND_SET_GET_BRIGHTNESS,
    COMMAND_SET_GET_COLOR_MODE,
    COMMAND_SET_GET_COLOR_ALIGNMENT,
    COMMAND_SET_GET_GAMMA,
    COMMAND_SET_GET_GAMMA_BOOST_MODE,
    COMMAND_SET_GET_MICROWEAVE_MODE,
    COMMAND_SET_GET_PHASE,
    COMMAND_FLIP_STATE,
    COMMAND_TEST_PATTERN,
    COMMAND_SET_GET_SENSING_STATE,
    COMMAND_UTILITIES,
    NUM_COMMANDS,
};

struct CommandView {
    CommandE command;
    Gtk::RadioButton* button;
    Gtk::Box* layout;
};


static int open_connection()
{
    int retval = 0;

    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_OpenLibrary(&library_handle_))) {

        PicoP_ConnectionInfoS picop_connection_info;
        picop_connection_info.connectionType = eUSB;
        picop_connection_info.usbInfo.productID = 0x0004;
        picop_connection_info.usbInfo.serialNumber = "serial";
        
        if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_OpenConnection(library_handle_, picop_connection_info, &connection_handle_))) {
            LOG_E("PicoP_OpenConnection() failure : picop_rc = %d\n", picop_rc);
            retval = -1;
        }
    } else {
        LOG_E("PicoP_OpenLibrary() failure : picop_rc = %d\n", picop_rc);
        retval = -1;
    }

    return retval;
}


static void close_connection()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_CloseConnection(connection_handle_))) {
        if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_CloseLibrary(library_handle_))) {
            LOG_E("PicoP_CloseLibrary() failure : picop_rc = %d\n", picop_rc);
        }
    } else {
        LOG_E("PicoP_CloseConnection() failure : picop_rc = %d\n", picop_rc);
    }
}

string failure_string(PicoP_ReturnStatus status)
{
    stringstream stream;
    stream << "failure: 0x" << hex << status;
    return stream.str();
}



// ****************************************************************************
//  PicoP_GetLibraryInfo
// ****************************************************************************

static PicoP_LibraryInfoS libraryInfo_;
static Gtk::Label* lib_info_cmd_status_val_ = nullptr;
static Gtk::Label* lib_info_major_version_val_ = nullptr;
static Gtk::Label* lib_info_minor_version_val_ = nullptr;
static Gtk::Label* lib_info_patch_version_val_ = nullptr;
static Gtk::Label* lib_info_capability_flags_val_ = nullptr;


static void library_info()
{
    PicoP_LibraryInfoS libraryInfo;
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetLibraryInfo(library_handle_, &libraryInfo))) {

        lib_info_cmd_status_val_->set_text("success");
        lib_info_major_version_val_->set_text(to_string(libraryInfo.majorVersion));
        lib_info_minor_version_val_->set_text(to_string(libraryInfo.minorVersion));
        lib_info_patch_version_val_->set_text(to_string(libraryInfo.patchVersion));
        lib_info_capability_flags_val_->set_text(to_string(libraryInfo.capabilityFlags));
    
    } else {
        LOG("PicoP_GetLibraryInfo() %s\n", failure_string(picop_rc).c_str());

        lib_info_cmd_status_val_->set_text(failure_string(picop_rc));
        lib_info_major_version_val_->set_text("");
        lib_info_minor_version_val_->set_text("");
        lib_info_patch_version_val_->set_text("");
        lib_info_capability_flags_val_->set_text("");
    }
}



// ****************************************************************************
//  PicoP_GetSystemInfo
// ****************************************************************************

static PicoP_SystemInfoS system_info_;
static Gtk::Label* sys_info_cmd_status_val_ = nullptr;
static Gtk::Label* sys_info_serial_number_val_ = nullptr;
static Gtk::Label* sys_info_firmware_version_val_ = nullptr;
static Gtk::Label* sys_info_electronics_version_val_ = nullptr;


static void get_system_info()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetSystemInfo(connection_handle_, &system_info_))) {

        // if (FALSE == g_utf8_validate (system_info_.serialNumber, 16, NULL)) {
        if (FALSE == g_utf8_validate(system_info_.serialNumber, -1, NULL)) {
            strncpy(system_info_.serialNumber, "invalid utf-8", 17);
            LOG_E("serial number string not utf-8 ascii\n");
        }

        sys_info_cmd_status_val_->set_text("success");
        sys_info_serial_number_val_->set_text(system_info_.serialNumber);
        sys_info_firmware_version_val_->set_text(to_string(system_info_.firmwareVersion));
        sys_info_electronics_version_val_->set_text(to_string(system_info_.electronicsVersion));

    } else {

        LOG_E("PicoP_GetSystemInfo() failure : picop_rc = %d\n", picop_rc);

        sys_info_cmd_status_val_->set_text(failure_string(picop_rc));
        sys_info_serial_number_val_->set_text("");
        sys_info_firmware_version_val_->set_text("");
        sys_info_electronics_version_val_->set_text("");
    }
}




// ****************************************************************************
//  PicoP_<Get/Set>InputVideoState
// ****************************************************************************

static Gtk::Label* input_video_state_cmd_status_val_ = nullptr;

static PicoP_ValueStorageTypeE input_video_state_storage_type_ = eCURRENT_VALUE;
static Gtk::Button* input_video_state_memory_space_startup_ = nullptr;
static Gtk::Button* input_video_state_memory_space_factory_ = nullptr;
static Gtk::Button* input_video_state_memory_commit_ = nullptr;

static Gtk::RadioButton* input_video_state_disabled_ = nullptr;
static Gtk::RadioButton* input_video_state_enabled_ = nullptr;

static void get_input_video_state()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    PicoP_InputVideoStateE input_video_state;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetInputVideoState(connection_handle_, &input_video_state, input_video_state_storage_type_))) {

        switch (input_video_state) {
            case eINPUT_VIDEO_DISABLED:
                input_video_state_disabled_->set_active(true);
                break;
            case eINPUT_VIDEO_ENABLED:
                input_video_state_enabled_->set_active(true);
                break;
            default:
                break;
        }

        input_video_state_cmd_status_val_->set_text("success");

    } else {
        input_video_state_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_input_video_state_memory_space_startup_button_clicked()
{
    input_video_state_storage_type_ = eVALUE_ON_STARTUP;
    get_input_video_state();
}


static void on_input_video_state_memory_space_factory_button_clicked()
{
    input_video_state_storage_type_ = eFACTORY_VALUE;
    get_input_video_state();
}


static void on_input_video_state_memory_commit_button_clicked()
{
    PicoP_InputVideoStateE input_video_state;
    if (input_video_state_disabled_->get_active()) {
        input_video_state = eINPUT_VIDEO_DISABLED;
    } else {
        input_video_state = eINPUT_VIDEO_ENABLED;
    }

    PicoP_ReturnStatus picop_rc;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetInputVideoState(connection_handle_, input_video_state, 1))) {
        input_video_state_cmd_status_val_->set_text("success");
    } else {
        input_video_state_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_input_video_state_disabled_radio_button_change()
{
    if (input_video_state_disabled_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetInputVideoState(connection_handle_, eINPUT_VIDEO_DISABLED, 0))) {
            input_video_state_cmd_status_val_->set_text("success");
        } else {
            input_video_state_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_input_video_state_enabled_radio_button_change()
{
    if (input_video_state_enabled_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetInputVideoState(connection_handle_, eINPUT_VIDEO_ENABLED, 0))) {
            input_video_state_cmd_status_val_->set_text("success");
        } else {
            input_video_state_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}




// ****************************************************************************
//  PicoP_GetInputVideoProperties
// ****************************************************************************

static Gtk::Label* input_video_properties_cmd_status_val_ = nullptr;
static Gtk::Label* input_video_properties_horz_res_val_ = nullptr;
static Gtk::Label* input_video_properties_vert_res_val_ = nullptr;
static Gtk::Label* input_video_properties_frame_rate_val_ = nullptr;

static void get_input_video_properties()
{
    UINT16 horizontal_pixels, vertical_lines;
    FP32 frame_rate;

    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetInputVideoProperties(connection_handle_, 
            &horizontal_pixels, 
            &vertical_lines, 
            &frame_rate))) {

        input_video_properties_cmd_status_val_->set_text("success");
        input_video_properties_horz_res_val_->set_text(to_string(horizontal_pixels));
        input_video_properties_vert_res_val_->set_text(to_string(vertical_lines));
        input_video_properties_frame_rate_val_->set_text(to_string(frame_rate));

    } else {

        LOG_E("PicoP_GetInputVideoProperties() failure : picop_rc = %d\n", picop_rc);

        input_video_properties_cmd_status_val_->set_text(failure_string(picop_rc));
        input_video_properties_horz_res_val_->set_text("");
        input_video_properties_vert_res_val_->set_text("");
        input_video_properties_frame_rate_val_->set_text("");
    }
}




// ****************************************************************************
//  PicoP_<Get/Set>OutputVideoState
// ****************************************************************************

static Gtk::Label* output_video_state_cmd_status_val_ = nullptr;

static PicoP_ValueStorageTypeE output_video_state_storage_type_ = eCURRENT_VALUE;
static Gtk::Button* output_video_state_memory_space_startup_ = nullptr;
static Gtk::Button* output_video_state_memory_space_factory_ = nullptr;
static Gtk::Button* output_video_state_memory_commit_ = nullptr;

static Gtk::RadioButton* output_video_state_disabled_ = nullptr;
static Gtk::RadioButton* output_video_state_enabled_ = nullptr;


static void get_output_video_state()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    PicoP_OutputVideoStateE output_video_state;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetOutputVideoState(connection_handle_, &output_video_state, output_video_state_storage_type_))) {

        switch (output_video_state) {
            case eOUTPUT_VIDEO_DISABLED:
                output_video_state_disabled_->set_active(true);
                break;
            case eOUTPUT_VIDEO_ENABLED:
                output_video_state_enabled_->set_active(true);
                break;
            default:
                break;
        }

        output_video_state_cmd_status_val_->set_text("success");

    } else {
        output_video_state_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_output_video_state_memory_space_startup_button_clicked()
{
    output_video_state_storage_type_ = eVALUE_ON_STARTUP;
    get_output_video_state();
}


static void on_output_video_state_memory_space_factory_button_clicked()
{
    output_video_state_storage_type_ = eFACTORY_VALUE;
    get_output_video_state();
}


static void on_output_video_state_memory_commit_button_clicked()
{
    PicoP_OutputVideoStateE output_video_state;
    if (output_video_state_disabled_->get_active()) {
        output_video_state = eOUTPUT_VIDEO_DISABLED;
    } else {
        output_video_state = eOUTPUT_VIDEO_ENABLED;
    }

    PicoP_ReturnStatus picop_rc;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetOutputVideoState(connection_handle_, output_video_state, 1))) {
        output_video_state_cmd_status_val_->set_text("success");
    } else {
        output_video_state_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_output_video_state_disabled_radio_button_change()
{
    if (output_video_state_disabled_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetOutputVideoState(connection_handle_, eOUTPUT_VIDEO_DISABLED, 0))) {
            output_video_state_cmd_status_val_->set_text("success");
        } else {
            output_video_state_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_output_video_state_enabled_radio_button_change()
{
    if (output_video_state_enabled_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetOutputVideoState(connection_handle_, eOUTPUT_VIDEO_ENABLED, 0))) {
            output_video_state_cmd_status_val_->set_text("success");
        } else {
            output_video_state_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}




// ****************************************************************************
//  PicoP_GetOutputVideoProperties
// ****************************************************************************

static PicoP_OutputVideoPropertiesS output_video_properties_;
static Gtk::Label* output_video_properties_cmd_status_val_ = nullptr;
static Gtk::Label* output_video_properties_horz_res_val_ = nullptr;
static Gtk::Label* output_video_properties_vert_res_val_ = nullptr;
static Gtk::Label* output_video_properties_frame_rate_val_ = nullptr;

static void get_output_video_properties()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetOutputVideoProperties(connection_handle_, &output_video_properties_))) {

        output_video_properties_cmd_status_val_->set_text("success");
        output_video_properties_horz_res_val_->set_text(to_string(output_video_properties_.horizontalResolution));
        output_video_properties_vert_res_val_->set_text(to_string(output_video_properties_.verticalResolution));
        output_video_properties_frame_rate_val_->set_text(to_string(output_video_properties_.frameRate));

    } else {

        LOG_E("PicoP_GetOutputVideoProperties() failure : picop_rc = %d\n", picop_rc);

        output_video_properties_cmd_status_val_->set_text(failure_string(picop_rc));
        output_video_properties_horz_res_val_->set_text("");
        output_video_properties_vert_res_val_->set_text("");
        output_video_properties_frame_rate_val_->set_text("");
    }
}





// ****************************************************************************
//  PicoP_<Get/Set>FrameRateMode
// ****************************************************************************

static PicoP_ValueStorageTypeE frame_rate_mode_storage_type_ = eCURRENT_VALUE;
static Gtk::Label* frame_rate_mode_cmd_status_val_ = nullptr;
static Gtk::Button* frame_rate_mode_memory_space_startup_ = nullptr;
static Gtk::Button* frame_rate_mode_memory_space_factory_ = nullptr;
static Gtk::Button* frame_rate_mode_memory_commit_ = nullptr;

static PicoP_FrameRateModeS frame_rate_mode_;

static Gtk::RadioButton* frame_rate_60hz_720p_ = nullptr;
static Gtk::RadioButton* frame_rate_70hz_640p_ = nullptr;
static Gtk::RadioButton* frame_rate_70hz_600p_ = nullptr;

static Gtk::RadioButton* vertical_scaling_down_ = nullptr;
static Gtk::RadioButton* vertical_scaling_crop_bottom_ = nullptr;
static Gtk::RadioButton* vertical_scaling_crop_even_ = nullptr;

static Gtk::RadioButton* horizontal_scaling_down_ = nullptr;
static Gtk::RadioButton* horizontal_scaling_none_ = nullptr;

static void get_frame_rate_mode()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetFrameRateMode(connection_handle_, &frame_rate_mode_, frame_rate_mode_storage_type_))) {

        switch (frame_rate_mode_.frameRate) {
            case eFRAMERATE_60HZ_DISPLAY_720p_SENSING_740p:
                frame_rate_60hz_720p_->set_active(true);
                break;
            case eFRAMERATE_70HZ_DISPLAY_640p_SENSING_640p:
                frame_rate_70hz_640p_->set_active(true);
                break;
            case eFRAMERATE_70HZ_DISPLAY_600p_SENSING_640p:
                frame_rate_70hz_600p_->set_active(true);
                break;
            default:
                break;
        }

        switch (frame_rate_mode_.verticalScalingMode) {
            case eDISPLAY_VERTICAL_SCALING_DOWN:
                vertical_scaling_down_->set_active(true);
                break;
            case eDISPLAY_VERTICAL_SCALING_CROP_BOTTOM:
                vertical_scaling_crop_bottom_->set_active(true);
                break;
            case eDISPLAY_VERTICAL_SCALING_CROP_EVEN:
                vertical_scaling_crop_even_->set_active(true);
                break;
            default:
                break;
        }

        switch (frame_rate_mode_.horizontalScalingMode) {
            case eDISPLAY_HORIZONTAL_SCALING_DOWN:
                horizontal_scaling_down_->set_active(true);
                break;
            case eDISPLAY_HORIZONTAL_SCALING_NONE:
                horizontal_scaling_none_->set_active(true);
                break;
            default:
                break;
        }

        frame_rate_mode_cmd_status_val_->set_text("success");

    } else {
        frame_rate_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_frame_rate_mode_memory_space_startup_button_clicked()
{
    frame_rate_mode_storage_type_ = eVALUE_ON_STARTUP;
    get_frame_rate_mode();
}


static void on_frame_rate_mode_memory_space_factory_button_clicked()
{
    frame_rate_mode_storage_type_ = eFACTORY_VALUE;
    get_frame_rate_mode();
}


static void on_frame_rate_mode_memory_commit_button_clicked()
{
    PicoP_ReturnStatus picop_rc;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetFrameRateMode(connection_handle_, frame_rate_mode_, 1))) {
        frame_rate_mode_cmd_status_val_->set_text("success");
    } else {
        frame_rate_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_frame_rate_60hz_720p_radio_button_change()
{
    if (frame_rate_60hz_720p_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        frame_rate_mode_.frameRate = eFRAMERATE_60HZ_DISPLAY_720p_SENSING_740p;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetFrameRateMode(connection_handle_, frame_rate_mode_, 0))) {
            frame_rate_mode_cmd_status_val_->set_text("success");
        } else {
            frame_rate_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_frame_rate_70hz_640p_radio_button_change()
{
    if (frame_rate_70hz_640p_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        frame_rate_mode_.frameRate = eFRAMERATE_70HZ_DISPLAY_640p_SENSING_640p;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetFrameRateMode(connection_handle_, frame_rate_mode_, 0))) {
            frame_rate_mode_cmd_status_val_->set_text("success");
        } else {
            frame_rate_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_frame_rate_70hz_600p_radio_button_change()
{
    if (frame_rate_70hz_600p_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        frame_rate_mode_.frameRate = eFRAMERATE_70HZ_DISPLAY_600p_SENSING_640p;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetFrameRateMode(connection_handle_, frame_rate_mode_, 0))) {
            frame_rate_mode_cmd_status_val_->set_text("success");
        } else {
            frame_rate_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void  on_vertical_scaling_down_radio_button_change()
{
    if (vertical_scaling_down_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        frame_rate_mode_.verticalScalingMode = eDISPLAY_VERTICAL_SCALING_DOWN;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetFrameRateMode(connection_handle_, frame_rate_mode_, 0))) {
            frame_rate_mode_cmd_status_val_->set_text("success");
        } else {
            frame_rate_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_vertical_scaling_crop_bottom_radio_button_change()
{
    if (vertical_scaling_crop_bottom_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        frame_rate_mode_.verticalScalingMode = eDISPLAY_VERTICAL_SCALING_CROP_BOTTOM;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetFrameRateMode(connection_handle_, frame_rate_mode_, 0))) {
            frame_rate_mode_cmd_status_val_->set_text("success");
        } else {
            frame_rate_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_vertical_scaling_crop_even_radio_button_change()
{
    if (vertical_scaling_crop_even_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        frame_rate_mode_.verticalScalingMode = eDISPLAY_VERTICAL_SCALING_CROP_EVEN;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetFrameRateMode(connection_handle_, frame_rate_mode_, 0))) {
            frame_rate_mode_cmd_status_val_->set_text("success");
        } else {
            frame_rate_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_horizontal_scaling_down_radio_button_change()
{
    if (horizontal_scaling_down_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        frame_rate_mode_.horizontalScalingMode = eDISPLAY_HORIZONTAL_SCALING_DOWN;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetFrameRateMode(connection_handle_, frame_rate_mode_, 0))) {
            frame_rate_mode_cmd_status_val_->set_text("success");
        } else {
            frame_rate_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_horizontal_scaling_none_radio_button_change()
{
    if (horizontal_scaling_none_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        frame_rate_mode_.horizontalScalingMode = eDISPLAY_HORIZONTAL_SCALING_NONE;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetFrameRateMode(connection_handle_, frame_rate_mode_, 0))) {
            frame_rate_mode_cmd_status_val_->set_text("success");
        } else {
            frame_rate_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}




// ****************************************************************************
//  PicoP_<Get/Set>BrightnessVal
// ****************************************************************************

static Gtk::Label* brightness_cmd_status_val_ = nullptr;

static PicoP_ValueStorageTypeE brightness_storage_type_ = eCURRENT_VALUE;
static Gtk::Button* brightness_memory_space_startup_ = nullptr;
static Gtk::Button* brightness_memory_space_factory_ = nullptr;
static Gtk::Button* brightness_memory_commit_ = nullptr;

static Glib::RefPtr<Gtk::Adjustment> brightness_adjustment_;
static Gtk::SpinButton* brightness_val_ = nullptr;
static Gtk::Scale* brightness_scale_ = nullptr;


static void get_brightness()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    uint8_t brightness;

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetBrightness(connection_handle_, &brightness, brightness_storage_type_))) {

        brightness_cmd_status_val_->set_text("success");
        brightness_val_->set_text(to_string(brightness));
        brightness_scale_->set_value(brightness);

    } else {
        brightness_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_brightness_memory_space_startup_button_clicked()
{
    brightness_storage_type_ = eVALUE_ON_STARTUP;
    get_brightness();
}


static void on_brightness_memory_space_factory_button_clicked()
{
    brightness_storage_type_ = eFACTORY_VALUE;
    get_brightness();
}


static void on_brightness_memory_commit_button_clicked()
{
    uint8_t brightness = brightness_adjustment_->get_value();

    PicoP_ReturnStatus picop_rc;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetBrightness(connection_handle_, brightness, 1))) {
        brightness_cmd_status_val_->set_text("success");
    } else {
        brightness_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_brightness_val_changed()
{
    uint8_t brightness = brightness_adjustment_->get_value();

    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetBrightness(connection_handle_, brightness, 0))) {
        brightness_cmd_status_val_->set_text("success");
    } else {
        brightness_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}




// ****************************************************************************
//  PicoP_<Get/Set>ColorMode
// ****************************************************************************

static Gtk::Label* color_mode_cmd_status_val_ = nullptr;

static PicoP_ValueStorageTypeE color_mode_storage_type_ = eCURRENT_VALUE;
static Gtk::Button* color_mode_memory_space_startup_ = nullptr;
static Gtk::Button* color_mode_memory_space_factory_ = nullptr;
static Gtk::Button* color_mode_memory_commit_ = nullptr;

static PicoP_ColorModeE current_color_mode_ = eCOLOR_MODE_STANDARD;

static Gtk::Button* custom_color_mode_cc_memory_commit_ = nullptr;

static Gtk::RadioButton* color_mode_brilliant_ = nullptr;
static Gtk::RadioButton* color_mode_standard_ = nullptr;
static Gtk::RadioButton* color_mode_inverted_ = nullptr;
static Gtk::RadioButton* color_mode_rich_ = nullptr;
static Gtk::RadioButton* color_mode_custom_ = nullptr;

static Gtk::Label* red_to_red_val_ = nullptr;
static Gtk::Label* green_to_red_val_ = nullptr;
static Gtk::Label* blue_to_red_val_ = nullptr;
static Gtk::Label* red_to_green_val_ = nullptr;
static Gtk::Label* blue_to_green_val_ = nullptr;
static Gtk::Label* green_to_green_val_ = nullptr;
static Gtk::Label* red_to_blue_val_ = nullptr;
static Gtk::Label* green_to_blue_val_ = nullptr;
static Gtk::Label* blue_to_blue_val_ = nullptr;

static Gtk::SpinButton* red_to_red_spin_ = nullptr;
static Gtk::SpinButton* green_to_red_spin_ = nullptr;
static Gtk::SpinButton* blue_to_red_spin_ = nullptr;
static Gtk::SpinButton* red_to_green_spin_ = nullptr;
static Gtk::SpinButton* blue_to_green_spin_ = nullptr;
static Gtk::SpinButton* green_to_green_spin_ = nullptr;
static Gtk::SpinButton* red_to_blue_spin_ = nullptr;
static Gtk::SpinButton* green_to_blue_spin_ = nullptr;
static Gtk::SpinButton* blue_to_blue_spin_ = nullptr;

static void on_red_to_red_val_changed();
static void on_red_to_green_val_changed();
static void on_red_to_blue_val_changed();
static void on_green_to_red_val_changed();
static void on_green_to_green_val_changed();
static void on_green_to_blue_val_changed();
static void on_blue_to_red_val_changed();
static void on_blue_to_green_val_changed();
static void on_blue_to_blue_val_changed();

static int32_t red_to_red_cc_, green_to_red_cc_, blue_to_red_cc_;
static int32_t red_to_green_cc_, green_to_green_cc_, blue_to_green_cc_;
static int32_t red_to_blue_cc_, green_to_blue_cc_, blue_to_blue_cc_;

// static void get_color_converter_values(PicoP_ColorModeE last_color_mode, PicoP_ColorModeE current_color_mode)
static void get_color_converter_values()
{
    PicoP_ReturnStatus picop_rc;
    PicoP_ReturnStatus picop_rc_accumulative = ePICOP_RETURN_SUCCESS;

    if (eCOLOR_MODE_CUSTOM == current_color_mode_) {

        // hide const color mode cc value labels and show custom color mode value widgets

        red_to_red_val_->hide();
        green_to_red_val_->hide();
        blue_to_red_val_->hide();
        red_to_green_val_->hide();
        blue_to_green_val_->hide();
        green_to_green_val_->hide();
        red_to_blue_val_->hide();
        green_to_blue_val_->hide();
        blue_to_blue_val_->hide();

        custom_color_mode_cc_memory_commit_->show();
        red_to_red_spin_->show();
        green_to_red_spin_->show();
        blue_to_red_spin_->show();
        red_to_green_spin_->show();
        blue_to_green_spin_->show();
        green_to_green_spin_->show();
        red_to_blue_spin_->show();
        green_to_blue_spin_->show();
        blue_to_blue_spin_->show();

    } else if (eCOLOR_MODE_CUSTOM != current_color_mode_) {

        // hide custom color mode value widgets and show const color mode cc value labels

        red_to_red_val_->show();
        green_to_red_val_->show();
        blue_to_red_val_->show();
        red_to_green_val_->show();
        blue_to_green_val_->show();
        green_to_green_val_->show();
        red_to_blue_val_->show();
        green_to_blue_val_->show();
        blue_to_blue_val_->show();

        custom_color_mode_cc_memory_commit_->hide();
        red_to_red_spin_->hide();
        green_to_red_spin_->hide();
        blue_to_red_spin_->hide();
        red_to_green_spin_->hide();
        blue_to_green_spin_->hide();
        green_to_green_spin_->hide();
        red_to_blue_spin_->hide();
        green_to_blue_spin_->hide();
        blue_to_blue_spin_->hide();
    }

    // current_color_mode_ = current_color_mode;

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetColorConverter(connection_handle_, eRED_TO_RED, &red_to_red_cc_, eCURRENT_VALUE))) {
        if (eCOLOR_MODE_CUSTOM == current_color_mode_)
            red_to_red_spin_->set_value(red_to_red_cc_);
        else 
            red_to_red_val_->set_text(to_string(red_to_red_cc_));
    } else {
        LOG_E("get eRED_TO_RED failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetColorConverter(connection_handle_, eGREEN_TO_RED, &green_to_red_cc_, eCURRENT_VALUE))) {
        if (eCOLOR_MODE_CUSTOM == current_color_mode_)
            green_to_red_spin_->set_value(green_to_red_cc_);
        else 
            green_to_red_val_->set_text(to_string(green_to_red_cc_));
    } else {
        LOG_E("get eGREEN_TO_RED failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetColorConverter(connection_handle_, eBLUE_TO_RED, &blue_to_red_cc_, eCURRENT_VALUE))) {
        if (eCOLOR_MODE_CUSTOM == current_color_mode_)
            blue_to_red_spin_->set_value(blue_to_red_cc_);
        else 
            blue_to_red_val_->set_text(to_string(blue_to_red_cc_));
    } else {
        LOG_E("get eBLUE_TO_RED failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetColorConverter(connection_handle_, eRED_TO_GREEN, &red_to_green_cc_, eCURRENT_VALUE))) {
        if (eCOLOR_MODE_CUSTOM == current_color_mode_)
            red_to_green_spin_->set_value(red_to_green_cc_);
        else
            red_to_green_val_->set_text(to_string(red_to_green_cc_));
    } else {
        LOG_E("get eRED_TO_GREEN failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetColorConverter(connection_handle_, eGREEN_TO_GREEN, &green_to_green_cc_, eCURRENT_VALUE))) {
        if (eCOLOR_MODE_CUSTOM == current_color_mode_) 
            green_to_green_spin_->set_value(green_to_green_cc_);
        else 
            green_to_green_val_->set_text(to_string(green_to_green_cc_));
    } else {
        LOG_E("get eGREEN_TO_GREEN failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetColorConverter(connection_handle_, eBLUE_TO_GREEN, &blue_to_green_cc_, eCURRENT_VALUE))) {
        if (eCOLOR_MODE_CUSTOM == current_color_mode_)
            blue_to_green_spin_->set_value(blue_to_green_cc_);
        else
            blue_to_green_val_->set_text(to_string(blue_to_green_cc_));
    } else {
        LOG_E("get eBLUE_TO_GREEN failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetColorConverter(connection_handle_, eRED_TO_BLUE, &red_to_blue_cc_, eCURRENT_VALUE))) {
        if (eCOLOR_MODE_CUSTOM == current_color_mode_)
            red_to_blue_spin_->set_value(red_to_blue_cc_);
        else
            red_to_blue_val_->set_text(to_string(red_to_blue_cc_));
    } else {
        LOG_E("get eRED_TO_BLUE failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetColorConverter(connection_handle_, eGREEN_TO_BLUE, &green_to_blue_cc_, eCURRENT_VALUE))) {
        if (eCOLOR_MODE_CUSTOM == current_color_mode_)
            green_to_blue_spin_->set_value(green_to_blue_cc_);
        else
            green_to_blue_val_->set_text(to_string(green_to_blue_cc_));
    } else {
        LOG_E("get eGREEN_TO_BLUE failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetColorConverter(connection_handle_, eBLUE_TO_BLUE, &blue_to_blue_cc_, eCURRENT_VALUE))) {
        if (eCOLOR_MODE_CUSTOM == current_color_mode_)
            blue_to_blue_spin_->set_value(blue_to_blue_cc_);
        else
            blue_to_blue_val_->set_text(to_string(blue_to_blue_cc_));
    } else {
        LOG_E("get eBLUE_TO_BLUE failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }

    if (ePICOP_RETURN_SUCCESS == picop_rc_accumulative) {
        color_mode_cmd_status_val_->set_text("success");
    } else {
        color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}

static void get_color_mode(PicoP_ValueStorageTypeE color_mode_storage_type)
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    PicoP_ColorModeE color_mode;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetColorMode(connection_handle_, &color_mode, color_mode_storage_type))) {

       switch (color_mode) {
            case eCOLOR_MODE_BRILLIANT:
                color_mode_brilliant_->set_active(true);
                current_color_mode_ = eCOLOR_MODE_BRILLIANT;
                break;
            case eCOLOR_MODE_STANDARD:
                color_mode_standard_->set_active(true);
                current_color_mode_ = eCOLOR_MODE_STANDARD;
                break;
            case eCOLOR_MODE_INVERTED:
                color_mode_inverted_->set_active(true);
                current_color_mode_ = eCOLOR_MODE_INVERTED;
                break;
            case eCOLOR_MODE_RICH:
                color_mode_rich_->set_active(true);
                current_color_mode_ = eCOLOR_MODE_RICH;
                break;
            case eCOLOR_MODE_CUSTOM:
                color_mode_custom_->set_active(true);
                current_color_mode_ = eCOLOR_MODE_CUSTOM;
                break;
            default:
                break;
        }

        color_mode_cmd_status_val_->set_text("success");

    } else {
        color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}

static void on_color_mode_memory_space_startup_button_clicked()
{
    get_color_mode(eVALUE_ON_STARTUP);
}


static void on_color_mode_memory_space_factory_button_clicked()
{
    get_color_mode(eFACTORY_VALUE);
}


static void on_color_mode_memory_commit_button_clicked()
{
    PicoP_ReturnStatus picop_rc;
    if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_SetColorMode(connection_handle_, current_color_mode_, 1))) {
        color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}

static void on_custom_color_mode_cc_memory_commit_button_clicked()
{
    PicoP_ReturnStatus picop_rc;
    PicoP_ReturnStatus picop_rc_accumulative  = ePICOP_RETURN_SUCCESS;

    if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_SetColorConverter(connection_handle_, eRED_TO_RED, red_to_red_cc_, 1))) {
        LOG_E("get eGREEN_TO_BLUE failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    } 

    if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_SetColorConverter(connection_handle_, eRED_TO_GREEN, red_to_green_cc_, 1))) {
        LOG_E("get eRED_TO_GREEN failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }

    if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_SetColorConverter(connection_handle_, eRED_TO_BLUE, red_to_blue_cc_, 1))) {
        LOG_E("get eRED_TO_BLUE failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }

    if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_SetColorConverter(connection_handle_, eGREEN_TO_RED, green_to_red_cc_, 1))) {
        LOG_E("get eGREEN_TO_RED failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }

    if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_SetColorConverter(connection_handle_, eGREEN_TO_GREEN, green_to_green_cc_, 1))) {
        LOG_E("get eGREEN_TO_GREEN failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }

    if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_SetColorConverter(connection_handle_, eGREEN_TO_BLUE, green_to_blue_cc_, 1))) {
        LOG_E("get eGREEN_TO_BLUE failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }

    if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_SetColorConverter(connection_handle_, eBLUE_TO_RED, blue_to_red_cc_, 1))) {
        LOG_E("get eBLUE_TO_RED failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }

    if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_SetColorConverter(connection_handle_, eBLUE_TO_GREEN, blue_to_green_cc_, 1))) {
        LOG_E("get eBLUE_TO_GREEN failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }

    if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_SetColorConverter(connection_handle_, eBLUE_TO_BLUE, blue_to_blue_cc_, 1))) {
        LOG_E("get eBLUE_TO_BLUE failure : %d\n", picop_rc);
        picop_rc_accumulative = picop_rc;
    }

    if (ePICOP_RETURN_SUCCESS == picop_rc_accumulative) {
        color_mode_cmd_status_val_->set_text("success");
    } else {
        color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_color_mode_brilliant_radio_button_change()
{
    if (color_mode_brilliant_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorMode(connection_handle_, eCOLOR_MODE_BRILLIANT, 0))) {
            color_mode_cmd_status_val_->set_text("success");
            current_color_mode_ = eCOLOR_MODE_BRILLIANT;
            get_color_converter_values();
        } else {
            color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_color_mode_standard_radio_button_change()
{
    if (color_mode_standard_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorMode(connection_handle_, eCOLOR_MODE_STANDARD, 0))) {
            color_mode_cmd_status_val_->set_text("success");
            current_color_mode_ = eCOLOR_MODE_STANDARD;
            get_color_converter_values();
        } else {
            color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_color_mode_inverted_radio_button_change()
{
    if (color_mode_inverted_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorMode(connection_handle_, eCOLOR_MODE_INVERTED, 0))) {
            color_mode_cmd_status_val_->set_text("success");
            current_color_mode_ = eCOLOR_MODE_INVERTED;
            get_color_converter_values();
        } else {
            color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_color_mode_rich_radio_button_change()
{
    if (color_mode_rich_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorMode(connection_handle_, eCOLOR_MODE_RICH, 0))) {
            color_mode_cmd_status_val_->set_text("success");
            current_color_mode_ = eCOLOR_MODE_RICH;
            get_color_converter_values();
        } else {
            color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_color_mode_custom_radio_button_change()
{
    if (color_mode_custom_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorMode(connection_handle_, eCOLOR_MODE_CUSTOM, 0))) {
            color_mode_cmd_status_val_->set_text("success");
            current_color_mode_ = eCOLOR_MODE_CUSTOM;
            get_color_converter_values();
        } else {
            color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    } 
}


static void on_red_to_red_val_changed()
{
    red_to_red_cc_ = red_to_red_spin_->get_value();
    PicoP_ReturnStatus picop_rc;

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorConverter(connection_handle_, eRED_TO_RED, red_to_red_cc_, 0))) {
        color_mode_cmd_status_val_->set_text("success");
    } else {
        LOG_E("PicoP_SetColorConverter() failure : %d\n", picop_rc);
        color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_red_to_green_val_changed()
{
    red_to_green_cc_ = red_to_green_spin_->get_value();
    PicoP_ReturnStatus picop_rc;

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorConverter(connection_handle_, eRED_TO_GREEN, red_to_green_cc_, 0))) {
        color_mode_cmd_status_val_->set_text("success");
    } else {
        LOG_E("PicoP_SetColorConverter() failure : %d\n", picop_rc);
        color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_red_to_blue_val_changed()
{
    red_to_blue_cc_ = red_to_blue_spin_->get_value();
    PicoP_ReturnStatus picop_rc;

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorConverter(connection_handle_, eRED_TO_BLUE, red_to_blue_cc_, 0))) {
        color_mode_cmd_status_val_->set_text("success");
    } else {
        LOG_E("PicoP_SetColorConverter() failure : %d\n", picop_rc);
        color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_green_to_red_val_changed()
{
    green_to_red_cc_ = green_to_red_spin_->get_value();
    PicoP_ReturnStatus picop_rc;

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorConverter(connection_handle_, eGREEN_TO_RED, green_to_red_cc_, 0))) {
        color_mode_cmd_status_val_->set_text("success");
    } else {
        LOG_E("PicoP_SetColorConverter() failure : %d\n", picop_rc);
        color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_green_to_green_val_changed()
{
    green_to_green_cc_ = green_to_green_spin_->get_value();
    PicoP_ReturnStatus picop_rc;

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorConverter(connection_handle_, eGREEN_TO_GREEN, green_to_green_cc_, 0))) {
        color_mode_cmd_status_val_->set_text("success");
    } else {
        LOG_E("PicoP_SetColorConverter() failure : %d\n", picop_rc);
        color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_green_to_blue_val_changed()
{
    green_to_blue_cc_ = green_to_blue_spin_->get_value();
    PicoP_ReturnStatus picop_rc;

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorConverter(connection_handle_, eGREEN_TO_BLUE, green_to_blue_cc_, 0))) {
        color_mode_cmd_status_val_->set_text("success");
    } else {
        LOG_E("PicoP_SetColorConverter() failure : %d\n", picop_rc);
        color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_blue_to_red_val_changed()
{
    blue_to_red_cc_ = blue_to_red_spin_->get_value();
    PicoP_ReturnStatus picop_rc;

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorConverter(connection_handle_, eBLUE_TO_RED, blue_to_red_cc_, 0))) {
        color_mode_cmd_status_val_->set_text("success");
    } else {
        LOG_E("PicoP_SetColorConverter() failure : %d\n", picop_rc);
        color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_blue_to_green_val_changed()
{
    blue_to_green_cc_ = blue_to_green_spin_->get_value();
    PicoP_ReturnStatus picop_rc;

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorConverter(connection_handle_, eBLUE_TO_GREEN, blue_to_green_cc_, 0))) {
        color_mode_cmd_status_val_->set_text("success");
    } else {
        LOG_E("PicoP_SetColorConverter() failure : %d\n", picop_rc);
        color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_blue_to_blue_val_changed()
{
    blue_to_blue_cc_ = blue_to_blue_spin_->get_value();
    PicoP_ReturnStatus picop_rc;

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorConverter(connection_handle_, eBLUE_TO_BLUE, blue_to_blue_cc_, 0))) {
        color_mode_cmd_status_val_->set_text("success");
    } else {
        LOG_E("PicoP_SetColorConverter() failure : %d\n", picop_rc);
        color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}




// ****************************************************************************
//  PicoP_<Get/Set>ColorAlignment
// ****************************************************************************

static Gtk::Label* color_alignment_cmd_status_val_ = nullptr;

static PicoP_ValueStorageTypeE color_alignment_storage_type_ = eCURRENT_VALUE;
static Gtk::Button* color_alignment_memory_space_startup_ = nullptr;
static Gtk::Button* color_alignment_memory_space_factory_ = nullptr;
static Gtk::Button* color_alignment_memory_commit_ = nullptr;

static Glib::RefPtr<Gtk::Adjustment> red_h_color_alignment_adjustment_;
static Gtk::SpinButton* red_h_color_alignment_val_ = nullptr;
static Glib::RefPtr<Gtk::Adjustment> red_v_color_alignment_adjustment_;
static Gtk::SpinButton* red_v_color_alignment_val_ = nullptr;

static Glib::RefPtr<Gtk::Adjustment> green_h_color_alignment_adjustment_;
static Gtk::SpinButton* green_h_color_alignment_val_ = nullptr;
static Glib::RefPtr<Gtk::Adjustment> green_v_color_alignment_adjustment_;
static Gtk::SpinButton* green_v_color_alignment_val_ = nullptr;

static Glib::RefPtr<Gtk::Adjustment> blue_h_color_alignment_adjustment_;
static Gtk::SpinButton* blue_h_color_alignment_val_ = nullptr;
static Glib::RefPtr<Gtk::Adjustment> blue_v_color_alignment_adjustment_;
static Gtk::SpinButton* blue_v_color_alignment_val_ = nullptr;

static void get_color_alignment()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    INT16 color_alignment;
    
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetColorAlignment(connection_handle_, eHORIZONTAL, eLASER_RED_1, &color_alignment, color_alignment_storage_type_))) {
        color_alignment_cmd_status_val_->set_text("success");
        red_h_color_alignment_adjustment_->set_value(color_alignment);
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
        return;
    }
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetColorAlignment(connection_handle_, eVERTICAL, eLASER_RED_1, &color_alignment, color_alignment_storage_type_))) {
        color_alignment_cmd_status_val_->set_text("success");
        red_v_color_alignment_adjustment_->set_value(color_alignment);
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
        return;
    }

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetColorAlignment(connection_handle_, eHORIZONTAL, eLASER_GREEN_1, &color_alignment, color_alignment_storage_type_))) {
        color_alignment_cmd_status_val_->set_text("success");
        green_h_color_alignment_adjustment_->set_value(color_alignment);
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
        return;
    }
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetColorAlignment(connection_handle_, eVERTICAL, eLASER_GREEN_1, &color_alignment, color_alignment_storage_type_))) {
        color_alignment_cmd_status_val_->set_text("success");
        green_v_color_alignment_adjustment_->set_value(color_alignment);
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
        return;
    }

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetColorAlignment(connection_handle_, eHORIZONTAL, eLASER_BLUE_1, &color_alignment, color_alignment_storage_type_))) {
        color_alignment_cmd_status_val_->set_text("success");
        blue_h_color_alignment_adjustment_->set_value(color_alignment);
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
        return;
    }
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetColorAlignment(connection_handle_, eVERTICAL, eLASER_BLUE_1, &color_alignment, color_alignment_storage_type_))) {
        color_alignment_cmd_status_val_->set_text("success");
        blue_v_color_alignment_adjustment_->set_value(color_alignment);
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
        return;
    }
}


static void on_color_alignment_memory_space_startup_button_clicked()
{
    color_alignment_storage_type_ = eVALUE_ON_STARTUP;
    get_color_alignment();
}


static void on_color_alignment_memory_space_factory_button_clicked()
{
    color_alignment_storage_type_ = eFACTORY_VALUE;
    get_color_alignment();
}


static void on_color_alignment_memory_commit_button_clicked()
{
    PicoP_ReturnStatus picop_rc;
    float color_alignment;

    color_alignment = red_h_color_alignment_adjustment_->get_value();
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorAlignment(connection_handle_, eHORIZONTAL, eLASER_RED_1, color_alignment, 1))) {
        color_alignment_cmd_status_val_->set_text("success");
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
        return;
    }
    color_alignment = red_v_color_alignment_adjustment_->get_value();
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorAlignment(connection_handle_, eVERTICAL, eLASER_RED_1, color_alignment, 1))) {
        color_alignment_cmd_status_val_->set_text("success");
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
        return;
    }

    color_alignment = green_h_color_alignment_adjustment_->get_value();
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorAlignment(connection_handle_, eHORIZONTAL, eLASER_GREEN_1, color_alignment, 1))) {
        color_alignment_cmd_status_val_->set_text("success");
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
        return;
    }
    color_alignment = green_v_color_alignment_adjustment_->get_value();
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorAlignment(connection_handle_, eVERTICAL, eLASER_GREEN_1, color_alignment, 1))) {
        color_alignment_cmd_status_val_->set_text("success");
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
        return;
    }

    color_alignment = blue_h_color_alignment_adjustment_->get_value();
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorAlignment(connection_handle_, eHORIZONTAL, eLASER_BLUE_1, color_alignment, 1))) {
        color_alignment_cmd_status_val_->set_text("success");
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
        return;
    }
    color_alignment = blue_v_color_alignment_adjustment_->get_value();
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorAlignment(connection_handle_, eVERTICAL, eLASER_BLUE_1, color_alignment, 1))) {
        color_alignment_cmd_status_val_->set_text("success");
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
        return;
    }

}


static void on_red_h_color_alignment_val_changed()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;

    float color_alignment = red_h_color_alignment_adjustment_->get_value();
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorAlignment(connection_handle_, eHORIZONTAL, eLASER_RED_1, color_alignment, 0))) {
        color_alignment_cmd_status_val_->set_text("success");
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_red_v_color_alignment_val_changed()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;

    float color_alignment = red_v_color_alignment_adjustment_->get_value();
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorAlignment(connection_handle_, eVERTICAL, eLASER_RED_1, color_alignment, 0))) {
        color_alignment_cmd_status_val_->set_text("success");
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_green_h_color_alignment_val_changed()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;

    float color_alignment = green_h_color_alignment_adjustment_->get_value();
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorAlignment(connection_handle_, eHORIZONTAL, eLASER_GREEN_1, color_alignment, 0))) {
        color_alignment_cmd_status_val_->set_text("success");
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_green_v_color_alignment_val_changed()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;

    float color_alignment = green_v_color_alignment_adjustment_->get_value();
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorAlignment(connection_handle_, eVERTICAL, eLASER_GREEN_1, color_alignment, 0))) {
        color_alignment_cmd_status_val_->set_text("success");
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_blue_h_color_alignment_val_changed()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;

    float color_alignment = blue_h_color_alignment_adjustment_->get_value();
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorAlignment(connection_handle_, eHORIZONTAL, eLASER_BLUE_1, color_alignment, 0))) {
        color_alignment_cmd_status_val_->set_text("success");
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_blue_v_color_alignment_val_changed()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;

    float color_alignment = blue_v_color_alignment_adjustment_->get_value();
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetColorAlignment(connection_handle_, eVERTICAL, eLASER_BLUE_1, color_alignment, 0))) {
        color_alignment_cmd_status_val_->set_text("success");
    } else {
        color_alignment_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}




// ****************************************************************************
//  PicoP_<Get/Set>Gamma
// ****************************************************************************

static Gtk::Label* gamma_cmd_status_val_ = nullptr;

static PicoP_ValueStorageTypeE gamma_storage_type_ = eCURRENT_VALUE;
static Gtk::Button* gamma_memory_space_startup_ = nullptr;
static Gtk::Button* gamma_memory_space_factory_ = nullptr;
static Gtk::Button* gamma_memory_commit_ = nullptr;

static Glib::RefPtr<Gtk::Adjustment> gamma_red_adjustment_;
static float gamma_red_current_ = 0.0f;
static Gtk::SpinButton* gamma_red_val_ = nullptr;
static Gtk::Scale* gamma_red_scale_ = nullptr;

static Glib::RefPtr<Gtk::Adjustment> gamma_green_adjustment_;
static float gamma_green_current_ = 0.0f;
static Gtk::SpinButton* gamma_green_val_ = nullptr;
static Gtk::Scale* gamma_green_scale_ = nullptr;

static Glib::RefPtr<Gtk::Adjustment> gamma_blue_adjustment_;
static float gamma_blue_current_ = 0.0f;
static Gtk::SpinButton* gamma_blue_val_ = nullptr;
static Gtk::Scale* gamma_blue_scale_ = nullptr;


static void get_gammas()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;

    float gamma_red_current;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetGamma(connection_handle_, ePICOP_COLOR_RED, &gamma_red_current, gamma_storage_type_))) {

        if (gamma_red_current_ != gamma_red_current) {
            gamma_red_current_ = gamma_red_current;
            gamma_red_adjustment_->set_value(gamma_red_current_);
        }

        float gamma_green_current;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetGamma(connection_handle_, ePICOP_COLOR_GREEN, &gamma_green_current, gamma_storage_type_))) {

            if (gamma_green_current_ != gamma_green_current) {
                gamma_green_current_ = gamma_green_current;
                gamma_green_adjustment_->set_value(gamma_green_current_);
            }

            float gamma_blue_current;
            if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetGamma(connection_handle_, ePICOP_COLOR_BLUE, &gamma_blue_current, gamma_storage_type_))) {

                if (gamma_blue_current_ != gamma_blue_current) {
                    gamma_blue_current_ = gamma_blue_current;
                    gamma_blue_adjustment_->set_value(gamma_blue_current_);
                }

                gamma_cmd_status_val_->set_text("success");

            } else {
                gamma_cmd_status_val_->set_text(failure_string(picop_rc));
            }
        } else {
            gamma_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    } else {
        gamma_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_gamma_memory_space_startup_button_clicked()
{
    gamma_storage_type_ = eVALUE_ON_STARTUP;
    get_gammas();
}


static void on_gamma_memory_space_factory_button_clicked()
{
    gamma_storage_type_ = eFACTORY_VALUE;
    get_gammas();
}


static void on_gamma_memory_commit_button_clicked()
{
    PicoP_ReturnStatus picop_rc;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetGamma(connection_handle_, ePICOP_COLOR_RED, gamma_red_current_, 1))) {

        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetGamma(connection_handle_, ePICOP_COLOR_GREEN, gamma_green_current_, 1))) {

            if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetGamma(connection_handle_, ePICOP_COLOR_BLUE, gamma_blue_current_, 1))) {
            
                gamma_cmd_status_val_->set_text("success");

            } else {
                gamma_cmd_status_val_->set_text(failure_string(picop_rc));
            }
        } else {
            gamma_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    } else {
        gamma_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}

static void on_gamma_red_val_changed()
{
    gamma_red_current_ = gamma_red_adjustment_->get_value();

    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetGamma(connection_handle_, ePICOP_COLOR_RED, gamma_red_current_, 0))) {
        gamma_cmd_status_val_->set_text("success");
    } else {
        gamma_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_gamma_green_val_changed()
{
    gamma_green_current_ = gamma_green_adjustment_->get_value();

    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetGamma(connection_handle_, ePICOP_COLOR_GREEN, gamma_green_current_, 0))) {
        gamma_cmd_status_val_->set_text("success");
    } else {
        gamma_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_gamma_blue_val_changed()
{
    gamma_blue_current_ = gamma_blue_adjustment_->get_value();

    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetGamma(connection_handle_, ePICOP_COLOR_BLUE, gamma_blue_current_, 0))) {
        gamma_cmd_status_val_->set_text("success");
    } else {
        gamma_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}





// ****************************************************************************
//  PicoP_<Get/Set>VideoGammaBoostMode
// ****************************************************************************

static PicoP_ValueStorageTypeE gamma_boost_storage_type_ = eCURRENT_VALUE;
static Gtk::Button* gamma_boost_memory_space_startup_ = nullptr;
static Gtk::Button* gamma_boost_memory_space_factory_ = nullptr;
static Gtk::Button* gamma_boost_memory_commit_ = nullptr;

static PicoP_GammaBoostModeE red_gamma_boost_mode_current_ = eGAMMA_BOOST_MODE_NONE;
static PicoP_GammaBoostModeE green_gamma_boost_mode_current_ = eGAMMA_BOOST_MODE_NONE;
static PicoP_GammaBoostModeE blue_gamma_boost_mode_current_ = eGAMMA_BOOST_MODE_NONE;
static Gtk::Label* gamma_boost_mode_cmd_status_val_ = nullptr;
static Gtk::RadioButton* red_gamma_boost_mode_none_ = nullptr;
static Gtk::RadioButton* red_gamma_boost_mode_30_percent_ = nullptr;
static Gtk::RadioButton* green_gamma_boost_mode_none_ = nullptr;
static Gtk::RadioButton* green_gamma_boost_mode_30_percent_ = nullptr;
static Gtk::RadioButton* blue_gamma_boost_mode_none_ = nullptr;
static Gtk::RadioButton* blue_gamma_boost_mode_30_percent_ = nullptr;


static void get_red_gamma_boost_mode()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetVideoGammaBoostMode(connection_handle_, ePICOP_COLOR_RED, &red_gamma_boost_mode_current_, gamma_boost_storage_type_))) {

        switch (red_gamma_boost_mode_current_) {
            case eGAMMA_BOOST_MODE_NONE:
                red_gamma_boost_mode_none_->set_active(true);
                break;
            case eGAMMA_BOOST_MODE_VIRTUAL_LUMENS_30PCT:
                red_gamma_boost_mode_30_percent_->set_active(true);
                break;
            default:
                break;
        }

        gamma_boost_mode_cmd_status_val_->set_text("success");

    } else {
        gamma_boost_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void get_green_gamma_boost_mode()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetVideoGammaBoostMode(connection_handle_, ePICOP_COLOR_GREEN, &green_gamma_boost_mode_current_, gamma_boost_storage_type_))) {

        switch (green_gamma_boost_mode_current_) {
            case eGAMMA_BOOST_MODE_NONE:
                green_gamma_boost_mode_none_->set_active(true);
                break;
            case eGAMMA_BOOST_MODE_VIRTUAL_LUMENS_30PCT:
                green_gamma_boost_mode_30_percent_->set_active(true);
                break;
            default:
                break;
        }

        gamma_boost_mode_cmd_status_val_->set_text("success");

    } else {
        gamma_boost_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void get_blue_gamma_boost_mode()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetVideoGammaBoostMode(connection_handle_, ePICOP_COLOR_BLUE, &blue_gamma_boost_mode_current_, gamma_boost_storage_type_))) {

        switch (blue_gamma_boost_mode_current_) {
            case eGAMMA_BOOST_MODE_NONE:
                blue_gamma_boost_mode_none_->set_active(true);
                break;
            case eGAMMA_BOOST_MODE_VIRTUAL_LUMENS_30PCT:
                blue_gamma_boost_mode_30_percent_->set_active(true);
                break;
            default:
                break;
        }

        gamma_boost_mode_cmd_status_val_->set_text("success");

    } else {
        gamma_boost_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_gamma_boost_memory_space_startup_button_clicked()
{
    gamma_boost_storage_type_ = eVALUE_ON_STARTUP;
    get_red_gamma_boost_mode();
    get_green_gamma_boost_mode();
    get_blue_gamma_boost_mode();
}


static void on_gamma_boost_memory_space_factory_button_clicked()
{
    gamma_boost_storage_type_ = eFACTORY_VALUE;
    get_red_gamma_boost_mode();
    get_green_gamma_boost_mode();
    get_blue_gamma_boost_mode();
}


static void on_gamma_boost_memory_commit_button_clicked()
{

    PicoP_ReturnStatus picop_rc;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetVideoGammaBoostMode(connection_handle_, ePICOP_COLOR_RED, red_gamma_boost_mode_current_, 1))) {
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetVideoGammaBoostMode(connection_handle_, ePICOP_COLOR_GREEN, green_gamma_boost_mode_current_, 1))) {
            if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetVideoGammaBoostMode(connection_handle_, ePICOP_COLOR_BLUE, blue_gamma_boost_mode_current_, 1))) {
                color_mode_cmd_status_val_->set_text("success");
            } else {
                color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
            }
        } else {
            color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    } else {
        color_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_red_gamma_boost_mode_none_radio_button_change()
{
    if (red_gamma_boost_mode_none_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        red_gamma_boost_mode_current_ = eGAMMA_BOOST_MODE_NONE;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetVideoGammaBoostMode(connection_handle_, ePICOP_COLOR_RED, red_gamma_boost_mode_current_, 0))) {
            gamma_boost_mode_cmd_status_val_->set_text("success");
        } else {
            gamma_boost_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_red_gamma_boost_mode_30_percent_radio_button_change()
{
    if (red_gamma_boost_mode_30_percent_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        red_gamma_boost_mode_current_ = eGAMMA_BOOST_MODE_VIRTUAL_LUMENS_30PCT;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetVideoGammaBoostMode(connection_handle_, ePICOP_COLOR_RED, red_gamma_boost_mode_current_, 0))) {
            gamma_boost_mode_cmd_status_val_->set_text("success");
        } else {
            gamma_boost_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_green_gamma_boost_mode_none_radio_button_change()
{
    if (green_gamma_boost_mode_none_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        green_gamma_boost_mode_current_ = eGAMMA_BOOST_MODE_NONE;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetVideoGammaBoostMode(connection_handle_, ePICOP_COLOR_GREEN, green_gamma_boost_mode_current_, 0))) {
            gamma_boost_mode_cmd_status_val_->set_text("success");
        } else {
            gamma_boost_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_green_gamma_boost_mode_30_percent_radio_button_change()
{
    if (green_gamma_boost_mode_30_percent_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        green_gamma_boost_mode_current_ = eGAMMA_BOOST_MODE_VIRTUAL_LUMENS_30PCT;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetVideoGammaBoostMode(connection_handle_, ePICOP_COLOR_GREEN, green_gamma_boost_mode_current_, 0))) {
            gamma_boost_mode_cmd_status_val_->set_text("success");
        } else {
            gamma_boost_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_blue_gamma_boost_mode_none_radio_button_change()
{
    if (blue_gamma_boost_mode_none_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        blue_gamma_boost_mode_current_ = eGAMMA_BOOST_MODE_NONE;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetVideoGammaBoostMode(connection_handle_, ePICOP_COLOR_BLUE, blue_gamma_boost_mode_current_, 0))) {
            gamma_boost_mode_cmd_status_val_->set_text("success");
        } else {
            gamma_boost_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_blue_gamma_boost_mode_30_percent_radio_button_change()
{
    if (blue_gamma_boost_mode_30_percent_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        blue_gamma_boost_mode_current_ = eGAMMA_BOOST_MODE_VIRTUAL_LUMENS_30PCT;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetVideoGammaBoostMode(connection_handle_, ePICOP_COLOR_BLUE, blue_gamma_boost_mode_current_, 0))) {
            gamma_boost_mode_cmd_status_val_->set_text("success");
        } else {
            gamma_boost_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}




// ****************************************************************************
//  PicoP_<Get/Set>MicroWeaveMode
// ****************************************************************************

static Gtk::Label* microweave_mode_cmd_status_val_ = nullptr;

static PicoP_ValueStorageTypeE microweave_mode_storage_type_ = eCURRENT_VALUE;
static Gtk::Button* microweave_mode_memory_space_startup_ = nullptr;
static Gtk::Button* microweave_mode_memory_space_factory_ = nullptr;
static Gtk::Button* microweave_mode_memory_commit_ = nullptr;

static Gtk::RadioButton* microweave_mode_disabled_ = nullptr;
static Gtk::RadioButton* microweave_mode_enabled_ = nullptr;

static void get_microweave_mode()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    PicoP_MicroWeaveE microweave_mode;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetMicroWeaveMode(connection_handle_, &microweave_mode, microweave_mode_storage_type_))) {

        switch (microweave_mode) {
            case eMICROWEAVE_OFF:
                microweave_mode_disabled_->set_active(true);
                break;
            case eMICROWEAVE_ON:
                microweave_mode_enabled_->set_active(true);
                break;
            default:
                break;
        }

        microweave_mode_cmd_status_val_->set_text("success");

    } else {
        microweave_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_microweave_mode_memory_space_startup_button_clicked()
{
    microweave_mode_storage_type_ = eVALUE_ON_STARTUP;
    get_microweave_mode();
}


static void on_microweave_mode_memory_space_factory_button_clicked()
{
    microweave_mode_storage_type_ = eFACTORY_VALUE;
    get_microweave_mode();
}


static void on_microweave_mode_memory_commit_button_clicked()
{
    PicoP_MicroWeaveE microweave_mode;
    if (microweave_mode_disabled_->get_active()) {
        microweave_mode = eMICROWEAVE_OFF;
    } else {
        microweave_mode = eMICROWEAVE_ON;
    }

    PicoP_ReturnStatus picop_rc;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetMicroWeaveMode(connection_handle_, microweave_mode, 1))) {
        microweave_mode_cmd_status_val_->set_text("success");
    } else {
        microweave_mode_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_microweave_mode_disabled_radio_button_change()
{
    if (microweave_mode_disabled_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetMicroWeaveMode(connection_handle_, eMICROWEAVE_OFF, 0))) {
            microweave_mode_cmd_status_val_->set_text("success");
        } else {
            microweave_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_microweave_mode_enabled_radio_button_change()
{
    if (microweave_mode_enabled_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetMicroWeaveMode(connection_handle_, eMICROWEAVE_ON, 0))) {
            microweave_mode_cmd_status_val_->set_text("success");
        } else {
            microweave_mode_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}




// ****************************************************************************
//  PicoP_<Get/Set>GetBiPhase
// ****************************************************************************

static int16_t biphase_current_ = 0;

static PicoP_ValueStorageTypeE biphase_storage_type_ = eCURRENT_VALUE;
static Gtk::Button* biphase_memory_space_startup_ = nullptr;
static Gtk::Button* biphase_memory_space_factory_ = nullptr;
static Gtk::Button* biphase_memory_commit_ = nullptr;

static Gtk::Label* biphase_cmd_status_val_ = nullptr;
static Glib::RefPtr<Gtk::Adjustment> biphase_adjustment_;
static Gtk::SpinButton* biphase_val_ = nullptr;
static Gtk::Scale* biphase_scale_ = nullptr;


static void get_biphase()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    int16_t biphase_current;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetBiPhase(connection_handle_, &biphase_current, biphase_storage_type_))) {

        biphase_cmd_status_val_->set_text("success");
        if (biphase_current_ != biphase_current) {
            biphase_current_ = biphase_current;
            biphase_adjustment_->set_value(biphase_current_);
        }

    } else {
        biphase_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_biphase_memory_space_startup_button_clicked()
{
    biphase_storage_type_ = eVALUE_ON_STARTUP;
    get_biphase();
}


static void on_biphase_memory_space_factory_button_clicked()
{
    biphase_storage_type_ = eFACTORY_VALUE;
    get_biphase();
}


static void on_biphase_memory_commit_button_clicked()
{
    PicoP_ReturnStatus picop_rc;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetBiPhase(connection_handle_, biphase_current_, 1))) {
        biphase_cmd_status_val_->set_text("success");
    } else {
        biphase_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_biphase_val_changed()
{
    biphase_current_ = biphase_adjustment_->get_value();

    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetBiPhase(connection_handle_, biphase_current_, 0))) {
        biphase_cmd_status_val_->set_text("success");
    } else {
        biphase_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}





// ****************************************************************************
//  PicoP_<Get/Set>FlipImage
// ****************************************************************************

static Gtk::Label* flip_state_cmd_status_val_ = nullptr;

static PicoP_ValueStorageTypeE flip_state_storage_type_ = eCURRENT_VALUE;
static Gtk::Button* flip_state_memory_space_startup_ = nullptr;
static Gtk::Button* flip_state_memory_space_factory_ = nullptr;
static Gtk::Button* flip_state_memory_commit_ = nullptr;

static Gtk::RadioButton* flip_state_neither_ = nullptr;
static Gtk::RadioButton* flip_state_horizontal_ = nullptr;
static Gtk::RadioButton* flip_state_vertical_ = nullptr;
static Gtk::RadioButton* flip_state_both_ = nullptr;


static void get_flip_state()
{
    PicoP_ReturnStatus picop_rc;
    PicoP_FlipStateE flip_state_val;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetFlipState(connection_handle_, &flip_state_val, flip_state_storage_type_))) {

        switch (flip_state_val) {
            case eFLIP_NEITHER:
                flip_state_neither_->set_active(true);
                break;
            case eFLIP_HORIZONTAL:
                flip_state_horizontal_->set_active(true);
                break;
            case eFLIP_VERTICAL:
                flip_state_vertical_->set_active(true);
                break;
            case eFLIP_BOTH:
                flip_state_both_->set_active(true);
                break;
            default:
                break;
        }

        flip_state_cmd_status_val_->set_text("success");
    } else {
        flip_state_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_flip_state_memory_space_startup_button_clicked()
{
    flip_state_storage_type_ = eVALUE_ON_STARTUP;
    get_flip_state();
}


static void on_flip_state_memory_space_factory_button_clicked()
{
    flip_state_storage_type_ = eFACTORY_VALUE;
    get_flip_state();
}


static void on_flip_state_memory_commit_button_clicked()
{
    PicoP_FlipStateE flip_state_val;
    if (flip_state_neither_->get_active()) {
        flip_state_val = eFLIP_NEITHER;
    } else if (flip_state_horizontal_->get_active()) {
        flip_state_val = eFLIP_HORIZONTAL;
    } else if (flip_state_vertical_->get_active()) {
        flip_state_val = eFLIP_VERTICAL;
    } else if (flip_state_both_->get_active()) {
        flip_state_val = eFLIP_BOTH;
    }

    PicoP_ReturnStatus picop_rc;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetFlipState(connection_handle_, flip_state_val, 1))) {
        flip_state_cmd_status_val_->set_text("success");
    } else {
        flip_state_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_flip_state_neither_radio_button_change()
{
    if (flip_state_neither_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetFlipState(connection_handle_, eFLIP_NEITHER, 0))) {
            flip_state_cmd_status_val_->set_text("success");
        } else {
            flip_state_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_flip_state_horizontal_radio_button_change()
{
    if (flip_state_horizontal_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetFlipState(connection_handle_, eFLIP_HORIZONTAL, 0))) {
            flip_state_cmd_status_val_->set_text("success");
        } else {
            flip_state_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_flip_state_vertical_radio_button_change()
{
    if (flip_state_vertical_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetFlipState(connection_handle_, eFLIP_VERTICAL, 0))) {
            flip_state_cmd_status_val_->set_text("success");
        } else {
            flip_state_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_flip_state_both_radio_button_change()
{
    if (flip_state_both_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetFlipState(connection_handle_, eFLIP_BOTH, 0))) {
            flip_state_cmd_status_val_->set_text("success");
        } else {
            flip_state_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}





// ****************************************************************************
//  PicoP_<Get/Set>DrawTestPattern
// ****************************************************************************

static PicoP_TestPatternE test_pattern_current_ = eTEST_PATTERN_OFF;
static Gtk::Label* test_pattern_cmd_status_val_ = nullptr;
static Gtk::ComboBoxText* test_pattern_comboboxtext_ = nullptr;


static void set_test_pattern()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;

    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_DrawTestPattern(connection_handle_, 
            test_pattern_current_,
            ((100u << 24) | (100u << 16) | (100u << 8)  | 0u), 
            ((0u << 24) | (0u << 16) | (0u << 8) | 0u)))) {

        test_pattern_cmd_status_val_->set_text("success");
    
    } else {
        test_pattern_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_test_pattern_change() {

    Glib::ustring text = test_pattern_comboboxtext_->get_active_text();
    if (!text.compare("OFF")) {
        test_pattern_current_ = eTEST_PATTERN_OFF;
    } else if (!text.compare("CHECKERBOARD")) {
        test_pattern_current_ = eCHECKERBOARD_PATTERN;
    } else if (!text.compare("CROSSHAIR")) {
        test_pattern_current_ = eCROSSHAIR_PATTERN;
    } else if (!text.compare("GRID")) {
        test_pattern_current_ = eGRID_PATTERN;
    } else if (!text.compare("CONSTANT_COLOR")) {
        test_pattern_current_ = eCONSTANT_COLOR_PATTERN;
    }
    set_test_pattern();
}




// ****************************************************************************
//  PicoP_<Get/Set>SensingState
// ****************************************************************************

static Gtk::Label* sensing_state_cmd_status_val_ = nullptr;

static PicoP_ValueStorageTypeE sensing_state_storage_type_ = eCURRENT_VALUE;
static Gtk::Button* sensing_state_memory_space_startup_ = nullptr;
static Gtk::Button* sensing_state_memory_space_factory_ = nullptr;
static Gtk::Button* sensing_state_memory_commit_ = nullptr;

static Gtk::RadioButton* sensing_state_disabled_ = nullptr;
static Gtk::RadioButton* sensing_state_enabled_ = nullptr;

static void get_sensing_state()
{
    PicoP_ReturnStatus picop_rc = ePICOP_RETURN_SUCCESS;
    PicoP_SensingStateE sensing_state;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_GetSensingState(connection_handle_, &sensing_state, sensing_state_storage_type_))) {

        switch (sensing_state) {
            case eSENSING_DISABLED:
                sensing_state_disabled_->set_active(true);
                break;
            case eSENSING_ENABLED:
                sensing_state_enabled_->set_active(true);
                break;
            default:
                break;
        }

        sensing_state_cmd_status_val_->set_text("success");

    } else {
        sensing_state_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_sensing_state_memory_space_startup_button_clicked()
{
    sensing_state_storage_type_ = eVALUE_ON_STARTUP;
    get_sensing_state();
}


static void on_sensing_state_memory_space_factory_button_clicked()
{
    sensing_state_storage_type_ = eFACTORY_VALUE;
    get_sensing_state();
}


static void on_sensing_state_memory_commit_button_clicked()
{
    PicoP_SensingStateE sensing_state;
    if (sensing_state_disabled_->get_active()) {
        sensing_state = eSENSING_DISABLED;
    } else {
        sensing_state = eSENSING_ENABLED;
    }

    PicoP_ReturnStatus picop_rc;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetSensingState(connection_handle_, sensing_state, 1))) {
        sensing_state_cmd_status_val_->set_text("success");
    } else {
        sensing_state_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_sensing_state_disabled_radio_button_change()
{
    if (sensing_state_disabled_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetSensingState(connection_handle_, eSENSING_DISABLED, 0))) {
            sensing_state_cmd_status_val_->set_text("success");
        } else {
            sensing_state_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}


static void on_sensing_state_enabled_radio_button_change()
{
    if (sensing_state_enabled_->get_active()) {
        PicoP_ReturnStatus picop_rc;
        if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_SetSensingState(connection_handle_, eSENSING_ENABLED, 0))) {
            sensing_state_cmd_status_val_->set_text("success");
        } else {
            sensing_state_cmd_status_val_->set_text(failure_string(picop_rc));
        }
    }
}




// ****************************************************************************
//  PicoP_ utility calls
// ****************************************************************************

static Gtk::Label* utilities_cmd_status_val_ = nullptr;

static Gtk::Button* utilities_select_restore_factory_ = nullptr;
static Gtk::Button* utilities_select_commit_all_ = nullptr;


static void on_utilities_select_restore_factory_button_clicked()
{
    PicoP_ReturnStatus picop_rc;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_RestoreFactorySettings(connection_handle_))) {
        utilities_cmd_status_val_->set_text("success");
    } else {
        utilities_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}


static void on_utilities_select_commit_all_button_clicked()
{
    PicoP_ReturnStatus picop_rc;
    if (ePICOP_RETURN_SUCCESS == (picop_rc = PicoP_CommitAll(connection_handle_))) {
        utilities_cmd_status_val_->set_text("success");
    } else {
        utilities_cmd_status_val_->set_text(failure_string(picop_rc));
    }
}




// ****************************************************************************
//  Command Selection Radio Group
// ****************************************************************************

static CommandView command_view[CommandE::NUM_COMMANDS];


static void on_command_button_clicked(CommandE command, bool enabled)
{
    if (enabled) {
        command_view[command].layout->show();
    } else {
        command_view[command].layout->hide();
    }
}


static void on_library_info_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_READ_LIBRARY_INFO].button->get_active();

    if (is_active) {
        library_info();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_READ_LIBRARY_INFO].command, is_active);
}


static void on_system_info_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_READ_SYSTEM_INFO].button->get_active();

    if (is_active) {
        get_system_info();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_READ_SYSTEM_INFO].command, is_active);
}


static void on_input_video_state_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_SET_GET_INPUT_VIDEO_STATE].button->get_active();

    if (is_active) {
        input_video_state_storage_type_ = eCURRENT_VALUE;
        get_input_video_state();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_SET_GET_INPUT_VIDEO_STATE].command, is_active);
}


static void on_input_video_properties_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_GET_INPUT_VIDEO_PROPERTIES].button->get_active();

    if (is_active) {
        get_input_video_properties();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_GET_INPUT_VIDEO_PROPERTIES].command, is_active);
}


static void on_output_video_state_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_SET_GET_OUTPUT_VIDEO_STATE].button->get_active();

    if (is_active) {
        output_video_state_storage_type_ = eCURRENT_VALUE;
        get_output_video_state();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_SET_GET_OUTPUT_VIDEO_STATE].command, is_active);
}


static void on_output_video_properties_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_GET_OUTPUT_VIDEO_PROPERTIES].button->get_active();

    if (is_active) {
        get_output_video_properties();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_GET_OUTPUT_VIDEO_PROPERTIES].command, is_active);
}


static void on_frame_rate_mode_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_GET_SET_FRAME_RATE_MODE].button->get_active();

    if (is_active) {
        get_frame_rate_mode();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_GET_SET_FRAME_RATE_MODE].command, is_active);
}


static void on_brightness_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_SET_GET_BRIGHTNESS].button->get_active();

    if (is_active) {
        brightness_storage_type_ = eCURRENT_VALUE;
        get_brightness();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_SET_GET_BRIGHTNESS].command, is_active);
}


static void on_color_mode_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_SET_GET_COLOR_MODE].button->get_active();

    if (is_active) {
        get_color_mode(eCURRENT_VALUE);
        get_color_converter_values();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_SET_GET_COLOR_MODE].command, is_active);
}


static void on_color_alignment_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_SET_GET_COLOR_ALIGNMENT].button->get_active();

    if (is_active) {
        get_color_alignment();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_SET_GET_COLOR_ALIGNMENT].command, is_active);
}


static void on_gamma_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_SET_GET_GAMMA].button->get_active();

    if (is_active) {
        gamma_storage_type_ = eCURRENT_VALUE;
        get_gammas();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_SET_GET_GAMMA].command, command_view[CommandE::COMMAND_SET_GET_GAMMA].button->get_active());
}


static void on_gamma_boost_mode_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_SET_GET_GAMMA_BOOST_MODE].button->get_active();

    if (is_active) {
        gamma_boost_storage_type_ = eCURRENT_VALUE;
        get_red_gamma_boost_mode();
        get_green_gamma_boost_mode();
        get_blue_gamma_boost_mode();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_SET_GET_GAMMA_BOOST_MODE].command, command_view[CommandE::COMMAND_SET_GET_GAMMA_BOOST_MODE].button->get_active());
}


static void on_microweave_mode_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_SET_GET_MICROWEAVE_MODE].button->get_active();

    if (is_active) {
        microweave_mode_storage_type_ = eCURRENT_VALUE;
        get_microweave_mode();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_SET_GET_MICROWEAVE_MODE].command, command_view[CommandE::COMMAND_SET_GET_MICROWEAVE_MODE].button->get_active());
}


static void on_phase_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_SET_GET_PHASE].button->get_active();

    if (is_active) {
        biphase_storage_type_ = eCURRENT_VALUE;
        get_biphase();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_SET_GET_PHASE].command, command_view[CommandE::COMMAND_SET_GET_PHASE].button->get_active());
}


static void on_flip_state_button_change()
{
    flip_state_cmd_status_val_->set_text("");

    bool is_active = command_view[CommandE::COMMAND_FLIP_STATE].button->get_active();

    if (is_active) {
        flip_state_storage_type_ = eCURRENT_VALUE;
        get_flip_state();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_FLIP_STATE].command, command_view[CommandE::COMMAND_FLIP_STATE].button->get_active());
}


static void on_test_pattern_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_TEST_PATTERN].button->get_active();

    if (is_active) {
        test_pattern_current_ = eTEST_PATTERN_OFF;
        test_pattern_comboboxtext_->set_active(0);
        set_test_pattern();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_TEST_PATTERN].command, is_active);
}


static void on_sensing_state_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_SET_GET_SENSING_STATE].button->get_active();

    if (is_active) {
        microweave_mode_storage_type_ = eCURRENT_VALUE;
        get_sensing_state();
    }

    on_command_button_clicked(command_view[CommandE::COMMAND_SET_GET_SENSING_STATE].command, command_view[CommandE::COMMAND_SET_GET_SENSING_STATE].button->get_active());
}


static void on_utilities_button_change()
{
    bool is_active = command_view[CommandE::COMMAND_UTILITIES].button->get_active();

    on_command_button_clicked(command_view[CommandE::COMMAND_UTILITIES].command, is_active);
}


int main(int argc, char **argv)
{
    LOG("GIT VERSION : %s\n",VERSION);

    auto app = Gtk::Application::create(argc, argv);

    //Load the GtkBuilder file and instantiate its widgets:
    auto builder = Gtk::Builder::create();
    try
    {
        builder->add_from_file("glade/main_window.glade");
    }
    catch(const Glib::FileError& ex)
    {
        LOG_E("FileError: %s\n", ex.what().c_str());
        return 1;
    }
    catch(const Glib::MarkupError& ex)
    {
        LOG_E("MarkupError: %s\n", ex.what().c_str());
        return 1;
    }
    catch(const Gtk::BuilderError& ex)
    {
        LOG_E("BuilderError: %s\n", ex.what().c_str());
        return 1;
    }

    CommandE command = CommandE::COMMAND_READ_LIBRARY_INFO;
    command_view[command].command = command;
    builder->get_widget("library_info", command_view[command].button);
    builder->get_widget("library_info_box", command_view[command].layout);
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_library_info_button_change));
    // layout specific widgets
    builder->get_widget("lib_info_cmd_status_val", lib_info_cmd_status_val_);
    builder->get_widget("lib_info_major_version_val", lib_info_major_version_val_);
    builder->get_widget("lib_info_minor_version_val", lib_info_minor_version_val_);
    builder->get_widget("lib_info_patch_version_val", lib_info_patch_version_val_);
    builder->get_widget("lib_info_capability_flags_val", lib_info_capability_flags_val_);

    command = CommandE::COMMAND_READ_SYSTEM_INFO;
    command_view[command].command = command;
    builder->get_widget("system_info", command_view[command].button); 
    builder->get_widget("system_info_box", command_view[command].layout);
    int minimum_width, natural_width;
    command_view[command].layout->get_preferred_width(minimum_width, natural_width);
    int width = command_view[command].layout->get_width();
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_system_info_button_change));
    // layout specific widgets
    builder->get_widget("sys_info_cmd_status_val", sys_info_cmd_status_val_);
    builder->get_widget("sys_info_serial_number_val", sys_info_serial_number_val_);
    builder->get_widget("sys_info_firmware_version_val", sys_info_firmware_version_val_);
    builder->get_widget("sys_info_electronics_version_val", sys_info_electronics_version_val_);

    command = CommandE::COMMAND_SET_GET_INPUT_VIDEO_STATE;
    command_view[command].command = command;
    builder->get_widget("input_video_state", command_view[command].button); 
    builder->get_widget("input_video_state_box", command_view[command].layout);
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_input_video_state_button_change));
    // layout specific widgets
    builder->get_widget("input_video_state_cmd_status_val", input_video_state_cmd_status_val_);
    builder->get_widget("input_video_state_memory_space_startup", input_video_state_memory_space_startup_);
    input_video_state_memory_space_startup_->signal_clicked().connect(sigc::ptr_fun(&on_input_video_state_memory_space_startup_button_clicked));
    builder->get_widget("input_video_state_memory_space_factory", input_video_state_memory_space_factory_);
    input_video_state_memory_space_factory_->signal_clicked().connect(sigc::ptr_fun(&on_input_video_state_memory_space_factory_button_clicked));
    builder->get_widget("input_video_state_memory_commit", input_video_state_memory_commit_);
    input_video_state_memory_commit_->signal_clicked().connect(sigc::ptr_fun(&on_input_video_state_memory_commit_button_clicked));
    builder->get_widget("input_video_state_disabled", input_video_state_disabled_);
    input_video_state_disabled_->signal_toggled().connect(sigc::ptr_fun(&on_input_video_state_disabled_radio_button_change));
    builder->get_widget("input_video_state_enabled", input_video_state_enabled_);
    input_video_state_enabled_->signal_toggled().connect(sigc::ptr_fun(&on_input_video_state_enabled_radio_button_change));

    command = CommandE::COMMAND_GET_INPUT_VIDEO_PROPERTIES;
    command_view[command].command = command;
    builder->get_widget("input_video_properties", command_view[command].button); 
    builder->get_widget("input_video_properties_box", command_view[command].layout);
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_input_video_properties_button_change));
    // layout specific widgets
    builder->get_widget("input_video_properties_cmd_status_val", input_video_properties_cmd_status_val_);
    builder->get_widget("input_video_properties_horz_res_val", input_video_properties_horz_res_val_);
    builder->get_widget("input_video_properties_vert_res_val", input_video_properties_vert_res_val_);
    builder->get_widget("input_video_properties_frame_rate_val", input_video_properties_frame_rate_val_);

    command = CommandE::COMMAND_SET_GET_OUTPUT_VIDEO_STATE;
    command_view[command].command = command;
    builder->get_widget("output_video_state", command_view[command].button); 
    builder->get_widget("output_video_state_box", command_view[command].layout);
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_output_video_state_button_change));
    // layout specific widgets
    builder->get_widget("output_video_state_cmd_status_val", output_video_state_cmd_status_val_);
    builder->get_widget("output_video_state_memory_space_startup", output_video_state_memory_space_startup_);
    output_video_state_memory_space_startup_->signal_clicked().connect(sigc::ptr_fun(&on_output_video_state_memory_space_startup_button_clicked));
    builder->get_widget("output_video_state_memory_space_factory", output_video_state_memory_space_factory_);
    output_video_state_memory_space_factory_->signal_clicked().connect(sigc::ptr_fun(&on_output_video_state_memory_space_factory_button_clicked));
    builder->get_widget("output_video_state_memory_commit", output_video_state_memory_commit_);
    output_video_state_memory_commit_->signal_clicked().connect(sigc::ptr_fun(&on_output_video_state_memory_commit_button_clicked));
    builder->get_widget("output_video_state_disabled", output_video_state_disabled_);
    output_video_state_disabled_->signal_toggled().connect(sigc::ptr_fun(&on_output_video_state_disabled_radio_button_change));
    builder->get_widget("output_video_state_enabled", output_video_state_enabled_);
    output_video_state_enabled_->signal_toggled().connect(sigc::ptr_fun(&on_output_video_state_enabled_radio_button_change));

    command = CommandE::COMMAND_GET_OUTPUT_VIDEO_PROPERTIES;
    command_view[command].command = command;
    builder->get_widget("output_video_properties", command_view[command].button); 
    builder->get_widget("output_video_properties_box", command_view[command].layout);
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_output_video_properties_button_change));
    // layout specific widgets
    builder->get_widget("output_video_properties_cmd_status_val", output_video_properties_cmd_status_val_);
    builder->get_widget("output_video_properties_horz_res_val", output_video_properties_horz_res_val_);
    builder->get_widget("output_video_properties_vert_res_val", output_video_properties_vert_res_val_);
    builder->get_widget("output_video_properties_frame_rate_val", output_video_properties_frame_rate_val_);

    command = CommandE::COMMAND_GET_SET_FRAME_RATE_MODE;
    command_view[command].command = command;
    builder->get_widget("frame_rate_mode", command_view[command].button); 
    builder->get_widget("frame_rate_mode_box", command_view[command].layout);
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_frame_rate_mode_button_change));
    // layout specific widgets
    builder->get_widget("frame_rate_mode_cmd_status_val", frame_rate_mode_cmd_status_val_);
    builder->get_widget("frame_rate_mode_memory_space_startup", frame_rate_mode_memory_space_startup_);
    frame_rate_mode_memory_space_startup_->signal_clicked().connect(sigc::ptr_fun(&on_frame_rate_mode_memory_space_startup_button_clicked));
    builder->get_widget("frame_rate_mode_memory_space_factory", frame_rate_mode_memory_space_factory_);
    frame_rate_mode_memory_space_factory_->signal_clicked().connect(sigc::ptr_fun(&on_frame_rate_mode_memory_space_factory_button_clicked));
    builder->get_widget("frame_rate_mode_memory_commit", frame_rate_mode_memory_commit_);
    frame_rate_mode_memory_commit_->signal_clicked().connect(sigc::ptr_fun(&on_frame_rate_mode_memory_commit_button_clicked));
    builder->get_widget("frame_rate_60hz_720p", frame_rate_60hz_720p_);
    frame_rate_60hz_720p_->signal_clicked().connect(sigc::ptr_fun(&on_frame_rate_60hz_720p_radio_button_change));
    builder->get_widget("frame_rate_70hz_640p", frame_rate_70hz_640p_);
    frame_rate_70hz_640p_->signal_clicked().connect(sigc::ptr_fun(&on_frame_rate_70hz_640p_radio_button_change));
    builder->get_widget("frame_rate_70hz_600p", frame_rate_70hz_600p_);
    frame_rate_70hz_600p_->signal_clicked().connect(sigc::ptr_fun(&on_frame_rate_70hz_600p_radio_button_change));
    builder->get_widget("vertical_scaling_down", vertical_scaling_down_);
    vertical_scaling_down_->signal_clicked().connect(sigc::ptr_fun(&on_vertical_scaling_down_radio_button_change));
    builder->get_widget("vertical_scaling_crop_bottom", vertical_scaling_crop_bottom_);
    vertical_scaling_crop_bottom_->signal_clicked().connect(sigc::ptr_fun(&on_vertical_scaling_crop_bottom_radio_button_change));
    builder->get_widget("vertical_scaling_crop_even", vertical_scaling_crop_even_);
    vertical_scaling_crop_even_->signal_clicked().connect(sigc::ptr_fun(&on_vertical_scaling_crop_even_radio_button_change));
    builder->get_widget("horizontal_scaling_down", horizontal_scaling_down_);
    horizontal_scaling_down_->signal_clicked().connect(sigc::ptr_fun(&on_horizontal_scaling_down_radio_button_change));
    builder->get_widget("horizontal_scaling_none", horizontal_scaling_none_);
    horizontal_scaling_none_->signal_clicked().connect(sigc::ptr_fun(&on_horizontal_scaling_none_radio_button_change));

    command = CommandE::COMMAND_SET_GET_BRIGHTNESS;
    command_view[command].command = command;
    builder->get_widget("brightness", command_view[command].button); 
    builder->get_widget("brightness_box", command_view[command].layout);
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_brightness_button_change));
    // layout specific widgets
    builder->get_widget("brightness_cmd_status_val", brightness_cmd_status_val_);
    builder->get_widget("brightness_memory_space_startup", brightness_memory_space_startup_);
    brightness_memory_space_startup_->signal_clicked().connect(sigc::ptr_fun(&on_brightness_memory_space_startup_button_clicked));
    builder->get_widget("brightness_memory_space_factory", brightness_memory_space_factory_);
    brightness_memory_space_factory_->signal_clicked().connect(sigc::ptr_fun(&on_brightness_memory_space_factory_button_clicked));
    builder->get_widget("brightness_memory_commit", brightness_memory_commit_);
    brightness_memory_commit_->signal_clicked().connect(sigc::ptr_fun(&on_brightness_memory_commit_button_clicked));
    builder->get_widget("brightness_scale", brightness_scale_);
    builder->get_widget("brightness_val", brightness_val_);
    brightness_adjustment_ = brightness_val_->get_adjustment();
    brightness_adjustment_->signal_value_changed().connect(sigc::ptr_fun(&on_brightness_val_changed));

    command = CommandE::COMMAND_SET_GET_COLOR_MODE;
    command_view[command].command = command;
    builder->get_widget("color_mode", command_view[command].button); 
    builder->get_widget("color_mode_box", command_view[command].layout);
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_color_mode_button_change));
    // layout specific widgets
    builder->get_widget("color_mode_cmd_status_val", color_mode_cmd_status_val_);
    builder->get_widget("color_mode_memory_space_startup", color_mode_memory_space_startup_);
    color_mode_memory_space_startup_->signal_clicked().connect(sigc::ptr_fun(&on_color_mode_memory_space_startup_button_clicked));
    builder->get_widget("color_mode_memory_space_factory", color_mode_memory_space_factory_);
    color_mode_memory_space_factory_->signal_clicked().connect(sigc::ptr_fun(&on_color_mode_memory_space_factory_button_clicked));
    builder->get_widget("color_mode_memory_commit", color_mode_memory_commit_);
    color_mode_memory_commit_->signal_clicked().connect(sigc::ptr_fun(&on_color_mode_memory_commit_button_clicked));
    builder->get_widget("custom_color_mode_cc_memory_commit", custom_color_mode_cc_memory_commit_);
    builder->get_widget("color_mode_brilliant", color_mode_brilliant_);
    builder->get_widget("color_mode_standard", color_mode_standard_);
    builder->get_widget("color_mode_inverted", color_mode_inverted_);
    builder->get_widget("color_mode_rich", color_mode_rich_);
    builder->get_widget("color_mode_custom", color_mode_custom_);
    custom_color_mode_cc_memory_commit_->signal_clicked().connect(sigc::ptr_fun(&on_custom_color_mode_cc_memory_commit_button_clicked));
    color_mode_brilliant_->signal_toggled().connect(sigc::ptr_fun(&on_color_mode_brilliant_radio_button_change));
    color_mode_standard_->signal_toggled().connect(sigc::ptr_fun(&on_color_mode_standard_radio_button_change));
    color_mode_inverted_->signal_toggled().connect(sigc::ptr_fun(&on_color_mode_inverted_radio_button_change));
    color_mode_rich_->signal_toggled().connect(sigc::ptr_fun(&on_color_mode_rich_radio_button_change));
    color_mode_custom_->signal_toggled().connect(sigc::ptr_fun(&on_color_mode_custom_radio_button_change));
    // current release does not support inverted color mode
    color_mode_inverted_->hide();
    builder->get_widget("_red_to_red_spin", red_to_red_spin_);
    builder->get_widget("_green_to_red_spin", green_to_red_spin_);
    builder->get_widget("_blue_to_red_spin", blue_to_red_spin_);
    builder->get_widget("_red_to_green_spin", red_to_green_spin_);
    builder->get_widget("_blue_to_green_spin", blue_to_green_spin_);
    builder->get_widget("_green_to_green_spin", green_to_green_spin_);
    builder->get_widget("_red_to_blue_spin", red_to_blue_spin_);
    builder->get_widget("_green_to_blue_spin", green_to_blue_spin_);
    builder->get_widget("_blue_to_blue_spin", blue_to_blue_spin_);
    red_to_red_spin_->set_numeric(true);
    green_to_red_spin_->set_numeric(true);
    blue_to_red_spin_->set_numeric(true);
    red_to_green_spin_->set_numeric(true);
    green_to_green_spin_->set_numeric(true);
    blue_to_green_spin_->set_numeric(true);
    red_to_blue_spin_->set_numeric(true);
    green_to_blue_spin_->set_numeric(true);
    blue_to_blue_spin_->set_numeric(true);
    builder->get_widget("red_to_red_val", red_to_red_val_);
    builder->get_widget("green_to_red_val", green_to_red_val_);
    builder->get_widget("blue_to_red_val", blue_to_red_val_);
    builder->get_widget("red_to_green_val", red_to_green_val_);
    builder->get_widget("blue_to_green_val", blue_to_green_val_);
    builder->get_widget("green_to_green_val", green_to_green_val_);
    builder->get_widget("red_to_blue_val", red_to_blue_val_);
    builder->get_widget("green_to_blue_val", green_to_blue_val_);
    builder->get_widget("blue_to_blue_val", blue_to_blue_val_);
    red_to_red_spin_->signal_value_changed().connect(sigc::ptr_fun(&on_red_to_red_val_changed));
    red_to_green_spin_->signal_value_changed().connect(sigc::ptr_fun(&on_red_to_green_val_changed));
    red_to_blue_spin_->signal_value_changed().connect(sigc::ptr_fun(&on_red_to_blue_val_changed));
    green_to_red_spin_->signal_value_changed().connect(sigc::ptr_fun(&on_green_to_red_val_changed));
    green_to_green_spin_->signal_value_changed().connect(sigc::ptr_fun(&on_green_to_green_val_changed));
    green_to_blue_spin_->signal_value_changed().connect(sigc::ptr_fun(&on_green_to_blue_val_changed));
    blue_to_red_spin_->signal_value_changed().connect(sigc::ptr_fun(&on_blue_to_red_val_changed));
    blue_to_green_spin_->signal_value_changed().connect(sigc::ptr_fun(&on_blue_to_green_val_changed));
    blue_to_blue_spin_->signal_value_changed().connect(sigc::ptr_fun(&on_blue_to_blue_val_changed));

    command = CommandE::COMMAND_SET_GET_COLOR_ALIGNMENT;
    command_view[command].command = command;
    builder->get_widget("color_alignment", command_view[command].button); 
    builder->get_widget("color_alignment_box", command_view[command].layout);
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_color_alignment_button_change));
    // layout specific widgets
    builder->get_widget("color_alignment_cmd_status_val", color_alignment_cmd_status_val_);
    builder->get_widget("color_alignment_memory_space_startup", color_alignment_memory_space_startup_);
    color_alignment_memory_space_startup_->signal_clicked().connect(sigc::ptr_fun(&on_color_alignment_memory_space_startup_button_clicked));
    builder->get_widget("color_alignment_memory_space_factory", color_alignment_memory_space_factory_);
    color_alignment_memory_space_factory_->signal_clicked().connect(sigc::ptr_fun(&on_color_alignment_memory_space_factory_button_clicked));
    builder->get_widget("color_alignment_memory_commit", color_alignment_memory_commit_);
    color_alignment_memory_commit_->signal_clicked().connect(sigc::ptr_fun(&on_color_alignment_memory_commit_button_clicked));
    builder->get_widget("red_h_color_alignment_val", red_h_color_alignment_val_);
    builder->get_widget("red_v_color_alignment_val", red_v_color_alignment_val_);
    builder->get_widget("green_h_color_alignment_val", green_h_color_alignment_val_);
    builder->get_widget("green_v_color_alignment_val", green_v_color_alignment_val_);
    builder->get_widget("blue_h_color_alignment_val", blue_h_color_alignment_val_);
    builder->get_widget("blue_v_color_alignment_val", blue_v_color_alignment_val_);
    red_h_color_alignment_adjustment_ = red_h_color_alignment_val_->get_adjustment();
    red_h_color_alignment_adjustment_->signal_value_changed().connect(sigc::ptr_fun(&on_red_h_color_alignment_val_changed));
    red_v_color_alignment_adjustment_ = red_v_color_alignment_val_->get_adjustment();
    red_v_color_alignment_adjustment_->signal_value_changed().connect(sigc::ptr_fun(&on_red_v_color_alignment_val_changed));
    green_h_color_alignment_adjustment_ = green_h_color_alignment_val_->get_adjustment();
    green_h_color_alignment_adjustment_->signal_value_changed().connect(sigc::ptr_fun(&on_green_h_color_alignment_val_changed));
    green_v_color_alignment_adjustment_ = green_v_color_alignment_val_->get_adjustment();
    green_v_color_alignment_adjustment_->signal_value_changed().connect(sigc::ptr_fun(&on_green_v_color_alignment_val_changed));
    blue_h_color_alignment_adjustment_ = blue_h_color_alignment_val_->get_adjustment();
    blue_h_color_alignment_adjustment_->signal_value_changed().connect(sigc::ptr_fun(&on_blue_h_color_alignment_val_changed));
    blue_v_color_alignment_adjustment_ = blue_v_color_alignment_val_->get_adjustment();
    blue_v_color_alignment_adjustment_->signal_value_changed().connect(sigc::ptr_fun(&on_blue_v_color_alignment_val_changed));

    command = CommandE::COMMAND_SET_GET_GAMMA;
    command_view[command].command = command;
    builder->get_widget("gamma", command_view[command].button); 
    builder->get_widget("gamma_box", command_view[command].layout);
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_gamma_button_change));
    // layout specific widgets
    builder->get_widget("gamma_cmd_status_val", gamma_cmd_status_val_);
    builder->get_widget("gamma_memory_space_startup", gamma_memory_space_startup_);
    gamma_memory_space_startup_->signal_clicked().connect(sigc::ptr_fun(&on_gamma_memory_space_startup_button_clicked));
    builder->get_widget("gamma_memory_space_factory", gamma_memory_space_factory_);
    gamma_memory_space_factory_->signal_clicked().connect(sigc::ptr_fun(&on_gamma_memory_space_factory_button_clicked));
    builder->get_widget("gamma_memory_commit", gamma_memory_commit_);
    gamma_memory_commit_->signal_clicked().connect(sigc::ptr_fun(&on_gamma_memory_commit_button_clicked));
    builder->get_widget("gamma_cmd_status_val", gamma_cmd_status_val_);
    builder->get_widget("gamma_red_scale", gamma_red_scale_);
    builder->get_widget("gamma_red_val", gamma_red_val_);
    gamma_red_adjustment_ = gamma_red_val_->get_adjustment();
    gamma_red_adjustment_->signal_value_changed().connect(sigc::ptr_fun(&on_gamma_red_val_changed));
    builder->get_widget("gamma_green_scale", gamma_green_scale_);
    builder->get_widget("gamma_green_val", gamma_green_val_);
    gamma_green_adjustment_ = gamma_green_val_->get_adjustment();
    gamma_green_adjustment_->signal_value_changed().connect(sigc::ptr_fun(&on_gamma_green_val_changed));
    builder->get_widget("gamma_blue_scale", gamma_blue_scale_);
    builder->get_widget("gamma_blue_val", gamma_blue_val_);
    gamma_blue_adjustment_ = gamma_blue_val_->get_adjustment();
    gamma_blue_adjustment_->signal_value_changed().connect(sigc::ptr_fun(&on_gamma_blue_val_changed));

    command = CommandE::COMMAND_SET_GET_GAMMA_BOOST_MODE;
    command_view[command].command = command;
    builder->get_widget("gamma_boost_mode", command_view[command].button); 
    builder->get_widget("gamma_boost_mode_box", command_view[command].layout);
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_gamma_boost_mode_button_change));
    // layout specific widgets
    builder->get_widget("gamma_boost_mode_cmd_status_val", gamma_boost_mode_cmd_status_val_);
    builder->get_widget("gamma_boost_memory_space_startup", gamma_boost_memory_space_startup_);
    gamma_boost_memory_space_startup_->signal_clicked().connect(sigc::ptr_fun(&on_gamma_boost_memory_space_startup_button_clicked));
    builder->get_widget("gamma_boost_memory_space_factory", gamma_boost_memory_space_factory_);
    gamma_boost_memory_space_factory_->signal_clicked().connect(sigc::ptr_fun(&on_gamma_boost_memory_space_factory_button_clicked));
    builder->get_widget("gamma_boost_memory_commit", gamma_boost_memory_commit_);
    gamma_boost_memory_commit_->signal_clicked().connect(sigc::ptr_fun(&on_gamma_boost_memory_commit_button_clicked));
    builder->get_widget("red_gamma_boost_mode_none", red_gamma_boost_mode_none_);
    red_gamma_boost_mode_none_->signal_toggled().connect(sigc::ptr_fun(&on_red_gamma_boost_mode_none_radio_button_change));
    builder->get_widget("red_gamma_boost_mode_30_percent", red_gamma_boost_mode_30_percent_);
    red_gamma_boost_mode_30_percent_->signal_toggled().connect(sigc::ptr_fun(&on_red_gamma_boost_mode_30_percent_radio_button_change));
    builder->get_widget("green_gamma_boost_mode_none", green_gamma_boost_mode_none_);
    green_gamma_boost_mode_none_->signal_toggled().connect(sigc::ptr_fun(&on_green_gamma_boost_mode_none_radio_button_change));
    builder->get_widget("green_gamma_boost_mode_30_percent", green_gamma_boost_mode_30_percent_);
    green_gamma_boost_mode_30_percent_->signal_toggled().connect(sigc::ptr_fun(&on_green_gamma_boost_mode_30_percent_radio_button_change));
    builder->get_widget("blue_gamma_boost_mode_none", blue_gamma_boost_mode_none_);
    blue_gamma_boost_mode_none_->signal_toggled().connect(sigc::ptr_fun(&on_blue_gamma_boost_mode_none_radio_button_change));
    builder->get_widget("blue_gamma_boost_mode_30_percent", blue_gamma_boost_mode_30_percent_);
    blue_gamma_boost_mode_30_percent_->signal_toggled().connect(sigc::ptr_fun(&on_blue_gamma_boost_mode_30_percent_radio_button_change));

    command = CommandE::COMMAND_SET_GET_MICROWEAVE_MODE;
    command_view[command].command = command;
    builder->get_widget("microweave_mode", command_view[command].button); 
    builder->get_widget("microweave_mode_box", command_view[command].layout);
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_microweave_mode_button_change));
    // layout specific widgets
    builder->get_widget("microweave_mode_cmd_status_val", microweave_mode_cmd_status_val_);
    builder->get_widget("microweave_mode_memory_space_startup", microweave_mode_memory_space_startup_);
    microweave_mode_memory_space_startup_->signal_clicked().connect(sigc::ptr_fun(&on_microweave_mode_memory_space_startup_button_clicked));
    builder->get_widget("microweave_mode_memory_space_factory", microweave_mode_memory_space_factory_);
    microweave_mode_memory_space_factory_->signal_clicked().connect(sigc::ptr_fun(&on_microweave_mode_memory_space_factory_button_clicked));
    builder->get_widget("microweave_mode_memory_commit", microweave_mode_memory_commit_);
    microweave_mode_memory_commit_->signal_clicked().connect(sigc::ptr_fun(&on_microweave_mode_memory_commit_button_clicked));
    builder->get_widget("microweave_mode_disabled", microweave_mode_disabled_);
    microweave_mode_disabled_->signal_toggled().connect(sigc::ptr_fun(&on_microweave_mode_disabled_radio_button_change));
    builder->get_widget("microweave_mode_enabled", microweave_mode_enabled_);
    microweave_mode_enabled_->signal_toggled().connect(sigc::ptr_fun(&on_microweave_mode_enabled_radio_button_change));

    command = CommandE::COMMAND_SET_GET_PHASE;
    command_view[command].command = command;
    builder->get_widget("biphase", command_view[command].button); 
    builder->get_widget("biphase_box", command_view[command].layout);
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_phase_button_change));
    // layout specific widgets
    builder->get_widget("biphase_cmd_status_val", biphase_cmd_status_val_);
    builder->get_widget("biphase_memory_space_startup", biphase_memory_space_startup_);
    biphase_memory_space_startup_->signal_clicked().connect(sigc::ptr_fun(&on_biphase_memory_space_startup_button_clicked));
    builder->get_widget("biphase_memory_space_factory", biphase_memory_space_factory_);
    biphase_memory_space_factory_->signal_clicked().connect(sigc::ptr_fun(&on_biphase_memory_space_factory_button_clicked));
    builder->get_widget("biphase_memory_commit", biphase_memory_commit_);
    biphase_memory_commit_->signal_clicked().connect(sigc::ptr_fun(&on_biphase_memory_commit_button_clicked));
    builder->get_widget("biphase_scale", biphase_scale_);
    builder->get_widget("biphase_val", biphase_val_);
    biphase_adjustment_ = biphase_val_->get_adjustment();
    biphase_adjustment_->signal_value_changed().connect(sigc::ptr_fun(&on_biphase_val_changed));

    command = CommandE::COMMAND_FLIP_STATE;
    command_view[command].command = command;
    builder->get_widget("flip", command_view[command].button); 
    builder->get_widget("flip_state_box", command_view[command].layout);
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_flip_state_button_change));
    // layout specific widgets
    builder->get_widget("flip_state_cmd_status_val", flip_state_cmd_status_val_);
    builder->get_widget("flip_state_memory_space_startup", flip_state_memory_space_startup_);
    flip_state_memory_space_startup_->signal_clicked().connect(sigc::ptr_fun(&on_flip_state_memory_space_startup_button_clicked));
    builder->get_widget("flip_state_memory_space_factory", flip_state_memory_space_factory_);
    flip_state_memory_space_factory_->signal_clicked().connect(sigc::ptr_fun(&on_flip_state_memory_space_factory_button_clicked));
    builder->get_widget("flip_state_memory_commit", flip_state_memory_commit_);
    flip_state_memory_commit_->signal_clicked().connect(sigc::ptr_fun(&on_flip_state_memory_commit_button_clicked));
    builder->get_widget("flip_state_neither", flip_state_neither_);
    flip_state_neither_->signal_toggled().connect(sigc::ptr_fun(&on_flip_state_neither_radio_button_change));
    builder->get_widget("flip_state_horizontal", flip_state_horizontal_);
    flip_state_horizontal_->signal_toggled().connect(sigc::ptr_fun(&on_flip_state_horizontal_radio_button_change));
    builder->get_widget("flip_state_vertical", flip_state_vertical_);
    flip_state_vertical_->signal_toggled().connect(sigc::ptr_fun(&on_flip_state_vertical_radio_button_change));
    builder->get_widget("flip_state_both", flip_state_both_);
    flip_state_both_->signal_toggled().connect(sigc::ptr_fun(&on_flip_state_both_radio_button_change));

    command = CommandE::COMMAND_TEST_PATTERN;
    command_view[command].command = command;
    builder->get_widget("test_pattern", command_view[command].button); 
    builder->get_widget("test_pattern_box", command_view[command].layout);
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_test_pattern_button_change));
    // layout specific widgets
    builder->get_widget("test_pattern_cmd_status_val", test_pattern_cmd_status_val_);
    builder->get_widget("test_pattern_comboboxtext", test_pattern_comboboxtext_);
    test_pattern_comboboxtext_->append("OFF");
    test_pattern_comboboxtext_->append("CHECKERBOARD");
    test_pattern_comboboxtext_->append("CROSSHAIR");
    test_pattern_comboboxtext_->append("GRID");
    test_pattern_comboboxtext_->append("CONSTANT_COLOR");
    test_pattern_comboboxtext_->set_active(0);
    test_pattern_comboboxtext_->signal_changed().connect(sigc::ptr_fun(&on_test_pattern_change));

    command = CommandE::COMMAND_SET_GET_SENSING_STATE;
    command_view[command].command = command;
    builder->get_widget("sensing_state", command_view[command].button); 
    builder->get_widget("sensing_state_box", command_view[command].layout);
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_sensing_state_button_change));
    // layout specific widgets
    builder->get_widget("sensing_state_cmd_status_val", sensing_state_cmd_status_val_);
    builder->get_widget("sensing_state_memory_space_startup", sensing_state_memory_space_startup_);
    sensing_state_memory_space_startup_->signal_clicked().connect(sigc::ptr_fun(&on_sensing_state_memory_space_startup_button_clicked));
    builder->get_widget("sensing_state_memory_space_factory", sensing_state_memory_space_factory_);
    sensing_state_memory_space_factory_->signal_clicked().connect(sigc::ptr_fun(&on_sensing_state_memory_space_factory_button_clicked));
    builder->get_widget("sensing_state_memory_commit", sensing_state_memory_commit_);
    sensing_state_memory_commit_->signal_clicked().connect(sigc::ptr_fun(&on_sensing_state_memory_commit_button_clicked));
    builder->get_widget("sensing_state_disabled", sensing_state_disabled_);
    sensing_state_disabled_->signal_toggled().connect(sigc::ptr_fun(&on_sensing_state_disabled_radio_button_change));
    builder->get_widget("sensing_state_enabled", sensing_state_enabled_);
    sensing_state_enabled_->signal_toggled().connect(sigc::ptr_fun(&on_sensing_state_enabled_radio_button_change));

    command = CommandE::COMMAND_UTILITIES;
    command_view[command].command = command;
    builder->get_widget("utilities", command_view[command].button); 
    builder->get_widget("utilities_box", command_view[command].layout);
    command_view[command].layout->hide();
    command_view[command].button->signal_toggled().connect(sigc::ptr_fun(&on_utilities_button_change));
    // layout specific widgets
    builder->get_widget("utilities_cmd_status_val", utilities_cmd_status_val_);
    builder->get_widget("utilities_select_restore_factory", utilities_select_restore_factory_);
    utilities_select_restore_factory_->signal_clicked().connect(sigc::ptr_fun(&on_utilities_select_restore_factory_button_clicked));
    builder->get_widget("utilities_select_commit_all", utilities_select_commit_all_);
    utilities_select_commit_all_->signal_clicked().connect(sigc::ptr_fun(&on_utilities_select_commit_all_button_clicked));

    if (!open_connection()) {

        library_info();

        builder->get_widget("main_window", main_window_); 

        app->run(*main_window_);

        close_connection();

        return 0;

    } else {

        return -1;
    }
}


void on_window_main_destroy()
{
    LOG("on_window_main_destroy()\n");
}
