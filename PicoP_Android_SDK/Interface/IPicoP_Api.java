/*
 * Copyright (C) 2019 MicroVision, Inc.
 */

package picop.interfaces.def;


/**
 * This class provides the interface for PicoP Display and Sensing API's
 */
public interface IPicoP_Api {
    /**
     * Opens the interface library and allocates resources necessary for operation. Returns
     * a handle to the library to be used for opening a connection to the PSE. The application
     * must call the PicoP_OpenLibrary() function first to populate connectionHandle that can then
     * be used to open a communications connection.
     *
     * @param connectionHandle A pointer to a {@link picop.interfaces.def.PicoP_Handle} that is set to an open handle
     *                         of the library
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_OpenLibrary(PicoP_Handle connectionHandle);

    /**
     * Closes the library, releases all resources and closes all open connections. This
     * function should be called last upon exiting the application.
     *
     * @param connectionHandle A valid {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_CloseLibrary(PicoP_Handle connectionHandle);

    /**
     * This function is used to retrieve the version and capability information of the SDK
     * Library.
     *
     * @param connectionHandle A valid {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param libraryInfo      A pointer to an {@link PicoP_LibraryInfoS}
     *                         that will be filled with the information
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetLibraryInfo(PicoP_Handle connectionHandle, PicoP_LibraryInfoS libraryInfo);

    /**
     * This function Opens a connection to the PSE.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param connectionType   Type of {@link picop.interfaces.def.PicoP_ConnectionTypeE} to be opened.
     * @param connectionInfo   Information about the connection as defined in {@link picop.interfaces.def.PicoP_ConnectionInfo}
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_OpenConnection(PicoP_Handle connectionHandle,
                                         PicoP_ConnectionTypeE connectionType, PicoP_ConnectionInfo connectionInfo);

    /**
     * This function Closes a previously opened connection to PSE.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param connectionType   Type of {@link picop.interfaces.def.PicoP_ConnectionTypeE} to be opened.
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_CloseConnection(PicoP_Handle connectionHandle, PicoP_ConnectionTypeE connectionType);

    /**
     * Commit all user settings to flash that have been set with commit = 0/false.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously populated by a call
     *                         to PicoP_OpenLibrary()
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_CommitAll(PicoP_Handle connectionHandle);

    /**
     * This function restores PSE system configuration to the factory default settings.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously populated by a call
     *                         to PicoP_OpenLibrary()
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_RestoreFactorySettings(PicoP_Handle connectionHandle);

    /**
     * Return information about the PSE system including System Serial
     * Number, Software Version and Electronics Version.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously populated by a call
     *                         to PicoP_OpenLibrary()
     * @param systemInfo       {@link picop.interfaces.def.PicoP_SystemInfoS} to hold system info
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetSystemInfo(PicoP_Handle connectionHandle, PicoP_SystemInfoS systemInfo);

    /**
     * Retun the system status, operational state, and temperature.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously populated by a call
     *                         to PicoP_OpenLibrary()
     * @param systemStatus     {@link picop.interfaces.def.PicoP_SystemStatusS} to hold system status
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetSystemStatus(PicoP_Handle connectionHandle, PicoP_SystemStatusS systemStatus);

    /**
     * Return the system event log.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously populated by a call
     *                         to PicoP_OpenLibrary()
     * @param numEvents        Number of event log to be retrieved.
     * @param pEvent           {@link picop.interfaces.def.PicoP_EventS} to hold event log.
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetEventLog(PicoP_Handle connectionHandle, short numEvents, PicoP_EventS[] pEvent);

    /**
     * This function enables or disables the PSE input video interface. When the input video
     * is disabled, the PSE frame buffer will not be updated and the output video will contain
     * the last received frame.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param state            Enable/Disable as defined in {@link picop.interfaces.def.PicoP_InputVideoStateE}
     * @param bCommit          False = Do not persist (value restored back after power cycle), True = Persist setting
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_SetInputVideoState(PicoP_Handle connectionHandle,
                                             PicoP_InputVideoStateE state, boolean bCommit);

    /**
     * Returns the current state of the input video.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param callback         Callback to store current {@link picop.interfaces.def.PicoP_InputVideoStateE}
     * @param storageType      Storage Types as defined in {@link picop.interfaces.def.PicoP_ValueStorageTypeE}
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetInputVideoState(PicoP_Handle connectionHandle,
                                             PicoP_Api.PicoP_Callback callback, PicoP_ValueStorageTypeE storageType);

    /**
     * Get input video properties.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param callback         Callback to store current value of PicoP_InputVideoPropertiesS as defined in {@link picop.interfaces.def.PicoP_InputVideoPropertiesS}
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetInputVideoProperties(PicoP_Handle connectionHandle,
                                                  PicoP_Api.PicoP_Callback callback);

    /**
     * This function enables or disables the output video. When the output video is
     * disabled, the display is blanked.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param state            Enable/Disable as defined in {@link picop.interfaces.def.PicoP_OutputVideoStateE}
     * @param bCommit          False = Do not persist (value restored back after power cycle), True = Persist setting
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_SetOutputVideoState(PicoP_Handle connectionHandle, PicoP_OutputVideoStateE state, boolean bCommit);


    /**
     * Returns the current state of the output video.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param callback         Callback to store current {@link picop.interfaces.def.PicoP_OutputVideoStateE}
     * @param storageType      Storage Types as defined in {@link picop.interfaces.def.PicoP_ValueStorageTypeE}
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetOutputVideoState(PicoP_Handle connectionHandle, PicoP_Api.PicoP_Callback callback, PicoP_ValueStorageTypeE storageType);

    /**
     * Set the 3D Sensing function On or Off.  When the 3D Sensing function
     * is off, the IR laser is not pulsing and no 3D sensing data is sent over the
     * Parallel Camera or MIPI-CSI2 interfaces.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param sensingState     State as defined in {@link picop.interfaces.def.PicoP_SensingStateE}
     * @param bCommit          False = Do not persist (value restored back after power cycle), True = Persist setting
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_SetSensingState(PicoP_Handle connectionHandle, PicoP_SensingStateE sensingState, boolean bCommit);

    /**
     * Return the 3D Sensing function state (on or off).
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param callback         Callback to store current {@link picop.interfaces.def.PicoP_SensingStateE}
     * @param storageType      Storage Types as defined in {@link picop.interfaces.def.PicoP_ValueStorageTypeE}
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetSensingState(PicoP_Handle connectionHandle, PicoP_Api.PicoP_Callback callback, PicoP_ValueStorageTypeE storageType);

    /**
     * This function allows the host to flip the display horizontally, vertically, or both
     * horizontally and vertically.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param flipState        State as defined in {@link picop.interfaces.def.PicoP_FlipStateE}
     * @param bCommit          False = Do not persist (value restored back after power cycle), True = Persist setting
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_SetFlipState(PicoP_Handle connectionHandle,
                                       PicoP_FlipStateE flipState, boolean bCommit);

    /**
     * Returns the flip state of the display.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param callback         Callback to store current {@link picop.interfaces.def.PicoP_FlipStateE}
     * @param storageType      Storage Types as defined in {@link picop.interfaces.def.PicoP_ValueStorageTypeE}
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetFlipState(PicoP_Handle connectionHandle,
                                       PicoP_Api.PicoP_Callback callback, PicoP_ValueStorageTypeE storageType);

    /**
     * The PSE projection display utilized Red (640 nm), Green (532 nm), Blue (450 nm) laser
     * diodes as light sources to illuminate the scanned beam projection display. The laser
     * diodes enable the PSE to provide a wide gamut of brilliant and reproducible colors. This
     * expanded color gamut is significantly larger than standard color gamuts, such as sRBG.
     * This function sets the PSE color mode.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param colorMode        Modes as defined in {@link picop.interfaces.def.PicoP_ColorModeE}
     * @param bCommit          False = Do not persist (value restored back after power cycle), True = Persist setting
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_SetColorMode(PicoP_Handle connectionHandle,
                                       PicoP_ColorModeE colorMode, boolean bCommit);

    /**
     * Returns the current color mode setting from the PSE.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param callback         Callback to store current {@link picop.interfaces.def.PicoP_ColorModeE}
     * @param storageType      Storage Type as defined in {@link picop.interfaces.def.PicoP_ValueStorageTypeE}
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetColorMode(PicoP_Handle connectionHandle,
                                       PicoP_Api.PicoP_Callback callback, PicoP_ValueStorageTypeE storageType);

    /**
     * Gamma is the relationship between the numerical value of a pixel in an
     * image file and the brightness of the projected pixel when viewed on screen. This
     * relationship is non-linear, meaning that a change in pixel value does not translate into an
     * equivalent change in brightness. For almost all TVs and computer monitors, a change in
     * pixel value results in a change in brightness raised to the 2.2 power. The gamma for these
     * devices, therefore, is said to be 2.2. This function allows the host to manipulate/adjust the
     * gamma setting of each displayed primary display.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param color            Color as defined in {@link picop.interfaces.def.PicoP_ColorE}
     * @param gammaValue       A floating point value for gamma, range 1.0 - 3.0
     * @param bCommit          False = Do not persist (value restored back after power cycle), True = Persist setting
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_SetGamma(PicoP_Handle connectionHandle, PicoP_ColorE color,
                                   float gammaValue, boolean bCommit);

    /**
     * Returns the current display gamma value.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param callback         Callback to store current float gamma value
     * @param color            Boost mode for the color as defined in {@link picop.interfaces.def.PicoP_ColorE}
     * @param storageType      Storage Type as defined in {@link picop.interfaces.def.PicoP_ValueStorageTypeE}
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetGamma(PicoP_Handle connectionHandle,
                                   PicoP_Api.PicoP_Callback callback, PicoP_ColorE color,
                                   PicoP_ValueStorageTypeE storageType);

    /**
     * The PSE supports an Enhanced Brightness mode which is achieved by boosting the
     * brightness of video codes around code level 0.3 (or 76 of 255).
     * These video code values are the most common in typical pictures and movie frames.
     * When the Gamma Boost mode is enabled, the brightness of these video codes
     * is increased up to 30% to correspond to a ~30% brighter display when typical pictures or
     * movies. This function allows boost to be performed to individual primary colors.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param color            Color as defined in {@link picop.interfaces.def.PicoP_ColorE}
     * @param boostMode        Boost Mode as defined in {@link picop.interfaces.def.PicoP_GammaBoostModeE}
     * @param bCommit          False = Do not persist (value restored back after power cycle), True = Persist setting
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_SetVideoGammaBoostMode(PicoP_Handle connectionHandle,
                                                 PicoP_ColorE color,
                                                 PicoP_GammaBoostModeE boostMode, boolean bCommit);

    /**
     * Returns gamma boost mode for a given color.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param callback         Callback to store current {@link picop.interfaces.def.PicoP_GammaBoostModeE}
     * @param color            Boost mode for the color as defined in {@link picop.interfaces.def.PicoP_ColorE}
     * @param storageType      Storage Type as defined in {@link picop.interfaces.def.PicoP_ValueStorageTypeE}
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetVideoGammaBoostMode(PicoP_Handle connectionHandle,
                                                 PicoP_Api.PicoP_Callback callback, PicoP_ColorE color,
                                                 PicoP_ValueStorageTypeE storageType);

    /**
     * The PSE Color converter is a 3x3 matrix that allows the display colors to be remapped.
     * This function sets the color converter value for a specific color pair in the 3x3 color
     * converter matrix.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param color            Color pair as defined in {@link picop.interfaces.def.PicoP_ColorConvertE}
     * @param nCoefficient     Value for the color converter (Range -65536 ... +65535)
     * @param bCommit          False = Do not persist (value restored back after power cycle), True = Persist setting
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_SetColorConverter(PicoP_Handle connectionHandle,
                                            PicoP_ColorConvertE color,
                                            int nCoefficient,
                                            boolean bCommit);

    /**
     * Returns the color converter value for a specific color pair in the 3x3 color
     * converter matrix.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param callback         Callback to store current value for the color converter
     * @param color            Color pair as defined in {@link picop.interfaces.def.PicoP_ColorConvertE}
     * @param storageType      Storage Type as defined in {@link picop.interfaces.def.PicoP_ValueStorageTypeE}
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetColorConverter(PicoP_Handle connectionHandle,
                                            PicoP_Api.PicoP_Callback callback,
                                            PicoP_ColorConvertE color,
                                            PicoP_ValueStorageTypeE storageType);

    /**
     * The PicoP Laser Beam Scanning display illuminates pixels during both horizonal left
     * to right and right to left scans. The PSE automatically adjusts the phase delay so that left
     * to right and right to left scanned pixels are perfectly aligned in the vertical direction.
     * However, in case one can observe a misalignment (display appears out of focus), this
     * function allows the host to manipulate the phase delay. This manual override should not
     * be needed in normal operation. This function sets scan line phase delay value.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param sPhaseValue      Value for the horizontal phase offset (Range -100 ... +100)
     * @param bCommit          False = Do not persist (value restored back after power cycle), True = Persist setting
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_SetBiPhase(PicoP_Handle connectionHandle, short sPhaseValue, boolean bCommit);

    /**
     * Returns scan line phase delay value.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param callback         Callback to store current value of line phase delay
     * @param storageType      Storage Type as defined in {@link picop.interfaces.def.PicoP_ValueStorageTypeE}
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetBiPhase(PicoP_Handle connectionHandle,
                                     PicoP_Api.PicoP_Callback callback, PicoP_ValueStorageTypeE storageType);

    /**
     * Set the brightness of the projection display.
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param brightnessValue  Brightness level to set. Range 0 - 100. 0
     *                         means display off, 100 indicates max brightness.
     * @param bCommit          False = Do not persist (value restored back after power cycle), True = Persist setting
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_SetBrightness(PicoP_Handle connectionHandle,
                                        int brightnessValue, boolean bCommit);

    /**
     * Return the brightness of the projection display.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param callback         Callback to store current value of brightness
     * @param storageType      Storage Type as defined in {@link picop.interfaces.def.PicoP_ValueStorageTypeE}
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetBrightness(PicoP_Handle connectionHandle,
                                        PicoP_Api.PicoP_Callback callback, PicoP_ValueStorageTypeE storageType);

    /**
     * Display a test pattern.  If connected to input video, use
     * PicoP_SetInputVideoState() to disable input video.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param testPattern      Test pattern to draw as defined in {@link picop.interfaces.def.PicoP_TestPatternE}
     * @param foregroundColor  Pattern Color - 24-Bit RGB foregound pattern color value, 0xRRBBGG.
     * @param backgroundColor  Background Color - 24-Bit RGB background color value, 0xRRBBGG
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_DrawTestPattern(PicoP_Handle connectionHandle,
                                          PicoP_TestPatternE testPattern, int foregroundColor, int backgroundColor);

    /**
     * Select the MicroWeave mode
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param microWeaveMode   MicroWeaveMode to set as defined in {@link picop.interfaces.def.PicoP_MicroWeaveE}
     *                         maximum scan angle
     * @param bCommit          False = Do not persist (value restored back after power cycle), True = Persist setting
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_SetMicroWeaveMode(PicoP_Handle connectionHandle, PicoP_MicroWeaveE microWeaveMode, boolean bCommit);

    /**
     * Get the MicroWeave mode.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param callback         Callback to store current value of MicroWeave Mode as defined in {@link picop.interfaces.def.PicoP_MicroWeaveE}
     * @param storageType      Storage Type as defined in {@link picop.interfaces.def.PicoP_ValueStorageTypeE}
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetMicroWeaveMode(PicoP_Handle connectionHandle,
                                            PicoP_Api.PicoP_Callback callback, PicoP_ValueStorageTypeE storageType);

    /**
     * Set the output frame rate mode (frame rate,vertical display and sensing resolutions) and display scaling.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param frameRateMode    Display Frame Rate and vertical and 3D sensing resolution as defined in {@link picop.interfaces.def.PicoP_FrameRateModeS}
     * @param bCommit          False = Do not persist (value restored back after power cycle), True = Persist setting
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_SetFrameRateMode(PicoP_Handle connectionHandle, PicoP_FrameRateModeS frameRateMode, boolean bCommit);

    /**
     * Get the output frame rate mode (frame rate, vertical display and sensing resolutions) and display scaling.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param callback         Callback to store Display Frame Rate and vertical and 3D sensing resolution as defined in {@link picop.interfaces.def.PicoP_FrameRateModeS}
     * @param storageType      Storage Type as defined in {@link picop.interfaces.def.PicoP_ValueStorageTypeE}
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetFrameRateMode(PicoP_Handle connectionHandle,
                                           PicoP_Api.PicoP_Callback callback, PicoP_ValueStorageTypeE storageType);

    /**
     * Set vertical or horizontal color alignment for the selected color.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param direction        Direction as defined in {@link picop.interfaces.def.PicoP_DirectionE}
     * @param color            Laser color as defined in {@link picop.interfaces.def.PicoP_LaserE}
     * @param offset           Offset value ranges from +32.0f to -32.0f
     * @param bCommit          False = Do not persist (value restored back after power cycle), True = Persist setting
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_SetColorAlignment(PicoP_Handle connectionHandle, PicoP_DirectionE direction, PicoP_LaserE color,
                                            float offset, boolean bCommit);

    /**
     * Get vertical or horizontal color alignment offset for the selected color.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param direction        Direction as defined in {@link picop.interfaces.def.PicoP_DirectionE}
     * @param color            Laser color as defined in {@link picop.interfaces.def.PicoP_LaserE}
     * @param callback         Callback to store current value of offset
     * @param storageType      Storage Type as defined in {@link picop.interfaces.def.PicoP_ValueStorageTypeE}
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetColorAlignment(PicoP_Handle connectionHandle, PicoP_DirectionE direction, PicoP_LaserE color,
                                            PicoP_Api.PicoP_Callback callback, PicoP_ValueStorageTypeE storageType);

    /**
     * Get output video properties.
     *
     * @param connectionHandle A valid library {@link picop.interfaces.def.PicoP_Handle} previously obtained by a call
     *                         to PicoP_OpenLibrary()
     * @param callback         Callback to store current value of PicoP_OutputVideoPropertiesS as defined in {@link picop.interfaces.def.PicoP_OutputVideoPropertiesS}
     * @return {@link picop.interfaces.def.PicoP_RC}
     */
    public PicoP_RC PicoP_GetOutputVideoProperties(PicoP_Handle connectionHandle,
                                                   PicoP_Api.PicoP_Callback callback);


}
