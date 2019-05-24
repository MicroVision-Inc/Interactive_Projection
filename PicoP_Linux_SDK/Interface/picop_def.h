//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!
//! \file picop_def.h
//!
//! \brief Common data types used by various MicroVision PicoP APIs.
//!
//! Copyright (C) 2018-2019 MicroVision, Inc.
//!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#ifndef PICOP_DEF_H
#define PICOP_DEF_H

#include "stdint.h"

// ****************************************************************************
//                                 GENERAL
// ****************************************************************************


#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (1)
#endif

#define SYSTEM_SN_LEN 17 // System Serial Number Length
#define MAX_UPGRADE_DATA_SIZE 4000

// ****************************************************************************
// All API function require a valid PicoP_HANDLE except MvXXX_OpenLibrary
typedef void *PicoP_HANDLE;

typedef int8_t INT8;     ///< An 8 bit wide signed type
typedef int16_t INT16;   ///< A 16 bit wide signed type
typedef int32_t INT32;   ///< A 32 bit wide signed type
typedef uint8_t UINT8;   ///< An 8 bit wide signed type
typedef uint16_t UINT16; ///< A 16 bit wide signed type
typedef uint32_t UINT32; ///< A 32 bit wide signed type
typedef float FP32;      ///< A 32 bit IEEE754 floating point type
typedef int32_t BOOL;    ///< A 32 bit wide signed type


// ****************************************************************************
// Defines enums for types of storage for the value queried on the system
typedef enum
{
    eCURRENT_VALUE = 0, ///< Current value
    eVALUE_ON_STARTUP,  ///< Value on system startup
    eFACTORY_VALUE      ///< Value set at the Factory
} PicoP_ValueStorageTypeE;


// ****************************************************************************
//                     CONNECTION MANAGEMENT
// ****************************************************************************

// ****************************************************************************
// Library information
typedef struct
{
    UINT8 majorVersion;     ///< Contains the major version of the library
    UINT8 minorVersion;     ///< Contains the minor version of the library
    UINT8 patchVersion;     ///< Contains the patch version of the library
    UINT32 capabilityFlags; ///< Flags that describe the capbility of this
                            ///< version of the ALC library
} PicoP_LibraryInfoS;


// ****************************************************************************
// Selects the Uart or USB interface for host communication
typedef enum
{
    eUART = 0, ///< Uart interface
    eUSB,       ///< USB interface
	eBTH,       ///< Bluetooth interface
} PicoP_ConnectionTypeE;


// ****************************************************************************
// Defines enums for Uart parity selection
typedef enum
{
    eUART_PARITY_NONE = 0, ///< No parity required
    eUART_PARITY_EVEN,     ///< Even parity required
    eUART_PARITY_ODD       ///< Odd parity required
} PicoP_UartParityE;


// ****************************************************************************
// Parameters for Uart communication
typedef struct
{
    union
    {
        char const *portName;   ///<  Windows port name, COM1, COM2, etc.
        char const *deviceName; ///<  Linux device name /dev/ttyS0, /dev/ttyS1,
                                ///etc.
    };

    UINT32 baudRate; ///<  The baud rate, valid values are:
                     ///<         4800
                     ///<         9600
                     ///<         115200

    PicoP_UartParityE parity; ///<  The parity type
    UINT8 stopBitCount;       ///<  Stop bit count, valid values are 1, and 2
} PicoP_UartInfoS;


// ****************************************************************************
// Describes any attributes required for a successful USB connection
typedef struct
{
    UINT32 productID;         ///<  Product ID
    char const *serialNumber; ///<  Serial Number
} PicoP_USBInfoS;


// ****************************************************************************
// Parameters for Bluetooth communication
typedef struct
{
    const char *bthMacAddr; // Bluetooth MAC address.
    const char *passKey;    // Bluetooth authentication key
} PicoP_BTHInfoS;


// ****************************************************************************
// Contains physical connection parameters for both USB or Uart
//     Only one is valid at a time, parameter type is given by connectionType.
typedef struct
{
    PicoP_ConnectionTypeE connectionType; ///<  Defines whether the struct
                                          ///contains
    ///<  info for an Uart or  USB connection

    union
    {
        PicoP_UartInfoS uartInfo; ///<  Parameters for Uart connection
        PicoP_USBInfoS usbInfo;   ///<  Parameter for USB connection
        PicoP_BTHInfoS bthInfo;   ///<  Parameter for bluetooth connection
	};
} PicoP_ConnectionInfoS;


