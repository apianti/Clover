///
/// @file Include/Uefi/Base.h
///
/// UEFI implementation declarations
///

#pragma once
#ifndef ___UEFI_BASE_HEADER__
#define ___UEFI_BASE_HEADER__

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#pragma pack()

#include <Uefi/Status.h>

// EFI_2_70_SYSTEM_TABLE_REVISION
/// The system table complies with UEFI 2.7
#define EFI_2_70_SYSTEM_TABLE_REVISION ((2 << 16) | (70))
// EFI_2_60_SYSTEM_TABLE_REVISION
/// The system table complies with UEFI 2.6
#define EFI_2_60_SYSTEM_TABLE_REVISION ((2 << 16) | (60))
// EFI_2_50_SYSTEM_TABLE_REVISION
/// The system table complies with UEFI 2.5
#define EFI_2_50_SYSTEM_TABLE_REVISION ((2 << 16) | (50))
// EFI_2_40_SYSTEM_TABLE_REVISION
/// The system table complies with UEFI 2.4
#define EFI_2_40_SYSTEM_TABLE_REVISION ((2 << 16) | (40))
// EFI_2_31_SYSTEM_TABLE_REVISION
/// The system table complies with UEFI 2.3.1
#define EFI_2_31_SYSTEM_TABLE_REVISION ((2 << 16) | (31))
// EFI_2_30_SYSTEM_TABLE_REVISION
/// The system table complies with UEFI 2.3
#define EFI_2_30_SYSTEM_TABLE_REVISION ((2 << 16) | (30))
// EFI_2_20_SYSTEM_TABLE_REVISION
/// The system table complies with UEFI 2.2
#define EFI_2_20_SYSTEM_TABLE_REVISION ((2 << 16) | (20))
// EFI_2_10_SYSTEM_TABLE_REVISION
/// The system table complies with UEFI 2.1
#define EFI_2_10_SYSTEM_TABLE_REVISION ((2 << 16) | (10))
// EFI_2_00_SYSTEM_TABLE_REVISION
/// The system table complies with UEFI 2.0
#define EFI_2_00_SYSTEM_TABLE_REVISION ((2 << 16) | (00))
// EFI_1_10_SYSTEM_TABLE_REVISION
/// The system table complies with UEFI 1.1
#define EFI_1_10_SYSTEM_TABLE_REVISION ((1 << 16) | (10))
// EFI_1_02_SYSTEM_TABLE_REVISION
/// The system table complies with UEFI 1.0.2
#define EFI_1_02_SYSTEM_TABLE_REVISION ((1 << 16) | (02))

// EFI_SPECIFICATION_VERSION
/// The UEFI specification version to which this implementation complies
#define EFI_SPECIFICATION_VERSION EFI_2_70_SYSTEM_TABLE_REVISION

// IN
/// Parameter modifier specifying the parameter is passed to function
#define IN
// OUT
/// Parameter modifier specifying the parameter is returned from function
#define OUT
// OPTIONAL
/// Parameter modifier specifying the parameter is optional and NULL may be passed if the value is not supplied
#define OPTIONAL
// CONST
/// Variable modifier specifying the variable is read-only
#define CONST const
// EXTERN
/// Scope modifier specifiying external scope
#define EXTERN extern
// STATIC
/// Static modifier specifying static allocation or local scope
#define STATIC static
// VOLATILE
/// Volatile modifier hint
#define VOLATILE volatile
// INLINE
/// Inline modifier hint
#define INLINE __inline

// EFIAPI
/// Calling convention for UEFI interfaces and functions
#define EFIAPI __cdecl

// TRUE
/// Logical boolean true
#define TRUE ((BOOLEAN)1)
// FALSE
/// Logical boolean false
#define FALSE ((BOOLEAN)0)

// NULL
/// Null pointer
#define NULL 0
// CHAR_NULL
/// Null character
#define CHAR_NULL ((CHAR16)0x0000)

