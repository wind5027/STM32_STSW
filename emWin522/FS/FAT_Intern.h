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
File        : FAT_Intern.h
Purpose     : Internal FAT File System Layer header
---------------------------END-OF-HEADER------------------------------
*/

#ifndef __FAT_INTERN_H__               // Avoid recursive and multiple inclusion
#define __FAT_INTERN_H__


#include "FS_Int.h"
#include "FAT.h"


#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       #define constants
*
**********************************************************************
*/

/*********************************************************************
*
*       defines non configurable
*
**********************************************************************
*/
#define FAT_FIRST_CLUSTER         2

#define FS_FAT_DENTRY_SIZE        0x20
#define FS_FAT_DENTRY_SIZE_SHIFT  0x05

#define FS_FAT_ATTR_READ_ONLY     FS_ATTR_READ_ONLY
#define FS_FAT_ATTR_HIDDEN        FS_ATTR_HIDDEN
#define FS_FAT_ATTR_SYSTEM        FS_ATTR_SYSTEM
#define FS_FAT_ATTR_VOLUME_ID     0x08
#define FS_FAT_ATTR_ARCHIVE       FS_ATTR_ARCHIVE
#define FS_FAT_ATTR_DIRECTORY     FS_ATTR_DIRECTORY
#define FS_FAT_ATTR_LONGNAME      (FS_FAT_ATTR_READ_ONLY | FS_FAT_ATTR_HIDDEN | FS_FAT_ATTR_SYSTEM | FS_FAT_ATTR_VOLUME_ID)
#define FS_FAT_ATTR_MASK          (FS_FAT_ATTR_READ_ONLY | FS_FAT_ATTR_HIDDEN | FS_FAT_ATTR_SYSTEM | FS_FAT_ATTR_VOLUME_ID | FS_FAT_ATTR_ARCHIVE | FS_FAT_ATTR_DIRECTORY)

#define FS_FAT_MAX_DIRNAME        255

#define FS_FAT_TYPE_FAT12         12
#define FS_FAT_TYPE_FAT16         16
#define FS_FAT_TYPE_FAT32         32

#define FS_FAT_WRITE_IN_PROGRESS  0x1   // Mask of the bit set by Windows to 1 when a volume has been removed during a write operation.

//
// These flags are set by Windows if the base or the extension of a file name should be displayed in lower case.
//
#define FS_FAT_LOWER_CASE_BASE    0x08
#define FS_FAT_LOWER_CASE_EXT     0x10

#define FAT32_CLUSTER_ID_MASK     0x0FFFFFFFuL

#define INVALID_NUM_FREE_CLUSTERS_VALUE   0xFFFFFFFFUL

/*********************************************************************
*
*       defines for Boot parameter block offsets
*
**********************************************************************
*/
#define BPB_OFF_SIGNATURE          510
#define BPB_OFF_BYTES_PER_SECTOR    11
#define BPB_OFF_NUM_FATS            16
#define BPB_OFF_SECTOR_PER_CLUSTER  13
#define BPB_OFF_NUMSECTORS_16BIT    19
#define BPB_OFF_NUMSECTORS_32BIT    32
#define BPB_OFF_FATSIZE_16BIT       22
#define BPB_OFF_FATSIZE_32BIT       36
#define BPB_OFF_FAT32_EXTFLAGS      40
#define BPB_OFF_ROOTDIR_CLUSTER     44
#define BPB_OFF_FAT32_FSINFO_SECTOR 48
#define BPB_OFF_FAT32_RESERVED1     65

/*********************************************************************
*
*       defines for FAT32 FSInfo Sector offsets
*
**********************************************************************
*/
#define FSINFO_OFF_SIGNATURE_1            0
#define FSINFO_OFF_SIGNATURE_2            484
#define FSINFO_OFF_FREE_CLUSTERS          488
#define FSINFO_OFF_NEXT_FREE_CLUSTER      492
#define FSINFO_OFF_SIGNATURE_3            508

/*********************************************************************
*
*       defines for FAT32 FSInfo related values
*
**********************************************************************
*/
#define FSINFO_SIGNATURE_1                0x41615252UL
#define FSINFO_SIGNATURE_2                0x61417272UL
#define FSINFO_SIGNATURE_3                0xAA550000UL

