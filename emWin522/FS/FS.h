/*********************************************************************
*                SEGGER MICROCONTROLLER GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 2003-2013     SEGGER Microcontroller GmbH & Co KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : FS.h
Purpose     : Define global functions and types to be used by an
              application using the file system.

              This file needs to be included by any module using the
              file system.
---------------------------END-OF-HEADER------------------------------
*/

#ifndef __FS_H__               // Avoid recursive and multiple inclusion
#define __FS_H__

/*********************************************************************
*
*             #include Section
*
**********************************************************************
*/

#include "FS_ConfDefaults.h"        /* FS Configuration */
#include "FS_Types.h"
#include "FS_Storage.h"
#include "FS_Dev.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*             #define constants
*
**********************************************************************
*/

/*********************************************************************
*
*       File system version
*/
#define FS_VERSION      33202UL

/*********************************************************************
*
*       Error codes passed to FS_X_Panic()
*/
#define FS_ERROR_ALLOC                  1
#define FS_ERROR_INVALID_PARA           2
#define FS_ERROR_ILLEGAL_FORMAT_STRING  3
#define FS_ERROR_UNKNOWN_DEVICE         4

/*********************************************************************
*
*       Error codes returned by FS_FError()
*/
#define FS_ERR_OK                    0
#define FS_ERR_EOF                  -1
#define FS_ERR_DISKFULL             -2
#define FS_ERR_INVALIDPAR           -3
#define FS_ERR_CMDNOTSUPPORTED      -4    // Deprecated
#define FS_ERR_WRITEONLY            -5
#define FS_ERR_READONLY             -6
#define FS_ERR_READERROR            -7
#define FS_ERR_WRITEERROR           -8
#define FS_ERR_FILE_ALREADY_OPENED  -9    // Deprecated

/*********************************************************************
*
*       File positioning defines
*/
#define FS_SEEK_CUR               1
#define FS_SEEK_END               2
#define FS_SEEK_SET               0

#define FS_FILE_CURRENT           FS_SEEK_CUR
#define FS_FILE_END               FS_SEEK_END
#define FS_FILE_BEGIN             FS_SEEK_SET

/*********************************************************************
*
*       I/O commands for driver (internal)
*/
#define FS_CMD_REQUIRES_FORMAT        1003
#define FS_CMD_GET_DEVINFO            1004
#define FS_CMD_FORMAT_LOW_LEVEL       1005            // Used internally by FS_FormatLow() to command the driver to perform low-level format
#define FS_CMD_FREE_SECTORS           1006            // Used internally: Allows the FS layer to inform driver about free sectors
#define FS_CMD_SET_DELAY              1007            // Used in the simulation to simulate a slow device with RAM driver

#define FS_CMD_UNMOUNT                1008            // Used internally by FS_STORAGE_Unmount() to inform the driver. Driver invalidates caches and all other information about medium.
#define FS_CMD_UNMOUNT_FORCED         1009            // Used internally by FS_STORAGE_UnmountForced() to inform the driver about an unforced remove of the device.
                                                      // Driver invalidates caches and all other information about medium.
#define FS_CMD_SYNC                   1010            // Tells the driver to clean caches. Typically, all dirty sectors are written.
#define FS_CMD_UNMOUNT_VOLUME         FS_CMD_UNMOUNT  // Obsolete: FS_CMD_UNMOUNT shall be used instead of FS_CMD_UNMOUNT_VOLUME.

#define FS_CMD_DEINIT                 1011            // Frees the resouces allocated by the file system.
#define FS_CMD_CLEAN_ONE              1012            // Tells the driver to perform one "clean" operation. Usually this operation converts a work block into a data block.
                                                      // Aux                    Not used
                                                      // pBuffer  [*int, OUT]   ==0 A second command will do nothing
                                                      //                        ==1 A second command will perform a "clean" operation
#define FS_CMD_CLEAN                  1013            // Executes the "clean" operation until there is nothing left to do.
#define FS_CMD_GET_SECTOR_USAGE       1014            // Queries the driver about the usage of a logical sector.
                                                      // Aux      [int,  IN]    Index of the sector to be queried
                                                      // pBuffer  [*int, OUT]   ==0 Sector in use
                                                      //                        ==1 Sector not used
                                                      //                        ==2 Unknown
                                                      //
#define FS_CMD_ENABLE_READ_AHEAD      1015            // Tells the driver that the following read operations should read sectors in advance
#define FS_CMD_DISABLE_READ_AHEAD     1016            // Tells the driver to stop reading sectors in advance

/*********************************************************************
*
*       CACHE Commands (internal)
*/
#define FS_CMD_CACHE_SET_MODE         6000L
#define FS_CMD_CACHE_CLEAN            6001L   // Write out all dirty sectors
#define FS_CMD_CACHE_SET_QUOTA        6002L
#define FS_CMD_CACHE_FREE_SECTORS     6003L
#define FS_CMD_CACHE_INVALIDATE       6004L   // Invalidate all sectors in cache
#define FS_CMD_CACHE_SET_ASSOC_LEVEL  6005L   // Sets the associativity level for the mult-way cache
#define FS_CMD_CACHE_GET_NUM_SECTORS  6006L   // Returns the number of sectors the cache is able to store
#define FS_CMD_CACHE_GET_TYPE         6007L   // Returns the type of the cache configured

/*********************************************************************
*
*       Directory entry attribute definitions
*/
#define FS_ATTR_READ_ONLY     0x01
#define FS_ATTR_HIDDEN        0x02
#define FS_ATTR_SYSTEM        0x04
#define FS_ATTR_ARCHIVE       0x20
#define FS_ATTR_DIRECTORY     0x10

/*********************************************************************
*
*       Directory entry attribute definitions
*/
#define FS_FILETIME_CREATE    0
#define FS_FILETIME_ACCESS    1
#define FS_FILETIME_MODIFY    2

/*********************************************************************
*
*       Volume mount flags
*/
#define FS_MOUNT_R            1
#define FS_MOUNT_RW           3

/*********************************************************************
*
*       File system selection
*/
#define FS_FAT                0
#define FS_EFS                1

/*********************************************************************
*
*       CheckDisk error codes
*/
enum {
  FS_ERRCODE_0FILE = 0x10,
  FS_ERRCODE_SHORTEN_CLUSTER,
  FS_ERRCODE_CROSSLINKED_CLUSTER,
  FS_ERRCODE_FEW_CLUSTER,
  FS_ERRCODE_CLUSTER_UNUSED,
  FS_ERRCODE_CLUSTER_NOT_EOC,
  FS_ERRCODE_INVALID_CLUSTER,
  FS_ERRCODE_INVALID_DIRECTORY_ENTRY
};

/*********************************************************************
*
*       FS_MTYPE_...
*
*  Ids to distinguish different message types
*/
#define FS_MTYPE_INIT         (1 << 0)
#define FS_MTYPE_API          (1 << 1)
#define FS_MTYPE_FS           (1 << 2)
#define FS_MTYPE_STORAGE      (1 << 3)
#define FS_MTYPE_JOURNAL      (1 << 4)
#define FS_MTYPE_CACHE        (1 << 5)
#define FS_MTYPE_DRIVER       (1 << 6)
#define FS_MTYPE_OS           (1 << 7)
#define FS_MTYPE_MEM          (1 << 8)

/*********************************************************************
*
*       Global data types
*
**********************************************************************
*/

/*********************************************************************
*
*       Callback functions
*
*/
typedef int    FS_ON_CHECK_DISK_ERROR_CALLBACK (int ErrCode, ...);
typedef void   FS_BUSY_LED_CALLBACK            (U8 OnOff);
typedef int    FS_MEMORY_IS_ACCESSIBLE_CALLBACK(void * p, U32 NumBytes);
typedef void   FS_PF_FREE                      (void * p);
typedef void * FS_PF_ALLOC                     (U32 NumBytes);

struct FS_DIRENT {
  char DirName[FS_MAX_PATH];
  U8   Attributes;
  U32  Size;
  U32  TimeStamp;
};

