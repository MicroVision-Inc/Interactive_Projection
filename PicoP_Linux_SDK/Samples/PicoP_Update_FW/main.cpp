// ****************************************************************************
// main.cpp
//
// Simple Linux console app demonstrating how to perform a Device Kit FW Upgrade
//
// Copyright (C) 2018-2019 MicroVision, Inc.
// This source code is subject to the Microvision Source Code License.
//
// THIS CODE IS FOR GUIDANCE ONLY. IT IS INTENDED AS AN EDUCATIONAL SAMPLE DEMONSTRATING
// SIMPLIFIED USE OF THE MICROVISION PRODUCT. THE CODE AND INFORMATION ARE PROVIDED "AS IS"
// WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include <fstream>

#include "picop_api.h"
#include "picop_def.h"

using namespace std;

static PicoP_HANDLE library_handle_ = NULL; 
static PicoP_HANDLE connection_handle_ = NULL;

#define COMPOSE_ERROR_STATUS(return_code, fault_type, fault_detail) \
        ((PicoP_ReturnStatus)( (((UINT32)(return_code) & 0xFF) << 24) | (((UINT8)(fault_type) & 0xFF) << 16) | (((UINT16)(fault_detail)) & 0xFFFF) ) )



PicoP_ReturnStatus PicoP_AutoUpgradeSoftware(PicoP_HANDLE connection_handle,
                                             const uint32_t byte_total,
                                             const uint8_t *image)
{
    if (NULL == connection_handle)
        return COMPOSE_ERROR_STATUS(ePICOP_INVALID_ARGUMENT, 0, 0);
    if (NULL == image)
        return COMPOSE_ERROR_STATUS(ePICOP_INVALID_ARGUMENT, 0, 0);

    constexpr uint32_t SIZE_OF_CHUNKS_BYTES = 2000u;

    const unsigned int SLEEP_TIME_US = 2000000u;

    const uint8_t *walker = image;
    uint32_t chunk_index = 0;
    uint32_t chunk_size = 0;
    PicoP_ReturnStatus rc;

    cout << "Attempting to signal bootloader" << endl;

    rc = PicoP_UpgradeSoftware(connection_handle, 0, 0, byte_total, walker);

    if (ePICOP_RETURN_SUCCESS != rc)
        return rc;

    cout << "Waiting for bootloader to be ready to receive" << endl;

    usleep(SLEEP_TIME_US);

    while (chunk_index < byte_total)
    {
        chunk_size = (byte_total - chunk_index) > SIZE_OF_CHUNKS_BYTES
                         ? SIZE_OF_CHUNKS_BYTES
                         : (byte_total - chunk_index);

        cout << "Flashing bytes " << chunk_index << " to " << (chunk_index + chunk_size) << " of "<< byte_total << endl;

        PicoP_ReturnStatus rc =
            PicoP_UpgradeSoftware(connection_handle, chunk_index, chunk_size,
                                  byte_total, walker);
        walker += chunk_size;
        if (ePICOP_RETURN_SUCCESS != rc)
            return rc;
        chunk_index += chunk_size;
    }
    return ePICOP_RETURN_SUCCESS;
}


void do_upgrade()
{
    streampos size;
    char * memblock;
    fstream file("pde_client.chunk.bin", ios::in | ios::binary | ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char[size];
        file.seekg(0, std::ios::beg);
        file.read(memblock, size);
        file.close();
    }
    else 
    {
        cout << "Failed to open file pde_client.chunk.bin." << endl;
        return;
    }

    PicoP_ReturnStatus rc{};

    cout << "Starting upgrade." << endl;
    if (ePICOP_RETURN_SUCCESS != (rc = PicoP_AutoUpgradeSoftware(connection_handle_, (uint32_t)size, (uint8_t*)memblock))) {
        cout << "PicoP_AutoUpgradeSoftware() failure : rc = 0x" << std::hex << rc << endl;
    }

    delete memblock;
    return;
}



int main(int argc, __attribute__ ((unused)) char *argv[])
{
    cout << "GIT VERSION : " << VERSION << endl;

    if (argc != 1) {
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

                do_upgrade();

                if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_CloseConnection(connection_handle_))) {
                    cout << "PicoP_CloseConnection() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
                }
                if (ePICOP_RETURN_SUCCESS != (picop_rc = PicoP_CloseLibrary(library_handle_))) {
                    cout << "PicoP_CloseConnection() failure : picop_rc = 0x" << std::hex << picop_rc << endl;
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
