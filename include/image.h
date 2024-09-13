/**
 * image.h
 *
 * Written by Renato Fermi <repizza@gmail.com>
 * 
 * Temporary path handling 
 * TODO: rework this logic.
 */

#ifndef _IMAGE_H_
  #define _IMAGE_H_
  
  #define ASSERT_IMG_PATH(ndx, ppConfigPath, ppDefaultPath) \
            bFileExist(ppConfigPath[ndx])\
              ? ppConfigPath[ndx]\
              : ppDefaultPath[ndx]
  
  #ifdef LINUX
    char *ppszInstalledImagePath[] = {
      "/usr/share/cattie/img/cat2.png",
      "/usr/share/cattie/img/forward.png",
      "/usr/share/cattie/img/laser.png",
      "/usr/share/cattie/img/rotate2.png",
      "/usr/share/cattie/img/gear.png",
      "/usr/share/cattie/img/confirm.png"
    };
  #else
    char *ppszInstalledImagePath[] = {
      "D:\\Renato\\Documents\\CSDL2\\Cattie_GXRF\\img\\cat2.png",
      "D:\\Renato\\Documents\\CSDL2\\Cattie_GXRF\\img\\forward.png",
      "D:\\Renato\\Documents\\CSDL2\\Cattie_GXRF\\img\\laser.png",
      "D:\\Renato\\Documents\\CSDL2\\Cattie_GXRF\\img\\rotate2.png",
      "D:\\Renato\\Documents\\CSDL2\\Cattie_GXRF\\img\\gear.png"
      "D:\\Renato\\Documents\\CSDL2\\Cattie_GXRF\\img\\confirm.png"
    };
  #endif

  char *ppszImagePath[] = {
    "img/cat2.png",
    "img/forward.png",
    "img/laser.png",
    "img/rotate2.png",
    "img/gear.png"
  };

  char *ppszMenuOpt[MAX_MENU_OPTIONS] = {
    "option 1",
    "option 2",
    "option 3",
    "option 4",
    "option 5",
    "Sair"
  };
  
  SDL_Surface *pSDL_SRFC_LoadImage( char *pszImgPath );

#endif /* _IMAGE_H */
  