typedef struct {
  U32 Cluster;                    // Cluster of current sector
  U32 FirstCluster;
  U32 DirEntryIndex;              // Directory entry index (first directory entry has index 0)
  U32 ClusterIndex;
} FS_DIR_POS;

typedef struct {
  FS_DIR_POS  DirPos;             // Current position in directory
  U16         DirEntryIndex;
  U32         FirstCluster;
  FS_VOLUME * pVolume;
  U8          InUse;
} FS__DIR;

typedef struct {
  U8     Attributes;
  U32    CreationTime;
  U32    LastAccessTime;
  U32    LastWriteTime;
  U32    FileSize;
  char * sFileName;
  int    SizeofFileName;
} FS_DIRENTRY_INFO;

typedef struct {
  U8     Attributes;
  U32    CreationTime;
  U32    LastAccessTime;
  U32    LastWriteTime;
  U32    FileSize;
  char * sFileName;
  //
  // Private elements. Not to be used by the application.
  //
  int SizeofFileName;
  FS__DIR Dir;
} FS_FIND_DATA;

typedef struct {
  U32 NumTotalClusters;
  U32 NumFreeClusters;
  U16 SectorsPerCluster;
  U16 BytesPerSector;
  U16 NumRootDirEntries;
  U16 FSType;
} FS_DISK_INFO;

typedef struct {
  U16           SectorsPerCluster;
  U16           NumRootDirEntries;    // Proposed, actual value depends on FATType
  FS_DEV_INFO * pDevInfo;
} FS_FORMAT_INFO;

typedef struct {
  U16           SectorsPerCluster;
  U16           NumRootDirEntries;    // Proposed, actual value depends on FATType
  U16           NumReservedSectors;
  U8            UpdatePartition;
  FS_DEV_INFO * pDevInfo;
} FS_FORMAT_INFO_EX;

typedef struct {
  U16 Year;
  U16 Month;
  U16 Day;
  U16 Hour;
  U16 Minute;
  U16 Second;
} FS_FILETIME;

typedef enum {
  FS_WRITEMODE_SAFE,
  FS_WRITEMODE_MEDIUM,
  FS_WRITEMODE_FAST,
  FS_WRITEMODE_UNKNOWN      // For internal use only.
} FS_WRITEMODE;

#if FS_SUPPORT_DEINIT
  struct FS_ON_EXIT_CB;

  typedef struct FS_ON_EXIT_CB {
    struct FS_ON_EXIT_CB * pNext;
    void (*pfOnExit)(void);
  } FS_ON_EXIT_CB;
#endif

typedef struct {
  U16 Cylinder;
  U8  Head;
  U8  Sector;
} FS_CHS_ADDR;

typedef struct {
  U32         NumSectors;
  U32         StartSector;
  FS_CHS_ADDR StartAddr;
  FS_CHS_ADDR EndAddr;
  U8          Type;
  U8          IsActive;
} FS_PARTITION_INFO;

/*********************************************************************
*
*             Non blocking (backgrounded) file I/O functions
*/
/* Information for background data */
typedef struct FS_BG_DATA {
  struct FS_BG_DATA * pNext;
  U32                 NumBytes;
  U32                 NumBytesRem;         /* Remaining bytes to transfer */
  void              * pData;
  FS_FILE           * pFile;
  U8                  Operation;
  void (*pfOnCompletion) (void * p);   /* Optional completion routine */
  void              * pCompletionData;     /* Optional data for completion routine */
  char                IsCompleted;
} FS_BG_DATA;

void FS_Daemon(void);
void FS_FReadNonBlock (void       * pData,
                       U32          NumBytes,
                       FS_FILE    * pFile,
                       FS_BG_DATA * pBGData,                    /* User supplied management block */
                       void (*pfOnCompletion) (void * p),   /* Optional completion routine */
                       void * pCompletionData                   /* Optional data for completion routine */
                      );
void FS_FWriteNonBlock(const void * pData,
                       U32          NumBytes,
                       FS_FILE    * pFile,
                       FS_BG_DATA * pBGData,                    /* User supplied management block */
                       void (*pfOnCompletion) (void * p),   /* Optional completion routine */
                       void * pCompletionData                   /* Optional data for completion routine */
                       );
char FS_IsCompleted(FS_BG_DATA * pBGData);

/*********************************************************************
*
*       Global function prototypes
*
**********************************************************************
*/

/*********************************************************************
*
*       "Standard" file I/O functions
*/
FS_FILE * FS_FOpen (const char * pFileName, const char * pMode);
int       FS_FClose(FS_FILE    * pFile);
U32       FS_FRead (      void * pData, U32 Size, U32 N, FS_FILE * pFile);
U32       FS_FWrite(const void * pData, U32 Size, U32 N, FS_FILE * pFile);

/*********************************************************************
*
*       Non-standard file I/O functions
*/
U32 FS_Read (FS_FILE * pFile,       void * pData, U32 NumBytes);
U32 FS_Write(FS_FILE * pFile, const void * pData, U32 NumBytes);

/*********************************************************************
*
*       File pointer handling
*/
int FS_FSeek       (FS_FILE * pFile, I32 Offset, int Origin);
int FS_SetEndOfFile(FS_FILE * pFile);
I32 FS_FTell       (FS_FILE * pFile);

#define FS_GetFilePos(pFile)                                FS_FTell(pFile)
#define FS_SetFilePos(pFile, DistanceToMove, MoveMethod)    FS_FSeek(pFile, DistanceToMove, MoveMethod)

/*********************************************************************
*
*       I/O error handling
*/
int          FS_FEof        (FS_FILE * pFile);
I16          FS_FError      (FS_FILE * pFile);
void         FS_ClearErr    (FS_FILE * pFile);
const char * FS_ErrorNo2Text(int       ErrCode);

/*********************************************************************
*
*       File functions
*/
int FS_CopyFile   (const char * sSource,       const char * sDest);
int FS_CopyFileEx (const char * sSource,       const char * sDest, void * pBuffer, U32 NumBytes);
U32 FS_GetFileSize(FS_FILE    * pFile);
int FS_Move       (const char * sExistingName, const char * sNewName);
int FS_Remove     (const char * pFileName);
int FS_Rename     (const char * sOldName,      const char * sNewName);
int FS_Truncate   (FS_FILE    * pFile,         U32          NewSize);
int FS_Verify     (FS_FILE    * pFile,         const void * pData, U32 NumBytes);
int FS_SyncFile   (FS_FILE    * pFile);
int FS_WipeFile   (const char * pFileName);

/*********************************************************************
*
*       IOCTL
*/
int         FS_IoCtl(const char *pDevName, I32 Cmd, I32 Aux, void *pBuffer);

/*********************************************************************
*
*       Volume related functions
*/
int         FS_GetVolumeName       (int Index, char * pBuffer, int MaxSize);
U32         FS_GetVolumeSize       (const char * sVolumeName);
U32         FS_GetVolumeSizeKB     (const char * sVolumeName);
U32         FS_GetVolumeFreeSpace  (const char * sVolumeName);
U32         FS_GetVolumeFreeSpaceKB(const char * sVolumeName);
int         FS_GetNumVolumes       (void);
FS_VOLUME * FS_AddDevice           (const FS_DEVICE_TYPE * pDevType);
void        FS_Unmount             (const char * sVolumeName);
int         FS_Mount               (const char * sVolumeName);
int         FS_MountEx             (const char * sVolumeName, U8 MountType);
int         FS_GetVolumeInfo       (const char * sVolumeName, FS_DISK_INFO * pInfo);
int         FS_IsVolumeMounted     (const char * sVolumeName);
int         FS_GetVolumeLabel      (const char * sVolumeName, char * pVolumeLabel, unsigned VolumeLabelSize);
int         FS_SetVolumeLabel      (const char * sVolumeName, const char * pVolumeLabel);
void        FS_UnmountForced       (const char * sVolumeName);
void        FS_SetAutoMount        (const char * sVolumeName, U8 MountType);
int         FS_GetVolumeStatus     (const char * sVolumeName);
FS_VOLUME * FS_FindVolume          (const char * sVolumeName);
void        FS_RemoveDevice        (const char * sVolumeName);
int         FS_Sync                (const char * sVolumeName);

