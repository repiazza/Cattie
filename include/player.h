/**
 * player.h
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com> in March 2024
 *
 * Player data structures and score evaluation. 
 *
 */

#ifndef _PLAYER_H_
  #define _PLAYER_H_

  #include <consts.h>
  #include <util.h>

  #define PLAYERS_FILE "players.dat"
  #define SCORE        100 // pontucao para cada fase

  typedef struct STRUCT_CATTIE_PLAYER {
    char szName[256];
    int iScore;
  } STRUCT_CATTIE_PLAYER, *PSTRUCT_CATTIE_PLAYER;

  extern STRUCT_CATTIE_PLAYER gstCattiePlayer;
  extern char gszPlayersFilePath[256];

  /**
   * Set the path of the file players.dat
   */
  void vSetPlayerFilePath( const char *kpszPath );

  void vUpdatePlayerScore( void );

  int iGetPlayerScore( void );

  void vSetPlayerName( const char *kpszName );

  /**
   * Register a score of a player in players.dat
   */
  int iRegisterPlayer( void );

  /**
   * Remove a player from players.dat
   */
  int iRemovePlayer( void );

#endif /* _PLAYER_H_ */

