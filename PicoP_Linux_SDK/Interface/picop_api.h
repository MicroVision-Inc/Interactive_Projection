//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!
//! \file picop_api.h
//!
//! \brief Interface file for PicoP Scanning Engine SDK
//!
//! Copyright (C) 2018-2019 MicroVision, Inc.
//!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#ifndef PICOP_API_H
#define PICOP_API_H

#include "picop_rc.h"
#include "picop_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


// ************************************************************************************************
//                                     CONNECTION MANAGEMENT
// ************************************************************************************************
// The following functions will be available in Host Library only:
//

//! \brief This function must be the first call into the SDK API library. It
//opens the library and allocates resources necessary for operation.
//! \param    libraryHandle [OUT]  Returned handle to the library that must be
//used in subsequent calls.
PicoP_ReturnStatus PicoP_OpenLibrary(PicoP_HANDLE *libraryHandle);

//! \brief Return the version and capability information of the SDK library
//! \param libraryHandle   [IN] A valid library handle previously obtained by a
//! call to PicoP_OpenLibrary()
//! \param libraryInfo   [OUT] Library info structure containing major version,
//! minor version and capability flags of the SDK Library
//!
PicoP_ReturnStatus PicoP_GetLibraryInfo(const PicoP_HANDLE libraryHandle,
                                        PicoP_LibraryInfoS *libraryInfo);


//! \brief Closes the SDK API library and releases all resources. It also closes
//any open connections.
//! \param libraryHandle [IN] A valid library handle previously obtained by a
//call to PicoP_OpenLibrary()
PicoP_ReturnStatus PicoP_CloseLibrary(const PicoP_HANDLE libraryHandle);


//! \brief Opens a connection to the PicoP Scanning Engine (PSE) using either
//USB or Uart
//! \param libraryHandle [IN] A valid library handle previously obtained by a
//call to PicoP_OpenLibrary()
//! \param connectionInfo [IN] Information about the connection, includes items
//such as type, port#, baud rate, etc.
//! \param connectionHandle [OUT] A pointer to an PicoP_HANDLE that is set to
//not NULL if the connection was successful.
PicoP_ReturnStatus PicoP_OpenConnection(
    const PicoP_HANDLE libraryHandle,
    const PicoP_ConnectionInfoS connectionInfo, PicoP_HANDLE *connectionHandle);


//! \brief Opens a connection to the PicoP Scanning Engine (PSE) using USB
//! \param libraryHandle [IN] A valid library handle previously obtained by a
//call to PicoP_OpenLibrary()
//! \param connectionInfo [IN] Information about the connection, includes items
//such as productID
//! \param connectionHandle [OUT] A pointer to an PicoP_HANDLE that is set to
//not NULL if the connection was successful.
PicoP_ReturnStatus PicoP_OpenConnectionUSB(const PicoP_HANDLE libraryHandle,
                                           const PicoP_USBInfoS connectionInfo,
                                           PicoP_HANDLE *connectionHandle);

//! \brief Opens a connection to the PicoP Scanning Engine (PSE) using Uart
//! \param libraryHandle [IN] A valid library handle previously obtained by a
//call to PicoP_OpenLibrary()
//! \param connectionInfo [IN] Information about the connection, includes items
//such as baud rate, parity, etc.
//! \param connectionHandle [OUT] A pointer to an PicoP_HANDLE that is set to
//not NULL if the connection was successful.
PicoP_ReturnStatus PicoP_OpenConnectionUart(
    const PicoP_HANDLE libraryHandle, const PicoP_UartInfoS connectionInfo,
    PicoP_HANDLE *connectionHandle);


//! \brief Closes a previously opened connection to the PicoP Scanning Engine
//! \param connectionHandle [IN] An open handle to the connection to be closed
PicoP_ReturnStatus PicoP_CloseConnection(const PicoP_HANDLE connectionHandle);



// ************************************************************************************************
//                                     INPUT CONTROL
// ************************************************************************************************
// Note: The functions parameters will change based on what is needed for MIPI-DSI controls

//! \brief Return detected input video Frame Rate and Lines per Frame.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param horizontalPixels [OUT] Detected number of pixels per line
//! \param verticalLines [OUT] Detected number of vertical lines per frames
//! \param frameRate [OUT] Detected Frames per second
PicoP_ReturnStatus PicoP_GetInputVideoProperties(PicoP_HANDLE connectionHandle,
                                                 UINT16 *horizontalPixels,
                                                 UINT16 *verticalLines,
                                                 FP32 *frameRate);


