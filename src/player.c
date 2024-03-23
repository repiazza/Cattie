/**
 * player.c
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
 *
 * Description: Modulo para a pontuacao dos jogares do cattie
 *
 * Date: 2024-03-23
 */

#include <player.h>

STRUCT_CATTIE_PLAYER gstCattiePlayer;
char gszPlayersFilePath[256] = "";

void vSetPlayerFilePath( const char *kpszPath ) {
  if( !bStrIsEmpty( kpszPath ) ) {
    snprintf( gszPlayersFilePath, sizeof( gszPlayersFilePath ), "%s/%s", kpszPath, PLAYERS_FILE);
  }
  else {
    snprintf( gszPlayersFilePath, sizeof( gszPlayersFilePath ), "%s", PLAYERS_FILE);
  }
}

void vUpdatePlayerScore( void ) {
  gstCattiePlayer.iScore += SCORE;
}

int iGetPlayerScore( void ) {
  return gstCattiePlayer.iScore;
}

void vSetPlayerName( const char *kpszName ) {
  if ( bStrIsEmpty( kpszName ) ) {
    return;
  }

  snprintf( gstCattiePlayer.szName, sizeof( gstCattiePlayer.szName ), "%s", kpszName );
}

int iRegisterPlayer( void ) {
  FILE *fpPlayers = NULL;

  vSetPlayerFilePath(CATTIE_INSTALL_PATH);

  if ( !bOpenFile( &fpPlayers, gszPlayersFilePath, "a" ) ) {
    return FALSE;
  }
  
  /**
   * TODO: Pegar o nome do player antes de escrever a linha abaixo do fprintf
   */

  fprintf(fpPlayers, "%s %d", gstCattiePlayer.szName, gstCattiePlayer.iScore);

  if ( !bCloseFile( &fpPlayers ) ) {
    return FALSE;
  }

  return TRUE;
}

/**
 * TODO: Implement it
 */
int iRemovePlayer( void ) {
  return 0;
}

