#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#define PATH_TO_TTF "fonts"  
#define TTF_FONT_TITLE "FiraCode.ttf"


TTF_Font *ttfSDL_InitFont(){
  char szFontPath[_MAX_PATH];

  if ( TTF_Init() )
    return NULL;
    
  sprintf(szFontPath, "%s/%s", PATH_TO_TTF, TTF_FONT_TITLE);
  return TTF_OpenFont(szFontPath, 16);
}