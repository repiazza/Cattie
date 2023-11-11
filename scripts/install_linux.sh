#!/bin/bash
# 
# install_linux.sh: Script to install cattie
# 
# Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
# 
# Date: 21/10/2023

# Checking if user is root
if [[ $EUID -ne 0 ]]; then
  # Checking if the terminal suport colored text
  if test -n $(tput colors) && test $(tput colors) -ge 8; then
    printf "\033[1;31mE:\033[m This script must be run as root\n"
  else
    printf "E: This script must be run as root\n"
  fi

  exit 1
fi

printf "\n#####################\n"
printf "# Installing cattie #\n"
printf "#####################\n"

# Creating the app directory
mkdir -pv /usr/share/cattie

# Creating the img directory of cattie
mkdir -pv /usr/share/cattie/img

# Creating the documentation directory
mkdir -pv /usr/share/doc/cattie

# Install the binary of software and yout autocomplete script
cp -rvf ./cattie /usr/bin
cp -rvf ./scripts/_cattie_complete.sh /usr/share/bash-completion/completions

# Installing the configuration file of the software
cp -rvf ./cattie.conf /etc

# Create .desktop file
cp -rvf ./scripts/cattie.desktop /usr/share/applications/

# Create the icon
cp -rvf ./img/cat.png /usr/share/pixmaps


# Copying the images
cp -rv ./img/cat2.png /usr/share/cattie/img
cp -rv ./img/forward.png /usr/share/cattie/img
cp -rv ./img/laser.png /usr/share/cattie/img
cp -rv ./img/rotate2.png /usr/share/cattie/img

# Installing the documentation
cp -rvf ./man/cattie.1 /usr/share/man/man1
gzip /usr/share/man/man1/cattie.1
#cp -rvf ./doc/* /usr/share/doc/cattie
#cp -rvf ./LICENSE.gz /usr/share/doc/cattie
#cp -rvf ./AUTHORS.gz /usr/share/doc/cattie
#cp -rvf ./NEWS.gz /usr/share/doc/cattie
#cp -rvf ./README.gz /usr/share/doc/cattie

printf "cattie was installed successfuly!\n"

