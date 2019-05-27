//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!
//! \file picop_console.cpp
//!
//! \brief Sample console application for using MicroVision's 4th Generation
//!        Windows SDK
//!
//! Copyright (C) 2018 MicroVision, Inc.
//!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#include "PicoP_Console.h"


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int GetInput()
{
    int ch;
    ch = _getch();
    ch = toupper(ch);
    return ch;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void DisplayMainMenu()
{
    printf("\nMain Menu\n");
    printf("Please make your selection:\n");
    printf("----------------------------------------------------\n");
    printf("1 - Get Library Info, System Info, and System Status\n");
    printf("2 - Turn Display Off\n");
    printf("3 - Turn Display On\n");
    printf("4 - Get Brightness\n");
    printf("5 - Set Brightness\n");
    printf("6 - Flip Display Horizontally\n");
    printf("7 - Flip Display Vertically\n");
    printf("8 - Draw Test Pattern\n");
    printf("9 - Turn Test Pattern Off\n");
    printf("A - Get ColorMode\n");
    printf("B - Set ColorMode\n");
    printf("C - Get ColorConverter\n");
    printf("D - Set ColorConverter - View Red Channel Only\n");
    printf("E - Set ColorConverter - Swap Red and Green\n");
    printf("F - Reset ColorConverter\n");
    printf("0 - Quit\n");
    printf("> ");
}



//!----------------------------------------------------------------------------
//! \brief This function gets the version of the SDK library and system info
//!        on the connected PSE
//! \param libraryHandle [IN] Handle to the SDK library
//! \param connectionHandle [IN] Connection handle to the PSE
//!----------------------------------------------------------------------------
PicoP_ReturnStatus GetInfo(PicoP_HANDLE hLibraryHandle,
                           PicoP_HANDLE hConnectionHandle)
{
    PicoP_LibraryInfoS libraryInfo;
    PicoP_SystemInfoS systemInfo;
    UINT32 fwVer;
    PicoP_SystemStatusS systemStatus;
    PicoP_ReturnStatus eResult = ePICOP_RETURN_SUCCESS;

    eResult = PicoP_GetLibraryInfo(hLibraryHandle, &libraryInfo);
    if (eResult == ePICOP_RETURN_SUCCESS)
    {
        printf("SDK Version : %d.%d.%d\n", libraryInfo.majorVersion,
               libraryInfo.minorVersion, libraryInfo.patchVersion);
    }
    else
    {
        printf("*** ERROR: PicoP_GetLibraryInfo() failed, rc = 0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_GetSystemInfo(hLibraryHandle, &systemInfo);
    if (eResult == ePICOP_RETURN_SUCCESS)
    {
        fwVer = systemInfo.firmwareVersion;
        printf("Firmware Version : %d.%d.%d.%d\n", fwVer >> 24,
               (fwVer & 0x00ff0000) >> 16, (fwVer & 0x0000ff00) >> 8,
               fwVer & 0x000000ff);
    }
    else
    {
        printf("*** ERROR: PicoP_GetSystemInfo() failed, rc = 0x%x\n", eResult);
        return eResult;
    }

    eResult = PicoP_GetSystemStatus(hConnectionHandle, &systemStatus);
    if (eResult == ePICOP_RETURN_SUCCESS)
    {
        printf("System State: %d\n", systemStatus.systemState);
        printf("System Faults: 0x%x\n", systemStatus.systemFault);
        printf("System Temperature: %.1f\n", systemStatus.temperature);
    }
    else
    {
        printf("*** ERROR: PicoP_GetSystemStatus() failed, rc = 0x%x\n",
               eResult);
    }

    return eResult;
}


//!----------------------------------------------------------------------------
//! \brief Turn the PicoP Display Off
//! \param connectionHandle [IN] Connection handle to the PSE
//!----------------------------------------------------------------------------
PicoP_ReturnStatus TurnDisplayOff(PicoP_HANDLE hConnectionHandle)
{
    PicoP_ReturnStatus eResult;

    eResult = PicoP_SetOutputVideoState(hConnectionHandle,
                                        eOUTPUT_VIDEO_DISABLED, FALSE);

    if (eResult == ePICOP_RETURN_SUCCESS)
    {
        printf("Display Turned Off!\n");
    }
    else
    {
        printf("*** ERROR: PicoP_SetOutputVideoState() failed, rc = 0x%x\n",
               eResult);
    }

    return eResult;
}


//!----------------------------------------------------------------------------
//! \brief Turn the PicoP Display On
//! \param connectionHandle [IN] Connection handle to the PSE
//!----------------------------------------------------------------------------
PicoP_ReturnStatus TurnDisplayOn(PicoP_HANDLE hConnectionHandle)
{
    PicoP_ReturnStatus eResult;

    eResult = PicoP_SetOutputVideoState(hConnectionHandle,
                                        eOUTPUT_VIDEO_ENABLED, FALSE);

    if (eResult == ePICOP_RETURN_SUCCESS)
    {
        printf("Display Turned On!\n");
    }
    else
    {
        printf("*** ERROR: PicoP_SetOutputVideoState() failed, rc = 0x%x\n",
               eResult);
    }

    return eResult;
}

//!----------------------------------------------------------------------------
//! \brief Read the current display the brightness setting of the PSE
//! \param connectionHandle [IN] Connection handle to the PSE
//!----------------------------------------------------------------------------
PicoP_ReturnStatus GetBrightness(PicoP_HANDLE hConnectionHandle)
{
    PicoP_ReturnStatus eResult;
    UINT8 brightnessValue;

    eResult = PicoP_GetBrightness(hConnectionHandle, &brightnessValue,
                                  eCURRENT_VALUE);

    if (eResult == ePICOP_RETURN_SUCCESS)
    {
        printf("Brightness = '%d'\n", brightnessValue);
    }
    else
    {
        printf("*** ERROR: PicoP_GetBrightness() failed, rc = 0x%x\n", eResult);
    }

    return eResult;
}

//!----------------------------------------------------------------------------
//! \brief Set the PSE Display Brightness
//! \param connectionHandle [IN] Connection handle to the PSE
//!----------------------------------------------------------------------------
PicoP_ReturnStatus SetBrightness(PicoP_HANDLE hConnectionHandle)
{
    PicoP_ReturnStatus eResult;
    int brightnessValue;

    printf("\nEnter Brightness Value to Set (0 - 100): ");
    scanf_s("%d", &brightnessValue);
    printf("\n");

    eResult = PicoP_SetBrightness(hConnectionHandle, brightnessValue, FALSE);

    if (eResult == ePICOP_RETURN_SUCCESS)
    {
        printf("Brightness set to '%d'!\n", brightnessValue);
    }
    else
    {
        printf("*** ERROR: PicoP_SetBrightness() failed, rc = 0x%x\n", eResult);
    }

    return eResult;
}



//!----------------------------------------------------------------------------
//! \brief Flip Display Horizontally
//! \param connectionHandle [IN] Connection handle to the PSE
//!----------------------------------------------------------------------------
PicoP_ReturnStatus FlipDisplayHorizontally(PicoP_HANDLE hConnectionHandle)
{
    PicoP_ReturnStatus eResult;
    PicoP_FlipStateE eOldDirection, eNewDirection;

    eResult =
        PicoP_GetFlipState(hConnectionHandle, &eOldDirection, eCURRENT_VALUE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_GetFlipState() failed, rc = 0x%x\n", eResult);
        return eResult;
    }

    // Figure out horizontal flip based on current flip state
    if (eOldDirection == eFLIP_NEITHER)
    {
        eNewDirection = eFLIP_HORIZONTAL;
    }
    else if (eOldDirection == eFLIP_HORIZONTAL)
    {
        eNewDirection = eFLIP_NEITHER;
    }
    else if (eOldDirection == eFLIP_VERTICAL)
    {
        eNewDirection = eFLIP_BOTH;
    }
    else if (eOldDirection = eFLIP_BOTH)
    {
        eNewDirection = eFLIP_VERTICAL;
    }

    eResult =
        PicoP_SetFlipState(hConnectionHandle, eNewDirection, eCURRENT_VALUE);

    if (eResult == ePICOP_RETURN_SUCCESS)
    {
        printf("Display Flipped Horizontally!\n");
    }
    else
    {
        printf("*** ERROR: PicoP_SetFlipState() failed, rc = 0x%x\n", eResult);
    }

    return eResult;
}


//!----------------------------------------------------------------------------
//! \brief Flip Display Vertically
//! \param connectionHandle [IN] Connection handle to the PSE
//!----------------------------------------------------------------------------
PicoP_ReturnStatus FlipDisplayVertically(PicoP_HANDLE hConnectionHandle)
{
    PicoP_ReturnStatus eResult;
    PicoP_FlipStateE eOldDirection, eNewDirection;

    eResult =
        PicoP_GetFlipState(hConnectionHandle, &eOldDirection, eCURRENT_VALUE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_GetFlipState() failed, rc = 0x%x\n", eResult);
        return eResult;
    }

    // Figure out vertical flip based on current flip state
    if (eOldDirection == eFLIP_NEITHER)
    {
        eNewDirection = eFLIP_VERTICAL;
    }
    else if (eOldDirection == eFLIP_HORIZONTAL)
    {
        eNewDirection = eFLIP_BOTH;
    }
    else if (eOldDirection == eFLIP_VERTICAL)
    {
        eNewDirection = eFLIP_NEITHER;
    }
    else if (eOldDirection = eFLIP_BOTH)
    {
        eNewDirection = eFLIP_HORIZONTAL;
    }

    eResult =
        PicoP_SetFlipState(hConnectionHandle, eNewDirection, eCURRENT_VALUE);

    if (eResult == ePICOP_RETURN_SUCCESS)
    {
        printf("Display Flipped Vertically!\n");
    }
    else
    {
        printf("*** ERROR: PicoP_SetFlipState() failed, rc = 0x%x\n", eResult);
    }

    return eResult;
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void DisplayTestPatternMenu()
{
    printf("Test Pattern Menu\n");
    printf("Please make your selection\n");
    printf("0 - Turn Test Pattern off\n");
    printf("1 - CheckerBoard Pattern\n");
    printf("2 - CrossHair Pattern\n");
    printf("3 - Grid Pattern\n");
    printf("4 - Constant Color Pattern\n");
    printf("> ");
}


//!----------------------------------------------------------------------------
//! \brief Displays a Test Pattern
//! \param connectionHandle [IN] Connection handle to the PSE
//!----------------------------------------------------------------------------
PicoP_ReturnStatus DisplayTestPattern(PicoP_HANDLE hConnectionHandle)
{
    PicoP_ReturnStatus eResult = ePICOP_RETURN_SUCCESS;
    PicoP_TestPatternE ePattern;
    UINT32 foreGroundColor = 0;
    UINT32 backgroundColor = 0;

    int choice; // User color mode selection

    DisplayTestPatternMenu();
    choice = GetInput();

    switch (choice)
    {
    case '0':
        ePattern = eTEST_PATTERN_OFF;
        foreGroundColor = 0x00000000; // Don't care
        backgroundColor = 0x00000000; // Don't care
        break;
    case '1':
        ePattern = eCHECKERBOARD_PATTERN;
        foreGroundColor = 0x000000FF; // Green
        backgroundColor = 0x00000000; // Black
        break;
    case '2':
        ePattern = eCROSSHAIR_PATTERN;
        foreGroundColor = 0x00FF0000; // Red
        backgroundColor = 0x00000000; // Black
        break;
    case '3':
        ePattern = eGRID_PATTERN;
        foreGroundColor = 0x00FF0000; // Red
        backgroundColor = 0x00FF00FF; // Yellow
        break;
    case '4':
        ePattern = eCONSTANT_COLOR_PATTERN;
        foreGroundColor = 0x0000FF00; // Blue
        backgroundColor = 0x00000000; // Don't card
        break;
    default:
        eResult = (PicoP_ReturnStatus)(ePICOP_INVALID_ARGUMENT << 24);
        return eResult;
        break;
    }

    eResult = PicoP_DrawTestPattern(hConnectionHandle, ePattern,
                                    foreGroundColor, backgroundColor);

    if (eResult == ePICOP_RETURN_SUCCESS)
    {
        printf("Test Pattern Displaying!\n");
    }
    else
    {
        printf("*** ERROR: PicoP_DrawTestPattern() failed, rc = 0x%x\n",
               eResult);
    }

    return eResult;
}


//!----------------------------------------------------------------------------
//! \brief Turns a test pattern off
//! \param connectionHandle [IN] Connection handle to the PSE
//!----------------------------------------------------------------------------
PicoP_ReturnStatus TurnTestPatternOff(PicoP_HANDLE hConnectionHandle)
{
    PicoP_ReturnStatus eResult;
    PicoP_TestPatternE ePattern = eTEST_PATTERN_OFF;
    UINT32 foreGroundColor = 0;
    UINT32 backgroundColor = 0;


    // Enable input video so that test pattern gets overwritten with video
    eResult = PicoP_DrawTestPattern(hConnectionHandle, ePattern,
                                    foreGroundColor, backgroundColor);
    if (eResult == ePICOP_RETURN_SUCCESS)
    {
        printf("Test Pattern Off!\n");
    }
    else
    {
        printf("*** ERROR: PicoP_DrawTestPattern(eTEST_PATTERN_OFF) failed, rc "
               "= 0x%x\n",
               eResult);
    }

    return eResult;
}


//!----------------------------------------------------------------------------
//! \brief Read the current color mode setting of the PSE
//! \param connectionHandle [IN] Connection handle to the PSE
//!----------------------------------------------------------------------------
PicoP_ReturnStatus GetColorMode(PicoP_HANDLE hConnectionHandle)
{
    PicoP_ReturnStatus eResult;
    PicoP_ColorModeE eColorMode;

    eResult =
        PicoP_GetColorMode(hConnectionHandle, &eColorMode, eCURRENT_VALUE);

    if (eResult == ePICOP_RETURN_SUCCESS)
    {
        printf("Current Color Mode is ");
        switch (eColorMode)
        {
        case eCOLOR_MODE_STANDARD:
            printf("'Standard'\n");
            break;
        case eCOLOR_MODE_BRILLIANT:
            printf("'Brilliant'\n");
            break;
        case eCOLOR_MODE_RICH:
            printf("'Rich'\n");
            break;
        case eCOLOR_MODE_INVERTED:
            printf("'Inverted'\n");
            break;
        case eCOLOR_MODE_CUSTOM:
            printf("'Custom'\n");
            break;
        default:
            printf("'Unknown' = %d\n", eColorMode);
            break;
        }
    }
    else
    {
        printf("*** ERROR: PicoP_GetColorMode() failed, rc = 0x%x\n", eResult);
    }

    return eResult;
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void DisplayColorModeMenu()
{
    printf("Color Mode Menu\n");
    printf("Please make your selection\n");
    printf("0 - Standard Color Mode\n");
    printf("1 - Brilliant Color Mode\n");
    printf("2 - Rich Color Mode\n");
    printf("3 - Inverted Color Mode\n");
    printf("4 - Custom Color Mode\n");
    printf("  - \n");
    printf("9 - Cancel, Go Back\n");
    printf("> ");
}

//!----------------------------------------------------------------------------
//! \brief Set the PSE Color Mode
//! \param connectionHandle [IN] Connection handle to the PSE
//!----------------------------------------------------------------------------
PicoP_ReturnStatus SetColorMode(PicoP_HANDLE hConnectionHandle)
{
    PicoP_ReturnStatus eResult = ePICOP_RETURN_SUCCESS;
    PicoP_ColorModeE eColorMode;
    int choice; // User color mode selection
    char *cColorModeLabel[] = {"Standard Color Mode", "Brilliant Color Mode",
                               "Rich Color Mode", "Inverted Color Mode",
                               "Custom Color Mode"};

    DisplayColorModeMenu();
    choice = GetInput();
    printf("\n===========================\n");

    switch (choice)
    {
    case '0':
        eColorMode = eCOLOR_MODE_STANDARD;
        break;
    case '1':
        eColorMode = eCOLOR_MODE_BRILLIANT;
        break;
    case '2':
        eColorMode = eCOLOR_MODE_RICH;
        break;
    case '3':
        eColorMode = eCOLOR_MODE_INVERTED;
        break;
    case '4':
        eColorMode = eCOLOR_MODE_CUSTOM;
        break;
    default:
        eResult = (PicoP_ReturnStatus)(ePICOP_INVALID_ARGUMENT << 24);
        return eResult;
        break;
    }

    eResult = PicoP_SetColorMode(hConnectionHandle, eColorMode, eCURRENT_VALUE);

    if (eResult == ePICOP_RETURN_SUCCESS)
    {
        printf("Color Mode set to %s\n", cColorModeLabel[eColorMode]);
    }
    else
    {
        printf("*** ERROR: PicoP_SetColorMode() failed, rc = 0x%x\n", eResult);
    }

    return eResult;
}


//!----------------------------------------------------------------------------
//! \brief Read the current color converter matrix
//! \param connectionHandle [IN] Connection handle to the PSE
//!----------------------------------------------------------------------------
PicoP_ReturnStatus GetColorConverter(PicoP_HANDLE hConnectionHandle)
{
    PicoP_ReturnStatus eResult;
    INT32 colorConvMatrix[3][3];

    eResult = PicoP_GetColorConverter(hConnectionHandle, eRED_TO_RED,
                                      &colorConvMatrix[0][0], eCURRENT_VALUE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_GetColorConverter(eRED_TO_RED) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_GetColorConverter(hConnectionHandle, eGREEN_TO_RED,
                                      &colorConvMatrix[0][1], eCURRENT_VALUE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_GetColorConverter(eGREEN_TO_RED) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_GetColorConverter(hConnectionHandle, eBLUE_TO_RED,
                                      &colorConvMatrix[0][2], eCURRENT_VALUE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_GetColorConverter(eBLUE_TO_RED) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_GetColorConverter(hConnectionHandle, eRED_TO_GREEN,
                                      &colorConvMatrix[1][0], eCURRENT_VALUE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_GetColorConverter(eRED_TO_GREEN) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_GetColorConverter(hConnectionHandle, eGREEN_TO_GREEN,
                                      &colorConvMatrix[1][1], eCURRENT_VALUE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_GetColorConverter(eGREEN_TO_GREEN) failed, rc "
               "= 0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_GetColorConverter(hConnectionHandle, eBLUE_TO_GREEN,
                                      &colorConvMatrix[1][2], eCURRENT_VALUE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_GetColorConverter(eBLUE_TO_GREEN) failed, rc "
               "= 0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_GetColorConverter(hConnectionHandle, eRED_TO_BLUE,
                                      &colorConvMatrix[2][0], eCURRENT_VALUE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_GetColorConverter(eRED_TO_BLUE) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_GetColorConverter(hConnectionHandle, eGREEN_TO_BLUE,
                                      &colorConvMatrix[2][1], eCURRENT_VALUE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_GetColorConverter(eGREEN_TO_BLUE) failed, rc "
               "= 0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_GetColorConverter(hConnectionHandle, eBLUE_TO_BLUE,
                                      &colorConvMatrix[2][2], eCURRENT_VALUE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_GetColorConverter(eBLUE_TO_BLUE) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    printf("\nColorConverter Matrix:");
    for (int i = 0; i < 3; i++)
    {
        printf("\n[ ");
        for (int j = 0; j < 3; j++)
        {
            printf("%d ", colorConvMatrix[i][j]);
        }
        printf(" ]");
    }
    printf("\n");

    return ePICOP_RETURN_SUCCESS;
}

//!----------------------------------------------------------------------------
//! \brief Set the color Converter to display red channel only
//! \param connectionHandle [IN] Connection handle to the PSE
//!----------------------------------------------------------------------------
PicoP_ReturnStatus SetColorConverterRedChannelOnly(
    PicoP_HANDLE hConnectionHandle)
{
    PicoP_ReturnStatus eResult;
    INT32 colorConvMatrix[3][3];

    // Switch to custom mode
    eResult = PicoP_SetColorMode(hConnectionHandle, eCOLOR_MODE_CUSTOM,
                                 eCURRENT_VALUE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorMode(eCOLOR_MODE_CUSTOM) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    colorConvMatrix[0][0] = 32768; // eRED_TO_RED
    colorConvMatrix[0][1] = 0;     // eGREEN_TO_RED
    colorConvMatrix[0][2] = 0;     // eBLUE_TO_RED
    colorConvMatrix[1][0] = 0;     // eRED_TO_GREEN
    colorConvMatrix[1][1] = 0;     // eGREEN_TO_GREEN
    colorConvMatrix[1][2] = 0;     // eBLUE_TO_GREEN
    colorConvMatrix[2][0] = 0;     // eRED_TO_BLUE
    colorConvMatrix[2][1] = 0;     // eGREEN_TO_BLUE
    colorConvMatrix[2][2] = 0;     // eBLUE_TO_BLUE

    eResult = PicoP_SetColorConverter(hConnectionHandle, eRED_TO_RED,
                                      colorConvMatrix[0][0], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eRED_TO_RED) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eGREEN_TO_RED,
                                      colorConvMatrix[0][1], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eGREEN_TO_RED) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eBLUE_TO_RED,
                                      colorConvMatrix[0][2], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eBLUE_TO_RED) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eRED_TO_GREEN,
                                      colorConvMatrix[1][0], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eRED_TO_GREEN) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eGREEN_TO_GREEN,
                                      colorConvMatrix[1][1], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eGREEN_TO_GREEN) failed, rc "
               "= 0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eBLUE_TO_GREEN,
                                      colorConvMatrix[1][2], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eBLUE_TO_GREEN) failed, rc "
               "= 0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eRED_TO_BLUE,
                                      colorConvMatrix[2][0], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eRED_TO_BLUE) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eGREEN_TO_BLUE,
                                      colorConvMatrix[2][1], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eGREEN_TO_BLUE) failed, rc "
               "= 0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eBLUE_TO_BLUE,
                                      colorConvMatrix[2][2], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eBLUE_TO_BLUE) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    printf("Color Converter Matrix Set to display Red channel only!\n");

    return ePICOP_RETURN_SUCCESS;
}


//!----------------------------------------------------------------------------
//! \brief Set the color Converter to swap red and green channels
//! \param connectionHandle [IN] Connection handle to the PSE
//!----------------------------------------------------------------------------
PicoP_ReturnStatus SetColorConverterSwapRedAndGreenChannels(
    PicoP_HANDLE hConnectionHandle)
{
    PicoP_ReturnStatus eResult;
    INT32 colorConvMatrix[3][3];

    // Switch to custom mode
    eResult = PicoP_SetColorMode(hConnectionHandle, eCOLOR_MODE_CUSTOM,
                                 eCURRENT_VALUE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorMode(eCOLOR_MODE_CUSTOM) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    colorConvMatrix[0][0] = 0;     // eRED_TO_RED
    colorConvMatrix[0][1] = 32768; // eGREEN_TO_RED
    colorConvMatrix[0][2] = 0;     // eBLUE_TO_RED
    colorConvMatrix[1][0] = 32768; // eRED_TO_GREEN
    colorConvMatrix[1][1] = 0;     // eGREEN_TO_GREEN
    colorConvMatrix[1][2] = 0;     // eBLUE_TO_GREEN
    colorConvMatrix[2][0] = 0;     // eRED_TO_BLUE
    colorConvMatrix[2][1] = 0;     // eGREEN_TO_BLUE
    colorConvMatrix[2][2] = 32768; // eBLUE_TO_BLUE

    eResult = PicoP_SetColorConverter(hConnectionHandle, eRED_TO_RED,
                                      colorConvMatrix[0][0], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eRED_TO_RED) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eGREEN_TO_RED,
                                      colorConvMatrix[0][1], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eGREEN_TO_RED) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eBLUE_TO_RED,
                                      colorConvMatrix[0][2], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eBLUE_TO_RED) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eRED_TO_GREEN,
                                      colorConvMatrix[1][0], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eRED_TO_GREEN) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eGREEN_TO_GREEN,
                                      colorConvMatrix[1][1], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eGREEN_TO_GREEN) failed, rc "
               "= 0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eBLUE_TO_GREEN,
                                      colorConvMatrix[1][2], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eBLUE_TO_GREEN) failed, rc "
               "= 0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eRED_TO_BLUE,
                                      colorConvMatrix[2][0], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eRED_TO_BLUE) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eGREEN_TO_BLUE,
                                      colorConvMatrix[2][1], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eGREEN_TO_BLUE) failed, rc "
               "= 0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eBLUE_TO_BLUE,
                                      colorConvMatrix[2][2], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eBLUE_TO_BLUE) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    printf("Color Converter Matrix Set to Swap Red and Green channels!\n");

    return ePICOP_RETURN_SUCCESS;
}