/*********************************************************************
*
*       File write mode
*/
void        FS_SetFileWriteMode    (FS_WRITEMODE WriteMode);
void        FS_SetFileWriteModeEx  (FS_WRITEMODE WriteMode, const char * sVolumeName);

/*********************************************************************
*
*       Journaling / transaction safety
*/
typedef struct {
  U32 WriteSectorCnt;     // Number of sectors written by the file system to journal
  U32 NumTransactions;    // Number of journal transactions performed
} FS_JOURNAL_STAT_COUNTERS;

int FS_JOURNAL_Begin            (const char * sVolumeName);
int FS_JOURNAL_End              (const char * sVolumeName);
int FS_JOURNAL_Create           (const char * sVolumeName, U32 NumBytes);
int FS_JOURNAL_Enable           (const char * sVolumeName);
int FS_JOURNAL_Disable          (const char * sVolumeName);
int FS_JOURNAL_GetStatCounters  (const char * sVolumeName, FS_JOURNAL_STAT_COUNTERS * pStat);
int FS_JOURNAL_ResetStatCounters(const char * sVolumeName);
int FS_JOURNAL_GetOpenCnt       (const char * sVolumeName);

/*********************************************************************
*
*       File/directory attribute functions
*/
int  FS_SetFileAttributes(const char * pName, U8 Attributes);
U8   FS_GetFileAttributes(const char * pName);

/*********************************************************************
*
*       File/directory timestamp functions
*/
void FS_FileTimeToTimeStamp(const FS_FILETIME * pFileTime, U32         * pTimeStamp);
int  FS_GetFileTime        (const char        * pName,     U32         * pTimeStamp);
int  FS_GetFileTimeEx      (const char        * pName,     U32         * pTimeStamp, int Index);
int  FS_SetFileTime        (const char        * pName,     U32           TimeStamp);
int  FS_SetFileTimeEx      (const char        * pName,     U32           TimeStamp,  int Index);
void FS_TimeStampToFileTime(U32                 TimeStamp, FS_FILETIME * pFileTime);

/*********************************************************************
*
*       File system misc. functions
*/
int  FS_GetNumFilesOpen(void);
U32  FS_GetMaxSectorSize(void);

/*********************************************************************
*
*       File system directory functions
*/
int  FS_CreateDir      (const char * sDirPath);
int  FS_MkDir          (const char * pDirName);
int  FS_RmDir          (const char * pDirName);
char FS_FindFirstFile  (FS_FIND_DATA * pfd, const char * sPath, char * sFilename, int sizeofFilename);
char FS_FindNextFile   (FS_FIND_DATA * pfd);
void FS_FindClose      (FS_FIND_DATA * pfd);
U32  FS_FAT_GrowRootDir(const char * sVolumeName, U32 NumAddEntries);

/*********************************************************************
*
*       Olsolete directory functions
*/
void        FS_DirEnt2Attr   (FS_DIRENT  * pDirEnt, U8   * pAttr);
void        FS_DirEnt2Name   (FS_DIRENT  * pDirEnt, char * pBuffer);
U32         FS_DirEnt2Size   (FS_DIRENT  * pDirEnt);
U32         FS_DirEnt2Time   (FS_DIRENT  * pDirEnt);
U32         FS_GetNumFiles   (FS_DIR     * pDir);

FS_DIR    * FS_OpenDir    (const char * pDirName);
int         FS_CloseDir   (FS_DIR     * pDir);
FS_DIRENT * FS_ReadDir    (FS_DIR     * pDir);
void        FS_RewindDir  (FS_DIR     * pDir);

/*********************************************************************
*
*       File system standard application
*/
void FSTask(void);

/*********************************************************************
*
*       File system control functions
*/
void     FS_Init            (void);
#if FS_SUPPORT_DEINIT
  void   FS_DeInit          (void);
  void   FS_AddOnExitHandler(FS_ON_EXIT_CB * pCB, void (*pfOnExit)(void));
#endif

/*********************************************************************
*
*       Formatting
*/
int          FS_FormatLLIfRequired(const char * sVolumeName);
int          FS_FormatLow         (const char * sVolumeName);
int          FS_Format            (const char * sVolumeName, FS_FORMAT_INFO * pFormatInfo);
int          FS_FormatSD          (const char * sVolumeName);
int          FS_IsLLFormatted     (const char * sVolumeName);
int          FS_IsHLFormatted     (const char * sVolumeName);

/*********************************************************************
*
*       Partitioning
*/
#define FS_NUM_PARTITIONS  4
int          FS_CreateMBR       (const char * sVolumeName, FS_PARTITION_INFO * pPartInfo, int NumPartitions);
int          FS_GetPartitionInfo(const char * sVolumeName, FS_PARTITION_INFO * pPartInfo, U8 PartIndex);

/*********************************************************************
*
*       CheckDisk functionality
*/
const char * FS_CheckDisk_ErrCode2Text(int ErrCode);
int          FS_CheckDisk             (const char * sVolumeName, void * pBuffer, U32 BufferSize, int MaxRecursionLevel, FS_ON_CHECK_DISK_ERROR_CALLBACK * pfOnError);
void         FS_FAT_AbortCheckDisk    (void);

/*********************************************************************
*
*       Configuration functions.
*/

/*********************************************************************
*
*       General runtime configuration functions
*/
void FS_SetMaxSectorSize      (unsigned MaxSectorSize);
void FS_ConfigUpdateDirOnWrite(char OnOff);
void FS_AssignMemory          (U32 *pMem, U32 NumBytes);
void FS_SetMemHandler         (FS_PF_ALLOC * pfAlloc, FS_PF_FREE * pfFree);

/*********************************************************************
*
*       FAT runtime configuration functions.
*/
void FS_FAT_SupportLFN(void);
void FS_FAT_DisableLFN(void);
#if FS_FAT_USE_FSINFO_SECTOR
  void FS_FAT_ConfigUseFSInfoSector(int OnOff);
#endif
#if FS_MAINTAIN_FAT_COPY
  void FS_FAT_ConfigMaintainFATCopy(int OnOff);
#endif

/*********************************************************************
*
*       File buffer related functions.
*/
#if FS_USE_FILE_BUFFER
  #define FS_FILE_BUFFER_WRITE   (1 << 0)
  void FS_ConfigFileBufferDefault(int BufferSize, int Flags);     // Configures default file buffer size and flags for all files which will be opened. Should only be called once, in FS_X_AddDevices()
  void FS_SetFileBufferFlags(FS_FILE * pFile, int Flags);         // Is only allowed to be called immediately after a FS_FOpen(), in order to change the buffer flags of a specific file
#endif

/*********************************************************************
*
*       BusyLED support
*/
void FS_SetBusyLEDCallback(const char * sVolumeName, FS_BUSY_LED_CALLBACK * pfBusyLEDCallback);

/*********************************************************************
*
*       Memory accessible support
*/
void FS_SetMemAccessCallback(const char * sVolumeName, FS_MEMORY_IS_ACCESSIBLE_CALLBACK * pfIsAccessibleCallback);

/*********************************************************************
*
*       Misc. functions
*/
int FS_GetFileId(const char * pFileName, U8 * pId);

/*********************************************************************
*
*       Device Drivers
*
*/
struct FS_DEVICE_TYPE {      // Note: This definition is really intern and should be in "FS_Int.h". In order to avoid problems with old compilers, keep it here.
  const char * (*pfGetName)    (U8 Unit);
  int          (*pfAddDevice)  (void);                                                       // Called from AddDevice. Usually the first call to the driver
  int          (*pfRead)       (U8 Unit, U32 SectorNo,       void * pBuffer, U32 NumSectors);
  int          (*pfWrite)      (U8 Unit, U32 SectorNo, const void * pBuffer, U32 NumSectors, U8 RepeatSame);
  int          (*pfIoCtl)      (U8 Unit, I32 Cmd, I32 Aux, void * pBuffer);
  int          (*pfInitMedium) (U8 Unit);                                                       // Called when formatting or mounting a device
  int          (*pfGetStatus)  (U8 Unit);
  int          (*pfGetNumUnits)(void);
};