// ****************************************************************************
//                               INPUT CONTROL
// ****************************************************************************

// ****************************************************************************
#if 0
// A point represents a location in (x,y) display space.
typedef struct
{
	UINT16 x; ///< horizontal x coordinate
	UINT16 y; ///< vertical y coordinate
} PicoP_PointS;


// ****************************************************************************
// Define display width and height
typedef struct
{
	UINT16 width;  ///< horizontal width
	UINT16 height; ///< vertical height
} PicoP_RectSizeS;


// ****************************************************************************
// Defines enums for vsync, hsync and clock polarities
typedef enum
{
	ePOLARITY_NEGATIVE = 0, ///< Negative polarity
	ePOLARITY_POSITIVE,     ///< Positive polarity
} PicoP_PolarityE;

// ****************************************************************************
// Defines enums for color space property selection
typedef enum
{
	eCOLOR_SPACE_RGB_DIRECT = 3,
	eCOLOR_SPACE_REC601_YCBCR,
	eCOLOR_SPACE_REC601_BT656,
	eCOLOR_SPACE_REC709_YCBCR // Reserved for Future use
} PicoP_VideoColorSpaceE;


// ****************************************************************************
// Video information struct
// TODO:  Note: this will change for MIPI-DSI in MV650!!!!
typedef struct
{
	PicoP_PointS videoStartPosition; ///< Video capture start line and pixel.
	PicoP_PolarityE hSyncPolarity;  ///< Polarity of HYSNC signal
	PicoP_PolarityE vSyncPolarity;  ///< Polarity of VSYNC signal
	PicoP_PolarityE pixelClockEdge; ///< Active edge of pixel clock
	PicoP_RectSizeS resolution;     ///< x & y resolution of video mode
	FP32 pixelAspectRatio;          ///< Pixel aspect ratio ranges from 0.8 to 1.2.
	PicoP_VideoColorSpaceE colorSpace; ///< color space of the input video mode
} PicoP_VideoCaptureInfoS;


// ****************************************************************************
// Built in video formats
typedef enum
{
	eINPUT_VIDEO_MODE_HANDLE_INVALID = 0,      ///< Invalid Input Video Mode
	eINPUT_VIDEO_MODE_HANDLE_1280X720 = 1,    ///< 1280x720 Input Video Mode
	eINPUT_VIDEO_MODE_HANDLE_1280x640 = 2,     ///< 1280x640 Input Video Mode
	eINPUT_VIDEO_MODE_HANDLE_1280x600 = 3,     ///< 1280x600 Input Video Mode
	eINPUT_VIDEO_MODE_HANDLE_1920X1080 = 4,    ///< 1920x1080 input Video Mode
	eINPUT_VIDEO_MODE_HANDLE_CUSTOM = 5	       ///< Custom Video Mode
} PicoP_VideoModeHandleE;
#endif

// ****************************************************************************
// Input Video States
typedef enum
{
    eINPUT_VIDEO_DISABLED = 0, ///< Input video is Disabled
    eINPUT_VIDEO_ENABLED       ///< Input video is Enabled
} PicoP_InputVideoStateE;



// ****************************************************************************
//                               DISPLAY CONTROL
// ****************************************************************************

// ****************************************************************************
// Color Modes
typedef enum
{
    eCOLOR_MODE_STANDARD = 0, ///< Standard Color mode
    eCOLOR_MODE_BRILLIANT,    ///< Brilliant color mode
    eCOLOR_MODE_RICH,         ///< Rich Color Mode
    eCOLOR_MODE_INVERTED,     ///< Inverted Color Mode, currently not supported
    eCOLOR_MODE_CUSTOM        ///< Custom Color Mode
} PicoP_ColorModeE;

#if 0
// ****************************************************************************
// White Points
typedef enum
{
	eWHITE_POINT_DEFAULT = 0,  ///< Default White Point
	eWHITE_POINT_1,            ///< White Point Setting 1 (See documentation for value)
	eWHITE_POINT_2,            ///< White Point Setting 2 (See documentation for value)
	eMAX_WHITE_POINTS
} PicoP_WhitePointE;
#endif

