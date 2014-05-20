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
File        : GUI_SPRITE_Private.h
Purpose     : Private header file for sprites
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_SPRITE_PRIVATE_H
#define GUI_SPRITE_PRIVATE_H

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define SPRITE_LOCK_H(h) (GUI_SPRITE_OBJ *)GUI_LOCK_H(h)

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
//
// The sprite object
//
typedef struct {
  GUI_DEVICE                   * pDevice;
  GUI_RECT                       Rect;
  GUI_HMEM                       hColors;
  U16                            Flags;
  const GUI_BITMAP GUI_UNI_PTR * pBM;
  void (* pCB)(GUI_HSPRITE hSprite, int Cmd); // Callback routine for animated sprites
  GUI_HMEM                       hContext;
} GUI_SPRITE_OBJ;


#endif // GUI_SPRITE_PRIVATE_H

/*************************** End of file ****************************/
