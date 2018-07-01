///
/// @file Include/Uefi/Status.h
///
/// UEFI implementation status codes
///

#pragma once
#ifndef ___UEFI_STATUS_HEADER__
#define ___UEFI_STATUS_HEADER__

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#include <Uefi/Defaults.h>

  // EFI_BIT_MAX
  /// The most significant bit
  /// @return The bit mask with only the most significant bit set
#define EFI_BIT_MAX (1ULL << ((sizeof(UINTN) << 3) - 1))

// EFI_ERROR
/// Check whether a status code is an error
#define EFI_ERROR(Status) (((INTN)(Status)) < 0)

// EFIWARN
/// Create a warning status
#define EFIWARN(Status) Status
// EFIERR
/// Create an error status
#define EFIERR(Status) (EFI_BIT_MAX | (Status))

// EFI_SUCCESS
/// The operation completed successfully.
#define EFI_SUCCESS 0

//
// Warning status codes
//

// EFI_WARN_UNKNOWN_GLYPH
/// The string contained one or more characters that the device could not render and were skipped
#define EFI_WARN_UNKNOWN_GLYPH EFIWARN(1)
// EFI_WARN_DELETE_FAILURE
/// The handle was closed, but the file was not deleted
#define EFI_WARN_DELETE_FAILURE EFIWARN(2)
// EFI_WARN_WRITE_FAILURE
/// The handle was closed, but the data to the file was not flushed properly
#define EFI_WARN_WRITE_FAILURE EFIWARN(3)
// EFI_WARN_BUFFER_TOO_SMALL
/// The resulting buffer was too small, and the data was truncated to the buffer size
#define EFI_WARN_BUFFER_TOO_SMALL EFIWARN(4)
// EFI_WARN_STALE_DATA
/// The data has not been updated within the timeframe set by local policy for this type of data
#define EFI_WARN_STALE_DATA EFIWARN(5)
// EFI_WARN_FILE_SYSTEM
/// The resulting buffer contains Efi-compliant file system
#define EFI_WARN_FILE_SYSTEM EFIWARN(6)
// EFI_WARN_RESET_REQUIRED
/// The operation will be processed across a system reset
#define EFI_WARN_RESET_REQUIRED EFIWARN(7)

//
// Error status codes
//

// EFI_LOAD_ERROR
/// The image failed to load
#define EFI_LOAD_ERROR EFIERR(1)
// EFI_INVALID_PARAMETER
/// A parameter was incorrect
#define EFI_INVALID_PARAMETER EFIERR(2)
// EFI_UNSUPPORTED
/// The operation is not supported
#define EFI_UNSUPPORTED EFIERR(3)
// EFI_BAD_BUFFER_SIZE
/// The buffer was not the proper size for the request
#define EFI_BAD_BUFFER_SIZE EFIERR(4)
// EFI_BUFFER_TOO_SMALL
/// The buffer is not large enough to hold the requested data. The required buffer size is returned in the appropriate parameter when this error occurs
#define EFI_BUFFER_TOO_SMALL EFIERR(5)
// EFI_NOT_READY
/// There is no data pending upon return
#define EFI_NOT_READY EFIERR(6)
// EFI_DEVICE_ERROR
/// The physical device reported an error while attempting the operation
#define EFI_DEVICE_ERROR EFIERR(7)
// EFI_WRITE_PROTECTED
/// The device cannot be written to
#define EFI_WRITE_PROTECTED EFIERR(8)
// EFI_OUT_OF_RESOURCES
/// A resource has run out
#define EFI_OUT_OF_RESOURCES EFIERR(9)
// EFI_VOLUME_CORRUPTED
/// An inCONSTancy was detected on the file system causing the operating to fail
#define EFI_VOLUME_CORRUPTED EFIERR(10)
// EFI_VOLUME_FULL
/// There is no more space on the file system
#define EFI_VOLUME_FULL EFIERR(11)
// EFI_NO_MEDIA
/// The device does not contain any medium to perform the operation
#define EFI_NO_MEDIA EFIERR(12)
// EFI_MEDIA_CHANGED
/// The medium in the device has changed since the last access
#define EFI_MEDIA_CHANGED EFIERR(13)
// EFI_NOT_FOUND
/// The item was not found
#define EFI_NOT_FOUND EFIERR(14)
// EFI_ACCESS_DENIED
/// Access was denied
#define EFI_ACCESS_DENIED EFIERR(15)
// EFI_NO_RESPONSE
/// The server was not found or did not respond to the request
#define EFI_NO_RESPONSE EFIERR(16)
// EFI_NO_MAPPING
/// A mapping to a device does not exist
#define EFI_NO_MAPPING EFIERR(17)
// EFI_TIMEOUT
/// The timeout time expired
#define EFI_TIMEOUT EFIERR(18)
// EFI_NOT_STARTED
/// The protocol has not been started
#define EFI_NOT_STARTED EFIERR(19)
// EFI_ALREADY_STARTED
/// The protocol has already been started
#define EFI_ALREADY_STARTED EFIERR(20)
// EFI_ABORTED
/// The operation was aborted
#define EFI_ABORTED EFIERR(21)
// EFI_ICMP_ERROR
/// An ICMP error occurred during the network operation
#define EFI_ICMP_ERROR EFIERR(22)
// EFI_TFTP_ERROR
/// A TFTP error occurred during the network operation
#define EFI_TFTP_ERROR EFIERR(23)
// EFI_PROTOCOL_ERROR
/// A protocol error occurred during the network operation
#define EFI_PROTOCOL_ERROR EFIERR(24)
// EFI_INCOMPATIBLE_VERSION
/// The function encountered an internal version that was incompatible with a version requested by the caller
#define EFI_INCOMPATIBLE_VERSION EFIERR(25)
// EFI_SECURITY_VIOLATION
/// The function was not performed due to a security violation
#define EFI_SECURITY_VIOLATION EFIERR(26)
// EFI_CRC_ERROR
/// A CRC error was detected
#define EFI_CRC_ERROR EFIERR(27)
// EFI_END_OF_MEDIA
/// Beginning or end of media was reached
#define EFI_END_OF_MEDIA EFIERR(28)
// EFI_END_OF_FILE
/// The end of the file was reached
#define EFI_END_OF_FILE EFIERR(31)
// EFI_INVALID_LANGUAGE
/// The language specified was invalid
#define EFI_INVALID_LANGUAGE EFIERR(32)
// EFI_COMPROMISED_DATA
/// The security status of the data is unknown or compromised and the data must be updated or replaced to restore a valid security status
#define EFI_COMPROMISED_DATA EFIERR(33)
// EFI_IP_ADDRESS_CONFLICT
/// There is an address conflict address allocation
#define EFI_IP_ADDRESS_CONFLICT EFIERR(34)
// EFI_HTTP_ERROR
/// A HTTP error occurred during the network operation
#define EFI_HTTP_ERROR EFIERR(35)

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif ___UEFI_STATUS_HEADER__