// ****************************************************************************
// Color selection
typedef enum
{
    ePICOP_COLOR_RED = 0, ///< Enumeration definition for Color Red
    ePICOP_COLOR_GREEN,   ///< Enumeration definition for Color Green
    ePICOP_COLOR_BLUE     ///< Enumeration definition for Color Blue
} PicoP_ColorE;


// ****************************************************************************
// Laser selection
typedef enum
{
    eLASER_RED_1 = 0, ///< Enumeration definition for Red Laser 1
    eLASER_GREEN_1,   ///< Enumeration definition for Green Laser 1
    eLASER_BLUE_1,    ///< Enumeration definition for Blue Laser 1
    eLASER_RED_2,     ///< Enumeration definition for Red Laser 2
    eLASER_GREEN_2    ///< Enumeration definition for Green Laser 2
} PicoP_LaserE;

#if 0
// ****************************************************************************
// A color represents a (R,G,B,A) color value
typedef struct
{
	UINT8 R; ///< Red color value
	UINT8 G; ///< Green color value
	UINT8 B; ///< Blue color value
	UINT8 A; ///< Alpha value
} PicoP_ColorS;
#endif

// ****************************************************************************
// Gamma Boost Modes
typedef enum
{
    eGAMMA_BOOST_MODE_NONE = 0, ///< Standard mode, no brightness boost
    eGAMMA_BOOST_MODE_VIRTUAL_LUMENS_30PCT ///< Boost mode, enhanced lumens
                                           ///boosted 30%
} PicoP_GammaBoostModeE;


// ****************************************************************************
// MicroWeave Modes
typedef enum
{
    eMICROWEAVE_OFF = 0, ///< MicroWeave is off
    eMICROWEAVE_ON       ///< MicroWeave is on
} PicoP_MicroWeaveE;


// ****************************************************************************
// FlipState
typedef enum
{
    eFLIP_NEITHER = 0, ///< Neither direction flipped, default configuration
    eFLIP_HORIZONTAL,  ///< Display flipped Horizontally
    eFLIP_VERTICAL,    ///< Display flipped Vertically
    eFLIP_BOTH         ///< Display flipped Both Horizontally and Vertically
} PicoP_FlipStateE;


// ****************************************************************************
// Output Video States
typedef enum
{
    eOUTPUT_VIDEO_DISABLED = 0, ///< Output video is Disabled
    eOUTPUT_VIDEO_ENABLED       ///< Output video is Enabled
} PicoP_OutputVideoStateE;


// ****************************************************************************
// Output Video Properties
typedef struct
{
    UINT16 horizontalResolution; ///< Display output horizontal resolution
    UINT16 verticalResolution;   ///< Display output vertical resolution
    FP32 frameRate;              ///< Display output frame rate
} PicoP_OutputVideoPropertiesS;


// ****************************************************************************
// Directions
typedef enum
{
    eHORIZONTAL = 0, ///< Horizontal direction
    eVERTICAL        ///< Vertical direction
} PicoP_DirectionE;


// ****************************************************************************
// Color Converter Operations
typedef enum
{
    eRED_TO_RED = 0, ///<  Red to Red
    eGREEN_TO_RED,   ///<  Green to Red
    eBLUE_TO_RED,    ///<  Blue to Red
    eRED_TO_GREEN,   ///<  Red to Green
    eGREEN_TO_GREEN, ///<  Green to Green
    eBLUE_TO_GREEN,  ///<  Blue to Green
    eRED_TO_BLUE,    ///<  Red to Blue
    eGREEN_TO_BLUE,  ///<  Green to Blue
    eBLUE_TO_BLUE    ///<  Blue to Blue
} PicoP_ColorConvertE;

#if 0
// ****************************************************************************
// Horizontal Brightness Compensation Modes
typedef enum
{
	eHBC_OFF = 0,     ///<  Horizontal Brigtness Compensation Off, display brighter at the edges
	eHBC_MEDIUM,      ///<  Horizontal Brightness Compensation set to mid level between Off and Unifor
	eHBC_UNIFORM_BRIGHTNESS,  ///< Horizontal Brigtness Compensation set to produce Uniform Brightness
	eHBC_NUM_MODES
} PicoP_HorizontalBrightnessCompensationModeE;
#endif