// ARRAY_COUNT
/// Get the count of items in a static array
/// @param Array The static array
#define ARRAY_COUNT(Array) (sizeof(Array) / sizeof((Array)[0]))

// OFFSET_OF
/// Get the offset in bytes of a structure member
/// @param TYPE   The type name of the structure
/// @param Member The member name of which to get the offset
#define OFFSET_OF(TYPE, Member) ((UINTN)(&(((TYPE *)0)->Member)))
// ADDRESS_OF
/// Get the address of a structure member instance
/// @param TYPE     The type name of the structure
/// @param Instance The instance of the structure
/// @param MTYPE    The type name of the member
/// @param Member   The member name of which to get the address
#define ADDRESS_OF(TYPE, Instance, MTYPE, Member) ((MTYPE *)(((UINT8 *)(Instance)) + OFFSET_OF(TYPE, Member)))
// ADDRESS_OFFSET
/// Get an offset from an address
/// @param TYPE    The type name of the address
/// @param Address The address
/// @param Offset  The offset of the address
#define ADDRESS_OFFSET(TYPE, Address, Offset) ((TYPE *)(((UINT8 *)(Address)) + (Offset)))

// UNUSED_PARAMETER
/// Unused parameter in function
#define UNUSED_PARAMETER(Unused) (Unused)

