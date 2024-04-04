/**
 * event.h
 *
 * Written by Renato Fermi <repiazza@gmail.com> in April 2023
 *  
 * Event Handling
 *
 */


#ifndef _EVENT_H_
  #define _EVENT_H_

  int iEVENT_HandlePollEv(SDL_Event *pSDL_EVENT_Ev, int iRedrawCurrentAction){
    int iRedrawReturnStatus = iRedrawCurrentAction;

    switch (pSDL_EVENT_Ev->type) {
      case SDL_QUIT: {
        iRedrawReturnStatus = REDRAW_ERROR;
        break;
      }
      case SDL_MOUSEBUTTONDOWN: {
        // TODO:
        // iRedrawReturnStatus = iHandleClick( pSDL_TXTR_TemporaryCmdList );
        iRedrawReturnStatus = iHandleClick( NULL );
        if ( iRedrawReturnStatus == REDRAW_MENU_CLKD )
          iRedrawReturnStatus = REDRAW_IMAGE; 
        break;
      }
      case SDL_KEYDOWN: {
        iRedrawReturnStatus = iHandleEventKey(pSDL_EVENT_Ev);
        break;
      }
      case SDL_MOUSEMOTION: {
        iRedrawReturnStatus = iHandleMouseMotion(pSDL_RECT_Menu);
        break;
      }
      default: break;
    } /* switch */
    
    return iRedrawReturnStatus;
  }

#endif /* _EVENT_H_ */