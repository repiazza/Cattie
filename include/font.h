/**
 * font.h
 *
 * Written by Renato Fermi <repizza@gmail.com>
 * 
 * SDL2 TTF Handling 
 * 
 */

#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#ifndef _FONT_H_
  #define _FONT_H_

  #define PATH_TO_TTF "fonts"  
  #define TTF_FONT_TITLE "FiraCode.ttf"

  TTF_Font *ttfSDL_InitFont( void ) {
    char szFontPath[_MAX_PATH] = "";

    memset( szFontPath, 0x00, sizeof( szFontPath ) );

    if ( TTF_Init() )
      return NULL;
      
    sprintf( szFontPath, "%s/%s", PATH_TO_TTF, TTF_FONT_TITLE );
    return TTF_OpenFont( szFontPath, 16 );
  } /* ttfSDL_InitFont */
#endif /* _FONT_H_ */