// ****************************************************************************
// Frame Rate mode (Frame Rate vs. Vertical Resolutions of display and 3D
// sensing)
typedef enum
{
    eFRAMERATE_60HZ_DISPLAY_720p_SENSING_740p =
        0, ///< 60Hz Frame Rate, 720 display vertical resolution, 740 sensing
           ///vertical resolution  (Default)
    eFRAMERATE_70HZ_DISPLAY_640p_SENSING_640p, ///< 70Hz Frame Rate, 640 display
                                               ///vertical resolution, 640
                                               ///sensing vertical resolution
    eFRAMERATE_70HZ_DISPLAY_600p_SENSING_640p  ///< 70Hz Frame Rate, 600 display
                                               ///vertical resolution, 640
                                               ///sensing vertical resolution
} PicoP_FrameRateE;

// ****************************************************************************
// Display Output Vertical Scaling Mode
typedef enum
{
    eDISPLAY_VERTICAL_SCALING_DOWN =
        0, ///< (Default) If input Resolution greater than display resolution,
           ///< downscale vertically to display resolution
    eDISPLAY_VERTICAL_SCALING_CROP_BOTTOM, ///< If input resolution greater than
                                           ///display resolution, crop the
                                           ///bottom
    ///< of the input video to reach the correct display vertical resolution
    eDISPLAY_VERTICAL_SCALING_CROP_EVEN ///< If input resolution greater than
                                        ///display resolution, crop evenly from
                                        ///the
    ///< top and bottom of the input video to reach the correct display vertical
    ///resolution
} PicoP_DisplayVerticalScalingModeE;


// ****************************************************************************
// Display Output Horizontal Scaling Mode
typedef enum
{
    eDISPLAY_HORIZONTAL_SCALING_DOWN = 0, ///< (Default) If input Resolution
                                          ///greater than display resolution,
                                          ///use horizontal interpolator
    ///< to map video horizontally to correct horizontal resolution to maintain
    ///16:9 aspect ratio
    eDISPLAY_HORIZONTAL_SCALING_NONE ///< Display horizontal pixels to fill the
                                     ///display horizontally with input pixels
    ///< without maintaining original aspect ratio.
} PicoP_DisplayHorizontalScalingModeE;


// ****************************************************************************
// Frame struct holding Frame Rate mode, vertical and horizontal scaling modes.
typedef struct
{
    PicoP_FrameRateE frameRate; ///< DIsplay and 3D sensing frame rate
    PicoP_DisplayVerticalScalingModeE verticalScalingMode; ///< Vertical scaling
    PicoP_DisplayHorizontalScalingModeE horizontalScalingMode; ///< Horizontal
                                                               ///scaling
} PicoP_FrameRateModeS;


// ****************************************************************************
//                               3D SENSING
// ****************************************************************************

// ****************************************************************************
// 3D Sensing States
typedef enum
{
    eSENSING_DISABLED = 0, ///< 3D Sensing is Disabled
    eSENSING_ENABLED       ///< 3D Sensing is Enabled
} PicoP_SensingStateE;

#if 0
// ****************************************************************************
// 3D Sensing Data Interfaces
typedef enum
{
	eSENSING_DATA_INTERFACE_PCAM = 0, ///< Enable Parallel Camera Interface
	eSENSING_DATA_INTERFACE_MIPI,     ///< Enable MIPI-CSI Interface
	eSENSING_DATA_INTERFACE_BOTH      ///< Enable Both PCAM and MIPI-CSI Interfaces
} PicoP_SensingDataInterfaceE;


// ****************************************************************************
// 3D Sensing Data Format
typedef enum
{
	eTOF_DATA_DEPTH_AMPLITUDE = 0,   ///< Depth & Amplitude Data (Default) 
	eTOF_DATA_DEPTH,                 ///< Depth data only Fused Amplitude & Depth Data
	eTOF_DATA_AMPLITUDE              ///< Left Detector Depth & Amplitude Data
} PicoP_ToFDataFormatE;


// ****************************************************************************
// 3D Sensing Pulsing Modes
typedef enum
{
	eTOF_PULSING_EQUAL_ANGLE = 0, ///< Equal Angle Pulsing 
	eTOF_PULSING_EQUAL_TIME,      ///< Equal Time Pulsing 
	eTOF_PULSING_POLYNOMIAL       ///< Polynomial Pulsing Pattern 
} PicoP_ToFPulsingModeE;


