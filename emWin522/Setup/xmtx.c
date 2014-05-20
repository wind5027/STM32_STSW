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
File    : xmtx.c
Purpose : xmtx system interface -- thread locking and unlocking
          functions, adapted to embOS
--------- END-OF-HEADER ----------------------------------------------
*/

#include <yvals.h>
#include "RTOS.h"

#if _MULTI_THREAD  // Used in multi thread supported libraries only

_STD_BEGIN

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       __iar_system_Mtxinit()
*/
void __iar_system_Mtxinit(__iar_Rmtx *m) {
  OS__iar_system_Mtxinit(m);
}

/*********************************************************************
*
*       __iar_system_Mtxdst()
*/
void __iar_system_Mtxdst(__iar_Rmtx *m) {
  OS__iar_system_Mtxdst(m);
}

/*********************************************************************
*
*       __iar_system_Mtxlock()
*/
void __iar_system_Mtxlock(__iar_Rmtx *m) {
  OS__iar_system_Mtxlock(m);
}

/*********************************************************************
*
*       __iar_system_Mtxunlock()
*/
void __iar_system_Mtxunlock(__iar_Rmtx *m) {
  OS__iar_system_Mtxunlock(m);
}

_STD_END

/********************************************************************/

#endif // _MULTI_THREAD

/****** End Of File *************************************************/
