/**
 * menu.h
 *
 * Written by Renato Fermi <repizza@gmail.com>
 */

#include <stdlib.h>
#include <font.h>
#include <cattie.h>
#include <cmdline.h>
#include <util.h>

#ifndef _MENU_H_
  #define _MENU_H_

  #define MAX_MENU_OPTIONS 6

  typedef enum eMenuOption {
    MENU_OPT_1 = 0,
    MENU_OPT_2 ,
    MENU_OPT_3 ,
    MENU_OPT_4 ,
    MENU_OPT_5 ,
    MENU_OPT_EXIT  // 5
  } eMenuOption, *peMenuOption;

  extern int giMENU_SelectedItem;
  int gbDrawMenu = FALSE;

  void vInitMenu ( SDL_Rect *pSDL_RECT_Mn, int iOptionCt ) {
    SDL_Rect *pSDL_RECT_Wrk = NULL;
    int iInitCt = 0;
    int iMenuOptWidth = iOptionCt * 20;
    int iMenuOptHeight = iMenuOptWidth;

    for ( pSDL_RECT_Wrk = pSDL_RECT_Mn; iInitCt < iOptionCt; pSDL_RECT_Wrk++, iInitCt++ ) {
      pSDL_RECT_Wrk->x = atoi(gstCmdLine.szWinWidth) / 2 - iMenuOptWidth;
      pSDL_RECT_Wrk->y = atoi(gstCmdLine.szWinHeight) / 2 - iMenuOptHeight;
      iMenuOptHeight -= 20;
      pSDL_RECT_Wrk->w = 100;
      pSDL_RECT_Wrk->h = 25;
    } /* for */
  } /* vInitMenu */

  void vDrawMenu( SDL_Renderer *renderer, SDL_Rect *pSDL_RECT_Mn, char **ppszOptionTitle, int iOptionCt, TTF_Font *font ) {
    int iInitCt = 0;
    SDL_Rect *pSDL_RECT_Wrk = NULL;
    SDL_Color textColor = { 0, 0, 0, 255 };

    if ( gbDrawMenu == FALSE ) 
      return ;
    for ( pSDL_RECT_Wrk = pSDL_RECT_Mn; iInitCt < iOptionCt; pSDL_RECT_Wrk++, iInitCt++ ) {
      if ( iInitCt == giMENU_SelectedItem ) {
        SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
      }
      else {
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
      }

      SDL_RenderFillRect( renderer, pSDL_RECT_Wrk );

      // Render text on the menu option
      SDL_Surface *textSurface = TTF_RenderText_Solid( font, ppszOptionTitle[iInitCt], textColor );
      SDL_Texture *textTexture = SDL_CreateTextureFromSurface( renderer, textSurface );

      SDL_Rect textRect;
      textRect.x = pSDL_RECT_Wrk->x + ( pSDL_RECT_Wrk->w - textSurface->w ) / 2;
      textRect.y = pSDL_RECT_Wrk->y + ( pSDL_RECT_Wrk->h - textSurface->h ) / 2;
      textRect.w = textSurface->w;
      textRect.h = textSurface->h;

      SDL_RenderCopy( renderer, textTexture, NULL, &textRect );

      SDL_FreeSurface( textSurface );
      SDL_DestroyTexture( textTexture );
    } /* for */
  } /* vDrawMenu */

  // Update vMENU_HandleKey function to handle text rendering
  void vMENU_HandleKey( SDL_Keycode peSDL_KeyCode, char **ppszOptionTitle ) {
    if ( gbDrawMenu == FALSE ) return;

    switch ( peSDL_KeyCode ) {
      case SDLK_UP: {
        giMENU_SelectedItem--;
        if (giMENU_SelectedItem < 0) {
            giMENU_SelectedItem = MAX_MENU_OPTIONS - 1;
        }
        break;
      }
      case SDLK_DOWN: {
        giMENU_SelectedItem++;
        if (giMENU_SelectedItem >= MAX_MENU_OPTIONS) {
            giMENU_SelectedItem = 0;
        }
        break;
      }
      case SDLK_RETURN: {
        if (giMENU_SelectedItem >= MAX_MENU_OPTIONS || giMENU_SelectedItem < 0)
          break;

        if (DEBUG_MSGS) 
          vTraceVarArgs("Pressed ENTER Menu Choice[%d]=[%s]", giMENU_SelectedItem, ppszOptionTitle[giMENU_SelectedItem]);
        
        if ( giMENU_SelectedItem == MENU_OPT_EXIT )
          gbDrawMenu = FALSE;

        // Handle menu item selection
        break;
      }
      default: break;
    } /* switch */
  } /* vMENU_HandleKey */

  int iMENU_HandleMouseMotion( SDL_Rect *pSDL_RECT_Mn, int iXCursor, int iYCursor ) {
    SDL_Rect *pSDL_RECT_Wrk = NULL;
    int iInitCt = 0;

    if( DEBUG_MSGS ) vTraceBegin();

    if ( gbDrawMenu == FALSE ) return -1;

    for ( pSDL_RECT_Wrk = pSDL_RECT_Mn; iInitCt < MAX_MENU_OPTIONS; pSDL_RECT_Wrk++, iInitCt++ ) {
      if ( pSDL_RECT_Wrk == NULL )
        break;

      if ( bAreCoordsInSDL_Rect( pSDL_RECT_Wrk, iXCursor, iYCursor ) ) {
        giMENU_SelectedItem = iInitCt;

        return REDRAW_IMAGE;
      }
    }

    if( DEBUG_MSGS ) vTraceEnd();

    return 0;
  } /* iMENU_HandleMouseMotion */

  void vMENU_ToggleVisibility( void ) {
      if ( gbDrawMenu == FALSE )
        gbDrawMenu = TRUE;

      return;
  } /* vMENU_ToggleVisibility */

  int iMENU_HandleMouseClick( SDL_Rect *pSDL_RECT_Mn, int iXCursor, int iYCursor, char **ppszOptionTitle ) {
    SDL_Rect *pSDL_RECT_Wrk = NULL;
    int iInitCt = 0;

    if( DEBUG_MSGS ) vTraceBegin();

    if ( gbDrawMenu == FALSE ) return -1;

    for ( pSDL_RECT_Wrk = pSDL_RECT_Mn; iInitCt < MAX_MENU_OPTIONS; pSDL_RECT_Wrk++, iInitCt++ ) {
      if ( pSDL_RECT_Wrk == NULL ) break;

      if ( bAreCoordsInSDL_Rect( pSDL_RECT_Wrk, iXCursor, iYCursor ) ) {
        if (giMENU_SelectedItem >= MAX_MENU_OPTIONS || giMENU_SelectedItem < 0)
          break;

        if (DEBUG_MSGS)
          vTraceVarArgs("Pressed ENTER Menu Choice[%d]=[%s]", giMENU_SelectedItem, ppszOptionTitle[giMENU_SelectedItem]);

        if ( giMENU_SelectedItem == MENU_OPT_EXIT )
          gbDrawMenu = FALSE;

        return giMENU_SelectedItem;
      }
    }

    if( DEBUG_MSGS ) vTraceEnd();

    return 0;
  } /* iMENU_HandleMouseClick */

#endif /* _MENU_H_ */