//! \brief Set the video input state to enabled or disabled.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param inputVideoState   [IN] Accepted values are eINPUT_VIDEO_DISABLED or
//!        eINPUT_VIDEO_ENABLED.
//! \param commit  [IN] 0 = No commit, 1 = commit.
//!
PicoP_ReturnStatus PicoP_SetInputVideoState(
    PicoP_HANDLE connectionHandle, const PicoP_InputVideoStateE inputVideoState,
    const BOOL commit);


//! \brief Return the current video input state.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param inputVideoState   [OUT] Current Video Input State. Returned value
//!                     can be eINPUT_VIDEO_DISABLED or eINPUT_VIDEO_ENABLED.
//! \param storageType [IN] eCURRENT_VALUE, eVALUE_ON_STARTUP, or
//eFACTORY_VALUE.
//!
PicoP_ReturnStatus PicoP_GetInputVideoState(
    PicoP_HANDLE connectionHandle, PicoP_InputVideoStateE *inputVideoState,
    const PicoP_ValueStorageTypeE storageType);

// ************************************************************************************************
//                                     DISPLAY CONTROL
// ************************************************************************************************

//! \brief Set the brightness of the projection display.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param brightnessValue   [IN] Brightness level to set. Range 0 - 100. 0
//! means display off, 100 indicates max brightness.
//! \param commit  [IN] FALSE = No commit, TRUE = commit/persist to non-volatile
//memory.
//!
PicoP_ReturnStatus PicoP_SetBrightness(PicoP_HANDLE connectionHandle,
                                       const UINT8 brightnessValue,
                                       const BOOL commit);


//! \brief Return the brightness of the projection display.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param brightnessValue   [OUT] Display Brightness  Range 0 - 100. 0 means
//! display off, 100 indicates max brightness.
//! \param storageType [IN] eCURRENT_VALUE, eVALUE_ON_STARTUP, or
//eFACTORY_VALUE.
//!
PicoP_ReturnStatus PicoP_GetBrightness(
    PicoP_HANDLE connectionHandle, UINT8 *brightnessValue,
    const PicoP_ValueStorageTypeE storageType);


//! \brief Set color mode for the output display.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param colorMode   [IN] Accepted values are eCOLOR_MODE_BRILLIANT,
//! eCOLOR_MODE_STANDARD, eCOLOR_MODE_RICH, or eCOLOR_MODE_INVERTED.
//! \param commit  [IN] FALSE = No commit, TRUE = commit/persist to non-volatile
//memory.
//!
PicoP_ReturnStatus PicoP_SetColorMode(PicoP_HANDLE connectionHandle,
                                      const PicoP_ColorModeE colorMode,
                                      const BOOL commit);


//! \brief Get color mode for the output display.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param colorMode   [OUT] Returned color mode 0 = Brilliant, 1 = Standard, 2
//= Inverted.
//! \param storageType [IN] eCURRENT_VALUE, eVALUE_ON_STARTUP, or
//eFACTORY_VALUE.
//!
PicoP_ReturnStatus PicoP_GetColorMode(
    PicoP_HANDLE connectionHandle, PicoP_ColorModeE *colorMode,
    const PicoP_ValueStorageTypeE storageType);

//! \brief Set the display gamma value.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param color   [IN] Color or Colors whose gamma is to be set.
//! \param gammaValue   [IN] gamma value, range 1.0 - 3.0
//! \param commit  [IN] FALSE = No commit, TRUE = commit/persist to non-volatile
//memory.
PicoP_ReturnStatus PicoP_SetGamma(PicoP_HANDLE connectionHandle,
                                  const PicoP_ColorE color,
                                  const FP32 gammaValue, const BOOL commit);


//! \brief Return display gamma value.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param color   [IN] Color whose gamma is to be returned.
//! \param gammaValue   [OUT] Gamma value.
//! \param storageType [IN] eCURRENT_VALUE, eVALUE_ON_STARTUP, or
//eFACTORY_VALUE.
//!
PicoP_ReturnStatus PicoP_GetGamma(PicoP_HANDLE connectionHandle,
                                  const PicoP_ColorE color, FP32 *gammaValue,
                                  const PicoP_ValueStorageTypeE storageType);


//! \brief Set the color specific video gamma boost mode.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param color   [IN] Color whose gamma mode is to be set.
//! \param boostMode   [IN] Video gamma boost mode.
//! \param commit  [IN] FALSE = No commit, TRUE = commit/persist to non-volatile
//memory.
//!
PicoP_ReturnStatus PicoP_SetVideoGammaBoostMode(
    PicoP_HANDLE connectionHandle, const PicoP_ColorE color,
    const PicoP_GammaBoostModeE boostMode, const BOOL commit);


