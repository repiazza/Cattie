# Checking if the user is root
if [[ $EUID -ne 0 ]]; then
  # Checking if the terminal support colored text
  if test -n $(tput colors) && test $(tput colors) -ge 8; then
    printf "\033[1;91mE:\033[m You are not root to use this script\n"
  else
    printf "E: You must be a root to use this script\n"
  fi

  printf "Please, type sudo ./mkinstall_linux.sh\n"
  
  exit 1
fi

make install LINUX=1