extern const FS_DEVICE_TYPE FS_RAMDISK_Driver;         // Driver which uses RAM as storage
extern const FS_DEVICE_TYPE FS_WINDRIVE_Driver;        // Driver for Windows drives
extern const FS_DEVICE_TYPE FS_MMC_CardMode_Driver;    // Driver for SD/MMC using card controller
extern const FS_DEVICE_TYPE FS_MMC_SPI_Driver;         // Driver for SD/MMC using SPI
extern const FS_DEVICE_TYPE FS_MMC_CM_Driver4Atmel;    // Driver for SD/MMC using Atmel card controller
extern const FS_DEVICE_TYPE FS_IDE_Driver;             // Driver for IDE and Compact Flash
extern const FS_DEVICE_TYPE FS_NOR_Driver;             // Driver for NOR flashes (fast write)
extern const FS_DEVICE_TYPE FS_NAND_Driver;            // Driver for SLC NAND flashes
extern const FS_DEVICE_TYPE FS_NOR_BM_Driver;          // Driver for NOR flashes (reduced RAM usage)
extern const FS_DEVICE_TYPE FS_NAND_UNI_Driver;        // Driver for SLC NAND flashes with ECC engine
extern const FS_DEVICE_TYPE FS_DISKPART_Driver;        // Logical driver for disk partitions
extern const FS_DEVICE_TYPE FS_CRYPT_Driver;           // Logical driver for encryption
extern const FS_DEVICE_TYPE FS_READAHEAD_Driver;       // Logical driver which reads sectors in advance

/*********************************************************************
*
*       NOR driver
*
**********************************************************************
*/
typedef struct {
  U32 Off;
  U32 Size;
  U32 EraseCnt;
  U16 NumUsedSectors;
  U16 NumFreeSectors;
  U16 NumEraseableSectors;
} FS_NOR_SECTOR_INFO;

typedef struct {
  U32 NumPhysSectors;
  U32 NumLogSectors;
  U32 NumUsedSectors;   // Number of used logical sectors
} FS_NOR_DISK_INFO;

typedef struct {
  U32 EraseCnt;
  U32 ReadSectorCnt;
  U32 WriteSectorCnt;
  U32 CopySectorCnt;
} FS_NOR_STAT_COUNTERS;

typedef struct {
  U16 NumPhySectors;
  U16 NumLogBlocks;
  U16 NumUsedPhySectors;
  U16 LSectorsPerPSector;
  U16 BytesPerSector;
  U32 EraseCntMax;
  U32 EraseCntMin;
  U32 EraseCntAvg;
  U8  HasFatalError;
  U8  ErrorType;
  U16 ErrorPSI;
} FS_NOR_BM_DISK_INFO;

typedef struct {
  U32 NumFreeBlocks;
  U32 EraseCnt;
  U32 ReadSectorCnt;
  U32 WriteSectorCnt;
  U32 ConvertViaCopyCnt;
  U32 ConvertInPlaceCnt;
  U32 NumValidSectors;
  U32 CopySectorCnt;
} FS_NOR_BM_STAT_COUNTERS;

typedef struct FS_NOR_PHY_TYPE {
  int  (*pfWriteOff)     (U8 Unit, U32 Off, const void * pSrc, U32 Len);
  int  (*pfReadOff)      (U8 Unit, void * pDest, U32 Off, U32 Len);
  int  (*pfEraseSector)  (U8 Unit, unsigned int SectorIndex);
  void (*pfGetSectorInfo)(U8 Unit, unsigned int SectorIndex, U32 * pOff, U32 * pLen);
  int  (*pfGetNumSectors)(U8 Unit);
  void (*pfConfigure)    (U8 Unit, U32 BaseAddr, U32 StartAddr, U32 NumBytes);
  void (*pfOnSelectPhy)  (U8 Unit);
  void (*pfDeInit)       (U8 Unit);
} FS_NOR_PHY_TYPE;

typedef void (FS_NOR_READ_CFI_FUNC)(U8 Unit, U32 BaseAddr, U32 Off, U8 * pData, unsigned NumItems);

extern const FS_NOR_PHY_TYPE FS_NOR_PHY_CFI_1x16;            // 1 x 16-bit CFI compliant NOR flash
extern const FS_NOR_PHY_TYPE FS_NOR_PHY_CFI_2x16;            // 2 x 16-bit CFI compliant NOR flash
extern const FS_NOR_PHY_TYPE FS_NOR_PHY_ST_M25;              // ST M25P compliant Serial NOR flash

void         FS_NOR_GetDiskInfo            (U8 Unit, FS_NOR_DISK_INFO * pDiskInfo);
void         FS_NOR_GetSectorInfo          (U8 Unit, U32 PhysSectorIndex, FS_NOR_SECTOR_INFO * pSectorInfo);
void         FS_NOR_Configure              (U8 Unit, U32 BaseAddr, U32 StartAddr, U32 NumBytes);
void         FS_NOR_ConfigureReserve       (U8 Unit, U8 Percentage2Reserve);
void         FS_NOR_SetPhyType             (U8 Unit, const FS_NOR_PHY_TYPE * pPhyType);
const void * FS_NOR_LogSector2PhySectorAddr(U8 Unit, U32 LogSectorNo);
void         FS_NOR_SetSectorSize          (U8 Unit, U16 SectorSize);
int          FS_NOR_FormatLow              (U8 Unit);
int          FS_NOR_IsLLFormatted          (U8 Unit);
void         FS_NOR_GetStatCounters        (U8 Unit, FS_NOR_STAT_COUNTERS * pStat);
void         FS_NOR_ResetStatCounters      (U8 Unit);
int          FS_NOR_ReadOff                (U8 Unit, U32 Off, void * pData, U32 NumBytes);

void         FS_NOR_BM_Configure           (U8 Unit, U32 BaseAddr, U32 StartAddr, U32 NumBytes);
void         FS_NOR_BM_SetPhyType          (U8 Unit, const FS_NOR_PHY_TYPE * pPhyType);
void         FS_NOR_BM_SetMaxEraseCntDiff  (U8 Unit, U32 EraseCntDiff);
void         FS_NOR_BM_SetNumWorkBlocks    (U8 Unit, unsigned NumWorkBlocks);
void         FS_NOR_BM_SetSectorSize       (U8 Unit, unsigned SectorSize);
int          FS_NOR_BM_GetDiskInfo         (U8 Unit, FS_NOR_BM_DISK_INFO * pDiskInfo);
void         FS_NOR_BM_GetStatCounters     (U8 Unit, FS_NOR_BM_STAT_COUNTERS * pStat);
void         FS_NOR_BM_ResetStatCounters   (U8 Unit);
int          FS_NOR_BM_ReadOff             (U8 Unit, void * pData, U32 Off, U32 NumBytes);

void         FS_NOR_SPI_Configure          (U8 Unit, U32 SectorSize, U16 NumSectors);
void         FS_NOR_SPI_SetPageSize        (U8 Unit, U16 BytesPerPage);
void         FS_NOR_CFI_SetReadCFIFunc     (U8 Unit, FS_NOR_READ_CFI_FUNC * pReadCFI);
void         FS_NOR_CFI_SetAddrGap         (U8 Unit, U32 StartAddr, U32 NumBytes);

/*********************************************************************
*
*       RAMDISK driver
*
**********************************************************************
*/
void FS_RAMDISK_Configure(U8 Unit, void * pData, U16 BytesPerSector, U32 NumSectors);

/*********************************************************************
*
*       MMC/SD driver
*
**********************************************************************
*/
typedef struct {
  U8 aData[16];
} MMC_CARD_ID;

