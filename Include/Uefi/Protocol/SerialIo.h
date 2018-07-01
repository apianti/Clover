///
/// @file Include/Uefi/Protocol/SerialIo.h
///
/// UEFI implementation serial i/o protocol
///

#pragma once
#ifndef __UEFI_SERIAL_IO_PROTOCOL_HEADER__
#define __UEFI_SERIAL_IO_PROTOCOL_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_SERIAL_IO_PROTOCOL_GUID
/// The serial i/o protocol unique identifier
#define EFI_SERIAL_IO_PROTOCOL_GUID { 0xBB25CF6F, 0xF1D4, 0x11D2, { 0x9A, 0x0C, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0xFD } }

// EFI_SERIAL_IO_PROTOCOL_REVISION
/// The serial i/o protocol revision
#define EFI_SERIAL_IO_PROTOCOL_REVISION 0x00010000

//
// Serial i/o control bits
//

// EFI_SERIAL_CLEAR_TO_SEND
/// Clear to send control bit
#define EFI_SERIAL_CLEAR_TO_SEND 0x0010
// EFI_SERIAL_DATA_SET_READY
/// Data set ready control bit
#define EFI_SERIAL_DATA_SET_READY 0x0020
// EFI_SERIAL_RING_INDICATE
/// Ring indicate control bit
#define EFI_SERIAL_RING_INDICATE 0x0040
// EFI_SERIAL_CARRIER_DETECT
/// Carrier detect control bit
#define EFI_SERIAL_CARRIER_DETECT 0x0080
// EFI_SERIAL_REQUEST_TO_SEND
/// Request device to send control bit
#define EFI_SERIAL_REQUEST_TO_SEND 0x0002
// EFI_SERIAL_DATA_TERMINAL_READY
/// Data terminal ready control bit
#define EFI_SERIAL_DATA_TERMINAL_READY 0x0001
// EFI_SERIAL_INPUT_BUFFER_EMPTY
/// Input buffer is empty control bit
#define EFI_SERIAL_INPUT_BUFFER_EMPTY 0x0100
// EFI_SERIAL_OUTPUT_BUFFER_EMPTY
/// Output buffer is empty control bit
#define EFI_SERIAL_OUTPUT_BUFFER_EMPTY 0x0200
// EFI_SERIAL_HARDWARE_LOOPBACK_ENABLE
/// Enable hardware loopback control bit
#define EFI_SERIAL_HARDWARE_LOOPBACK_ENABLE 0x1000
// EFI_SERIAL_SOFTWARE_LOOPBACK_ENABLE
/// Enable software loopback control bit
#define EFI_SERIAL_SOFTWARE_LOOPBACK_ENABLE 0x2000
// EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE
/// Enable hardware flow control control bit
#define EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE 0x4000

// gEfiSerialIoProtocolGuid
/// The serial i/o protocol unique identifier
EXTERN EFI_GUID gEfiSerialIoProtocolGuid;

// EFI_SERIAL_IO_PROTOCOL
/// The serial i/o protocol
typedef struct EFI_SERIAL_IO_PROTOCOL EFI_SERIAL_IO_PROTOCOL;

// EFI_PARITY_TYPE
/// Serial i/o parity
typedef enum EFI_PARITY_TYPE EFI_PARITY_TYPE;
enum EFI_PARITY_TYPE {

  // DefaultParity
  /// Default parity
  DefaultParity,
  // NoParity
  /// No parity
  NoParity,
  // EvenParity
  /// Even parity
  EvenParity,
  // OddParity
  /// Odd parity
  OddParity,
  // MarkParity
  /// Mark parity
  MarkParity,
  // SpaceParity
  /// Space parity
  SpaceParity

};
// EFI_STOP_BITS_TYPE
/// Serial i/o stop bits
typedef enum EFI_STOP_BITS_TYPE EFI_STOP_BITS_TYPE;
enum EFI_STOP_BITS_TYPE {

  // DefaultStopBits
  /// Default stop bits
  DefaultStopBits,
  // OneStopBit
  /// One stop bit
  OneStopBit,
  // OneFiveStopBits
  /// One and a half stop bits
  OneFiveStopBits,
  // TwoStopBits
  /// Two stop bits
  TwoStopBits

};

