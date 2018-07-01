///
/// @file Include/Uefi/Queue.h
///
/// GUI queue
///

#pragma once
#ifndef __EFI_QUEUE_HEADER__
#define __EFI_QUEUE_HEADER__

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#include <Uefi.h>

// EFI_QUEUE_ITEM
/// A queue item
typedef struct EFI_QUEUE_ITEM EFI_QUEUE_ITEM;
struct EFI_QUEUE_ITEM {

  // Next
  /// The next queue item
  EFI_QUEUE_ITEM *Next;
  // Item
  /// The queue item
  VOID           *Item;

};
// EFI_QUEUE
/// An atomic queue
typedef struct EFI_QUEUE EFI_QUEUE;
struct EFI_QUEUE {

  // Lock
  /// The queue lock
  EFI_LOCK        Lock;
  // Items
  /// The queue items
  EFI_QUEUE_ITEM *Items;

};

// EfiQueueInitialize
/// Initialize a queue
/// @param Queue The queue to initialize
EXTERN
VOID
EFIAPI
EfiQueueInitialize (
  IN OUT EFI_QUEUE *Queue
);
// EfiQueueAllocate
/// Allocate and initialize a queue
/// @return The allocated and initialized queue which needs freed
EXTERN
EFI_QUEUE *
EFIAPI
EfiQueueAllocate (
  VOID
);
// EfiEnqueue
/// Append an item to the end of a queue
/// @param Queue The queue to enqueue
/// @param Item  The item enqueue at the end of the queue
/// @retval EFI_INVALID_PARAMETER If Queue or Item is NULL
/// @retval EFI_OUT_OF_RESOURCES  There was not enough memory to enqueue the item
/// @retval EFI_ACCESS_DENIED     The queue lock could not be acquired
/// @retval EFI_SUCCESS           The item at the beginning of the queue was returned successfully
EXTERN
EFI_STATUS
EFIAPI
EfiEnqueue (
  IN OUT EFI_QUEUE *Queue,
  IN     VOID      *Item
);
// EfiDequeue
/// Remove an item from the queue beginning of a queue
/// @param Queue The queue to dequeue
/// @param Item  On output, the item dequeue from beginning of the queue which the caller now owns and may need to free
/// @retval EFI_INVALID_PARAMETER If Queue or Item is NULL
/// @retval EFI_ACCESS_DENIED     The queue lock could not be acquired
/// @retval EFI_NOT_FOUND         There were no items to dequeue
/// @retval EFI_SUCCESS           The item at the beginning of the queue was returned successfully
EXTERN
EFI_STATUS
EFIAPI
EfiDequeue (
  IN  EFI_QUEUE  *Queue,
  OUT VOID      **Item
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __EFI_QUEUE_HEADER__
