#include <stdlib.h>

#ifndef _CATTIE_H_INC
  #include <cattie.h>
#endif

#include <cmdline.h>

#define MAX_MENU_OPTIONS 6

extern int giMENU_SelectedItem;

void vInitMenu(SDL_Rect *pSDL_RECT_Mn, int iOptionCt){
  SDL_Rect *pSDL_RECT_Wrk;
  int iInitCt = 0;
  int iMenuOptWidth = iOptionCt*20;
  int iMenuOptHeight = iMenuOptWidth;
  for ( pSDL_RECT_Wrk = pSDL_RECT_Mn; 
        iInitCt < iOptionCt; 
        pSDL_RECT_Wrk++, iInitCt++ ){
    pSDL_RECT_Wrk->x= atoi(gstCmdLine.szWinWidth)  / 2 - iMenuOptWidth;
    pSDL_RECT_Wrk->y= atoi(gstCmdLine.szWinHeight) / 2 - iMenuOptHeight;
    iMenuOptHeight -=20;
    pSDL_RECT_Wrk->w = 100;
    pSDL_RECT_Wrk->h = 25;
  }
}

void vMENU_HandleKey(SDL_Keycode peSDL_KeyCode){
  switch (peSDL_KeyCode) {
    case SDLK_UP:
      
      giMENU_SelectedItem--;
      if (giMENU_SelectedItem < 0) {
          giMENU_SelectedItem = MAX_MENU_OPTIONS-1;
      }
      break;
    case SDLK_DOWN:
      giMENU_SelectedItem++;
      if (giMENU_SelectedItem >= MAX_MENU_OPTIONS) {
          giMENU_SelectedItem = 0;
      }
      break;
    case SDLK_RETURN:
      // Handle menu item selection
      break;
  }
}

void vDrawMenu(SDL_Renderer *renderer, SDL_Rect *pSDL_RECT_Mn, int iOptionCt){
  int iInitCt = 0;
  SDL_Rect *pSDL_RECT_Wrk;
  for ( pSDL_RECT_Wrk = pSDL_RECT_Mn; 
        iInitCt < iOptionCt; 
        pSDL_RECT_Wrk++, iInitCt++) {
    if (iInitCt == giMENU_SelectedItem) {
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    else {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }
    SDL_RenderFillRect(renderer, pSDL_RECT_Wrk);
  }
}
