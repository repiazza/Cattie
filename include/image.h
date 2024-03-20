
#ifndef IMAGE_h_
  #define IMAGE_h_

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
      "/usr/share/cattie/img/gear.png"
    };
  #else
    char *ppszInstalledImagePath[] = {
      "C:\\Renato\\Documents\\CSDL2\\Cattie_GXRF\\img\\cat2.png",
      "C:\\Renato\\Documents\\CSDL2\\Cattie_GXRF\\img\\forward.png",
      "C:\\Renato\\Documents\\CSDL2\\Cattie_GXRF\\img\\laser.png",
      "C:\\Renato\\Documents\\CSDL2\\Cattie_GXRF\\img\\rotate2.png",
      "C:\\Renato\\Documents\\CSDL2\\Cattie_GXRF\\img\\gear.png"
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

#endif /* IMAGE_H */
  