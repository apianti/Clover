///
/// @file Library/Uefi/Queue.c
///
/// UEFI queue
///

#include <Uefi.h>

// EfiQueueInitialize
/// Initialize a queue
/// @param Queue The queue to initialize
VOID
EFIAPI
EfiQueueInitialize (
  IN OUT EFI_QUEUE *Queue
) {
  if (Queue != NULL) {
    EfiLockInitialize(&(Queue->Lock));
    Queue->Items = NULL;
  }
}
// EfiQueueAllocate
/// Allocate and initialize a queue
/// @return The allocated and initialized queue which needs freed
EFI_QUEUE *
EFIAPI
EfiQueueAllocate (
  VOID
) {
  EFI_QUEUE *Queue = EfiAllocateByType(EFI_QUEUE);
  if (Queue != NULL) {
    EfiQueueInitialize(Queue);
  }
  return Queue;
}
// EfiEnqueue
/// Append an item to the end of a queue
/// @param Queue The queue to enqueue
/// @param Item  The item enqueue at the end of the queue
/// @retval EFI_INVALID_PARAMETER If Queue or Item is NULL
/// @retval EFI_OUT_OF_RESOURCES  There was not enough memory to enqueue the item
/// @retval EFI_ACCESS_DENIED     The queue lock could not be acquired
/// @retval EFI_SUCCESS           The item at the beginning of the queue was returned successfully
EFI_STATUS
EFIAPI
EfiEnqueue (
  IN OUT EFI_QUEUE *Queue,
  IN     VOID      *Item
) {
  EFI_QUEUE_ITEM *QueueItem;
  // Check parameters
  if ((Queue == NULL) || (Item == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Create new item for the queue
  QueueItem = EfiAllocateByType(EFI_QUEUE_ITEM);
  if (QueueItem == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Lock the queue
  EfiLock(&(Queue->Lock));
  // Set the item value
  QueueItem->Item = Item;
  QueueItem->Next = NULL;
  // Add the item to the queue
  if (Queue->Items == NULL) {
    Queue->Items = QueueItem;
  } else {
    EFI_QUEUE_ITEM *Ptr = Queue->Items;
    while (Ptr->Next != NULL) {
      Ptr = Ptr->Next;
    }
    Ptr->Next = QueueItem;
  }
  // Unlock the queue
  EfiUnlock(&(Queue->Lock));
  return EFI_SUCCESS;
}
// EfiDequeue
/// Remove an item from the queue beginning of a queue
/// @param Queue The queue to dequeue
/// @param Item  On output, the item dequeue from beginning of the queue which the caller now owns and may need to free
/// @retval EFI_INVALID_PARAMETER If Queue or Item is NULL
/// @retval EFI_ACCESS_DENIED     The queue lock could not be acquired
/// @retval EFI_NOT_FOUND         There were no items to dequeue
/// @retval EFI_SUCCESS           The item at the beginning of the queue was returned successfully
EFI_STATUS
EFIAPI
EfiDequeue (
  IN OUT EFI_QUEUE  *Queue,
  OUT    VOID      **Item
) {
  EFI_STATUS      Status;
  EFI_QUEUE_ITEM *QueueItem;
  // Check parameters
  if ((Queue == NULL) || (Item == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Lock the queue
  EfiLock(&(Queue->Lock));
  // Remove the item to the queue
  QueueItem = Queue->Items;
  if (QueueItem == NULL) {
    // Queue empty underflow
    Status = EFI_NOT_FOUND;
  } else {
    // Remove the item from the queue and return the value
    Queue->Items = QueueItem->Next;
    *Item = QueueItem->Item;
    EfiFreePool(QueueItem);
    Status = EFI_SUCCESS;
  }
  // Unlock the queue
  EfiUnlock(&(Queue->Lock));
  return Status;
}
