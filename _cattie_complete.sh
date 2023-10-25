##
# _cattie_complete.sh
#
# Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
#  
# Description: My cattie file
# 
# Date: 21/10/2023
# 

_cattie_complete()
{
  local cur_word
  local prev_word
  local long_type_list
  local short_type_list

  cur_word="${COMP_WORDS[COMP_CWORD]}"
  prev_word="${COMP_WORDS[COMP_CWORD-1]}"

  long_type_list=`cattie --help --version --trace \
    --debug-level`
  short_type_list=`cattie -h -v -t -d`
  
  case "$cur_word" in
    --*) COMPREPLY=( $(compgen -W "${long_type_list}" -- ${cur_word}) );;
    -*) COMPREPLY=( $(compgen -W "${short_type_list}" -- ${cur_word}) );;
  esac

  return 0
}

complete -F _cattie_complete -o default cattie

