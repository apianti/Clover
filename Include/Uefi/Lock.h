///
/// @file Include/Uefi/Runtime.h
///
/// UEFI locking mechanism
///

#pragma once
#ifndef __UEFI_LOCK_HEADER__
#define __UEFI_LOCK_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

  // EFI_LOCK_RELEASED
  /// Lock released
#define EFI_LOCK_RELEASED 0ULL
  // EFI_LOCK_ACQUIRED
  // Lock acquired
#define EFI_LOCK_ACQUIRED 1ULL


// EFI_LOCK
/// A locking mechanism
typedef UINT32 EFI_LOCK;

// EfiLockInitialize
/// Initialize a lock for use
/// @param Lock The lock to initialize
/// @return Whether the lock was valid and initialized or not
EXTERN
BOOLEAN
EFIAPI
EfiLockInitialize (
  IN OUT EFI_LOCK *Lock
);
// EfiLock
/// Wait for the lock to be locked
/// @param Lock The lock to lock
/// @return Whether the lock was valid and locked or not
EXTERN
BOOLEAN
EFIAPI
EfiLock (
  IN OUT EFI_LOCK *Lock
);
// EfiTryLock
/// Try to lock the lock without waiting
/// @param Lock The lock to try to lock
/// @return Whether the lock was valid and locked or not
EXTERN
BOOLEAN
EFIAPI
EfiTryLock (
  IN OUT EFI_LOCK *Lock
);
// EfiUnlock
/// Unlock a lock
/// @param Lock The lock to unlock
/// @return Whether the lock was valid and unlocked or not
EXTERN
BOOLEAN
EFIAPI
EfiUnlock (
  IN OUT EFI_LOCK *Lock
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __UEFI_LOCK_HEADER__
