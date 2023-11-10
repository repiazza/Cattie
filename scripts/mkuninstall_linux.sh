# Verifing if the user is root
if [[ $EUID -ne 0 ]]; then
  #Verifing if the terminal support colored text
  if test $(tput colors) -ge 8; then
    printf "\033[1;91mE:\033[m You must be a root to use this script\n"
  else
    printf "E: You must be a root to use this script\n"
  fi
  
  printf "Please, type sudo ./mkuninstall_linux.sh"

  exit 1
fi

make uninstall LINUX=1