/*********************************************************************
*
*       defines for directory entry offsets
*
**********************************************************************
*/
#define DIR_ENTRY_OFF_EXTENSION           8
#define DIR_ENTRY_OFF_ATTRIBUTES          11
#define DIR_ENTRY_OFF_RESERVED            12
#define DIR_ENTRY_OFF_SIZE                28
#define DIR_ENTRY_OFF_FIRSTCLUSTER_LOW    26
#define DIR_ENTRY_OFF_FIRSTCLUSTER_HIGH   20
#define DIR_ENTRY_OFF_CREATION_TIME       14
#define DIR_ENTRY_OFF_CREATION_DATE       16
#define DIR_ENTRY_OFF_LAST_ACCESS_DATE    18
#define DIR_ENTRY_OFF_WRITE_TIME          22
#define DIR_ENTRY_OFF_WRITE_DATE          24

#define DIR_ENTRY_SHIFT                   5

/*********************************************************************
*
*             Global data types
*
**********************************************************************
*/

/* FAT directory entry */
typedef struct {
  U8   data[FS_FAT_DENTRY_SIZE];
} FS_FAT_DENTRY;

typedef struct {
  char ac[11];
} FS_83NAME;

typedef struct {
  int             (*pfReadDirEntryInfo)(FS__DIR * pDir, FS_DIRENTRY_INFO * pDirEntryInfo, FS_SB * pSB);
  FS_FAT_DENTRY * (*pfFindDirEntry)    (FS_VOLUME * pVolume, FS_SB * pSB, const char * sLongName,  int Len, U32 DirStart, U8 AttribRequired, I32 * pLongDirEntryIndex);
  FS_FAT_DENTRY * (*pfCreateDirEntry)  (FS_VOLUME * pVolume, FS_SB * pSB, const char * pFileName, U32 DirStart, U32 ClusterId, U8 Attributes, U32 Size, U16 Time, U16 Date);
  void            (*pfDelLongEntry)    (FS_VOLUME * pVolume, FS_SB * pSB, U32 DirStart, I32 LongDirEntryIndex);
} FAT_DIRENTRY_API;


/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
extern const FAT_DIRENTRY_API   FAT_SFN_API;
extern const FAT_DIRENTRY_API   FAT_LFN_API;
extern const FAT_DIRENTRY_API * FAT_pDirEntryAPI;

/*********************************************************************
*
*             Global function prototypes
*
**********************************************************************
*/

/*********************************************************************
*
*       FAT_Misc
*/
U32             FS_FAT_ClusterId2SectorNo      (const FS_FAT_INFO * pFATInfo, U32 ClusterId);
U32             FS_FAT_FindFreeCluster         (FS_VOLUME * pVolume, FS_SB * pSB, U32 FirstCluster, FS_FILE * pFile);
void            FS_FAT_LinkCluster             (FS_VOLUME * pVolume, FS_SB * pSB, U32 LastCluster, U32 NewCluster);
char            FS_FAT_MarkClusterEOC          (FS_VOLUME * pVolume, FS_SB * pSB, U32 Cluster);
U32             FS_FAT_WalkCluster             (FS_VOLUME * pVolume, FS_SB * pSB, U32 StartCluster, U32   NumClusters,  U8 ReadFromCache);
U32             FS_FAT_WalkClusterEx           (FS_VOLUME * pVolume, FS_SB * pSB, U32 Cluster,      U32 * pNumClusters, U8 ReadFromCache);
U32             FS_FAT_AllocCluster            (FS_VOLUME * pVolume, FS_SB * pSB, U32 LastCluster, FS_FILE * pFile);
U32             FS_FAT_ReadFATEntry            (FS_VOLUME * pVolume, FS_SB * pSB, U32 ClusterId);
U32             FS_FAT_FindLastCluster         (FS_VOLUME * pVolume, FS_SB * pSB, U32 ClusterId, U32 * pNumClusters);
U32             FS_FAT_FreeClusterChain        (FS_VOLUME * pVolume, FS_SB * pSB, U32 ClusterId, U32 NumClusters);

U16             FS_FAT_GetNumAdjClustersInChain(FS_VOLUME * pVolume, FS_SB * pSB, U32 CurCluster, U8 ReadFromCache);
int             FS_FAT_GotoCluster             (const FS_FILE   * pFile,   FS_SB * pSBfat);

int             FS_FAT_GotoClusterAllocIfReq   (FS_FILE *pFile, FS_SB * pSBfat);
int             FS_FAT_AllocClusterBlock       (FS_VOLUME * pVolume, U32 FirstCluster, U32 NumClusters, FS_SB * pSB);

