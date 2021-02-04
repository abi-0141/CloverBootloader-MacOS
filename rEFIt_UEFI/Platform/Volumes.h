/*
 * Volumes.h
 *
 *  Created on: Feb 4, 2021
 *      Author: jief
 */

#ifndef PLATFORM_VOLUMES_H_
#define PLATFORM_VOLUMES_H_

#include "../include/Efi.h"
#include "../cpp_foundation/XString.h"
#include "../libeg/libeg.h"

class REFIT_VOLUME {
public:
  EFI_DEVICE_PATH     *DevicePath;
  EFI_HANDLE          DeviceHandle;
  EFI_FILE            *RootDir;
  XStringW            DevicePathString;
  XStringW            VolName; // comes from EfiLibFileSystemInfo, EfiLibFileSystemVolumeLabelInfo, "EFI" if gEfiPartTypeSystemPartGuid or "Unknown HD"
  XStringW            VolLabel; // comes from \\.VolumeLabel.txt, or empty.
  UINT8               DiskKind;
  LEGACY_OS           *LegacyOS;
  BOOLEAN             Hidden;
  UINT8               BootType;
  BOOLEAN             IsAppleLegacy;
  BOOLEAN             HasBootCode;
  BOOLEAN             IsMbrPartition;
  UINTN               MbrPartitionIndex;
  EFI_BLOCK_IO        *BlockIO;
  UINT64              BlockIOOffset;
  EFI_BLOCK_IO        *WholeDiskBlockIO;
  EFI_DEVICE_PATH     *WholeDiskDevicePath;
  EFI_HANDLE          WholeDiskDeviceHandle;
  MBR_PARTITION_INFO  *MbrPartitionTable;
  UINT32              DriveCRC32;
  EFI_GUID            RootUUID; //for recovery it is UUID of parent partition
  UINT64              SleepImageOffset;
  XStringW            osxVolumeName = NullXStringW; // comes from \\System\\Library\\CoreServices\\.disk_label.contentDetails, or empty.
  XString8            ApfsFileSystemUUID; // apfs file system UUID of that partition. It's not the UUID of subfolder like in Preboot.
  XString8            ApfsContainerUUID = NullXString8;
  APPLE_APFS_VOLUME_ROLE  ApfsRole = 0;
  XString8Array        ApfsTargetUUIDArray; // this is the array of folders that are named as UUID

  REFIT_VOLUME() : DevicePath(0), DeviceHandle(0), RootDir(0), DevicePathString(), VolName(), VolLabel(), DiskKind(0), LegacyOS(0), Hidden(0), BootType(0), IsAppleLegacy(0), HasBootCode(0),
                   IsMbrPartition(0), MbrPartitionIndex(0), BlockIO(0), BlockIOOffset(0), WholeDiskBlockIO(0), WholeDiskDevicePath(0), WholeDiskDeviceHandle(0),
                   MbrPartitionTable(0), DriveCRC32(0), RootUUID({0,0,0,{0,0,0,0,0,0,0,0}}), SleepImageOffset(0), ApfsFileSystemUUID(), ApfsTargetUUIDArray()
                 {}
  REFIT_VOLUME(const REFIT_VOLUME& other) = delete; // Can be defined if needed
  const REFIT_VOLUME& operator = ( const REFIT_VOLUME & ) = delete; // Can be defined if needed
  ~REFIT_VOLUME() {}

  const XStringW getVolLabelOrOSXVolumeNameOrVolName() {
    if ( VolLabel.notEmpty() ) return VolLabel;
    if ( osxVolumeName.notEmpty() ) return osxVolumeName;
    return VolName;
  }
};



class VolumesArrayClass : public XObjArray<REFIT_VOLUME>
{
  public:
//    REFIT_VOLUME* getApfsPartitionWithUUID(const XString8& ApfsContainerUUID, const XString8& APFSTargetUUID);
    REFIT_VOLUME* getVolumeWithApfsContainerUUIDAndFileSystemUUID(const XString8& ApfsContainerUUID, const XString8& ApfsFileSystemUUID);
    /*
     * Return : NULL if not found OR more than one partition with this role is found in this container
     */
    REFIT_VOLUME* getVolumeWithApfsContainerUUIDAndRole(const XString8& ApfsContainerUUID, APPLE_APFS_VOLUME_ROLE roleMask);

};

extern VolumesArrayClass Volumes;
extern REFIT_VOLUME     *SelfVolume;




#endif /* PLATFORM_VOLUMES_H_ */
