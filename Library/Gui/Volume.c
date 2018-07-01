///
/// @file Library/Gui/Volume.c
///
/// File system volume
///

#include <Gui/Client.h>

//namespace Client {
//
//// ~Volume
///// Volume destructor
//Volume::~Volume (
//  VOID
//) {
//  mDeviceHandle = NULL;
//  mFileSystem = NULL;
//  if (mVolumeLabel != NULL) {
//    Boot::Memory::Free(mVolumeLabel);
//    mVolumeLabel = NULL;
//  }
//  if (mRootDirectory != NULL) {
//    mRootDirectory->Release();
//    mRootDirectory = NULL;
//  }
//}
//
//// Volume
///// Volume constructor
///// @param DeviceHandle The handle of the device that implements the simple file system protocol
///// @param FileSystem   The root directory of the file system
///// @param VolumeLabel  The volume label
//Volume::Volume (
//  EFI_HANDLE         DeviceHandle,
//  EFI_FILE_PROTOCOL *FileSystem,
//  CONST CHAR16      *VolumeLabel
//) {
//  mDeviceHandle = DeviceHandle;
//  mFileSystem = FileSystem;
//  mVolumeLabel = String::Duplicate(VolumeLabel, __FILE__, __LINE__);
//  mDevicePath = EfiDevicePathFullTextFromHandle(DeviceHandle);
//  // Create root directory
//  mRootDirectory = new Directory(mFileSystem);
//  if (mRootDirectory == NULL) {
//    if (mFileSystem != NULL) {  
//      mFileSystem->Close(mFileSystem);
//      mFileSystem = NULL;
//    }
//  }
//  // TODO: Determine volume characteristics
//}
//
//// Volume::OpenRoot
///// Open the root directory
///// @return The root directory which must be released
//Directory *
//Volume::OpenRoot (
//  VOID
//) CONST {
//  if ((mRootDirectory == NULL) || (mRootDirectory->Reference() == 0)) {
//    return NULL;
//  }
//  return mRootDirectory;
//}
//// Volume::OpenDirectory
///// Open a directory from the volume file system
///// @param Path             The path of the directory to open
///// @param CreateIfNotFound Whether to create the directory if it does not exist already
///// @param Recursive        Whether to recursively create any parent directories if needed
///// @return The opened directory which must be released or NULL if the directory was not found or there was an error
//Directory    *
//Volume::OpenDirectory (
//  CONST CHAR16 *Path,
//  BOOLEAN       CreateIfNotFound,
//  BOOLEAN       Recursive
//) CONST {
//  Directory         *Dir = NULL;
//  // Open directory path
//  EFI_FILE_PROTOCOL *Ptr = NULL;
//  if (!EFI_ERROR(EfiFileOpen(mFileSystem, &Ptr, (CHAR16 *)Path, EFI_FILE_MODE_READ, 0)) && (Ptr != NULL)) {
//    // Get the file information
//    EFI_FILE_INFO *FileInfo = EfiFileInfo(Ptr);
//    if (FileInfo != NULL) {
//      // Check file is directory
//      if ((FileInfo->Attribute & EFI_FILE_DIRECTORY) != 0) {
//        // Create new directory instance from file interface
//        Dir = new Directory(Ptr);
//      }
//      // Free file information
//      EfiFreePool(FileInfo);
//    }
//    // Close the opened file if not a directory or not found
//    if (Dir == NULL) {
//      Ptr->Close(Ptr);
//    }
//  } else if (CreateIfNotFound) {
//    // TODO: Create the directory
//    if (Recursive) {
//    }
//  }
//  return Dir;
//}
//// Volume::Open
///// Open a file from the volume file system
///// @param Path             The path of the file to open
///// @param ReadOnly         Whether to open the file read-only or read-write
///// @param CreateIfNotFound Whether to create a read-write file if it does not exist already
///// @return The opened file which must be released or NULL if the file was not found or there was an error
//File *
//Volume::Open (
//  CONST CHAR16 *Path,
//  BOOLEAN       ReadOnly,
//  BOOLEAN       CreateIfNotFound
//) CONST {
//  File              *F = NULL;
//  EFI_FILE_PROTOCOL *Ptr = NULL;
//  // Set correct mode and attributes
//  UINTN              Mode = EFI_FILE_MODE_READ;
//  UINTN              Attributes = 0;
//  if (ReadOnly) {
//    Attributes |= EFI_FILE_READ_ONLY;
//  } else {
//    Mode |= EFI_FILE_MODE_WRITE;
//  }
//  if (CreateIfNotFound) {
//    Mode |= EFI_FILE_MODE_CREATE;
//  }
//  // Open directory path
//  if (!EFI_ERROR(EfiFileOpen(mFileSystem, &Ptr, (CHAR16 *)Path, Mode, Attributes)) && (Ptr != NULL)) {
//    // Get the file information
//    EFI_FILE_INFO *FileInfo = EfiFileInfo(Ptr);
//    if (FileInfo != NULL) {
//      // Check file is not directory
//      if ((FileInfo->Attribute & EFI_FILE_DIRECTORY) == 0) {
//        // Create new directory instance from file interface
//        F = new File(Ptr);
//      }
//      // Free file information
//      EfiFreePool(FileInfo);
//    }
//    // Close the opened file if not a directory or not found
//    if (F == NULL) {
//      Ptr->Close(Ptr);
//    }
//  }
//  return F;
//}
//// Volume::Exists
///// Check a file or directory exists on the volume file system
///// @param Path The path of the file or directory to check exists
///// @return Whether the file or directory exists
//BOOLEAN
//Volume::Exists (
//  CONST CHAR16 *Path
//) CONST {
//  EFI_FILE_PROTOCOL *Ptr = NULL;
//  BOOLEAN            Exists = FALSE;
//  // Open file path
//  if (!EFI_ERROR(EfiFileOpen(mFileSystem, &Ptr, (CHAR16 *)Path, EFI_FILE_MODE_READ, 0)) && (Ptr != NULL)) {
//    // File exists
//    Exists = TRUE;
//    // Close the opened file
//    Ptr->Close(Ptr);
//  }
//  return Exists;
//}
//// IsDirectory
///// Check whether a path is a directory on the volume file system
///// @param Path The path to check is a directory
///// @return Whether the path exists and is a directory
//BOOLEAN
//Volume::IsDirectory (
//  CONST CHAR16 *Path
//) CONST {
//  EFI_FILE_PROTOCOL *Ptr = NULL;
//  BOOLEAN            IsDirectory = FALSE;
//  // Open directory path
//  if (!EFI_ERROR(EfiFileOpen(mFileSystem, &Ptr, (CHAR16 *)Path, EFI_FILE_MODE_READ, 0)) && (Ptr != NULL)) {
//    // Get the file information
//    EFI_FILE_INFO *FileInfo = EfiFileInfo(Ptr);
//    if (FileInfo != NULL) {
//      // Check file is directory
//      IsDirectory = ((FileInfo->Attribute & EFI_FILE_DIRECTORY) != 0);
//      // Free file information
//      EfiFreePool(FileInfo);
//    }
//    // Close the opened file
//    Ptr->Close(Ptr);
//  }
//  // Return whether the path is a directory
//  return IsDirectory;
//}
//
//// Volume::DeviceHandle
///// Get the volume device handle
//EFI_HANDLE
//Volume::DeviceHandle (
//  VOID
//) CONST {
//  return mDeviceHandle;
//}
//// Volume::DevicePath
///// Get the volume device path text
///// @return The volume device path text
//CONST CHAR16 *
//Volume::DevicePath (
//  VOID
//) CONST {
//  return mDevicePath;
//}
//// Volume::Label
///// Get the volume label
///// @return The volume label
//CONST CHAR16 *
//Volume::Label (
//  VOID
//) CONST {
//  return mVolumeLabel;
//}
//
//// VolumeLabelDuplicate
///// Duplicate a volume label if it is valid
///// @param VolumeLabel The volume label to duplicate
///// @return The duplicated volume label or NULL if it was not valid
//STATIC
//CHAR16 *
//VolumeLabelDuplicate (
//  CONST CHAR16 *VolumeLabel
//) {
//  if (VolumeLabel == NULL) {
//    return NULL;
//  }
//  while ((*VolumeLabel == '\\') || (*VolumeLabel == '/')) {
//    ++VolumeLabel;
//  }
//  if (*VolumeLabel == 0) {
//    return NULL;
//  }
//  return String::Duplicate(VolumeLabel, __FILE__, __LINE__);
//}
//
//// Enumerate
///// Enumerate all file system volumes
///// @return The list of file system volumes
//ObjectList<CONST Volume>
//Volume::Enumerate (
//  VOID
//) {
//  ObjectList<CONST Volume> List;
//  // Get list of simple file system handles
//  EFI_HANDLE *Handles = NULL;
//  UINTN       Count = 0;
//  if (!EFI_ERROR(EfiLocateHandleBuffer(ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL, &Count, &Handles)) && (Handles != NULL)) {
//    // Iterate through list of handles
//    for (UINTN Index = 0; Index < Count; ++Index) {
//      if (Handles[Index] != NULL) {
//        // Get the simple file system protocol interface from each handle
//        EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem = NULL;
//        if (!EFI_ERROR(EfiHandleProtocol(Handles[Index], &gEfiSimpleFileSystemProtocolGuid, (VOID **)&FileSystem)) && (FileSystem != NULL)) {
//          // Open the root directory of the file system
//          EFI_FILE_PROTOCOL *Root = NULL;
//          if (!EFI_ERROR(EfiFileOpenRoot(FileSystem, &Root)) && (Root != NULL)) {
//            CHAR16 *VolumeLabel = NULL;
//            // Determine volume label from file system information
//            EFI_FILE_SYSTEM_INFO *FileSystemInfo = EfiFileSystemInfo(Root);
//            if (FileSystemInfo != NULL) {
//              // Get the volume label and make sure it's valid
//              VolumeLabel = VolumeLabelDuplicate(FileSystemInfo->VolumeLabel);
//              EfiFreePool(FileSystemInfo);
//            }
//            if (VolumeLabel == NULL) {
//              // Determine volume label from file system volume label information
//              EFI_FILE_SYSTEM_VOLUME_LABEL_INFO *FileSystemVolumeLabelInfo = EfiFileSystemVolumeLabelInfo(Root);
//              if (FileSystemVolumeLabelInfo != NULL) {
//                // Get the volume label and make sure it's valid
//                VolumeLabel = VolumeLabelDuplicate(FileSystemVolumeLabelInfo->VolumeLabel);
//                EfiFreePool(FileSystemVolumeLabelInfo);
//              }
//              if (VolumeLabel == NULL) {
//                // Determine volume label from file information
//                EFI_FILE_INFO *FileInfo = EfiFileInfo(Root);
//                if (FileInfo != NULL) {
//                  // Get the volume label and make sure it's valid
//                  VolumeLabel = VolumeLabelDuplicate(FileInfo->FileName);
//                  EfiFreePool(FileInfo);
//                }
//                if (VolumeLabel == NULL) {
//                  // Check if EFI system partition
//                  VOID *Instance = NULL;
//                  if (!EFI_ERROR(EfiHandleProtocol(Handles[Index], &gEfiPartTypeSystemPartGuid, &Instance))) {
//                    // Set volume label to EFI for EFI system partition
//                    VolumeLabel = StrDup(L"EFI");
//                  }
//                  if (VolumeLabel == NULL) {
//                    // Use device path as volume label as fallback
//                    VolumeLabel = EfiDevicePathFullTextFromHandle(Handles[Index]);
//                  }
//                }
//              }
//            }
//            // Create new file system volume instance
//            Volume *FSVolume = new Volume(Handles[Index], Root, VolumeLabel);
//            // Free the volume label
//            if (VolumeLabel != NULL) {
//              EfiFreePool(VolumeLabel);
//            }
//            // Add file system volume to list
//            if (FSVolume != NULL) {
//              List.Add(FSVolume);
//              FSVolume->Release();
//            } else {
//              // Close root directory interface
//              EfiFileClose(Root);
//            }
//          }
//        }
//      }
//    }
//    // Free list of handles
//    EfiFreePool(Handles);
//  }
//  return List;
//}
//
//};
