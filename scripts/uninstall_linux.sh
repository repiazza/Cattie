#!/bin/bash
# 
# uninstall_linux.sh: Script to uninstall cattie
# 
# Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
# 
# Date: 21/10/2023

# Checking if user is root
if [[ $EUID -ne 0 ]]; then
  # Checking if the terminal suport colored text
  if test -n $(tput colors) && test $(tput colors) -ge 8; then
    printf "\033[1;31mE:\033[m This script must be run as root\n"
  fi
  
  print "E: This script must be run as root\n"

  exit 1
fi

if test -f /usr/bin/cattie; then
  printf "\n#######################\n"
  printf "# Uninstalling cattie #\n"
  printf "#######################\n"

  # Removing the documentation directory
  rm -rv /usr/share/doc/cattie
  
  # Removing the cattie directory
  rm -rv /usr/share/cattie

  # Uninstall the binary of software and yout autocomplete script
  rm -rvf /usr/bin/cattie
  rm -rvf /usr/share/bash-completion/completions/_cattie_complete.sh 

  # Removing cattie.desktop file
  rm -rvf /usr/share/applications/cattie.desktop

  # Uninstalling the configuration file of the software
#  rm -rvf /etc/cattie.conf

  # Removing the icon
  rm -rvf /usr/share/pixmaps/cat.png

  # Uninstalling the documentation
  rm -rvf /usr/share/man/man1/cattie.1.gz
  rm -rvf /usr/share/doc/cattie

  printf "\ncattie was uninstalled successfuly!\n"
else
  # Checking if the terminal suport colored text
  if test -n $(tput colors) && test $(tput colors) -ge 8; then
    printf "\033[1;31mE:\033[m cattie is not installed!\n"
  else
    printf "E: cattie is not installed!\n"
  fi

  printf "Nothing to do\n"

  exit 1
fi

