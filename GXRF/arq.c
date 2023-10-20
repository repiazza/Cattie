#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define DATA_LOCATION 5
#define ADDRESS_RANGE 0
#define PERMISSIONS 1

int main(int argc, char *argv[]){
  
  char code[] = "\x0f\x05\xc3\x58\x5e\x5b\x41\x5f";
  char path[] = "./flag.txt";
  char maps[10024];
  char c;
  char *ptok;
  unsigned long input;
  off_t offset;
  long l_size;
  FILE *mapfd;
  int i;
  int memfd;
  int index;
  int length;
  int args;

  /*
  syscall -> \x0f\x05
  ret -> \xc3
  pop rax -> \x58
  pop rsi -> \x5e
  pop rdx -> \x5b
  pop rsi; pop r15 -> \x5e\x41\x5f
  */
  
  //printf("%x\n", path[1]);
  mapfd = fopen("/proc/self/maps", "rb");
  memset(maps, 0, sizeof(maps));

  while( (c = fgetc(mapfd)) != EOF ){
    char sz[2];
    memset(sz, 0, sizeof(sz));
    sprintf(sz, "%c", c);
    strcat(maps, sz);
  }

  


  {
    FILE *pfPOpen;
    char szLine[1024];
    char *szPath = "./wrkmaps.txt";
    char szWrkCmd[2048];
    sprintf(szWrkCmd, "cat %s | cut -d ' ' -f1,2,23,24,31", szPath);
    if ((pfPOpen = popen(szPath, "w")) == NULL)
      return 0;

    fprintf(pfPOpen, "%s", maps);
    fclose(pfPOpen);
    
    if ((pfPOpen = popen(szWrkCmd, "r")) == NULL)
      return 0;
    memset(szLine, 0, sizeof(szLine));
    while ( fgets(szLine, sizeof(szLine), pfPOpen) ){
      strtok(szLine, "\n");
      printf("[%-50.50s]\n",szLine);
      memset(szLine, 0, sizeof(szLine));
    }
    pclose(pfPOpen);
  }

  // mapfd = fopen("/proc/self/maps", "rb");
  // memset(maps, 0, sizeof(maps));

  // while( (c = fgetc(mapfd)) != EOF ){
  //   char sz[2];
  //   memset(sz, 0, sizeof(sz));
  //   sprintf(sz, "%c", c);
  //   strcat(maps, sz);
  // }

  
  // ptok = strtok(maps, "\n ");
  // i = ADDRESS_RANGE;
  // while(ptok != NULL){
    
  //   if (ptok == " "){
  //     strtok(NULL, "\n ");
  //     i--;
  //     continue;
  //   }

  //   if (i >= DATA_LOCATION)
  //     i = ADDRESS_RANGE;
    
  //   //if (i == ADDRESS_RANGE)

  //   if (i == PERMISSIONS)
  //     printf("[%s]\n", ptok);

  //   // if (strchr(ptok, '\n') != NULL){
  //   //   //printf("%s\n", ptok);
  //   //   ptok = strtok(NULL, "\n  ");
  //   //   if (ptok == NULL)
  //   //     break;
  //   //   i++;
  //   //   continue;
  //   // }
  //   ptok = strtok(NULL, "\n ");
  //   i++;
  //}


  // fseek(mapfd, 0, SEEK_END);
  // l_size = ftell(mapfd);
  // rewind(mapfd);
  
  // maps = (char *)malloc(l_size);
  // fread(maps, l_size, 1, mapfd);
  //fclose(mapfd);

  printf("\nA ideia do desafio Ã© conseguir ler o arquivo ./flag.txt\nutilizando apenas uma  permissao de escrita  limitada a\nseguinte lista de bytes:\n0x0f 0x05 0xc3 0x58 0x5e 0x5b 0x41 0x5f.\n\n");
  
  /*
  Entradas:
    <endereco> <index> <tamanho>
  Exemplo:
    0x55daa6847137 3 2
    Dessa forma o programa escrevera 0x585e no endereco 0x55daa6847137
  */
  //printf("%s\n", ptok);
  
  return 0;
  // while(true){
  //   input = 0;
  //   offset = 0;
  //   index = 0;
  //   length = 0;
  //   printf(">");
  //   if(scanf("0x%llx %d %d", &offset, &index, &length) != 3) break;
  //   if(index + length > 7 || index > 7 ) break;
  //   memfd = open("/proc/self/mem", 2);
  //   lseek(memfd, offset, 0);
    
  //   for (int i = 0; i < length; ++i) {
  //     write(memfd, &code[index + i], 1);
  //   }
  //   close(memfd);
  // }
  // exit(0);
}