// ****************************************************************************
// 3D Sensing Pulsing Configuration
typedef struct
{
	PicoP_ToFPulsingModeE pulsingMode; ///< Pulsing Mode 
	UINT16 nrPulsesPerLine; ///< number of pulses per line
	UINT32 nrLinePhases;  ///< Number of phased lines: 0, 2, or 4
	UINT32 nrFramePhases; ///< Number of phased frames: 0 - 4
	UINT32 params0;       ///< For future expansion
	UINT32 params1;       ///< For future expansion 
} PicoP_TofPulsingConfigS;
#endif



// ****************************************************************************
//                            SYSTEM MANAGEMENT
// ****************************************************************************

// ****************************************************************************
// System information struct
typedef struct
{
    char serialNumber[SYSTEM_SN_LEN]; ///< System serial number
    UINT32 firmwareVersion;           ///< PSE Firmware version
    UINT32 electronicsVersion;        ///< Electronics version
    UINT32 versionA;                  ///< Reserved for future use
    UINT32 versionB;                  ///< Reserved for future use
    UINT32 versionC;                  ///< Reserved for future use
    UINT32 versionD;                  ///< Reserved for future use
    UINT32 versionE;                  ///< Reserved for future use
} PicoP_SystemInfoS;


//****************************************************************************
// System State
typedef enum
{
    eON = 0,   ///< System Running and ready to accept input video and/or output
               ///point cloud
    eSTARTING, ///< System Starting Up
    eCALIBRATION, ///< System in Calibration State
    eFAULT        ///< System at Fault state
} PicoP_SystemStateE;



//****************************************************************************
// System Status
typedef struct
{
    PicoP_SystemStateE systemState; ///< System State. E.g. ready accept video,
                                    ///starting up, fault, calibration
    UINT32 systemFault; ///< 32 bit value, 0 means system OK, non-zero value
    /// indicates a fault code and system not OK
    FP32 temperature; ///< PSE Temperature
    UINT32 data0;     ///< Data 0 for future expansion
    UINT32 data1;     ///< Data 1 for future expansion
    UINT32 data2;     ///< Data 2 for future expansion
    UINT32 data3;     ///< Data 3 for future expansion
    UINT32 data4;     ///< Data 4 for future expansion
} PicoP_SystemStatusS;



// ****************************************************************************
// System Event struct
typedef struct
{
    UINT16 session; ///<  session number when event occureed
    UINT16 eventId; ///<  Event ID
    UINT32 time;    ///<  Event time stamp
    UINT32 data;    ///<  Event data
    UINT16 cid;     ///<  Event component ID
    UINT16 line;    ///<  Event line number
} PicoP_EventS;

#if 0
typedef enum
{
    eRUNNING = 0,              ///<  System Running
    ePOWERSAVER_DISPLAY_ONLY,  ///<  System using display only, Sensing turned off
	ePOWERSAVER_SENSING_ONLY,  ///<  System using sensing only, Visible display turned off
	ePOWERSAVER_STANDBY,       ///<  System in Standby
    eSHUTDOWN                  ///<  System will Shutdown
} PicoP_PowerStateE;
#endif


// ****************************************************************************
//                               RENDERING
// ****************************************************************************

#if 0
// ****************************************************************************
// On-Screen Display (OSD) States
typedef enum
{
	eOSD_DISABLED = 0, ///<  OSD is Disabled
	eOSD_ENABLED       ///<  OSD is Enabled
} PicoP_OSDStateE;


// ****************************************************************************
// Target rendering buffers for draw functions
typedef enum
{
	eOSD_0 = 0,      ///<  On Screen Display 0
	eOSD_1,          ///<  On Screen Display 1
	eFRAME_BUFFER_0, ///<  FrameBuffer 0
	eFRAME_BUFFER_1, ///<  FrameBuffer 1
	eFRAME_BUFFER_2  ///<  FrameBuffer 2
} PicoP_RenderTargetE;
#endif

// ****************************************************************************
// Test Patterns
typedef enum
{
    eTEST_PATTERN_OFF = 0,  ///< Test Patterns Off
    eCHECKERBOARD_PATTERN,  ///<  Checkerboard pattern
    eCROSSHAIR_PATTERN,     ///<  Bordered Crosshair pattern
    eGRID_PATTERN,          ///<  Bordered Grid Pattern
    eCONSTANT_COLOR_PATTERN ///<  Constant Single Color Pattern
} PicoP_TestPatternE;


#endif // PICOP_DEF_H