void FS_MMC_ActivateCRC          (void);
void FS_MMC_DeactivateCRC        (void);
int  FS_MMC_GetCardId            (U8 Unit, MMC_CARD_ID * pCardId);
void FS_MMC_CM_Allow4bitMode     (U8 Unit, U8 OnOff);
void FS_MMC_CM_Allow8bitMode     (U8 Unit, U8 OnOff);
void FS_MMC_CM_AllowHighSpeedMode(U8 Unit, U8 OnOff);
int  FS_MMC_CM_GetCardId         (U8 Unit, MMC_CARD_ID * pCardId);
int  FS_MMC_CM_UnlockCardForced  (U8 Unit);

/*********************************************************************
*
*       IDE/CF driver
*
**********************************************************************
*/
void FS_IDE_Configure(U8 Unit, U8 IsSlave);

/*********************************************************************
*
*       NAND driver
*
**********************************************************************
*/

/*********************************************************************
*
*       NAND block types
*/
#define FS_NAND_BLOCK_TYPE_UNKNOWN   0
#define FS_NAND_BLOCK_TYPE_BAD       1
#define FS_NAND_BLOCK_TYPE_EMPTY     2
#define FS_NAND_BLOCK_TYPE_WORK      3
#define FS_NAND_BLOCK_TYPE_DATA      4

/*********************************************************************
*
*       FS_NAND_SECTOR_INFO
*
*   Information about a logical sector stored on the NAND flash.
*/
typedef struct FS_NAND_SECTOR_INFO {
  U16 brsi;              // Block relative sector index.
  U32 ECC;               // ECC stored for this sector.
  U8  IsValid;           // Sector contains valid data.
} FS_NAND_SECTOR_INFO;

/*********************************************************************
*
*       FS_NAND_BLOCK_INFO
*
*   Information about a NAND flash block.
*/
typedef struct FS_NAND_BLOCK_INFO {
  U32 EraseCnt;                   // Number of times the block has been erased
  U32 lbi;                        // Logical block index assigned to queried physical block.
  U16 NumSectorsBlank;            // Sectors are not used yet.
  U16 NumSectorsValid;            // Sectors contain correct data.
  U16 NumSectorsInvalid;          // Sectors have been invalidated.
  U16 NumSectorsECCError;         // Sectors have incorrect ECC.
  U16 NumSectorsECCCorrectable;   // Sectors have correctable ECC error.
  U16 NumSectorsErrorInECC;       // Sectors have error in ECC.
  const char * sType;             // Zero-terminated string holding the block type.
  U8  Type;                       // Type of data stored in the block (see the FS_NAND_BLOCK_TYPE_... defines).
} FS_NAND_BLOCK_INFO;

/*********************************************************************
*
*       FS_NAND_DISK_INFO
*
*   Information about the NAND flash.
*/
typedef struct FS_NAND_DISK_INFO {
  U32 NumPhyBlocks;
  U32 NumLogBlocks;
  U32 NumUsedPhyBlocks;
  U32 NumBadPhyBlocks;
  U32 NumPagesPerBlock;
  U32 NumSectorsPerBlock;
  U32 BytesPerPage;
  U32 BytesPerSpareArea;
  U32 BytesPerSector;
  U32 EraseCntMin;
  U32 EraseCntMax;
  U32 EraseCntAvg;
  U8  IsWriteProtected;
  U8  HasFatalError;
  U8  ErrorType;
  U32 ErrorSectorIndex;
} FS_NAND_DISK_INFO;

/*********************************************************************
*
*       FS_NAND_ECC_INFO
*
*   Information about the ECC used to protect data stored on the NAND flash.
*/
typedef struct FS_NAND_ECC_INFO {
  U8 NumBitsCorrectable;      // Number of bits the ECC should be able to correct
  U8 ldBytesPerBlock;         // Number of bytes the ECC should protect
} FS_NAND_ECC_INFO;

/*********************************************************************
*
*       FS_NAND_DEVICE_INFO
*
*   Information about the NAND flash device.
*/
typedef struct FS_NAND_DEVICE_INFO {
  U8  BPP_Shift;              // Bytes per page shift: 9 for 512 bytes/page or 11 for 2048 bytes/page
  U8  PPB_Shift;              // Pages per block shift: 32 -> 5, 64 -> 6
  U16 NumBlocks;              // Number of blocks in device
  U16 BytesPerSpareArea;      // Number of bytes in the spare area. Usually this is (1 << BPP_Shift)/32. Micron MT29F8G08ABABA has a spare area of 224 bytes for 4096 bytes/page.
  FS_NAND_ECC_INFO ECC_Info;  // Required ECC capability
} FS_NAND_DEVICE_INFO;

/*********************************************************************
*
*       FS_NAND_STAT_COUNTERS
*
*   Statistic counters of NAND flash driver.
*/
typedef struct FS_NAND_STAT_COUNTERS {
  U32 NumFreeBlocks;
  U32 NumBadBlocks;
  U32 EraseCnt;
  U32 ReadDataCnt;
  U32 ReadSpareCnt;
  U32 ReadSectorCnt;
  U32 NumReadRetries;
  U32 WriteDataCnt;
  U32 WriteSpareCnt;
  U32 WriteSectorCnt;
  U32 ConvertViaCopyCnt;
  U32 ConvertInPlaceCnt;
  U32 NumValidSectors;
  U32 CopySectorCnt;
} FS_NAND_STAT_COUNTERS;

/*********************************************************************
*
*       FS_NAND_FATAL_ERROR_INFO
*
*   Information passed to fatal error callback.
*/
typedef struct {
  U8  Unit;
} FS_NAND_FATAL_ERROR_INFO;

typedef int FS_NAND_ON_FATAL_ERROR_CALLBACK(FS_NAND_FATAL_ERROR_INFO * pFatalErrorInfo);

/*********************************************************************
*
*       FS_NAND_ECC_HOOK
*
*   External ECC computation module.
*/
typedef struct FS_NAND_ECC_HOOK {
  void      (*pfCompute)        (const U32 * pData, U8 * pSpare);
  int       (*pfApply)          (      U32 * pData, U8 * pSpare);
  unsigned  NumBitsCorrectable; // Number of bits the ECC algorithm can correct per 512 bytes.
} FS_NAND_ECC_HOOK;

typedef struct FS_NAND_PHY_TYPE {
  int (*pfEraseBlock)       (U8 Unit, U32 Block);
  int (*pfInitGetDeviceInfo)(U8 Unit, FS_NAND_DEVICE_INFO * pDevInfo);
  int (*pfIsWP)             (U8 Unit);
  int (*pfRead)             (U8 Unit, U32 PageNo,       void * pData, unsigned Off, unsigned NumBytes);
  int (*pfReadEx)           (U8 Unit, U32 PageNo,       void * pData, unsigned Off, unsigned NumBytes,       void * pSpare, unsigned OffSpare, unsigned NumBytesSpare);
  int (*pfWrite)            (U8 Unit, U32 PageNo, const void * pData, unsigned Off, unsigned NumBytes);
  int (*pfWriteEx)          (U8 Unit, U32 PageNo, const void * pData, unsigned Off, unsigned NumBytes, const void * pSpare, unsigned OffSpare, unsigned NumBytesSpare);
  int (*pfEnableECC)        (U8 Unit);
  int (*pfDisableECC)       (U8 Unit);
  int (*pfConfigureECC)     (U8 Unit, U8 NumBitsCorrectable, U16 BytesPerECCBlock);
} FS_NAND_PHY_TYPE;

/*********************************************************************
*
*       Available physical layers
*/
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_x;          // 512 or 2048 byte pages,  8-bit or 16-bit interface
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_x8;         // 512 or 2048 byte pages,  8-bit interface
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_x16;        // 512 or 2048 byte pages, 16-bit interface
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_512x8;      // 512 byte pages, 8-bit interface
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_512x16;     // 512 byte pages, 16-bit interface
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_2048x8;     // 2048 byte pages, 8-bit interface
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_2048x16;    // 2048 byte pages, 16-bit interface
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_4096x8;     // 4096 byte pages, 8-bit interface
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_DataFlash;  // Physical layer for ATMEL serial DataFlash
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_ONFI;       // Physical layer for NAND flashes which support ONFI
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_SPI;        // Physical layer for NAND flashes with SPI interface