// EFI_SERIAL_RESET
/// Resets the serial device
/// @param This The serial i/o protocol interface
/// @retval EFI_DEVICE_ERROR The serial device could not be reset
/// @retval EFI_SUCCESS      The serial device was reset
typedef
EFI_STATUS
(EFIAPI
*EFI_SERIAL_RESET) (
  IN EFI_SERIAL_IO_PROTOCOL *This
);
// EFI_SERIAL_SET_ATTRIBUTES
/// Sets the baud rate, receive FIFO depth, transmit/receive time out, parity, data bits, and stop bits on a serial device
/// @param This The serial i/o protocol interface
/// @param BaudRate         The requested baud rate
/// @param ReceiveFifoDepth The requested depth of the FIFO on the receive side of the serial interface
/// @param Timeout          The requested time out for a single character in microseconds
/// @param Parity           The type of parity to use on this serial device
/// @param DataBits         The number of data bits to use on this serial device
/// @param StopBits         The number of stop bits to use on this serial device
/// @retval EFI_INVALID_PARAMETER One or more of the attributes has an unsupported value
/// @retval EFI_DEVICE_ERROR      The serial device is not functioning correctly
/// @retval EFI_SUCCESS           The new attributes were set on the serial device
typedef
EFI_STATUS
(EFIAPI
*EFI_SERIAL_SET_ATTRIBUTES) (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN UINT64                  BaudRate,
  IN UINT32                  ReceiveFifoDepth,
  IN UINT32                  Timeout,
  IN EFI_PARITY_TYPE         Parity,
  IN UINT8                   DataBits,
  IN EFI_STOP_BITS_TYPE      StopBits
);
// EFI_SERIAL_SET_CONTROL_BITS
/// Sets the control bits on a serial device
/// @param This    The serial i/o protocol interface
/// @param Control The control bits to set for the serial device
/// @retval EFI_UNSUPPORTED  The serial device does not support this operation
/// @retval EFI_DEVICE_ERROR The serial device is not functioning correctly
/// @retval EFI_SUCCESS      The new control bits were set on the serial device
typedef
EFI_STATUS
(EFIAPI
*EFI_SERIAL_SET_CONTROL_BITS) (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN UINT32                  Control
);
// EFI_SERIAL_GET_CONTROL_BITS
/// Retrieves the status of the control bits on a serial device
/// @param This    The serial i/o protocol interface
/// @param Control On output, the controls bits for the serial device
/// @retval EFI_DEVICE_ERROR The serial device is not functioning correctly
/// @retval EFI_SUCCESS      The control bits were read from the serial device
typedef
EFI_STATUS
(EFIAPI
*EFI_SERIAL_GET_CONTROL_BITS) (
  IN  EFI_SERIAL_IO_PROTOCOL *This,
  OUT UINT32                 *Control
);
// EFI_SERIAL_WRITE
/// Writes data to a serial device
/// @param This       The serial i/o protocol interface
/// @param BufferSize On input, the size in bytes of the data to write, on output, the size in bytes of the data written
/// @param Buffer     The data to write
/// @retval EFI_DEVICE_ERROR The device reported an error
/// @retval EFI_TIMEOUT      The data write was stopped due to a timeout
/// @retval EFI_SUCCESS      The data was written
typedef
EFI_STATUS
(EFIAPI
*EFI_SERIAL_WRITE) (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN OUT UINTN              *BufferSize,
  IN VOID                   *Buffer
);
// EFI_SERIAL_READ
/// Reads data from a serial device
/// @param This       The serial i/o protocol interface
/// @param BufferSize On input, the size in bytes of the read buffer, on output, the size in bytes of the read data
/// @param Buffer     On output, the data read from the serial device
/// @retval EFI_DEVICE_ERROR The serial device reported an error
/// @retval EFI_TIMEOUT      The operation was stopped due to a timeout or overrun
/// @retval EFI_SUCCESS      The data was read
typedef
EFI_STATUS
(EFIAPI
*EFI_SERIAL_READ) (
  IN     EFI_SERIAL_IO_PROTOCOL *This,
  IN OUT UINTN                  *BufferSize,
  OUT    VOID                   *Buffer
);

// SERIAL_IO_MODE
/// The serial i/o mode information
typedef struct SERIAL_IO_MODE SERIAL_IO_MODE;
struct SERIAL_IO_MODE {

