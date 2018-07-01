///
/// @file Library/Uefi/Status.c
///
/// UEFI status character strings
///

#include <Uefi.h>

// 
/// 
#define DEFINE_STATUS_STRING(Status, String) { Status, L ## String, String }

// EFI_STATUS_STRING
/// Map a status to a character string
typedef struct EFI_STATUS_STRING EFI_STATUS_STRING;
struct EFI_STATUS_STRING {

  // Status
  /// The status
  EFI_STATUS  Status;
  // String
  /// The status description character string
  CHAR16     *String;
  // Ascii
  /// The status description character string
  CHAR8      *Ascii;

};

// mUnknownStatus
/// Status description for unknown status
STATIC CONST EFI_STATUS_STRING mUnknownStatus = DEFINE_STATUS_STRING(0, "Unknown status");
// mStatusStrings
/// Status description character strings
STATIC CONST EFI_STATUS_STRING mStatusStrings[] = {
  DEFINE_STATUS_STRING(EFI_SUCCESS, "Success"),
  DEFINE_STATUS_STRING(EFI_WARN_UNKNOWN_GLYPH, "Warning Unknown Glyph"),
  DEFINE_STATUS_STRING(EFI_WARN_DELETE_FAILURE, "Warning Delete Failure"),
  DEFINE_STATUS_STRING(EFI_WARN_WRITE_FAILURE, "Warning Write Failure"),
  DEFINE_STATUS_STRING(EFI_WARN_BUFFER_TOO_SMALL, "Warning Buffer Too Small"),
  DEFINE_STATUS_STRING(EFI_WARN_STALE_DATA, "Warning Stale Data"),
  DEFINE_STATUS_STRING(EFI_WARN_FILE_SYSTEM, "Warning Efi-Compliant File System"),
  DEFINE_STATUS_STRING(EFI_WARN_RESET_REQUIRED, "Warning Reset Required"),
  DEFINE_STATUS_STRING(EFI_LOAD_ERROR, "Load Error"),
  DEFINE_STATUS_STRING(EFI_INVALID_PARAMETER, "Invalid Parameter"),
  DEFINE_STATUS_STRING(EFI_UNSUPPORTED, "Unsupported"),
  DEFINE_STATUS_STRING(EFI_BAD_BUFFER_SIZE, "Bad Buffer Size"),
  DEFINE_STATUS_STRING(EFI_BUFFER_TOO_SMALL, "Buffer Too Small"),
  DEFINE_STATUS_STRING(EFI_NOT_READY, "Not Ready"),
  DEFINE_STATUS_STRING(EFI_DEVICE_ERROR, "Device Error"),
  DEFINE_STATUS_STRING(EFI_WRITE_PROTECTED, "Write Protected"),
  DEFINE_STATUS_STRING(EFI_OUT_OF_RESOURCES, "Out of Resources"),
  DEFINE_STATUS_STRING(EFI_VOLUME_CORRUPTED, "Volume Corrupt"),
  DEFINE_STATUS_STRING(EFI_VOLUME_FULL, "Volume Full"),
  DEFINE_STATUS_STRING(EFI_NO_MEDIA, "No Media"),
  DEFINE_STATUS_STRING(EFI_MEDIA_CHANGED, "Media changed"),
  DEFINE_STATUS_STRING(EFI_NOT_FOUND, "Not Found"),
  DEFINE_STATUS_STRING(EFI_ACCESS_DENIED, "Access Denied"),
  DEFINE_STATUS_STRING(EFI_NO_RESPONSE, "No Response"),
  DEFINE_STATUS_STRING(EFI_NO_MAPPING, "No mapping"),
  DEFINE_STATUS_STRING(EFI_TIMEOUT, "Time out"),
  DEFINE_STATUS_STRING(EFI_NOT_STARTED, "Not started"),
  DEFINE_STATUS_STRING(EFI_ALREADY_STARTED, "Already started"),
  DEFINE_STATUS_STRING(EFI_ABORTED, "Aborted"),
  DEFINE_STATUS_STRING(EFI_ICMP_ERROR, "ICMP Error"),
  DEFINE_STATUS_STRING(EFI_TFTP_ERROR, "TFTP Error"),
  DEFINE_STATUS_STRING(EFI_PROTOCOL_ERROR, "Protocol Error"),
  DEFINE_STATUS_STRING(EFI_INCOMPATIBLE_VERSION, "Incompatible Version"),
  DEFINE_STATUS_STRING(EFI_SECURITY_VIOLATION, "Security Violation"),
  DEFINE_STATUS_STRING(EFI_CRC_ERROR, "CRC Error"),
  DEFINE_STATUS_STRING(EFI_END_OF_MEDIA, "End of Media"),
  DEFINE_STATUS_STRING(EFI_END_OF_FILE, "End of File"),
  DEFINE_STATUS_STRING(EFI_INVALID_LANGUAGE, "Invalid Language"),
  DEFINE_STATUS_STRING(EFI_COMPROMISED_DATA, "Compromised Data"),
  DEFINE_STATUS_STRING(EFI_IP_ADDRESS_CONFLICT, "IP Address Conflict"),
  DEFINE_STATUS_STRING(EFI_HTTP_ERROR, "HTTP Network Error"),
};

// StrFromStatus
/// Convert a status code to string
/// @param Status The status code to convert
/// @return The string for the status code
CONST CHAR16 *
EFIAPI
StrFromStatus (
  EFI_STATUS Status
) {
  // Search for the status
  for (UINTN Index = 0; Index < ARRAY_COUNT(mStatusStrings); ++Index) {
    if (mStatusStrings[Index].Status == Status) {
      // Return the status description
      return mStatusStrings[Index].String;
    }
  }
  // Not found so return unknown status description
  return mUnknownStatus.String;
}
// AsciiStrFromStatus
/// Convert a status code to string
/// @param Status The status code to convert
/// @return The string for the status code
CONST CHAR8 *
EFIAPI
AsciiStrFromStatus (
  EFI_STATUS Status
) {
  // Search for the status
  for (UINTN Index = 0; Index < ARRAY_COUNT(mStatusStrings); ++Index) {
    if (mStatusStrings[Index].Status == Status) {
      // Return the status description
      return mStatusStrings[Index].Ascii;
    }
  }
  // Not found so return unknown status description
  return mUnknownStatus.Ascii;
}