// EFI_BIT
/// Get a bit by significance
/// @param Bit The bit significance
/// @return The bit mask with only the bit set
#define EFI_BIT(Bit) (1ULL << (Bit))
// EFI_BIT_SET
/// Set a bit by significance
/// @param Value The value of which to set the bit
/// @param Bit   The bit significance
/// @return The bit mask with the additional bit set
#define EFI_BIT_SET(Value, Bit) ((Value) | EFI_BIT(Bit))
// EFI_BIT_UNSET
/// Unset a bit by significance
/// @param Value The value of which to unset the bit
/// @param Bit   The bit significance
/// @return The bit mask with the additional bit unset
#define EFI_BIT_UNSET(Value, Bit) ((Value) & ~EFI_BIT(Bit))
// EFI_BIT_IS_SET
/// Check a bit is set by significance
/// @param Value The value of which to set the bit
/// @param Bit   The bit significance
/// @return The bit mask with the additional bit set
#define EFI_BIT_IS_SET(Value, Bit) (((Value) & EFI_BIT(Bit)) != 0)
// EFI_BIT_IS_UNSET
/// Check a bit is unset by significance
/// @param Value The value of which to unset the bit
/// @param Bit   The bit significance
/// @return The bit mask with the additional bit unset
#define EFI_BIT_IS_UNSET(Value, Bit) (((Value) & EFI_BIT(Bit)) == 0)
// EFI_BITS_SET
/// Set the specified bits
/// @param Value The value of which to set the specified bits
/// @param Bits  The bits to set
/// @return The value with the specified bits set
#define EFI_BITS_SET(Value, Bits) ((Value) | (Bits))
// EFI_BITS_UNSET
/// Unset the specified bits
/// @param Value The value of which to set the specified bits
/// @param Bits  The bits to unset
/// @return The value with the specified bits set
#define EFI_BITS_UNSET(Value, Bits) ((Value) & ~(Bits))
// EFI_BITS_ARE_SET
/// Check whether all specified bits are set
/// @param Value The value of which to check which bit is set
/// @param Bits  The bits to check are set
/// @return Whether the bits are set or not
#define EFI_BITS_ARE_SET(Value, Bits) (((Value) & (Bits)) == (Bits))
// EFI_BITS_ARE_UNSET
/// Check whether all bits are unset
/// @param Value The value of which to check which bit is unset
/// @param Bits  The bits to check are unset
/// @return Whether the bits are unset or not
#define EFI_BITS_ARE_UNSET(Value, Bits) (((Value) & (Bits)) == 0)
// EFI_BITS_ANY_SET
/// Check whether any bits are set
/// @param Value The value of which to check which bit is set
/// @param Bits  The bits to check are set
/// @return Whether the bits are set or not
#define EFI_BITS_ANY_SET(Value, Bits) (((Value) & (Bits)) != 0)
// EFI_BITS_ANY_UNSET
/// Check whether any bits are unset
/// @param Value The value of which to check which bit is unset
/// @param Bits  The bits to check are unset
/// @return Whether the bits are unset or not
#define EFI_BITS_ANY_UNSET(Value, Bits) (((Value) & (Bits)) != (Bits))
// EFI_BITMASK
/// Get a mask of bits
/// @param LowBit  The lowest significant bit in the mask
/// @param HighBit The highest significant bit in the mask
/// @return The bit mask with only bits from LowBit to HighBit set
#define EFI_BITMASK(LowBit, HighBit) ((EFI_BIT(((HighBit) - (LowBit)) + 1) - 1) << (LowBit))
// EFI_BITFIELD
/// Get a field value of bits
/// @param Field   The value from which to retrieve the field
/// @param LowBit  The lowest significant bit in the mask
/// @param HighBit The highest significant bit in the mask
/// @return The bit field with bits from LowBit to HighBit set shifted right by LowBit
#define EFI_BITFIELD(Field, LowBit, HighBit) (((Field) & EFI_BITMASK((LowBit), (HighBit))) >> (LowBit))
// EFI_BITMASKFIELD
/// Set the field value of bits
/// @param Value   The value where to set the field
/// @param Field   The value of the field to set
/// @param LowBit  The lowest significant bit in the mask
/// @param HighBit The highest significant bit in the mask
/// @return The bit mask with bits from LowBit to HighBit set to the specified value
#define EFI_BITMASKFIELD(Value, Field, LowBit, HighBit) (((Value) & ~EFI_BITMASK(LowBit, HighBit)) | (((Field) << (LowBit)) & EFI_BITMASK(LowBit, HighBit)))
// EFI_BITFIELDMASK
/// Get the field value of bits
/// @param Value   The value where to get the field mask
/// @param LowBit  The lowest significant bit in the mask
/// @param HighBit The highest significant bit in the mask
/// @return The bit mask with bits from LowBit to HighBit
#define EFI_BITFIELDMASK(Value, LowBit, HighBit) ((Value) & EFI_BITMASK(LowBit, HighBit))
// EFI_BITFIELDSHIFTRIGHT
/// Get a field value of bits
/// @param Field   The value from which to retrieve the field
/// @param Shift   The count of bits to shift the field right
/// @param LowBit  The lowest significant bit in the mask
/// @param HighBit The highest significant bit in the mask
/// @return The bit field with bits from LowBit to HighBit set shifted right by LowBit
#define EFI_BITFIELDSHIFTRIGHT(Field, Shift, LowBit, HighBit) (((Field) & EFI_BITMASK((LowBit), (HighBit))) >> (Shift))
// EFI_BITFIELDSHIFTLEFT
/// Get a field value of bits
/// @param Field   The value from which to retrieve the field
/// @param Shift   The count of bits to shift the field left
/// @param LowBit  The lowest significant bit in the mask
/// @param HighBit The highest significant bit in the mask
/// @return The bit field with bits from LowBit to HighBit set shifted right by LowBit
#define EFI_BITFIELDSHIFTLEFT(Field, Shift, LowBit, HighBit) (((Field) & EFI_BITMASK((LowBit), (HighBit))) << (Shift))

// EFI_PAGE_SIZE
/// The size in bytes of a memory page
#define EFI_PAGE_SIZE 0x1000
// EFI_PAGE_MASK
/// The bit mask for an page offset
#define EFI_PAGE_MASK 0xFFF
// EFI_PAGE_SHIFT
/// The bit shift to convert from pages to bytes and vice versa
#define EFI_PAGE_SHIFT 12

