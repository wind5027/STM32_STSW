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
File        : FS_Performance.c
Purpose     : Sample program which can be used to measure the performance of emFile.

At first, the device is formatted in order to have a blank one.
After formatting the device three different test scenarios are performed:

1. Checking the write performance by writing into a file which was created with cluster pre-allocation.
   This is done in order to make sure that when writing to the file, no further
   writes to the FAT and to the directory entry are necessary which would have
   bad influence on the performance values.
   The directory entry is updated when the file is closed.

2. Checking the write performance by writing into a file which was created without cluster pre-allocation.
   The directory entry is updated when the file is closed.

3. Same as test 1. with the directory entry updated on each write

4. Same as test 2. with the directory entry updated on each write

5. Checking the read performance by reading the contents of the file that was previously created.

By default, in all three test scenarios the file is written/read in:
  16 chunks (512 KB per chunk)
  1 chunk = 64 blocks = 64 calls of FS_FWrite/FS_FRead
  1 block = 8 KB = 8 KB per write/read transaction.

  a total file size of 8 MB

Sample output:

Start
High-level format
T0: Write 16 chunks of 524288 bytes................OK
T1: Write 16 chunks of 524288 bytes................OK
T2: Write 16 chunks of 524288 bytes................OK
T3: Write 16 chunks of 524288 bytes................OK
T4: Read  16 chunks of 524288 bytes................OK
Test 0: Write, pre-allocated clusters
  Chunk (512 KB)
    Time (Min/Max/Av):   73/104/87 ms
    Speed:               5885.06 KB/s
  Block (8 KB)
    Time (Min/Max/Av):   0/6/1 ms
    Speed:               8000.00 KB/s
  Counters
    ReadOperationCnt:    0
    ReadSectorCnt:       0
    WriteOperationCnt:   0
    WriteSectorCnt:      0

Test 1: Write, clusters dynamically allocated, fast write mode
  Chunk (512 KB)
    Time (Min/Max/Av):   72/314/101 ms
    Speed:               5069.31 KB/s
  Block (8 KB)
    Time (Min/Max/Av):   1/217/1 ms
    Speed:               8000.00 KB/s
  Counters
    ReadOperationCnt:    0
    ReadSectorCnt:       0
    WriteOperationCnt:   0
    WriteSectorCnt:      0

Test 2: Write, clusters dynamically allocated, medium write mode
  Chunk (512 KB)
    Time (Min/Max/Av):   120/319/150 ms
    Speed:               3413.33 KB/s
  Block (8 KB)
    Time (Min/Max/Av):   0/66/2 ms
    Speed:               4000.00 KB/s
  Counters
    ReadOperationCnt:    0
    ReadSectorCnt:       0
    WriteOperationCnt:   0
    WriteSectorCnt:      0

Test 3: Write, clusters dynamically allocated, safe write mode
  Chunk (512 KB)
    Time (Min/Max/Av):   279/539/324 ms
    Speed:               1580.25 KB/s
  Block (8 KB)
    Time (Min/Max/Av):   3/157/5 ms
    Speed:               1600.00 KB/s
  Counters
    ReadOperationCnt:    0
    ReadSectorCnt:       0
    WriteOperationCnt:   0
    WriteSectorCnt:      0

Test 4: Read
  Chunk (512 KB)
    Time (Min/Max/Av):   72/76/73 ms
    Speed:               7013.70 KB/s
  Block (8 KB)
    Time (Min/Max/Av):   1/2/1 ms
    Speed:               8000.00 KB/s
  Counters
    ReadOperationCnt:    0
    ReadSectorCnt:       0
    WriteOperationCnt:   0
    WriteSectorCnt:      0

Test 0 Speed (chunk/block): 5885.06/8000.00 KB/s
Test 1 Speed (chunk/block): 5069.31/8000.00 KB/s
Test 2 Speed (chunk/block): 3413.33/4000.00 KB/s
Test 3 Speed (chunk/block): 1580.25/1600.00 KB/s
Test 4 Speed (chunk/block): 7013.70/8000.00 KB/s

Finished
---------------------------END-OF-HEADER------------------------------
*/