FS_FAT_DENTRY * FS_FAT_FindDirEntryShort       (FS_VOLUME * pVolume, FS_SB * pSB, const char *pEntryName, int Len, U32 DirStart, U8 AttrMask);
FS_FAT_DENTRY * FS_FAT_FindDirEntryShortEx     (FS_VOLUME * pVolume, FS_SB * pSB, const char *pEntryName, int Len, FS_DIR_POS * pDirPos, U8 AttrMask);
FS_FAT_DENTRY * FS_FAT_FindEmptyDirEntry       (FS_VOLUME * pVolume, FS_SB * pSB, U32 DirStart);
FS_FAT_DENTRY * FS_FAT_GetDirEntry             (FS_VOLUME * pVolume, FS_SB* pSB,  FS_DIR_POS * pDirPos);
FS_FAT_DENTRY * FS_FAT_FindDirEntry            (FS_VOLUME * pVolume, FS_SB * pSB, const char *pEntryName, int Len, U32 DirStart, U8 AttrMask, I32 * pLongDirEntryIndex);
void            FS_FAT_IncDirPos               (FS_DIR_POS * pDirPos);

char            FS_FAT_GetFATType              (U32 NumClusters);
void            FS_FAT_WriteDirEntryCluster    (FS_FAT_DENTRY * pDirEntry, U32 Cluster);
U32             FS_FAT_GetFirstCluster         (FS_FAT_DENTRY * pDirEntry);
void            FS_FAT_WriteDirEntry83         (FS_FAT_DENTRY * pDirEntry, const FS_83NAME * pFileName, U32 ClusterId, U8 Attrib, U32 Size, U16 Time, U16 Date);
void            FS_FAT_WriteDirEntryShort      (FS_FAT_DENTRY * pDirEntry, const char      * pFileName, U32 ClusterId, U8 Attrib, U32 Size, U16 Time, U16 Date);


char            FS_FAT_FindPath                (FS_VOLUME * pVolume, FS_SB * pSB,  const char *pFullName, const char * *ppFileName, U32 *pDirStart);
char            FS_FAT_DeleteFileOrDir         (FS_VOLUME * pVolume, FS_SB * pSB, FS_FAT_DENTRY  * pDirEntry, U8 IsFile);
void            FS_FAT_InitDirEntryScan        (FS_FAT_INFO * pFATInfo, FS_DIR_POS * pDirPos, U32 DirCluster);
char            FS_FAT_Make83Name              (FS_83NAME * pOutName, const char *pOrgName, int Len);

void            FS_FAT_UpdateDirEntry          (const FS_FILE_OBJ * pFileObj, FS_SB * pSB);
void            FS_FAT_SyncFAT                 (FS_VOLUME   * pVolume,  FS_SB * pSB);

/*********************************************************************
*
*       File buffer related functions
*/
#if FS_USE_FILE_BUFFER
  int FS_FAT_CleanWriteBuffer(FS_FILE * pFile, const U8 * pData, I32 NumBytesData, FS_SB * pSBData, FS_SB * pSBCrypt);
#endif

/*********************************************************************
*
*       Data sector access functions
*/
int  FS_FAT_ReadDataSector (U32 SectorNo, U32 FilePos,                                                       const FS_FILE_OBJ * pFileObj, FS_SB * pSBData, FS_SB * pSBCrypt);
int  FS_FAT_WriteDataSector(U32 SectorNo, U32 FilePos, U32 NumBytesToWrite, U32 FileSize, U8 WriteToJournal,       FS_FILE_OBJ * pFileObj, FS_SB * pSBData, FS_SB * pSBCrypt);

/*********************************************************************
*
*             FAT_Move
*/
char FS__FAT_Move(FS_VOLUME * pVolume, U32 DirStartOld, U32 DirStartNew, const char * sOldName, const char * sNewName, FS_SB * pSB);


/*********************************************************************
*
*       Directory handling
*/
int  FS_FAT_CreateDirEx(FS_VOLUME * pVolume, const char *pDirName, U32 DirStart, FS_SB * pSB);

/*********************************************************************
*
*       String handling
*/
int  FS_FAT_IsValidShortNameChar(U8 c);
void FS_FAT_CopyShortName       (char * pDest, const char* pSrc, int MaxNumBytes);
void FS_FAT_CopyDirEntryInfo    (const FS_FAT_DENTRY * pDirEntry, FS_DIRENTRY_INFO * pDirEntryInfo);

#if defined(__cplusplus)
}                /* Make sure we have C-declarations in C++ programs */
#endif

#endif // __FAT_INTERN_H__

/*************************** End of file ****************************/