// EFI_SIZE_TO_PAGES
/// Convert size in bytes to count of pages
/// @param Size The size in bytes
/// @return The count of pages associated with the specified size in bytes
#define EFI_SIZE_TO_PAGES(Size)  ((UINTN)(((Size) >> EFI_PAGE_SHIFT) + (EFI_BITS_ANY_SET((Size), EFI_PAGE_MASK) ? 1 : 0)))
// EFI_PAGES_TO_SIZE
/// Convert count of pages to size in bytes
/// @param Pages The count of pages
/// @return The number of bytes in the specified count of pages
#define EFI_PAGES_TO_SIZE(Pages) ((Pages) << EFI_PAGE_SHIFT)

// EFI_TIME_ADJUST_DAYLIGHT
/// Indicates if the time is affected by daylight saving or not
#define EFI_TIME_ADJUST_DAYLIGHT 0x01
// EFI_TIME_IN_DAYLIGHT
/// Indicates the time has been adjusted for daylight savings time or not
#define EFI_TIME_IN_DAYLIGHT 0x02

// EFI_UNSPECIFIED_TIMEZONE
/// Timezone should be interpreted as local time
#define EFI_UNSPECIFIED_TIMEZONE 0x07FF

// EFI_ARCH_BITS
/// The bit count of a native width integer
#if defined(EFI_ARCH_X64) || defined(EFI_ARCH_AA64) || defined(EFI_ARCH_IA64)
# define EFI_ARCH_BITS 64
#else
# define EFI_ARCH_BITS 32
#endif

// MIN_INT8
/// The minimum value of a 1-byte signed integer
#define MIN_INT8 ((INT8)0x80)
// MIN_UINT8
/// The minimum value of a 1-byte unsigned integer
#define MIN_UINT8 ((UINT8)0)
// MIN_INT16
/// The minimum value of a 2-byte signed integer
#define MIN_INT16 ((INT16)0x8000)
// MIN_UINT16
/// The minimum value of a 2-byte unsigned integer
#define MIN_UINT16 ((UINT16)0)
// MIN_INT32
/// The minimum value of a 4-byte signed integer
#define MIN_INT32 ((INT32)0x80000000)
// MIN_UINT32
/// The minimum value of a 4-byte unsigned integer
#define MIN_UINT32 ((UINT32)0)
// MIN_INT64
/// The minimum value of an 8-byte signed integer
#define MIN_INT64 ((INT64)0x8000000000000000ULL)
// MIN_UINT64
/// The minimum value of an 8-byte unsigned integer
#define MIN_UINT64 ((UINT64)0ULL)
// MIN_INTN
/// The minimum value of an native width integer
#if EFI_ARCH_BITS == 64
# define MIN_INTN MIN_INT64
#else
# define MIN_INTN MIN_INT32
#endif
// MIN_UINTN
/// The minimum value of an native width unsigned integer
#if EFI_ARCH_BITS == 64
# define MIN_UINTN MIN_UINT64
#else
# define MIN_UINTN MIN_UINT32
#endif

// MAX_INT8
/// The maximum value of a 1-byte signed integer
#define MAX_INT8 ((INT8)0x7F)
// MAX_UINT8
/// The maximum value of a 1-byte unsigned integer
#define MAX_UINT8 ((UINT8)0xFF)
// MAX_INT16
/// The maximum value of a 2-byte signed integer
#define MAX_INT16 ((INT16)0x7FFF)
// MAX_UINT16
/// The maximum value of a 2-byte unsigned integer
#define MAX_UINT16 ((UINT16)0xFFFF)
// MAX_INT32
/// The maximum value of a 4-byte signed integer
#define MAX_INT32 ((INT32)0x7FFFFFFF)
// MAX_UINT32
/// The maximum value of a 4-byte unsigned integer
#define MAX_UINT32 ((UINT32)0xFFFFFFFF)
// MAX_INT64
/// The maximum value of an 8-byte signed integer
#define MAX_INT64 ((INT64)0x7FFFFFFFFFFFFFFFULL)
// MAX_UINT64
/// The maximum value of an 8-byte unsigned integer
#define MAX_UINT64 ((UINT64)0xFFFFFFFFFFFFFFFFULL)
// MAX_INTN
/// The maximum value of an native width integer
#if EFI_ARCH_BITS == 64
# define MAX_INTN MAX_INT64
#else
# define MAX_INTN MAX_INT32
#endif
// MAX_UINTN
/// The maximum value of an native width unsigned integer
#if EFI_ARCH_BITS == 64
# define MAX_UINTN MAX_UINT64
#else
# define MAX_UINTN MAX_UINT32
#endif