/*********************************************************************
*
*       ECC computation modules
*/
extern const FS_NAND_ECC_HOOK FS_NAND_ECC_HW_NULL;    // Uses the HW ECC of the NAND flash device
extern const FS_NAND_ECC_HOOK FS_NAND_ECC_HW_4BIT;    // Uses the HW ECC of the NAND flash controller
extern const FS_NAND_ECC_HOOK FS_NAND_ECC_HW_8BIT;    // Uses the HW ECC of the NAND flash controller
extern const FS_NAND_ECC_HOOK FS_NAND_ECC_SW_1BIT;    // 1-bit SW ECC over 512 bytes of page and 4 bytes of spare area

/*********************************************************************
*
*       SLC1 NAND driver functions
*/
void FS_NAND_EraseFlash             (U8 Unit);
int  FS_NAND_FormatLow              (U8 Unit);
void FS_NAND_GetBlockInfo           (U8 Unit, U32 PhyBlockIndex, FS_NAND_BLOCK_INFO * pBlockInfo);
void FS_NAND_GetDiskInfo            (U8 Unit, FS_NAND_DISK_INFO * pDiskInfo);
void FS_NAND_GetSectorInfo          (U8 Unit, U32 PhySectorIndex, FS_NAND_SECTOR_INFO * pBlockInfo);
void FS_NAND_GetStatCounters        (U8 Unit, FS_NAND_STAT_COUNTERS * pStat);
int  FS_NAND_IsLLFormatted          (U8 Unit);
int  FS_NAND_ReadPhySector          (U8 Unit, U32 PhySectorIndex, void * pData, unsigned * pNumBytesData, void * pSpare, unsigned * pNumBytesSpare);
void FS_NAND_ResetStatCounters      (U8 Unit);
void FS_NAND_SetBlockRange          (U8 Unit, U16 FirstBlock, U16 MaxNumBlocks);
void FS_NAND_SetPhyType             (U8 Unit, const FS_NAND_PHY_TYPE * pPhyType);
void FS_NAND_SetMaxEraseCntDiff     (U8 Unit, U32 EraseCntDiff);
void FS_NAND_SetNumWorkBlocks       (U8 Unit, U32 NumWorkBlocks);
void FS_NAND_SetOnFatalErrorCallback(FS_NAND_ON_FATAL_ERROR_CALLBACK * pfOnFatalError);
void FS_NAND_Validate               (void);

/*********************************************************************
*
*       Universal NAND driver functions
*/
int  FS_NAND_UNI_EraseFlash             (U8 Unit);
int  FS_NAND_UNI_GetBlockInfo           (U8 Unit, U32 PhysBlockIndex, FS_NAND_BLOCK_INFO * pBlockInfo);
int  FS_NAND_UNI_GetDiskInfo            (U8 Unit, FS_NAND_DISK_INFO * pDiskInfo);
void FS_NAND_UNI_GetStatCounters        (U8 Unit, FS_NAND_STAT_COUNTERS * pStat);
int  FS_NAND_UNI_ReadPhySector          (U8 Unit, U32 PhySectorIndex, void * pData, unsigned * pNumBytesData, void * pSpare, unsigned * pNumBytesSpare);
void FS_NAND_UNI_ResetStatCounters      (U8 Unit);
void FS_NAND_UNI_SetPhyType             (U8 Unit, const FS_NAND_PHY_TYPE * pPhyType);
void FS_NAND_UNI_SetECCHook             (U8 Unit, const FS_NAND_ECC_HOOK * pECCHook);
void FS_NAND_UNI_SetBlockRange          (U8 Unit, U16 FirstBlock, U16 MaxNumBlocks);
void FS_NAND_UNI_SetMaxEraseCntDiff     (U8 Unit, U32 EraseCntDiff);
void FS_NAND_UNI_SetNumWorkBlocks       (U8 Unit, U32 NumWorkBlocks);
void FS_NAND_UNI_SetOnFatalErrorCallback(FS_NAND_ON_FATAL_ERROR_CALLBACK * pfOnFatalError);

/*********************************************************************
*
*       NAND physical layer specific functions
*/
void FS_NAND_PHY_ReadDeviceId       (U8 Unit, U8 * pId, U32 NumBytes);
int  FS_NAND_PHY_ReadONFIPara       (U8 Unit, void * pPara);
void FS_NAND_2048x8_EnableReadCache (U8 Unit);
void FS_NAND_2048x8_DisableReadCache(U8 Unit);

/*********************************************************************
*
*       Data Flash physical layer
*/
void FS_DF_ChipErase(U8 Unit);

/*********************************************************************
*
*       WinDrive driver
*
**********************************************************************
*/
void WINDRIVE_Configure(U8 Unit, const char * sWindowsDriveName);

/*********************************************************************
*
*       Logical drivers
*/
int  FS_AddPhysDevice     (const FS_DEVICE_TYPE * pDevType);

/*********************************************************************
*
*       Logical volume
*/
int  FS_LOGVOL_Create     (const char * sVolName);
int  FS_LOGVOL_AddDevice  (const char * sLogVolName, const FS_DEVICE_TYPE * pDevice, U8 Unit, U32 StartOff, U32 NumSectors);

/*********************************************************************
*
*       MBR disk partition
*/
void FS_DISKPART_Configure(U8 Unit, const FS_DEVICE_TYPE * pDevice, U8 DeviceUnit, U8 PartIndex);

/*********************************************************************
*
*       Encryption
*/
typedef struct {
  void (*pfPrepare)(void * pContext, const U8 * pKey);
  void (*pfEncrypt)(void * pContext, U8 * pDest, const U8 * pSrc, U32 NumBytes, U32 BlockIndex);
  void (*pfDecrypt)(void * pContext, U8 * pDest, const U8 * pSrc, U32 NumBytes, U32 BlockIndex);
  U16    BitsPerBlock;     // Number of bits encrypted at once by the algorithm: 128 -> AES, 64 -> DES.
} FS_CRYPT_ALGO_TYPE;

/*********************************************************************
*
*       FS_CRYPT_OBJ
*
*   FS internal structure. Describes an encryption object.
*/
typedef struct {
  void                     * pContext;
  const FS_CRYPT_ALGO_TYPE * pAlgoType;
  U16                        ldBytesPerBlock;
} FS_CRYPT_OBJ;

/*********************************************************************
*
*       FS_DES_CONTEXT
*
*   FS internal structure. Describes the context of the DES encryption algorithm.
*/
typedef struct {
  U32 _aRoundKey[32];
} FS_DES_CONTEXT;

/*********************************************************************
*
*       FS_AES_CONTEXT
*
*   FS internal structure. Describes the context of the AES encryption algorithm.
*/
typedef struct {
  U32 aRoundKeyEnc[60];
  U32 aRoundKeyDec[60];
} FS_AES_CONTEXT;

extern const FS_CRYPT_ALGO_TYPE   FS_CRYPT_ALGO_DES;      // 56-bit DES encryption
extern const FS_CRYPT_ALGO_TYPE   FS_CRYPT_ALGO_AES128;   // 128-bit AES encryption
extern const FS_CRYPT_ALGO_TYPE   FS_CRYPT_ALGO_AES256;   // 256-bit AES encryption

void FS_CRYPT_Configure(U8 Unit, const FS_DEVICE_TYPE * pDeviceType, U8 DeviceUnit, const FS_CRYPT_ALGO_TYPE * pAlgoType, void * pContext, const U8 * pKey);
void FS_CRYPT_Prepare  (FS_CRYPT_OBJ * pCryptObj, const FS_CRYPT_ALGO_TYPE * pAlgoType, void * pContext, U32 BytesPerBlock, const U8 * pKey);
void FS_CRYPT_Encrypt  (const FS_CRYPT_OBJ * pCryptObj, U8 * pDest, const U8 * pSrc, U32 NumBytes, U32 * pBlockIndex);
void FS_CRYPT_Decrypt  (const FS_CRYPT_OBJ * pCryptObj, U8 * pDest, const U8 * pSrc, U32 NumBytes, U32 * pBlockIndex);

