//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!
//! \file picop_rc.h
//!
//! \brief Function return codes for the PicoP API.
//!
//! Copyright (C) 2018 MicroVision, Inc.
//!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#ifndef PICOP_RC_H
#define PICOP_RC_H

//! \enum PicoP_ReturnCodeE
//!
//! These are the only return codes returned by the PicoP; they must match
//! PDELib_ReturnCodeE.
//!
typedef enum
{
    ePICOP_SUCCESS = 0, ///< Operation completed successfully
    ePICOP_FAILURE,
    ePICOP_NOT_SUPPORTED,
    ePICOP_TIMEOUT,
    ePICOP_UNINITIALIZED,
    ePICOP_INVALID_ARGUMENT,
    ePICOP_INIT_FAILURE,
    ePICOP_INCOMPLETE,
    ePICOP_NOT_IMPLEMENTED,
    ePICOP_INTERNAL_ERROR,
    ePICOP_DEVICE_ERROR,
    ePICOP_RESOURCE_BUSY,
    ePICOP_RESOURCE_FULL,
    ePICOP_INVALID_CHECKSUM,
    ePICOP_INVALID_DATA,
    ePICOP_INVALID_CONTEXT,
    ePICOP_ARGUMENT_OUT_OF_RANGE,
    ePICOP_INVALID_CALL_STATE,
    ePICOP_CONDITIONS_NOT_CORRECT,
    ePICOP_NO_MORE_RESOURCES,
    ePICOP_OS_ERROR,
    ePICOP_EMPTY,
    ePICOP_NOT_CONNECTED,
    // linux sdk specific error codes
    ePICOP_COMMUNICATION_ERROR,
    ePICOP_ALREADY_OPENED,
    ePICOP_BTH_DISABLED,
    ePICOP_PAYLOAD_TOO_LARGE,
    ePICOP_RETRY,
    ePICOP_RECONNECT_FAILED,

    ePICOP_MAX_RETURN_CODE_ENUM,
} PicoP_ReturnCodeE;

//! \typedef PicoP_ReturnStatus
//!
//! This is the return type for all PicoP API functions.
//! - The value is 0 for success.
//! - If the value is non-zero, it is a bitfield of the following data:
//!     [PicoP_ReturnCodeE (8)] | [FaultTypeE (8)] | [FaultDetail (16)]
//!
//! PicoP_ReturnStatus should match PDELib_ReturnCodeE.
//!
//! FaultTypeE and FaultDetail form a unique fault code which can be analyzed
//! using the fault code triage manual.
//!
//! Specific fault code information is proprietary.
//!
typedef enum
{
    ePICOP_RETURN_SUCCESS = 0,
    /* no other values are defined,
     * non-zero values must be decoded as a bitfield.
     * MAX is defined to ensure 32bit size */
    ePICOP_MAX_RETURN_STATUS_ENUM = 0xFFFFFFFFu
} PicoP_ReturnStatus;

#endif // PICOP_RC_H