#include "FS_Int.h"
#include "FS_OS.h"
#include <string.h>
#include <stdio.h>

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
#define FILE_SIZE             (8192L * 1024L)             // Defines the file size that should be used
#define BLOCK_SIZE            (8L    * 1024L)             // Block size for individual read / write operation in bytes
#define NUM_BLOCKS_MEASURE    64                          // Number of blocks for individual measurement
#define VOLUME_NAME           ""                          // Defines the volume that should be used
#define FILE_NAME             VOLUME_NAME"\\default.txt"  // Defines the name of the file to write to

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  const char * sName;
  I32 MinChunk;
  I32 MaxChunk;
  I32 AvChunk;
  I32 SumChunk;
  I32 NumSamplesChunk;
  U32 NumBytesChunk;
  I32 MinBlock;
  I32 MaxBlock;
  I32 AvBlock;
  I32 SumBlock;
  I32 NumSamplesBlock;
  U32 NumBytesBlock;
  FS_STORAGE_COUNTERS StorageCounter;
} RESULT;

/*********************************************************************
*
*       static data
*
**********************************************************************
*/
static FS_FILE * _pFile;
static U32       _aBuffer[BLOCK_SIZE / 4];
static RESULT    _aResult[5];
static int       _TestNo;
static char      _ac[512];
static U32       _Space;
static U32       _NumLoops;
static U32       _NumBytes;
static U32       _NumBytesAtOnce;

/*********************************************************************
*
*             Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _StartTest
*/
static void _StartTest(const char * sName, U32 NumBytesChunk, U32 NumBytesBlock) {
  RESULT * pResult;

  if ((_TestNo + 1) < (int)COUNTOF(_aResult)) {
    pResult = &_aResult[++_TestNo];
    pResult->sName           = sName;
    pResult->MinChunk        =  0x7fffffff;
    pResult->MaxChunk        = -0x7fffffff;
    pResult->NumSamplesChunk = 0;
    pResult->SumChunk        = 0;
    pResult->NumBytesChunk   = NumBytesChunk;
    pResult->MinBlock        =  0x7fffffff;
    pResult->MaxBlock        = -0x7fffffff;
    pResult->NumSamplesBlock = 0;
    pResult->SumBlock        = 0;
    pResult->NumBytesBlock   = NumBytesBlock;
  }
}

/*********************************************************************
*
*       _StoreResultChunk
*/
static void _StoreResultChunk(I32 t) {
  RESULT * pResult;

  pResult = &_aResult[_TestNo];
  if (t < 0) {
    FS_X_Panic(110);
  }
  if (t > pResult->MaxChunk) {
    pResult->MaxChunk = t;
  }
  if (t < pResult->MinChunk) {
    pResult->MinChunk = t;
  }
  pResult->NumSamplesChunk++;
  pResult->SumChunk += (I32)t;
  pResult->AvChunk   = pResult->SumChunk / pResult->NumSamplesChunk;
}

/*********************************************************************
*
*       _GetAverageChunk
*/
static float _GetAverageChunk(int Index) {
  RESULT * pResult;
  float v;

  pResult = &_aResult[Index];
  v = (float)pResult->AvChunk;
  if (v == 0) {
    return 0;
  }
  v = (float)1000.0 / v;
  v = v * (pResult->NumBytesChunk / 1024);
  return v;
}

/*********************************************************************
*
*       _StoreResultBlock
*/
static void _StoreResultBlock(I32 t) {
  RESULT * pResult;

  pResult = &_aResult[_TestNo];
  if (t < 0) {
    FS_X_Panic(110);
  }
  if (t > pResult->MaxBlock) {
    pResult->MaxBlock = t;
  }
  if (t < pResult->MinBlock) {
    pResult->MinBlock = t;
  }
  pResult->NumSamplesBlock++;
  pResult->SumBlock += (I32)t;
  pResult->AvBlock   = pResult->SumBlock / pResult->NumSamplesBlock;
}

/*********************************************************************
*
*       _GetAverageBlock
*/
static float _GetAverageBlock(int Index) {
  RESULT * pResult;
  float v;

  pResult = &_aResult[Index];
  v = (float)pResult->AvBlock;
  if (v == 0) {
    return 0;
  }
  v = (float)1000.0 / v;
  v = v * (pResult->NumBytesBlock / 1024);
  return v;
}