#if FS_SUPPORT_ENCRYPTION
  int FS_SetEncryptionObject(FS_FILE * pFile, FS_CRYPT_OBJ * pCryptObj);
#endif

/*********************************************************************
*
*       Encryption performance test routines
*/
void FS_CRYPT_DES_Encrypt1MB   (void);
void FS_CRYPT_DES_Decrypt1MB   (void);
void FS_CRYPT_AES256_Encrypt1MB(void);
void FS_CRYPT_AES256_Decrypt1MB(void);
void FS_CRYPT_AES128_Encrypt1MB(void);
void FS_CRYPT_AES128_Decrypt1MB(void);

/*********************************************************************
*
*       Sector read ahead driver
*/
typedef struct {
  U32 ReadSectorCnt;
  U32 ReadSectorCachedCnt;
} FS_READAHEAD_STAT_COUNTERS;

void FS_READAHEAD_Configure        (U8 Unit, const FS_DEVICE_TYPE * pDevice, U8 DeviceUnit, U32 * pData, U32 NumBytes);
void FS_READAHEAD_GetStatCounters  (U8 Unit, FS_READAHEAD_STAT_COUNTERS * pStat);
void FS_READAHEAD_ResetStatCounters(U8 Unit);

/*********************************************************************
*
*       Cache handling
*/
#if FS_SUPPORT_CACHE

/*********************************************************************
*
*       General Cache mode defines, do not use in application
*/
#define FS_CACHE_MODE_R         0x01
#define FS_CACHE_MODE_W         0x02
#define FS_CACHE_MODE_D         0x04

#define FS_CACHE_MODE_FULL      (FS_CACHE_MODE_R | FS_CACHE_MODE_W | FS_CACHE_MODE_D)   // Obsolete Cache mode define, do not use

/*********************************************************************
*
*       Cache mode defines that can be used in application
*/
#define FS_CACHE_MODE_WT        (FS_CACHE_MODE_R | FS_CACHE_MODE_W)
#define FS_CACHE_MODE_WB        (FS_CACHE_MODE_R | FS_CACHE_MODE_W | FS_CACHE_MODE_D)

/*********************************************************************
*
*       Cache specific defines
*/
#define FS_CACHE_NONE      NULL
#define FS_CACHE_MAN       FS_CacheMan_Init
#define FS_CACHE_RW        FS_CacheRW_Init
#define FS_CACHE_RW_QUOTA  FS_CacheRWQuota_Init
#define FS_CACHE_MULTI_WAY FS_CacheMultiWay_Init
#define FS_CACHE_ALL       FS_CacheAll_Init

/*********************************************************************
*
*       Defines to compute the cache size
*/

/*********************************************************************
*
*       CACHE_ALL_BLOCK_INFO
*
*  FS internal structure. One instance per block. Every cache block can cache a single sector.
*  It starts with CACHE_ALL_BLOCK_INFO, followed by the cached data.
*/
typedef struct {
  U32 SectorNo;
} CACHE_ALL_BLOCK_INFO;

/*********************************************************************
*
*       CACHE_ALL_DATA
*
*  FS internal structure. Once instance per cache. Size needs to be a multiple of 4.
*/
typedef struct {
  U32 NumSectors;
  U32 SectorSize;
  U32 NumBytesCache;
} CACHE_ALL_DATA;

/*********************************************************************
*
*       CACHE_MAN_BLOCK_INFO
*
*  FS internal structure. One instance per block. Every cache block can cache a single sector.
*  It starts with CACHE_MAN_BLOCK_INFO, followed by the cached data.
*/
typedef struct {
  U32 SectorNo;
} CACHE_MAN_BLOCK_INFO;

/*********************************************************************
*
*       CACHE_MAN_DATA
*
*  FS internal structure. Once instance per cache. Size needs to be a multiple of 4.
*/
typedef struct {
  U32 NumSectors;
  U32 SectorSize;
  U32 NumBytesCache;
} CACHE_MAN_DATA;

/*********************************************************************
*
*       CACHE_RW_BLOCK_INFO
*
*  FS internal structure. One instance per block. Every cache block can cache a single sector.
*  It starts with CACHE_RW_BLOCK_INFO, followed by the cached data.
*/
typedef struct {
  U32      SectorNo;
  unsigned IsDirty;
} CACHE_RW_BLOCK_INFO;

/*********************************************************************
*
*       CACHE_RW_DATA
*
*  FS internal structure. Once instance per cache. Size needs to be a multiple of 4.
*/
typedef struct {
  U32 NumSectors;
  U32 SectorSize;
  U8  aCacheMode[FS_SECTOR_TYPE_COUNT];
#if FS_SECTOR_TYPE_COUNT % 4
  U8  aPadding[4 - (FS_SECTOR_TYPE_COUNT % 4)];     // Make sure we pad this to a multiple of 4 bytes
#endif
  U32 NumBytesCache;
} CACHE_RW_DATA;

/*********************************************************************
*
*       CACHE_RW_QUOTA_BLOCK_INFO
*
*  FS internal structure. One instance per block. Every cache block can cache a single sector.
*  It starts with CACHE_RW_QUOTA_BLOCK_INFO, followed by the cached data.
*/
typedef struct {
  U32      SectorNo;
  unsigned IsDirty;
} CACHE_RW_QUOTA_BLOCK_INFO;

/*********************************************************************
*
*       CACHE_RW_QUOTA_DATA
*
*  FS internal structure. Once instance per cache. Size needs to be a multiple of 4.
*/
typedef struct {
  U32 NumSectors;
  U32 SectorSize;
  U8  aCacheMode[FS_SECTOR_TYPE_COUNT];
  U32 aNumSectorsReq[FS_SECTOR_TYPE_COUNT];
  U32 aNumSectorsAct[FS_SECTOR_TYPE_COUNT];
  U32 aHashStart[FS_SECTOR_TYPE_COUNT];
  U32 NumBytesCache;
} CACHE_RW_QUOTA_DATA;

/*********************************************************************
*
*       CACHE_MULTI_WAY_BLOCK_INFO
*
*  FS internal structure. One instance per block. Every cache block can cache a single sector.
*  It starts with CACHE_MULTI_WAY_BLOCK_INFO, followed by the cached data.
*/
typedef struct {
  U32 SectorNo;       // Index of the sector stored in this block
  U16 AccessCnt;      // Access count used for the replacement policy
  U16 IsDirty;        // 1 if the sector has been modified and needs to be written to medium
} CACHE_MULTI_WAY_BLOCK_INFO;

/*********************************************************************
*
*       CACHE_MULTI_WAY_DATA
*
*  FS internal structure. Once instance per cache. Size needs to be a multiple of 4.
*  This structure is stored a the beginning of cache memory.
*  It is followed by an array of cache blocks.
*/
typedef struct {
  U32 NumSets;            // Total number of sets that can be stored in the cache
  U32 ldAssocLevel;       // Number of sectors in a set. Power of 2 value as bit shift.
  U32 SectorSize;         // Size of a sector in bytes
  U8  aCacheMode[FS_SECTOR_TYPE_COUNT];           // Cache strategy for each sector type (management, directory and data)
#if FS_SECTOR_TYPE_COUNT % 4
  U8  aPadding[4 - (FS_SECTOR_TYPE_COUNT % 4)];   // Make sure the aCacheMode[] is padded to a multiple of 4 bytes
#endif
  U32 NumBytesCache;      // Total size of the cache in bytes.
} CACHE_MULTI_WAY_DATA;