// INT8
/// 1-byte signed value
typedef signed char INT8;
// UINT8
/// 1-byte unsigned value
typedef unsigned char UINT8;
// INT16
/// 2-byte signed value
typedef signed short INT16;
// UINT16
/// 2-byte unsigned value
typedef unsigned short UINT16;
// INT32
/// 4-byte signed value
typedef signed int INT32;
// UINT32
/// 4-byte unsigned value
typedef unsigned int UINT32;
// INT64
/// 8-byte signed value
typedef signed long long INT64;
// UINT64
/// 8-byte unsigned value
typedef unsigned long long UINT64;
// IGNORE: INT128
/// 16-byte signed value
// typedef UINT64[2] INT128;
// IGNORE: UINT128
/// 16-byte unsigned value
// typedef UINT64[2] UINT128;
// INTN
/// Signed value of native width (4 bytes on 32-bit processor, 8 bytes on 64-bit processor, 16 bytes on 128-bit processor)
#if EFI_ARCH_BITS == 64
  typedef INT64 INTN;
#else
  typedef INT32 INTN;
#endif
// UINTN
/// Unsigned value of native width (4 bytes on 32-bit processor, 8 bytes on 64-bit processor, 16 bytes on 128-bit processor)
#if EFI_ARCH_BITS == 64
  typedef UINT64 UINTN;
#else
  typedef UINT32 UINTN;
#endif

// FLOAT32
/// 4-byte floating point value
typedef float FLOAT32;
// FLOAT64
/// 8-byte floating point value
typedef double FLOAT64;

// BOOLEAN
/// 1-byte logical boolean value containing a 0 for FALSE or a 1 for TRUE, any other values are undefined
typedef UINT8 BOOLEAN;

// CHAR8
/// 1-byte encoding character
typedef char CHAR8;
// CHAR16
/// 2-byte UTF-16 character
typedef UINT16 CHAR16;

// VOID
/// Undeclared type
typedef void VOID;
// EFI_HANDLE
/// A collection of related interfaces
typedef VOID *EFI_HANDLE;
// EFI_EVENT
/// Handle to an event structure
typedef VOID *EFI_EVENT;

// EFI_STATUS
/// Status code
typedef UINTN EFI_STATUS;
// EFI_TPL
/// Task priority level
typedef UINTN EFI_TPL;

// EFI_LBA
/// Logical block address
typedef UINT64 EFI_LBA;
// EFI_PHYSICAL_ADDRESS
/// Physical memory address
typedef UINT64 EFI_PHYSICAL_ADDRESS;
// EFI_VIRTUAL_ADDRESS
/// Virtual memory address
typedef UINT64 EFI_VIRTUAL_ADDRESS;

// EFI_GUID
/// 128-bit buffer containing a unique identifier value
typedef struct EFI_GUID EFI_GUID;
struct EFI_GUID {
  UINT32 Data1;
  UINT16 Data2;
  UINT16 Data3;
  UINT8  Data4[8];
};

// EFI_TIME
/// The current time information
typedef struct EFI_TIME EFI_TIME;
struct EFI_TIME {

