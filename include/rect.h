/**
 * rect.h
 *
 * Written by Renato Fermi <repiazza@gmail.com> in March 2024
 *
 * Rect handling
 *
 */
#include <consts.h>
#ifndef _RECT_H_
  #define _RECT_H_

  /**
   * Set 
  */
  int iRECT_SetDimensions( SDL_Rect *pSDL_Rect, int iX, int iY, int iWidth, int iHeight ){
    if ( pSDL_Rect == NULL )
      return FALSE;

    pSDL_Rect->x = iX;
    pSDL_Rect->y = iY;
    pSDL_Rect->w = iWidth;
    pSDL_Rect->h = iHeight;

    return TRUE;
  }

#endif