/*********************************************************************
*
*       _WriteFile
*
*   Function description
*     Write to file and measure time it takes to complete the operation
*/
static I32 _WriteFile(const void * pData, U32 NumBytes) {
  I32 t0;
  I32 t1;
  int i;

  t0 = (I32)FS_X_OS_GetTime();
  for (i = 0; i < NUM_BLOCKS_MEASURE; i++) {
    t1 = (I32)FS_X_OS_GetTime();
    FS_Write(_pFile, pData, NumBytes);
    _StoreResultBlock(FS_X_OS_GetTime() - t1);
  }
  return FS_X_OS_GetTime() - t0;
}

/*********************************************************************
*
*       _ReadFile
*
*   Function description
*     Read from file and measure time it takes to complete the operation
*/
static I32 _ReadFile(void * pData, U32 NumBytes) {
  I32 t0;
  I32 t1;
  int i;
  t0 = (I32)FS_X_OS_GetTime();
  for (i = 0; i < NUM_BLOCKS_MEASURE; i++) {
    t1 = (I32)FS_X_OS_GetTime();
    FS_Read(_pFile, pData, NumBytes);
    _StoreResultBlock(FS_X_OS_GetTime() - t1);
  }
  return FS_X_OS_GetTime() - t0;
}

/*********************************************************************
*
*       _TestWriteWithPreAllocation
*/
static void _TestWriteWithPreAllocation(const char * sName) {
  unsigned i;
  I32      t;

  _StartTest(sName, _NumBytes, BLOCK_SIZE);
  //
  // Create file of full size
  //
  _pFile = FS_FOpen(FILE_NAME, "w");
  FS_SetFilePos(_pFile, _Space, FS_FILE_BEGIN);
  FS_SetEndOfFile(_pFile);
  FS_SetFilePos(_pFile, 0, FS_FILE_BEGIN);
  sprintf(_ac, "T%d: Write %lu chunks of %lu bytes", _TestNo, _NumLoops, _NumBytes);
  FS_X_Log(_ac);
  FS_STORAGE_ResetCounters();
  for (i = 0; i < _NumLoops ; i++) {
    t = _WriteFile(&_aBuffer[0], _NumBytesAtOnce);
    _StoreResultChunk(t);
    FS_X_Log(".");
  }
  FS_STORAGE_GetCounters(&_aResult[_TestNo].StorageCounter);
  FS_X_Log("OK\n");
  FS_FClose(_pFile);
}

/*********************************************************************
*
*       _TestWriteWithDynamicAllocation
*/
static void _TestWriteWithDynamicAllocation(const char * sName) {
  unsigned i;
  I32      t;

  _StartTest(sName, _NumBytes, BLOCK_SIZE);
  _pFile = FS_FOpen(FILE_NAME, "w");
  sprintf(_ac, "T%d: Write %lu chunks of %lu bytes", _TestNo, _NumLoops, _NumBytes);
  FS_X_Log(_ac);
  FS_STORAGE_ResetCounters();
  for (i = 0; i < _NumLoops ; i++) {
    t = _WriteFile(&_aBuffer[0], _NumBytesAtOnce);
    _StoreResultChunk(t);
    FS_X_Log(".");
  }
  FS_STORAGE_GetCounters(&_aResult[_TestNo].StorageCounter);
  FS_X_Log("OK\n");
  FS_FClose(_pFile);
}

/*********************************************************************
*
*       _TestRead
*/
static void _TestRead(const char * sName) {
  unsigned i;
  I32      t;

  _StartTest(sName, _NumBytes, BLOCK_SIZE);
  sprintf(_ac, "T%d: Read  %lu chunks of %lu bytes", _TestNo, _NumLoops, _NumBytes);
  FS_X_Log(_ac);
  _pFile = FS_FOpen(FILE_NAME, "r");
  FS_STORAGE_ResetCounters();
  for (i = 0; i < _NumLoops; i++) {
    t = _ReadFile(_aBuffer, _NumBytesAtOnce);
    _StoreResultChunk(t);
    FS_X_Log(".");
  }
  FS_STORAGE_GetCounters(&_aResult[_TestNo].StorageCounter);
  FS_X_Log("OK\n");
  FS_FClose(_pFile);
}