  // Year
  /// The year
  UINT16 Year;
  // Month
  /// The month of the year (1 - 12)
  UINT8  Month;
  // Day
  /// The day of the month (1 - 31)
  UINT8  Day;
  // Hour
  /// The hour of the day (0 - 23)
  UINT8  Hour;
  // Minute
  /// The minute of the hour (0 - 59)
  UINT8  Minute;
  // Second
  /// The second of the minute (0 - 60)
  UINT8  Second;
  // Pad1
  /// Padding for 4-byte alignment
  UINT8  Pad1;
  // Nanosecond
  /// Nanosecond of the second (0 - 999,999,999)
  UINT32 Nanosecond;
  // TimeZone
  /// Time zone offset from UTC (-1440 to 1440)
  INT16  TimeZone;
  // Daylight
  /// Daylight saving time information
  UINT8  Daylight;
  // Pad2
  /// Padding for 4-byte alignment
  UINT8  Pad2;

};

// EFI_TABLE_HEADER
///Data structure that precedes all of the standard EFI table types
typedef struct EFI_TABLE_HEADER EFI_TABLE_HEADER;
struct EFI_TABLE_HEADER {

  // Signature
  /// A signature that identifies the type of table
  UINT64 Signature;
  // Revision
  /// The revision of the EFI Specification to which this table conforms
  UINT32 Revision;
  // HeaderSize
  /// The size, in bytes, of the entire table including the EFI_TABLE_HEADER
  UINT32 HeaderSize;
  // CRC32
  /// The 32-bit CRC for the entire table
  UINT32 CRC32;
  // Reserved
  /// Reserved
  UINT32 Reserved;

};

#if defined(__GNUC__)

// VA_LIST
/// 
typedef UINT8 *VA_LIST;
// VA_START
/// 
/// @param Marker    
/// @param Parameter 
# define VA_START(Marker, Parameter) __builtin_va_start(Marker, Parameter)
// VA_ARG
/// 
/// @param Marker 
/// @param TYPE   
/// @return 
# define VA_ARG(Marker, TYPE) __builting_va_arg(Marker, TYPE)
// VA_END
/// 
/// @param Marker 
# define VA_END(Marker) __builtin_va_end(Marker)
// VA_COPY
/// 
/// @param Dest  
/// @param Start 
# define VA_COPY(Dest, Start) __builtin_va_copy(Dest, Start)

#elif defined(_MSC_VER)

// VA_ARG_SIZE
/// 
/// @param TYPE 
/// @return 
# define VA_ARG_SIZE(TYPE) ((sizeof(TYPE) + sizeof(UINTN) - 1) & ~(sizeof(UINTN) - 1))
// VA_ADVANCE
/// 
/// @param Marker 
/// @param Size   
/// @return 
# define VA_ADVANCE(Marker, Size) ((Marker += Size) - Size)

// VA_LIST
/// 
typedef UINT8 *VA_LIST;
// VA_START
/// 
/// @param Marker    
/// @param Parameter 
# define VA_START(Marker, Parameter) (Marker = (VA_LIST)(((UINTN)&(Parameter)) + VA_ARG_SIZE(Parameter)))
// VA_ADDRESS
/// 
/// @param Marker 
/// @param TYPE   
/// @return 
#  define VA_ADDRESS(Marker, TYPE) ((TYPE *)(VA_ADVANCE(Marker, VA_ARG_SIZE(TYPE))))
// VA_ARG
/// 
/// @param Marker 
/// @param TYPE   
/// @return 
#  define VA_ARG(Marker, TYPE) (*(VA_ADDRESS(Marker, TYPE)))

// VA_END
/// 
/// @param Marker 
# define VA_END(Marker) (Marker = (VA_LIST)0)
// VA_COPY
/// 
/// @param Dest  
/// @param Start 
# define VA_COPY(Dest, Start) ((VOID)((Dest) = (Start)))

#else

# error Could not define VA_LIST

#endif

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_BASE_HEADER__
