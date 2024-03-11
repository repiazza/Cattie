#include <stdlib.h>
#include <font.h>
#include <cattie.h>
#include <cmdline.h>

#ifndef _MENU_H
  #define _MENU_H

  #define MAX_MENU_OPTIONS 6

  enum eMenuOption{
    MENU_OPT_1 = 0,
    MENU_OPT_2 ,
    MENU_OPT_3 ,
    MENU_OPT_4 ,
    MENU_OPT_5 ,
    MENU_OPT_EXIT  // 5
  } eMenuOption;

  extern int giMENU_SelectedItem;
  int gbDrawMenu = FALSE;

  void vInitMenu(SDL_Rect *pSDL_RECT_Mn, int iOptionCt) {
    SDL_Rect *pSDL_RECT_Wrk;
    int iInitCt = 0;
    int iMenuOptWidth = iOptionCt * 20;
    int iMenuOptHeight = iMenuOptWidth;

    for (pSDL_RECT_Wrk = pSDL_RECT_Mn; iInitCt < iOptionCt; pSDL_RECT_Wrk++, iInitCt++) {
      pSDL_RECT_Wrk->x = atoi(gstCmdLine.szWinWidth) / 2 - iMenuOptWidth;
      pSDL_RECT_Wrk->y = atoi(gstCmdLine.szWinHeight) / 2 - iMenuOptHeight;
      iMenuOptHeight -= 20;
      pSDL_RECT_Wrk->w = 100;
      pSDL_RECT_Wrk->h = 25;
    }
  }

  void vDrawMenu(SDL_Renderer *renderer, SDL_Rect *pSDL_RECT_Mn, char **ppszOptionTitle, int iOptionCt, TTF_Font *font) {
    int iInitCt = 0;
    SDL_Rect *pSDL_RECT_Wrk;
    SDL_Color textColor = {0, 0, 0, 255};

    if ( gbDrawMenu == FALSE ) 
      return ;
    for (pSDL_RECT_Wrk = pSDL_RECT_Mn; iInitCt < iOptionCt; pSDL_RECT_Wrk++, iInitCt++) {
      if (iInitCt == giMENU_SelectedItem) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      }

      SDL_RenderFillRect(renderer, pSDL_RECT_Wrk);

      // Render text on the menu option
      SDL_Surface *textSurface = TTF_RenderText_Solid(font, ppszOptionTitle[iInitCt], textColor);
      SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

      SDL_Rect textRect;
      textRect.x = pSDL_RECT_Wrk->x + (pSDL_RECT_Wrk->w - textSurface->w) / 2;
      textRect.y = pSDL_RECT_Wrk->y + (pSDL_RECT_Wrk->h - textSurface->h) / 2;
      textRect.w = textSurface->w;
      textRect.h = textSurface->h;

      SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

      SDL_FreeSurface(textSurface);
      SDL_DestroyTexture(textTexture);
    }
  }

  // Update vMENU_HandleKey function to handle text rendering
  void vMENU_HandleKey(SDL_Keycode peSDL_KeyCode, char **ppszOptionTitle) {
    if ( gbDrawMenu == FALSE ) 
      return ;
    switch (peSDL_KeyCode) {
      case SDLK_UP:
        giMENU_SelectedItem--;
        if (giMENU_SelectedItem < 0) {
            giMENU_SelectedItem = MAX_MENU_OPTIONS - 1;
        }
        break;
      case SDLK_DOWN:
        giMENU_SelectedItem++;
        if (giMENU_SelectedItem >= MAX_MENU_OPTIONS) {
            giMENU_SelectedItem = 0;
        }
        break;
      case SDLK_RETURN:
        if (giMENU_SelectedItem >= MAX_MENU_OPTIONS || giMENU_SelectedItem < 0)
          break;

        if (DEBUG_MSGS) 
          vTraceVarArgs("Pressed ENTER Menu Choice[%d]=[%s]", giMENU_SelectedItem, ppszOptionTitle[giMENU_SelectedItem]);
        
        if ( giMENU_SelectedItem == MENU_OPT_EXIT )
          gbDrawMenu = FALSE;

        // Handle menu item selection
        break;
      default:
        break;
    }
  }

  void vMENU_ToggleVisibility(){
      if ( gbDrawMenu == FALSE ) 
        gbDrawMenu = TRUE;
        
      return ;
  }

#endif /* _MENU_H */ 
