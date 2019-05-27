//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!
//! \file Hello_PicoP.cpp
//!
//! \brief PicoP "Hello, World!" application.
//!
//! Copyright (C) 2018 MicroVision, Inc.
//!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include <SDKDDKVer.h>
#include <stdio.h>
#include <string.h>

#include "../../interface/picop_api.h"

int main()
{
    PicoP_HANDLE hLibraryHandle = NULL;              // Library handle
    PicoP_HANDLE hConnectionHandle = NULL;           // Connection handle
    PicoP_ConnectionInfoS connectionInfo;            // Connection Information
    PicoP_USBInfoS sUsbInfo = {0x0004, "123456789"}; // USB Info
    PicoP_FlipStateE eOldDirection;
    PicoP_FlipStateE eNewDirection;

    PicoP_ReturnStatus eResult;

    // Open the SDK library
    eResult = PicoP_OpenLibrary(&hLibraryHandle);
    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("*** ERROR: PicoP_OpenLibrary() failed, rc = 0x%x\n", eResult);
        return eResult;
    }

    connectionInfo.connectionType = eUSB;
    memcpy(&connectionInfo.usbInfo, &sUsbInfo, sizeof(PicoP_USBInfoS));
    // Connect to PSE with USB
    eResult = PicoP_OpenConnection(hLibraryHandle, connectionInfo,
                                   &hConnectionHandle);

    if (eResult != ePICOP_RETURN_SUCCESS)
    {
        printf("** ERROR: PicoP_OpenConnection() failed, rc = 0x%x\n", eResult);
        PicoP_CloseLibrary(hLibraryHandle);
        return eResult;
    }

    // Read current Flip State
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

    // Flip the display vertically
    eResult =
        PicoP_SetFlipState(hConnectionHandle, eNewDirection, eCURRENT_VALUE);
    if (eResult == ePICOP_RETURN_SUCCESS)
    {
        printf("Display flipped vertcally!\n");
    }
    else
    {
        printf("** ERROR: PicoP_SetFlipState() failed, rc = 0x%x\n", eResult);
    }

    // Close connection & library
    PicoP_CloseConnection(hConnectionHandle);
    PicoP_CloseLibrary(hLibraryHandle);

    return 0;
}