//!----------------------------------------------------------------------------
//! \brief Reset Custom color converter back to default
//! \param connectionHandle [IN] Connection handle to the PSE
//!----------------------------------------------------------------------------
PicoP_ReturnStatus ResetColorConverter(PicoP_HANDLE hConnectionHandle)
{
    PicoP_ReturnStatus eResult;
    INT32 colorConvMatrix[3][3];

    // Switch to custom mode
    eResult = PicoP_SetColorMode(hConnectionHandle, eCOLOR_MODE_CUSTOM,
                                 eCURRENT_VALUE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorMode(eCOLOR_MODE_CUSTOM) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    colorConvMatrix[0][0] = 32768; // eRED_TO_RED
    colorConvMatrix[0][1] = 0;     // eGREEN_TO_RED
    colorConvMatrix[0][2] = 0;     // eBLUE_TO_RED
    colorConvMatrix[1][0] = 0;     // eRED_TO_GREEN
    colorConvMatrix[1][1] = 32768; // eGREEN_TO_GREEN
    colorConvMatrix[1][2] = 0;     // eBLUE_TO_GREEN
    colorConvMatrix[2][0] = 0;     // eRED_TO_BLUE
    colorConvMatrix[2][1] = 0;     // eGREEN_TO_BLUE
    colorConvMatrix[2][2] = 32768; // eBLUE_TO_BLUE

    eResult = PicoP_SetColorConverter(hConnectionHandle, eRED_TO_RED,
                                      colorConvMatrix[0][0], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eRED_TO_RED) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eGREEN_TO_RED,
                                      colorConvMatrix[0][1], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eGREEN_TO_RED) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eBLUE_TO_RED,
                                      colorConvMatrix[0][2], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eBLUE_TO_RED) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eRED_TO_GREEN,
                                      colorConvMatrix[1][0], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eRED_TO_GREEN) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eGREEN_TO_GREEN,
                                      colorConvMatrix[1][1], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eGREEN_TO_GREEN) failed, rc "
               "= 0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eBLUE_TO_GREEN,
                                      colorConvMatrix[1][2], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eBLUE_TO_GREEN) failed, rc "
               "= 0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eRED_TO_BLUE,
                                      colorConvMatrix[2][0], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eRED_TO_BLUE) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eGREEN_TO_BLUE,
                                      colorConvMatrix[2][1], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eGREEN_TO_BLUE) failed, rc "
               "= 0x%x\n",
               eResult);
        return eResult;
    }

    eResult = PicoP_SetColorConverter(hConnectionHandle, eBLUE_TO_BLUE,
                                      colorConvMatrix[2][2], FALSE);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_SetColorConverter(eBLUE_TO_BLUE) failed, rc = "
               "0x%x\n",
               eResult);
        return eResult;
    }

    printf("Color Converter Matrix Reset!\n");

    return ePICOP_RETURN_SUCCESS;
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// \brief main()
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int main()
{
    PicoP_HANDLE hLibraryHandle = NULL;    // Library handle
    PicoP_HANDLE hConnectionHandle = NULL; // Connection handle
    PicoP_ConnectionInfoS connectionInfo;
    PicoP_USBInfoS sUsbInfo = {0x0004, "123456789"};
    int choice = 0; // User selection choice

    PicoP_ReturnStatus eResult = ePICOP_RETURN_SUCCESS;

    printf("Sample Windows console application for MicroVision PicoP Scannning "
           "Engine\n");
    printf("==================================================================="
           "======\n");

    // Open the SDK library
    eResult = PicoP_OpenLibrary(&hLibraryHandle);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_OpenLibrary() failed, rc = 0x%x\n", eResult);
        return eResult;
    }

    // Connect to PSE with USB
    connectionInfo.connectionType = eUSB;
    memcpy(&connectionInfo.usbInfo, &sUsbInfo, sizeof(PicoP_USBInfoS));
    eResult = PicoP_OpenConnection(hLibraryHandle, connectionInfo,
                                   &hConnectionHandle);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_OpenConnectionUSB() failed, rc = 0x%x\n",
               eResult);
        PicoP_CloseLibrary(hLibraryHandle);
        return eResult;
    }

    //! Main Loop
    do
    {
        DisplayMainMenu();
        choice = GetInput();
        printf("\n===========================\n");

        switch (choice)
        {
        case '1':
            eResult = GetInfo(hLibraryHandle, hConnectionHandle);
            break;
        case '2':
            eResult = TurnDisplayOff(hConnectionHandle);
            break;
        case '3':
            eResult = TurnDisplayOn(hConnectionHandle);
            break;
        case '4':
            eResult = GetBrightness(hConnectionHandle);
            break;
        case '5':
            eResult = SetBrightness(hConnectionHandle);
            break;
        case '6':
            eResult = FlipDisplayHorizontally(hConnectionHandle);
            break;
        case '7':
            eResult = FlipDisplayVertically(hConnectionHandle);
            break;
        case '8':
            eResult = DisplayTestPattern(hConnectionHandle);
            break;
        case '9':
            eResult = TurnTestPatternOff(hConnectionHandle);
            break;
        case 'A':
            eResult = GetColorMode(hConnectionHandle);
            break;
        case 'B':
            eResult = SetColorMode(hConnectionHandle);
            break;
        case 'C':
            eResult = GetColorConverter(hConnectionHandle);
            break;
        case 'D':
            eResult = SetColorConverterRedChannelOnly(hConnectionHandle);
            break;
        case 'E':
            eResult =
                SetColorConverterSwapRedAndGreenChannels(hConnectionHandle);
            break;
        case 'F':
            eResult = ResetColorConverter(hConnectionHandle);
            break;
        default:
            break;
        }
    } while (choice != '0');

    // Cleanup
    PicoP_CloseConnection(hConnectionHandle);
    PicoP_CloseLibrary(hLibraryHandle);


    return 0;
}
