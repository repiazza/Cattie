/**
 * hud.h
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com> in March 2024
 *
 * Description: Hud module
 */

#ifndef _HUD_H_
  #define _HUD_H_
  
  typedef struct STRUCT_HUD_LIST {
    SDL_Rect *pSDL_Rect;
    char szRGBA[5];
    struct STRUCT_HUD_LIST *pstNext;
  } STRUCT_HUD_LIST, *PSTRUCT_HUD_LIST;
  
  extern STRUCT_HUD_LIST gstHudList;

  void vHUD_InitList(void) {
    memset( &gstHudList, 0x00, sizeof( STRUCT_HUD_LIST ) );
    gstHudList.pSDL_Rect = NULL;
    gstHudList.pstNext = NULL;
  }

  void vHUD_FreeList(void) {
    STRUCT_HUD_LIST *pstWrkHud;

    if ( DEBUG_MSGS ) vTraceBegin();

    for ( pstWrkHud = gstHudList.pstNext; pstWrkHud != NULL; ) {
      STRUCT_HUD_LIST *pstLastHud = pstWrkHud;

      pstWrkHud = pstWrkHud->pstNext;

      free( pstLastHud );
      pstLastHud = NULL;
    }

    if ( DEBUG_MSGS ) vTraceEnd();
  }

  void vHUD_SetRGBA(SDL_Rect *pSDL_Rect_HUD, const char *kpszRGBA) {
    PSTRUCT_HUD_LIST pstWrkHud = NULL;

    for ( pstWrkHud = &gstHudList; pstWrkHud != NULL && pstWrkHud->pSDL_Rect != pSDL_Rect_HUD; );

    if ( pstWrkHud == NULL ) return;

    snprintf(pstWrkHud->szRGBA, sizeof(pstWrkHud->szRGBA), "%s", kpszRGBA);
  }
  
  void vHUD_Draw(SDL_Renderer *renderer, STRUCT_HUD_LIST *pSDL_HUD_List) {
    SDL_Rect *pSDL_RECT_Hud = NULL;
    char *pszRGBA = NULL;

    if( DEBUG_MSGS ) vTraceBegin();
    
    if ( pSDL_HUD_List == NULL ) return;
    
    pszRGBA = pSDL_HUD_List->szRGBA;
    pSDL_RECT_Hud = pSDL_HUD_List->pSDL_Rect;

    SDL_SetRenderTarget( renderer, NULL );
    SDL_SetRenderDrawColor( renderer, (int) pszRGBA[0], (int) pszRGBA[1], (int) pszRGBA[2], (int) pszRGBA[3] );
    SDL_RenderFillRect( renderer, pSDL_RECT_Hud );
    SDL_SetRenderDrawColor( renderer, (int) pszRGBA[0], (int) pszRGBA[1], (int) pszRGBA[2], 0xFF );
    SDL_RenderDrawRect( renderer, pSDL_RECT_Hud );
    
    if( DEBUG_MSGS ) vTraceEnd();
  }

  void vHUD_DrawList(SDL_Renderer *renderer) {
    PSTRUCT_HUD_LIST pstWrkHud = NULL;

    for ( pstWrkHud = &gstHudList; pstWrkHud != NULL; ) {
      vHUD_Draw(renderer, pstWrkHud);
    }
  }

  SDL_Rect *pSDL_HUD_AddToList( SDL_Rect *pSDL_Rect, const char *kpszRGBA ) {
    STRUCT_HUD_LIST *pstWrkHud;

    if ( DEBUG_MSGS ) vTraceBegin();

    if ( pSDL_Rect == NULL ) return NULL;

    for ( pstWrkHud = &gstHudList; pstWrkHud->pstNext != NULL; pstWrkHud = pstWrkHud->pstNext );
  
    if ( pstWrkHud != &gstHudList || gstHudList.pSDL_Rect != NULL ) {
      if ( (pstWrkHud->pstNext = ( PSTRUCT_HUD_LIST ) malloc( sizeof( STRUCT_HUD_LIST ) ) ) == NULL )
        return NULL;
        
      pstWrkHud = pstWrkHud->pstNext; 
    }

    memset( pstWrkHud, 0x00, sizeof( STRUCT_HUD_LIST ) );

    snprintf(pstWrkHud->szRGBA, sizeof(pstWrkHud->szRGBA), "%s", kpszRGBA);
    pstWrkHud->pSDL_Rect = pSDL_Rect;
    pstWrkHud->pstNext = NULL;

    if ( DEBUG_MSGS ) vTraceEnd();

    return pstWrkHud->pSDL_Rect;
  } /* pSDL_HUD_AddToList */