//! \brief Return the color specific video gamma boost mode.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param color   [IN] Color whose gamma mode is to be returned.
//! \param boostMode   [OUT] Video gamma boost mode.
//! \param storageType [IN] eCURRENT_VALUE, eVALUE_ON_STARTUP, or
//eFACTORY_VALUE.
//!
PicoP_ReturnStatus PicoP_GetVideoGammaBoostMode(
    PicoP_HANDLE connectionHandle, const PicoP_ColorE color,
    PicoP_GammaBoostModeE *boostMode,
    const PicoP_ValueStorageTypeE storageType);


//! \brief Select the MicroWeave mode.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param microWeaveMode   [IN] MicroWeaveMode to set.
//! \param commit  [IN] FALSE = No commit, TRUE = commit/persist to non-volatile
//memory.
//!
PicoP_ReturnStatus PicoP_SetMicroWeaveMode(PicoP_HANDLE connectionHandle,
                                           PicoP_MicroWeaveE microWeaveMode,
                                           const BOOL commit);


//! \brief Return the MicroWeave mode.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param microWeaveMode  [OUT] current MicroWeave Mode.
//! \param storageType [IN] eCURRENT_VALUE, eVALUE_ON_STARTUP, or
//eFACTORY_VALUE.
//!
PicoP_ReturnStatus PicoP_GetMicroWeaveMode(
    PicoP_HANDLE connectionHandle, PicoP_MicroWeaveE *microWeaveMode,
    const PicoP_ValueStorageTypeE storageType);


//! \brief Set the Flip State.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param flipState   [IN] flipState.Accepted values are eFLIP_NEITHER or
//!                         HORIZONTAL or eFLIP_VERTICAL or eFLIP_BOTH.
//! \param commit  [IN] FALSE = No commit, TRUE = commit/persist to non-volatile
//memory.
//!
PicoP_ReturnStatus PicoP_SetFlipState(PicoP_HANDLE connectionHandle,
                                      const PicoP_FlipStateE flipState,
                                      const BOOL commit);


//! \brief Return the current Flip state.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param flipState   [OUT] Current flip State. eFLIP_NEITHER or HORIZONTAL or
//!                             eFLIP_VERTICAL or eFLIP_BOTH.
//! \param storageType [IN] eCURRENT_VALUE, eVALUE_ON_STARTUP, or
//eFACTORY_VALUE.
//!
PicoP_ReturnStatus PicoP_GetFlipState(
    PicoP_HANDLE connectionHandle, PicoP_FlipStateE *flipState,
    const PicoP_ValueStorageTypeE storageType);


//! \brief Set the video output state to enabled or disabled.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param outputVideoState   [IN] Accepted values are eVIDEO_OUTPUT_DISABLED
//!                 or eVIDEO_OUTPUT_ENABLED.
//! \param commit  [IN] FALSE = No commit, TRUE = commit/persist to non-volatile
//memory.
//!
PicoP_ReturnStatus PicoP_SetOutputVideoState(
    PicoP_HANDLE connectionHandle,
    const PicoP_OutputVideoStateE outputVideoState, const BOOL commit);


//! \brief Returns the video output state (enabled or disabled).
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param outputVideoState   [OUT] Current Video Output State. Returned value
//!                     can be eVIDEO_OUTPUT_DISABLED or eVIDEO_OUTPUT_ENABLED.
//! \param storageType [IN] eCURRENT_VALUE, eVALUE_ON_STARTUP, or
//eFACTORY_VALUE.
//!
PicoP_ReturnStatus PicoP_GetOutputVideoState(
    PicoP_HANDLE connectionHandle, PicoP_OutputVideoStateE *outputVideoState,
    const PicoP_ValueStorageTypeE storageType);


//! \brief Return output video properties.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param outputVideoProperties [OUT] current output horizontal resolution,
//!        output vertical resolution, and frame rate.
//!
PicoP_ReturnStatus PicoP_GetOutputVideoProperties(
    PicoP_HANDLE connectionHandle,
    PicoP_OutputVideoPropertiesS *outputVideoProperties);

//! \brief Set the scan line phase delay to align the forward and reverse scan
//video.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param phaseValue   [IN] Phase Value. Range -100 to 100
//! \param commit  [IN] FALSE = No commit, TRUE = commit/persist to non-volatile
//memory.
//!
PicoP_ReturnStatus PicoP_SetBiPhase(PicoP_HANDLE connectionHandle,
                                    const INT16 phaseValue, const BOOL commit);