  // ControlMask
  /// A mask of the Control bits that the device supports, the Input Buffer Empty control bit must always be supported
  UINT32 ControlMask;
  // Timeout
  /// If applicable, the number of microseconds to wait before timing out a Read or Write operation
  UINT32 Timeout;
  // BaudRate
  /// If applicable, the current baud rate setting of the device or zero to indicate that device runs at the device’s designed speed
  UINT64 BaudRate;
  // ReceiveFifoDepth
  /// The number of characters the device will buffer on input
  UINT32 ReceiveFifoDepth;
  // DataBits
  /// The number of data bits in each character
  UINT32 DataBits;
  // Parity
  /// If applicable, the parity that is computed or checked as each character is transmitted or received
  UINT32 Parity;
  // StopBits
  /// If applicable, the number of stop bits per character
  UINT32 StopBits;

};

// EFI_SERIAL_IO_PROTOCOL
/// The serial i/o protocol
struct EFI_SERIAL_IO_PROTOCOL {

  // Revision
  /// The revision of the protocol
  UINT32                       Revision;
  // Reset
  /// Resets the hardware device
  EFI_SERIAL_RESET             Reset;
  // SetAttributes
  /// Sets communication parameters for a serial device
  EFI_SERIAL_SET_ATTRIBUTES    SetAttributes;
  // SetControl
  /// Sets the control bits on a serial device
  EFI_SERIAL_SET_CONTROL_BITS  SetControl;
  // GetControl
  /// Reads the status of the control bits on a serial device
  EFI_SERIAL_GET_CONTROL_BITS  GetControl;
  // Write
  /// Sends a buffer of characters to a serial device
  EFI_SERIAL_WRITE             Write;
  // Read
  /// Receives a buffer of characters from a serial device
  EFI_SERIAL_READ              Read;
  // Mode
  /// The serial i/o mode information
  SERIAL_IO_MODE              *Mode;

};

