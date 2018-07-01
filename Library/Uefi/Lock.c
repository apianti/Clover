///
/// @file Library/Uefi/Lock.c
///
/// UEFI locking mechanism
///

#include <Uefi.h>

// EfiLockInitialize
/// Initialize a lock for use
/// @param Lock The lock to initialize
BOOLEAN
EFIAPI
EfiLockInitialize (
  IN OUT EFI_LOCK *Lock
) {
  if (Lock == NULL) {
    return FALSE;
  }
  // Initialize the lock to be released
  EfiStore32(Lock, EFI_LOCK_RELEASED);
  return TRUE;
}
// EfiLock
/// Wait for the lock to be locked
/// @param Lock The lock to lock
BOOLEAN
EFIAPI
EfiLock (
  IN OUT EFI_LOCK *Lock
) {
  if (Lock == NULL) {
    return FALSE;
  }
  // Compare and exchange with release to acquire the lock
  while (EfiCompareAndExchange32(Lock, EFI_LOCK_RELEASED, EFI_LOCK_ACQUIRED) == EFI_LOCK_RELEASED) {
    // Pause and try again
    EfiCpuPause();
  }
  return TRUE;
}
// EfiTryLock
/// Try to lock the lock
/// @param Lock The lock to try to lock
/// @return Whether the lock was locked or not
BOOLEAN
EFIAPI
EfiTryLock (
  IN OUT EFI_LOCK *Lock
) {
  if (Lock == NULL) {
    return FALSE;
  }
  // Compare and exchange with release to acquire the lock
  if (EfiCompareAndExchange32(Lock, EFI_LOCK_RELEASED, EFI_LOCK_ACQUIRED) == EFI_LOCK_RELEASED) {
    return FALSE;
  }
  return TRUE;
}
// EfiUnlock
/// Unlock a lock
/// @param Lock The lock to unlock
BOOLEAN
EFIAPI
EfiUnlock (
  IN OUT EFI_LOCK *Lock
) {
  if (Lock == NULL) {
    return FALSE;
  }
  // Release the lock
  EfiStore32(Lock, EFI_LOCK_RELEASED);
  return TRUE;
}