//! \brief Return the scan line phase delay to align the forward and reverse
//video lines.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param phaseValue   [OUT] Phase Value. Range -100 to 100
//! \param storageType [IN] eCURRENT_VALUE, eVALUE_ON_STARTUP, or
//eFACTORY_VALUE.
//!
PicoP_ReturnStatus PicoP_GetBiPhase(PicoP_HANDLE connectionHandle,
                                    INT16 *phaseValue,
                                    const PicoP_ValueStorageTypeE storageType);


//! \brief Perform vertical or horizontal color alignment for the selected
//color.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param direction   [IN] Valid values are eHORIZONTAL and eVERTICAL.
//! \param color   [IN] Valid values are eRED, eGREEN or eBLUE.
//! \param offset  [IN] This value ranges from +32 to -32.
//! \param commit  [IN] FALSE = No commit, TRUE = commit/persist to non-volatile
//memory.
//!
PicoP_ReturnStatus PicoP_SetColorAlignment(PicoP_HANDLE connectionHandle,
                                           const PicoP_DirectionE direction,
                                           const PicoP_LaserE color,
                                           const INT16 offset,
                                           const BOOL commit);


//! \brief Return the vertical or horizontal color alignment offset value
//!       for the selected color.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param direction   [IN] Valid values are eHORIZONTAL and eVERTICAL.
//! \param color   [IN] Valid values are eRED, eGREEN or eBLUE.
//! \param offset  [OUT] This value ranges from +32 to -32.
//! \param storageType [IN] eCURRENT_VALUE, eVALUE_ON_STARTUP, or
//eFACTORY_VALUE.
//!
PicoP_ReturnStatus PicoP_GetColorAlignment(
    PicoP_HANDLE connectionHandle, const PicoP_DirectionE direction,
    const PicoP_LaserE color, INT16 *offset,
    const PicoP_ValueStorageTypeE storageType);


//! \brief Sets the color converter values allowing input colors to be remapped
//! to different display colors.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param color   [IN] Valid values are eRED_TO_RED, eGREEN_TO_RED,
//! eBLUE_TO_RED, eRED_TO_GREEN, eGREEN_TO_GREEN, eBLUE_TO_GREEN, eRED_TO_BLUE,
//! eGREEN_TO_BLUE and eBLUE_TO_BLUE.
//! \param coefficient [IN] This value ranges from -65536 to +65535.
//! \param commit  [IN] FALSE = No commit, TRUE = commit/persist to non-volatile
//memory.
//!
PicoP_ReturnStatus PicoP_SetColorConverter(PicoP_HANDLE connectionHandle,
                                           const PicoP_ColorConvertE color,
                                           const INT32 coefficient,
                                           const BOOL commit);


//! \brief Get the color converter values.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param color   [IN] Valid values are eRED_TO_RED, eGREEN_TO_RED,
//! eBLUE_TO_RED, eRED_TO_GREEN, eGREEN_TO_GREEN, eBLUE_TO_GREEN, eRED_TO_BLUE,
//! eGREEN_TO_BLUE and eBLUE_TO_BLUE
//! \param coefficient [OUT] This value ranges from -65536 to +65535.
//! \param storageType [IN] eCURRENT_VALUE, eVALUE_ON_STARTUP, or
//eFACTORY_VALUE.
//!
PicoP_ReturnStatus PicoP_GetColorConverter(
    PicoP_HANDLE connectionHandle, const PicoP_ColorConvertE color,
    INT32 *coefficient, const PicoP_ValueStorageTypeE storageType);


//! \brief Set the output frame rate mode (frame rate vs. vertical display and
//sensing resolutions) and display scaling.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param frameRateMode [IN] Display Frame Rate and vertical & 3D sensing
//resolution.  Also specify
//!       how display is scaled vertically and horizontally if input resolution
//> display resolution.
//! \param commit  [IN] FALSE = No commit, TRUE = commit/persist to non-volatile
//memory.
//!
PicoP_ReturnStatus PicoP_SetFrameRateMode(
    PicoP_HANDLE connectionHandle, const PicoP_FrameRateModeS frameRateMode,
    const BOOL commit);


//! \brief Return the output frame rate mode (frame rate vs. vertical display
//and sensing resolutions) and display scaling.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param frameRateMode [IN] Display Frame Rate and vertical & 3D sensing
//resolution.  Also specify
//!       how display is scaled vertically and horizontally if input resolution
//> display resolution.
//! \param storageType [IN] eCURRENT_VALUE, eVALUE_ON_STARTUP, or
//eFACTORY_VALUE.
//!
PicoP_ReturnStatus PicoP_GetFrameRateMode(
    PicoP_HANDLE connectionHandle, PicoP_FrameRateModeS *frameRateMode,
    const PicoP_ValueStorageTypeE storageType);