// EfiSerialReset
/// Resets the serial device
/// @param This The serial i/o protocol interface
/// @retval EFI_DEVICE_ERROR The serial device could not be reset
/// @retval EFI_SUCCESS      The serial device was reset
EXTERN
EFI_STATUS
EFIAPI
EfiSerialReset (
  IN EFI_SERIAL_IO_PROTOCOL *This
);
// EfiSerialSetAttributes
/// Sets the baud rate, receive FIFO depth, transmit/receive time out, parity, data bits, and stop bits on a serial device
/// @param This The serial i/o protocol interface
/// @param BaudRate         The requested baud rate
/// @param ReceiveFifoDepth The requested depth of the FIFO on the receive side of the serial interface
/// @param Timeout          The requested time out for a single character in microseconds
/// @param Parity           The type of parity to use on this serial device
/// @param DataBits         The number of data bits to use on this serial device
/// @param StopBits         The number of stop bits to use on this serial device
/// @retval EFI_INVALID_PARAMETER One or more of the attributes has an unsupported value
/// @retval EFI_DEVICE_ERROR      The serial device is not functioning correctly
/// @retval EFI_SUCCESS           The new attributes were set on the serial device
EXTERN
EFI_STATUS
EFIAPI
EfiSerialSetAttributes (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN UINT64                  BaudRate,
  IN UINT32                  ReceiveFifoDepth,
  IN UINT32                  Timeout,
  IN EFI_PARITY_TYPE         Parity,
  IN UINT8                   DataBits,
  IN EFI_STOP_BITS_TYPE      StopBits
);
// EfiSerialSetMode
/// Sets the baud rate, receive FIFO depth, transmit/receive time out, parity, data bits, and stop bits on a serial device
/// @param This The serial i/o protocol interface
/// @param Mode The serial i/o mode information to set
/// @retval EFI_INVALID_PARAMETER One or more of the attributes has an unsupported value
/// @retval EFI_DEVICE_ERROR      The serial device is not functioning correctly
/// @retval EFI_SUCCESS           The new attributes were set on the serial device
EXTERN
EFI_STATUS
EFIAPI
EfiSerialSetMode (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN SERIAL_IO_MODE         *Mode
);
// EfiSerialSetControlBits
/// Sets the control bits on a serial device
/// @param This    The serial i/o protocol interface
/// @param Control The control bits to set for the serial device
/// @retval EFI_UNSUPPORTED  The serial device does not support this operation
/// @retval EFI_DEVICE_ERROR The serial device is not functioning correctly
/// @retval EFI_SUCCESS      The new control bits were set on the serial device
EXTERN
EFI_STATUS
EFIAPI
EfiSerialSetControlBits (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN UINT32                  Control
);
// EfiSerialGetControlBits
/// Retrieves the status of the control bits on a serial device
/// @param This    The serial i/o protocol interface
/// @param Control On output, the controls bits for the serial device
/// @retval EFI_DEVICE_ERROR The serial device is not functioning correctly
/// @retval EFI_SUCCESS      The control bits were read from the serial device
EXTERN
EFI_STATUS
EFIAPI
EfiSerialGetControlBits (
  IN  EFI_SERIAL_IO_PROTOCOL *This,
  OUT UINT32                 *Control
);
// EfiSerialAreControlBitsSet
/// Checks whether the specified control bits are set for a serial device
/// @param This The serial i/o protocol interface
/// @param ControlBits The control bits to check are set
/// @return Whether all the specified control bits are set or not
EXTERN
BOOLEAN
EFIAPI
EfiSerialAreControlBitsSet (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN UINT32                  ControlBits
);
// EfiSerialWrite
/// Writes data to a serial device
/// @param This       The serial i/o protocol interface
/// @param BufferSize On input, the size in bytes of the data to write, on output, the size in bytes of the data written
/// @param Buffer     The data to write
/// @retval EFI_DEVICE_ERROR The device reported an error
/// @retval EFI_TIMEOUT      The data write was stopped due to a timeout
/// @retval EFI_SUCCESS      The data was written
EXTERN
EFI_STATUS
EFIAPI
EfiSerialWrite (
  IN     EFI_SERIAL_IO_PROTOCOL *This,
  IN OUT UINTN                  *BufferSize,
  IN     VOID                   *Buffer
);
// EfiSerialStrWrite
/// Writes string to a serial device
/// @param This The serial i/o protocol interface
/// @param Str  The string to write
/// @retval EFI_DEVICE_ERROR The device reported an error
/// @retval EFI_TIMEOUT      The data write was stopped due to a timeout
/// @retval EFI_SUCCESS      The data was written
EXTERN
EFI_STATUS
EFIAPI
EfiSerialStrWrite (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN CONST CHAR16           *Str
);
// EfiSerialAsciiWrite
/// Writes string to a serial device
/// @param This  The serial i/o protocol interface
/// @param Ascii The string to write
/// @retval EFI_DEVICE_ERROR The device reported an error
/// @retval EFI_TIMEOUT      The data write was stopped due to a timeout
/// @retval EFI_SUCCESS      The data was written
EXTERN
EFI_STATUS
EFIAPI
EfiSerialAsciiWrite (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN CONST CHAR8            *Ascii
);
// EfiSerialRead
/// Reads data from a serial device
/// @param This       The serial i/o protocol interface
/// @param BufferSize On input, the size in bytes of the read buffer, on output, the size in bytes of the read data
/// @param Buffer     On output, the data read from the serial device
/// @retval EFI_DEVICE_ERROR The serial device reported an error
/// @retval EFI_TIMEOUT      The operation was stopped due to a timeout or overrun
/// @retval EFI_SUCCESS      The data was read
EXTERN
EFI_STATUS
EFIAPI
EfiSerialRead (
  IN     EFI_SERIAL_IO_PROTOCOL *This,
  IN OUT UINTN                  *BufferSize,
  OUT    VOID                   *Buffer
);

// EfiSerialWriteAll
/// Writes data to all serial devices
/// @param BufferSize The size in bytes of the data to write
/// @param Buffer     The data to write
/// @retval EFI_DEVICE_ERROR The device reported an error
/// @retval EFI_TIMEOUT      The data write was stopped due to a timeout
/// @retval EFI_SUCCESS      The data was written
EXTERN
EFI_STATUS
EFIAPI
EfiSerialWriteAll (
  IN UINTN  BufferSize,
  IN VOID  *Buffer
);
// EfiSerialStrWriteAll
/// Writes string to all serial devices
/// @param Str The string to write
/// @retval EFI_DEVICE_ERROR The device reported an error
/// @retval EFI_TIMEOUT      The data write was stopped due to a timeout
/// @retval EFI_SUCCESS      The data was written
EXTERN
EFI_STATUS
EFIAPI
EfiSerialStrWriteAll (
  IN CONST CHAR16 *Str
);
// EfiSerialAsciiWriteAll
/// Writes string to all serial devices
/// @param Ascii The string to write
/// @retval EFI_DEVICE_ERROR The device reported an error
/// @retval EFI_TIMEOUT      The data write was stopped due to a timeout
/// @retval EFI_SUCCESS      The data was written
EXTERN
EFI_STATUS
EFIAPI
EfiSerialAsciiWriteAll (
  IN CONST CHAR8 *Ascii
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __UEFI_SERIAL_IO_PROTOCOL_HEADER__
