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
  // typedef struct STRUCT_TEXTURE_LIST {
  //   SDL_Texture *pSDL_Texture;
  //   SDL_Rect *pSDL_Rect;
  //   char *pszImgPath;
  //   struct STRUCT_TEXTURE_LIST* pstNext;
  // } STRUCT_TEXTURE_LIST, *PSTRUCT_TEXTURE_LIST;
  void vRenderCmdButton(int iAction){
    if( DEBUG_MSGS ) vTraceBegin();
    if( DEBUG_MSGS ) {
      char szDbg[128];
      sprintf(szDbg, "vRenderCmdButton iAction[%d]", iAction);
      vTraceMsg(szDbg);
    }
    if( DEBUG_MSGS ) vTraceEnd();
  }
  void vUpdateCmdTmpList(int iAction, SDL_Rect *pSDL_Rect_CmdList){
    if( DEBUG_MSGS ) vTraceBegin();
    if( DEBUG_MSGS ) vRECT_Trace(pSDL_Rect_CmdList);
    switch ( iAction ){
      case FORWARD:
      case TURN:
      case FIRE_LASER:
        vRenderCmdButton(iAction);
        break;
      case ERASE:
        break;
      default:
        break;
    }
    if( DEBUG_MSGS ) vTraceEnd();
  }
  int iWasClicked(void) {
    int iRsl = MENU_OPT_NONE;
    int iX;
    int iY;
  
    if( DEBUG_MSGS ) vTraceBegin();
    
    SDL_GetMouseState( &iX, &iY );
  
    if ( (iRsl = iBUTTON_CheckInteraction(iX, iY)) != REDRAW_NONE ){
      return iRsl;
    }
  
    iRsl = iMENU_HandleMouseClick( pSDL_RECT_Menu, iX, iY, ppszMenuOpt );
  
    if( DEBUG_MSGS ) vTraceEnd();
  
    return iRsl;
  } /* iWasClicked */
  int iHandleEventKey( SDL_Event *pSDL_EVENT_Ev ) {
    int iRslAction = REDRAW_NONE;
    
    if ( iMENU_HandleKey( pSDL_EVENT_Ev->key.keysym.sym, ppszMenuOpt ) == REDRAW_IMAGE )
      return REDRAW_IMAGE;
  
    if ( DEBUG_MSGS ) vTraceBegin();
  
    switch (pSDL_EVENT_Ev->key.keysym.sym) {
      case SDLK_UP: {
        break;
      }
      case SDLK_DOWN: {
        break;
      }
      case SDLK_RETURN: {
        break;
      }
      default: break;
    }
  
    if( DEBUG_MSGS ) vTraceEnd();
  
    return iRslAction;
  } /* iHandleEventKey */
  int iHandleClick( SDL_Rect *pSDL_Rect_CmdList ) {
    int iAction = REDRAW_NONE;
  
    if( DEBUG_MSGS ) vTraceBegin();
    
    // UNUSED( pSDL_TXTR_CmdListHud );
  
    switch( ( iAction = iWasClicked() ) ) {
      case FORWARD:
      case TURN:
      case FIRE_LASER:
      case ERASE: {
        iACTION_AddStep2List( iAction );
        vUpdateCmdTmpList(iAction, pSDL_Rect_CmdList);
        if ( DEBUG_MSGS ) vACTION_TraceList();
        break;
      }
      case CONFIRM: {
        if ( DEBUG_MSGS ) vTraceMsg("Confirm!\n");
        gbACTION_Check = TRUE;
        break;
      }
      case CONFIGURE: {
        if ( DEBUG_MSGS ) vTraceMsg("Configure!\n");
        gbDrawMenu = iMENU_ToggleVisibility();
        return REDRAW_MENU_CLKD;
      }
      case MENU_OPT_1:
      case MENU_OPT_2:
      case MENU_OPT_3:
      case MENU_OPT_4:
      case MENU_OPT_5:
      case MENU_OPT_EXIT: {
        return REDRAW_MENU_CLKD;
      }
      default: break;
    }
  
    if ( DEBUG_MSGS ) vTraceEnd();
  
    return REDRAW_NONE;
  } /* iHandleClick */

  int iEVENT_HandlePollEv(SDL_Event *pSDL_EVENT_Ev, int iRedrawCurrentAction, SDL_Rect *pSDL_Rect_CmdList){
    int iRedrawReturnStatus = iRedrawCurrentAction;

    switch (pSDL_EVENT_Ev->type) {
      case SDL_QUIT: {
        iRedrawReturnStatus = REDRAW_ERROR;
        break;
      }
      case SDL_MOUSEBUTTONDOWN: {
        // TODO:
        iRedrawReturnStatus = iHandleClick( pSDL_Rect_CmdList );
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