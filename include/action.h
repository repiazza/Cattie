/**
 * action.h
 *
 * Written by Renato Fermi <repizza@gmail.com>
 * 
 * Action step handling
 */

#include <cattie.h>
#include <board.h>
#include <button.h>

#ifndef _ACTION_H_
  #define _ACTION_H_

  #define _MAX_MOV_ACTION BOARD_COLS*BOARD_ROWS*10
  #define ACTIONS_PER_LINE 20
  #define FULL_TURN   360
  #define SINGLE_TURN 90

  extern int gbACTION_Check;
  extern int giACTION_List[_MAX_MOV_ACTION];
  extern int giACTION_StepCtr;
  extern int giACTION_AssertedSteps;

  int iACTION_ExecuteStep ( int iStep ) {
    int iRsl = 0;
    
    if ( iStep < 0 )
      return 0;

    if ( iStep >= _MAX_MOV_ACTION )
      return 0;

    switch ( giACTION_List[iStep] ) {
      case FORWARD: {
        iRsl = iWalk();
        break;
      }
      case TURN: {
        iRsl = iTurn();
        break;
      }
      case FIRE_LASER: {
        break;
      }
      default: break;
    }

    return iRsl;   
  } /* iACTION_ExecuteStep */

  void vACTION_TraceList( void ) {
    int ii;
    char szWrkMsg[64];
    
    memset( szWrkMsg, 0x00, sizeof ( szWrkMsg ) );

    if ( DEBUG_MSGS ) vTraceMsg("========== Action List ==========");

    for ( ii = 0; ii < giACTION_StepCtr; ii++ ) {
      char szWrk[32];

      memset(szWrk, 0, sizeof(szWrk));

      sprintf(szWrk, "[%d] ", giACTION_List[ii]);

      if ( ( ii % ACTIONS_PER_LINE ) == 0 )
        if ( DEBUG_MSGS ) vTraceMsg("\n\t");

      if ( DEBUG_MSGS ) vTraceMsg( szWrk );
    }
    memset( szWrkMsg, 0x00, sizeof ( szWrkMsg ) );
    sprintf
    (
      szWrkMsg,
      "giACTION_StepCtr=%d giACTION_AssertedSteps=%d\n",
      giACTION_StepCtr,
      giACTION_AssertedSteps
    );
      
    if ( DEBUG_MSGS ) vTraceMsg(szWrkMsg);
    if ( DEBUG_MSGS ) vTraceMsg("\n=================================");
  } /* vACTION_TraceList */

  void vACTION_InitList( void ) {
    memset( giACTION_List, 0x00, sizeof( int ) * _MAX_MOV_ACTION );
    giACTION_AssertedSteps = 0;
    giACTION_StepCtr = 0;
  } /* vACTION_InitList */

  int iACTION_AddStep2List( int iCmd ) {
    if ( DEBUG_MSGS ) vTraceBegin();

    if ( iCmd == ERASE ) {
      if ( giACTION_StepCtr <= 0 ) {
        vACTION_InitList();
        vTraceEnd();
        return 0;
      }

      if ( giACTION_StepCtr == giACTION_AssertedSteps )
        giACTION_AssertedSteps--;
      
      giACTION_List[giACTION_StepCtr--] = 0;
      if ( DEBUG_MSGS ) vTraceEnd();
      return 0;
    }
    if ( iCmd == CONFIRM ) {
      giACTION_List[giACTION_StepCtr] = -1;
    }
    
    if ( giACTION_StepCtr >= _MAX_MOV_ACTION - 1 ) {
      if ( DEBUG_MSGS ) vTraceEnd();
      return -1;
    }
    giACTION_List[giACTION_StepCtr++] = iCmd;
    
    if ( DEBUG_MSGS ) vTraceEnd();

    return 0;
  } /* iACTION_AddStep2List */

  int iACTION_CheckSteps ( int iRedrawCurrentAction ) {
    int iRedrawReturnStatus = iRedrawCurrentAction;

    if ( giACTION_AssertedSteps > 0 )
      SDL_Delay( SECOND );

    if ( giACTION_AssertedSteps >= giACTION_StepCtr ) {
      gbACTION_Check = FALSE;
      return iRedrawReturnStatus;
    }

    iRedrawReturnStatus = iACTION_ExecuteStep( giACTION_AssertedSteps++ );
    if  ( iRedrawReturnStatus == ERROR_WALKING ){
      return iRedrawReturnStatus;
    }

    if ( DEBUG_MSGS ) { 
      char szMsg[256] = "";

      memset( szMsg, 0x00, sizeof( szMsg ) );

      sprintf(szMsg,
    "iACTION_CheckSteps Redraw(b4/now)=%d/%d"
    " giACTION_AssertedSteps=%d giACTION_StepCtr=%d\n",
        iRedrawCurrentAction,
        iRedrawReturnStatus,
        giACTION_AssertedSteps,
        giACTION_StepCtr
      );

      vTraceMsg( szMsg );
    }
    return iRedrawReturnStatus;
  }

#endif /* _ACTION_H_ */