#define FS_SIZEOF_CACHE_ALL(NumSectors, SectorSize)         (sizeof(CACHE_ALL_DATA) + (sizeof(CACHE_ALL_BLOCK_INFO) + (SectorSize)) * (NumSectors))
#define FS_SIZEOF_CACHE_MAN(NumSectors, SectorSize)         (sizeof(CACHE_MAN_DATA) + (sizeof(CACHE_MAN_BLOCK_INFO) + (SectorSize)) * (NumSectors))
#define FS_SIZEOF_CACHE_RW(NumSectors, SectorSize)          (sizeof(CACHE_RW_DATA) + (sizeof(CACHE_RW_BLOCK_INFO) + (SectorSize)) * (NumSectors))
#define FS_SIZEOF_CACHE_RW_QUOTA(NumSectors, SectorSize)    (sizeof(CACHE_RW_QUOTA_DATA) + (sizeof(CACHE_RW_QUOTA_BLOCK_INFO) + (SectorSize)) * (NumSectors))
#define FS_SIZEOF_CACHE_MULTI_WAY(NumSectors, SectorSize)   (sizeof(CACHE_MULTI_WAY_DATA) + (sizeof(CACHE_MULTI_WAY_BLOCK_INFO) + (SectorSize)) * (NumSectors))
#define FS_SIZEOF_CACHE_ANY(NumSectors, SectorSize)         FS_SIZEOF_CACHE_RW_QUOTA(NumSectors, SectorSize)

/*********************************************************************
*
*       Cache specific types
*/
typedef U32    FS_INIT_CACHE (FS_DEVICE * pDevice, void * pData, I32 NumBytes);
typedef U32 (* FS_CACHE_TYPE)(FS_DEVICE * pDevice, void * pData, I32 NumBytes);

/*********************************************************************
*
*       Cache specific prototypes
*/
U32    FS_AssignCache        (const char * pVolumeName, void * pData, I32 NumBytes, FS_INIT_CACHE * pfInit);
void   FS_CACHE_Clean        (const char * pVolumeName);
int    FS_CACHE_Command      (const char * pVolumeName, int   Cmd,      void * pData);
int    FS_CACHE_SetMode      (const char * pVolumeName, int   TypeMask, int    ModeMask);
int    FS_CACHE_SetQuota     (const char * pVolumeName, int   TypeMask, U32    NumSectors);
int    FS_CACHE_SetAssocLevel(const char * pVolumeName, int   AssocLevel);
int    FS_CACHE_GetNumSectors(const char * pVolumeName, U32 * pNumSectors);
int    FS_CACHE_Invalidate   (const char * sVolumeName);

/*********************************************************************
*
*       Cache initialization prototypes
*/
U32 FS_CacheAll_Init     (FS_DEVICE * pDev, void * pData, I32 NumBytes);
U32 FS_CacheMan_Init     (FS_DEVICE * pDev, void * pData, I32 NumBytes);
U32 FS_CacheRW_Init      (FS_DEVICE * pDev, void * pData, I32 NumBytes);
U32 FS_CacheRWQuota_Init (FS_DEVICE * pDev, void * pData, I32 NumBytes);
U32 FS_CacheMultiWay_Init(FS_DEVICE * pDev, void * pData, I32 NumBytes);

#endif // FS_SUPPORT_CACHE

/*********************************************************************
*
*       File system selection prototype
*/
#if FS_SUPPORT_MULTIPLE_FS
  int FS_SetFSType(const char * sVolumeName, int FSType);
  int FS_GetFSType(const char * sVolumeName);
#endif // FS_SUPPORT_MULTIPLE_FS

/*********************************************************************
*
*       Memory allocation functions
*/
void * FS_Alloc         (I32     NumBytes);
void * FS_AllocZeroed   (I32     NumBytes);
void   FS_Free          (void *  p);
void   FS_AllocZeroedPtr(void ** pp, I32 NumBytes);
void * FS_TryAlloc      (I32     NumBytesReq);

/*********************************************************************
*
*       File system locking
*/
#if FS_OS_LOCKING
  void FS_Lock        (void);
  void FS_Unlock      (void);
  void FS_LockVolume  (const char * sVolumeName);
  void FS_UnlockVolume(const char * sVolumeName);
#endif // FS_OS_LOCKING

/*********************************************************************
*
*       Application supplied functions FS_X_...
*/

/*********************************************************************
*
*       Debug support
*/
void FS_X_Log     (const char *s);
void FS_X_Warn    (const char *s);
void FS_X_ErrorOut(const char *s);

/*********************************************************************
*
*       Misc. functions
*/
void FS_X_Panic      (int ErrorCode);
void FS_X_AddDevices (void);
U32  FS_X_GetTimeDate(void);

/*********************************************************************
*
*       Compatibility defines
*/

/*********************************************************************
*
*       Cache API
*/
#define FS_CACHEALL_API             FS_CacheAll_Init
#define FS_CACHEMAN_API             FS_CacheMan_Init

/*********************************************************************
*
*       Configuration API
*/
#define FS_WriteUpdateDir(OnOff)                    FS_ConfigUpdateDirOnWrite(OnOff)
#define FS_ConfigFileEncryption(pFile, pCryptObj)   FS_SetEncryptionObject(pFile, pCryptObj)
#define FS_ConfigFileBufferFlags(pFile, Flags)      FS_SetFileBufferFlags(pFile, Flags)

/*********************************************************************
*
*       File API
*/
#define FS_FFlush(pFile)            FS_SyncFile(pFile)

/*********************************************************************
*
*       NAND flash driver
*/
#define FS_NAND_ON_FATAL_ERROR_CB                       FS_NAND_ON_FATAL_ERROR_CALLBACK
#define FS_NAND_SetOnFatalErrorCB(pfOnFatalError)       FS_NAND_SetOnFatalErrorCallback(pfOnFatalError)
#define FS_NAND_ReadDeviceId(Unit, pId, NumBytes)       FS_NAND_PHY_ReadDeviceId(Unit, pId, NumBytes)
#define FS_NAND_ECC_NULL                                FS_NAND_ECC_HW_NULL
#define FS_NAND_ECC_1BIT                                FS_NAND_ECC_SW_1BIT
#define FS_NAND_UNI_SetOnFatalErrorCB(pfOnFatalError)   FS_NAND_UNI_SetOnFatalErrorCallback(pfOnFatalError)
#define NAND_BLOCK_TYPE_UNKNOWN                         FS_NAND_BLOCK_TYPE_UNKNOWN
#define NAND_BLOCK_TYPE_BAD                             FS_NAND_BLOCK_TYPE_BAD
#define NAND_BLOCK_TYPE_EMPTY                           FS_NAND_BLOCK_TYPE_EMPTY
#define NAND_BLOCK_TYPE_WORK                            FS_NAND_BLOCK_TYPE_WORK
#define NAND_BLOCK_TYPE_DATA                            FS_NAND_BLOCK_TYPE_DATA

/*********************************************************************
*
*       Journaling add-on
*/
#define FS_CreateJournal(sVolumeName, NumBytes)         FS_JOURNAL_Create(sVolumeName, NumBytes)
#define FS_BeginTransaction(sVolumeName)                FS_JOURNAL_Begin(sVolumeName)
#define FS_EndTransaction(sVolumeName)                  FS_JOURNAL_End(sVolumeName)

/*********************************************************************
*
*       Disk checking
*/
#define FS_QUERY_F_TYPE  FS_ON_CHECK_DISK_ERROR_CALLBACK
#define FS_EFS_CheckDisk(sVolumeName, pBuffer, BufferSize, MaxRecursionLevel, pfOnError)  FS_CheckDisk(sVolumeName, pBuffer, BufferSize, MaxRecursionLevel, pfOnError)
#define FS_FAT_CheckDisk(sVolumeName, pBuffer, BufferSize, MaxRecursionLevel, pfOnError)  FS_CheckDisk(sVolumeName, pBuffer, BufferSize, MaxRecursionLevel, pfOnError)
#define FS_EFS_CheckDisk_ErrCode2Text(ErrCode)                                            FS_CheckDisk_ErrCode2Text(ErrCode)
#define FS_FAT_CheckDisk_ErrCode2Text(ErrCode)                                            FS_CheckDisk_ErrCode2Text(ErrCode)

/*********************************************************************
*
*       Space information functions
*/
#define FS_GetFreeSpace(sVolume)  FS_GetVolumeFreeSpace(sVolume)
#define FS_GetTotalSpace(sVolume) FS_GetVolumeSize(sVolume)

#if defined(__cplusplus)
  }              /* Make sure we have C-declarations in C++ programs */
#endif

#endif // __FS_H__

/*************************** End of file ****************************/
