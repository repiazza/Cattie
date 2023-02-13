#ifndef _CATTIE_H_INC
  #include <cattie.h>
#endif
#ifndef _BOARD_H_INC
  #include <board.h>
#endif

#define _MAX_MOV_ACTION BOARD_COLS*BOARD_ROWS*10

extern int gbACTION_Check;
extern int giACTION_List[_MAX_MOV_ACTION];
extern int giACTION_StepCtr;
extern int giACTION_AssertedSteps;

int iACTION_ExecuteStep(int iStep){
  int iRsl = 0;
  if ( iStep < 0 )
    return 0;

  if ( iStep >= _MAX_MOV_ACTION )
    return 0;

  switch(giACTION_List[iStep]){
  case FORWARD:
    iRsl = iWalk();
    break;
  case TURN:
    iRsl = iTurn();
    break;
  case FIRE_LASER:
    break;
  default:
    break;
  }

  return iRsl;   
}

void vACTION_TraceList(){

  int ii;
  
  vTraceMsg("==========Action List========");
  
  for ( ii=0; ii<giACTION_StepCtr; ii++){
    char szWrk[32];
    memset(szWrk, 0, sizeof(szWrk));
    sprintf(szWrk, "[%d] ", giACTION_List[ii]);
    if ( (ii % 20) == 0 )
      vTraceMsg("\n\t");

    vTraceMsg(szWrk);
  }

  vTraceMsg("\n========================\n");

}

int iACTION_AddStep2List(int iCmd){
  if ( iCmd == ERASE ){
    if ( giACTION_StepCtr <= 0 )
      return 0;

    giACTION_List[giACTION_StepCtr--] = 0;
    return 0;
  }
  if ( iCmd == CONFIRM ){
    giACTION_List[giACTION_StepCtr] = -1;
  }
  
  if ( giACTION_StepCtr >= _MAX_MOV_ACTION - 1 )
    return -1;

  giACTION_List[giACTION_StepCtr++] = iCmd;

  return 0;
}