// ************************************************************************************************
//                                     3D SENSING
// ************************************************************************************************

//! \brief Set the 3D Sensing function On or Off.  When the 3D Sensing function
//! is off, the IR laser is not pulsing and no 3D sensing data is sent over the
//! Parallel Camera or MIPI-CSI2 interfaces.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param sensingState   [IN]  eSENSING_DISABLED or eSENSING_ENABLED
//! \param commit  [IN] FALSE = No commit, TRUE = commit/persist to non-volatile
//memory.
//!
PicoP_ReturnStatus PicoP_SetSensingState(PicoP_HANDLE connectionHandle,
                                         const PicoP_SensingStateE sensingState,
                                         const BOOL commit);


//! \brief Return the 3D Sensing function state (on or off).
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param sensingState   [OUT]  eSENSING_DISABLED or eSENSING_ENABLED
//! \param storageType [IN] eCURRENT_VALUE, eVALUE_ON_STARTUP, or
//eFACTORY_VALUE.
//!
PicoP_ReturnStatus PicoP_GetSensingState(
    PicoP_HANDLE connectionHandle, PicoP_SensingStateE *sensingState,
    const PicoP_ValueStorageTypeE storageType);


// ************************************************************************************************
//                                     SYSTEM MANAGEMENT
// ************************************************************************************************

//! \brief Return information about the PSE system including System Serial
//! Number, Software Version and Electronics Version.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param systemInfo   [OUT] Returned system information structure.
//!
PicoP_ReturnStatus PicoP_GetSystemInfo(PicoP_HANDLE connectionHandle,
                                       PicoP_SystemInfoS *systemInfo);

//! \brief Retun the system status, operational state, and temperature.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param systemStatus   [OUT] System status structure.  Status = 0 means
//! system Ok. Non-zero value indicates system not OK.
//!
PicoP_ReturnStatus PicoP_GetSystemStatus(PicoP_HANDLE connectionHandle,
                                         PicoP_SystemStatusS *systemStatus);


//! \brief Return the system event log.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param numEvents [IN] Number of event log to be retrieved.
//! \param event [OUT] Pointer to hold event log.
//!
PicoP_ReturnStatus PicoP_GetEventLog(PicoP_HANDLE connectionHandle,
                                     const UINT16 numEvents,
                                     PicoP_EventS *event);


//! \brief Restore all user settings to factory defaults.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//!
PicoP_ReturnStatus PicoP_RestoreFactorySettings(PicoP_HANDLE connectionHandle);


//! \brief Commit all user settings to non-volatile flash memory.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//!
PicoP_ReturnStatus PicoP_CommitAll(PicoP_HANDLE connectionHandle);


//! \brief This function loads the EP software image for firmware upgrade.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param byte_index [IN] Size of SW image.
//! \param byte_size [IN] Index of data in image.
//! \param image [IN] Data of SW image.
//! \param byte_total [IN] Total SW image size.
//!
PicoP_ReturnStatus PicoP_UpgradeSoftware(
    PicoP_HANDLE connectionHandle, const UINT32 byte_index,
    const UINT32 byte_size, const UINT32 byte_total,
    const UINT8 image[MAX_UPGRADE_DATA_SIZE]);



// ************************************************************************************************
//                                     RENDERING
// ************************************************************************************************


//! \brief Display a test pattern.  If connected to input video, use
//! PicoP_SetInputVideoState() to disable input video.
//! \param connectionHandle [IN] A valid connection handle previously obtained
//by a call to OpenConnection().
//! \param testPattern  [IN] Test pattern to draw. Supported Test patterns:
//! eCHECKERBOARD_PATTERN, eCROSSHAIR_PATTERN,  eGRID_PATTERN,
//eCONSTANT_COLOR_PATTERN
//! \param foregroundColor   [IN] Pattern Color - 24-Bit RGB foregound pattern
//color value, 0xRRGGBB.
//! \param backgroundColor   [IN] Background Color - 24-Bit RGB background color
//value, 0xRRGGBB.
//!
PicoP_ReturnStatus PicoP_DrawTestPattern(PicoP_HANDLE connectionHandle,
                                         const PicoP_TestPatternE testPattern,
                                         const UINT32 foregroundColor,
                                         const UINT32 backgroundColor);


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
#endif // PICOP_API_H