/*********************************************************************
*
*       _ShowResults
*/
static void _ShowResults(void) {
  unsigned i;

  //
  // Show measurement results.
  //
  for (i = 0; i <= (unsigned)_TestNo; i++) {
    sprintf(_ac, "Test %u: %s\n"
                 "  Chunk (%lu KB)\n"
                 "    Time (Min/Max/Av):   %ld/%ld/%ld ms\n"
                 "    Speed:               %4.2f KB/s\n"
                 "  Block (%ld KB)\n"
                 "    Time (Min/Max/Av):   %ld/%ld/%ld ms\n"
                 "    Speed:               %4.2f KB/s\n"
                 "  Counters\n"
                 "    ReadOperationCnt:    %lu\n"
                 "    ReadSectorCnt:       %lu\n"
                 "    WriteOperationCnt:   %lu\n"
                 "    WriteSectorCnt:      %lu\n", i, _aResult[i].sName,
                                                   ((_Space / _NumLoops) >> 10),
                                                   _aResult[i].MinChunk,
                                                   _aResult[i].MaxChunk,
                                                   _aResult[i].AvChunk,
                                                   _GetAverageChunk(i),
                                                   BLOCK_SIZE >> 10,
                                                   _aResult[i].MinBlock,
                                                   _aResult[i].MaxBlock,
                                                   _aResult[i].AvBlock,
                                                   _GetAverageBlock(i),
                                                   _aResult[i].StorageCounter.ReadOperationCnt,
                                                   _aResult[i].StorageCounter.ReadSectorCnt,
                                                   _aResult[i].StorageCounter.WriteOperationCnt,
                                                   _aResult[i].StorageCounter.WriteSectorCnt);
    FS_X_Log(_ac);
    FS_X_Log("\n");
  }
  //
  // Show summary.
  //
  for (i = 0; i <= (unsigned)_TestNo; i++) {
    sprintf(_ac, "Test %u Speed (chunk/block): %4.2f/%4.2f KB/s\n", i,
                                                                    _GetAverageChunk(i),
                                                                    _GetAverageBlock(i));
    FS_X_Log(_ac);
  }
  FS_X_Log("\n");
}

/*********************************************************************
*
*             Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       MainTask
*/
void MainTask(void);
void MainTask(void) {
  int r;

  FS_X_Log("Start\n");
  FS_Init();
  _TestNo = -1;
  if (FS_IsLLFormatted(VOLUME_NAME) == 0) {
    FS_X_Log("Low-level format\n");
    FS_FormatLow(VOLUME_NAME);
  }
  FS_X_Log("High-level format\n");
#if FS_SUPPORT_FAT
  r = FS_FormatSD(VOLUME_NAME);
#else
  r = FS_Format(VOLUME_NAME, NULL);
#endif
  if (r == 0) {
    FS_MEMSET((void*)_aBuffer, 'a', sizeof(_aBuffer));
    //
    // Get some general info
    //
    _Space          = FS_GetVolumeFreeSpace(VOLUME_NAME);
    _Space          = MIN(_Space, FILE_SIZE);
    _NumBytes       = BLOCK_SIZE * NUM_BLOCKS_MEASURE;
    _NumBytesAtOnce = BLOCK_SIZE;
    _NumLoops       = _Space / _NumBytes;
    //
    // Write to a pre-allocated file.
    //
    _TestWriteWithPreAllocation("Write, pre-allocated clusters");
    //
    // Update the directory entry and FAT only when the file is closed to speed up the write process.
    //
    FS_SetFileWriteMode(FS_WRITEMODE_FAST);
    _TestWriteWithDynamicAllocation("Write, clusters dynamically allocated, fast write mode");
    //
    // Update the directory entry only when the file is closed to speed up the write process.
    //
    FS_SetFileWriteMode(FS_WRITEMODE_MEDIUM);
    _TestWriteWithDynamicAllocation("Write, clusters dynamically allocated, medium write mode");
    //
    // Update the directory entry on every write operation.
    //
    FS_SetFileWriteMode(FS_WRITEMODE_SAFE);
    _TestWriteWithDynamicAllocation("Write, clusters dynamically allocated, safe write mode");
    //
    // Measure the read speed.
    //
    _TestRead("Read");
    //
    // Display the results on console.
    //
    _ShowResults();
  }
  FS_X_Log("Finished\n");
  FS_Unmount("");
  while (1) {
    ;
  }
}

/*************************** End of file ****************************/