#if 0
  void vDrawCommandHUD( SDL_Renderer *renderer, SDL_Rect *pSDL_RECT_Hud ) {
    if( DEBUG_MSGS ) vTraceBegin();

    SDL_SetRenderTarget( renderer, NULL );
    SDL_SetRenderDrawColor( renderer, 17, 84, 143, 96 );
    SDL_RenderFillRect( renderer, pSDL_RECT_Hud );
    SDL_SetRenderDrawColor( renderer, 17, 84, 143, 255 );
    SDL_RenderDrawRect( renderer, pSDL_RECT_Hud );
    
    if( DEBUG_MSGS ) vTraceEnd();
  } /* vDrawCommandHUD */

  void vDrawTmpHUD( SDL_Renderer *renderer, SDL_Rect *pSDL_RECT_Hud ) {
    if( DEBUG_MSGS ) vTraceBegin();

    SDL_SetRenderTarget( renderer, NULL );
    SDL_SetRenderDrawColor( renderer, 17, 84, 143, 96 );
    SDL_RenderFillRect( renderer, pSDL_RECT_Hud );
    SDL_SetRenderDrawColor( renderer, 17, 84, 143, 255 );
    SDL_RenderDrawRect( renderer, pSDL_RECT_Hud );
    
    if( DEBUG_MSGS ) vTraceEnd();
  } /* vDrawTmpHUD */

  void vDrawButtonHUD( SDL_Renderer *renderer, SDL_Rect *pSDL_RECT_Hud ) {
    if( DEBUG_MSGS ) vTraceBegin();

    SDL_SetRenderTarget( renderer, NULL );
    SDL_SetRenderDrawColor( renderer, 128, 4, 0, 128 );
    SDL_RenderFillRect( renderer, pSDL_RECT_Hud );
    SDL_SetRenderDrawColor( renderer, 128, 0, 0, 255 );
    SDL_RenderDrawRect( renderer, pSDL_RECT_Hud );

    if( DEBUG_MSGS ) vTraceEnd();
  } /* vDrawButtonHUD */
#endif /* 0 */
  void vSetTmpHUDRect( SDL_Rect *pSDL_RECT_Hud ) {
    if ( DEBUG_MSGS ) vTraceBegin();

    pSDL_RECT_Hud->w = 2 * atoi( gstCmdLine.szWinWidth ) / 4 - 20;
    pSDL_RECT_Hud->h = COL_RATIO - 10;
    pSDL_RECT_Hud->x = 0.06 * atoi( gstCmdLine.szWinWidth ) - 10;
    pSDL_RECT_Hud->y = atoi( gstCmdLine.szWinHeight ) - 0.06 * atoi( gstCmdLine.szWinHeight ) - ( 30 + pSDL_RECT_Hud->h );

    if ( DEBUG_MSGS ) vTraceEnd();
  } /* vSetTmpHUDRect */

  void vSetCmdHUDRect( SDL_Rect *pSDL_RECT_Hud ) {
    if ( DEBUG_MSGS ) vTraceBegin();

    pSDL_RECT_Hud->x = atoi( gstCmdLine.szWinWidth ) / 4;
    pSDL_RECT_Hud->y = 0;
    pSDL_RECT_Hud->w = 2 * pSDL_RECT_Hud->x;
    pSDL_RECT_Hud->h = COL_RATIO;

    if ( DEBUG_MSGS ) vTraceEnd();
  } /* vSetCmdHUDRect */

  void vSetButtonHUDRect( SDL_Rect *pSDL_RECT_Hud ) {
    if ( DEBUG_MSGS ) vTraceBegin();

    pSDL_RECT_Hud->x = 0.06 * atoi( gstCmdLine.szWinWidth ) - 10;
    pSDL_RECT_Hud->y = atoi( gstCmdLine.szWinHeight ) - 0.06 * atoi( gstCmdLine.szWinHeight ) - 30;
    pSDL_RECT_Hud->w = 2 * atoi( gstCmdLine.szWinWidth ) / 4 - 20;
    pSDL_RECT_Hud->h = COL_RATIO - 10;

    if( DEBUG_MSGS ) vTraceEnd();
  } /* vSetButtonHUDRect */

#endif /* _HUD_H_ */

