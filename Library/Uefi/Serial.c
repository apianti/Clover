///
/// @file Library/Uefi/Serial.c
///
/// UEFI implementation serial i/o protocol
///

#include <Uefi.h>

// EFI_SERIAL_HANDLE_COUNT
/// The count of handles to use in getting the protocol interfaces
#define EFI_SERIAL_HANDLE_COUNT 32

// gEfiSerialIoProtocolGuid
/// The serial i/o protocol unique identifier
EFI_GUID gEfiSerialIoProtocolGuid = EFI_SERIAL_IO_PROTOCOL_GUID;

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
) {
  if ((This == NULL) || (This->Reset == NULL) || (This->Mode == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Reset the device
  return This->Reset(This);
}
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
) {
  if ((This == NULL) || (This->SetAttributes == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return This->SetAttributes(This, BaudRate, ReceiveFifoDepth, Timeout, Parity, DataBits, StopBits);
}
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
) {
  if ((This == NULL) || (Mode == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Set the attributes for the serial device from the mode information
  return EfiSerialSetAttributes(This, Mode->BaudRate, Mode->ReceiveFifoDepth, Mode->Timeout, (EFI_PARITY_TYPE)(Mode->Parity), (UINT8)(Mode->DataBits), (EFI_STOP_BITS_TYPE)(Mode->StopBits));
}
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
) {
  if ((This == NULL) || (This->SetControl == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return This->SetControl(This, Control);
}
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
) {
  if ((This == NULL) || (This->GetControl == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return This->GetControl(This, Control);
}
// EfiSerialAreControlBitsSet
/// Checks whether the specified control bits are set for a serial device
/// @param This The serial i/o protocol interface
/// @param ControlBits The control bits to check are set
/// @return Whether all the specified control bits are set or not
BOOLEAN
EFIAPI
EfiSerialAreControlBitsSet (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN UINT32                  ControlBits
) {
  UINT32 Control = 0;
  if (ControlBits == 0) {
    return FALSE;
  }
  if (EFI_ERROR(EfiSerialGetControlBits(This, &Control))) {
    return FALSE;
  }
  return EFI_BITS_ARE_SET(Control, ControlBits);
}
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
) {
  // Check parameters
  if ((This == NULL) || (BufferSize == NULL) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (This->Write == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Check the buffer the buffer size
  if (*BufferSize == 0) {
    return EFI_SUCCESS;
  }
  // Write the buffer
  return This->Write(This, BufferSize, Buffer);
}
// EfiSerialStrWrite
/// Writes string to a serial device
/// @param This The serial i/o protocol interface
/// @param Str  The string to write
/// @retval EFI_DEVICE_ERROR The device reported an error
/// @retval EFI_TIMEOUT      The data write was stopped due to a timeout
/// @retval EFI_SUCCESS      The data was written
EFI_STATUS
EFIAPI
EfiSerialStrWrite (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN CONST CHAR16           *Str
) {
  UINTN Size = StrLen(Str) * sizeof(CHAR16);
  return EfiSerialWrite(This, &Size, (VOID *)Str);
}
// EfiSerialAsciiWrite
/// Writes string to a serial device
/// @param This  The serial i/o protocol interface
/// @param Ascii The string to write
/// @retval EFI_DEVICE_ERROR The device reported an error
/// @retval EFI_TIMEOUT      The data write was stopped due to a timeout
/// @retval EFI_SUCCESS      The data was written
EFI_STATUS
EFIAPI
EfiSerialAsciiWrite (
  IN EFI_SERIAL_IO_PROTOCOL *This,
  IN CONST CHAR8            *Ascii
) {
  UINTN Size = AsciiStrLen(Ascii) * sizeof(CHAR8);
  return EfiSerialWrite(This, &Size, (VOID *)Ascii);
}
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
) {
  if ((This == NULL) || (BufferSize == NULL) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (This->Read == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Check the buffer the buffer size
  if (*BufferSize == 0) {
    return EFI_SUCCESS;
  }
  // Read the buffer
  return This->Read(This, BufferSize, Buffer);
}

// EfiSerialWriteAll
/// Writes data to all serial devices
/// @param BufferSize The size in bytes of the data to write
/// @param Buffer     The data to write
/// @retval EFI_DEVICE_ERROR The device reported an error
/// @retval EFI_TIMEOUT      The data write was stopped due to a timeout
/// @retval EFI_SUCCESS      The data was written
EFI_STATUS
EFIAPI
EfiSerialWriteAll (
  IN UINTN  BufferSize,
  IN VOID  *Buffer
) {
  EFI_STATUS Status;
  EFI_HANDLE Handles[EFI_SERIAL_HANDLE_COUNT];
  UINTN      Index;
  UINTN      Count;
  UINTN      Size = sizeof(Handles);
  // Check parameters
  if ((BufferSize == 0) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Locate all the handles with serial i/o protocol interfaces
  Status = EfiLocateHandle(ByProtocol, &gEfiSerialIoProtocolGuid, NULL, &Size, Handles);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Get the count of handles
  Count = (Size / sizeof(EFI_HANDLE));
  if (Handles != NULL) {
    // Traverse the handles to get each serial i/o protocol interface
    for (Index = 0; Index < Count; ++Index) {
      EFI_SERIAL_IO_PROTOCOL *SerialIo = NULL;
      if (Handles[Index] == NULL) {
        continue;
      }
      // Get the serial i/o protocol interface from this handle
      Status = EfiHandleProtocol(Handles[Index], &gEfiSerialIoProtocolGuid, (VOID **)&SerialIo);
      if (EFI_ERROR(Status)) {
        continue;
      }
      if (SerialIo == NULL) {
        continue;
      }
      // Write to the serial device
      Size = BufferSize;
      Status = EfiSerialWrite(SerialIo, &Size, Buffer);
      if (!EFI_ERROR(Status)) {
        break;
      }
      if (BufferSize <= Size) {
        Status = EFI_SUCCESS;
        break;
      }
      Buffer = ADDRESS_OFFSET(VOID *, Buffer, Size);
      BufferSize -= Size;
    }
  }
  return Status;
}
// EfiSerialStrWriteAll
/// Writes string to all serial devices
/// @param Str The string to write
/// @retval EFI_DEVICE_ERROR The device reported an error
/// @retval EFI_TIMEOUT      The data write was stopped due to a timeout
/// @retval EFI_SUCCESS      The data was written
EFI_STATUS
EFIAPI
EfiSerialStrWriteAll (
  IN CONST CHAR16 *Str
) {
  return EfiSerialWriteAll(StrLen(Str) * sizeof(CHAR16), (VOID *)Str);
}
// EfiSerialAsciiWriteAll
/// Writes string to all serial devices
/// @param Ascii The string to write
/// @retval EFI_DEVICE_ERROR The device reported an error
/// @retval EFI_TIMEOUT      The data write was stopped due to a timeout
/// @retval EFI_SUCCESS      The data was written
EFI_STATUS
EFIAPI
EfiSerialAsciiWriteAll (
  IN CONST CHAR8 *Ascii
) {
  return EfiSerialWriteAll(AsciiStrLen(Ascii) * sizeof(CHAR8), (VOID *)Ascii);
}
