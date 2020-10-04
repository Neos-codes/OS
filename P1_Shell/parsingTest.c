#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){

  char *str, *sep;

  str = strdup("Wena los k me llamo Fabian");   // Copiar mensaje en "str"
  printf("%s\n", str);

  int count = 0, words = 0;
  while(str[count] != '\0'){
    if(str[count] == ' ')
      words++;
    count++;
  }
  words++;
  printf("Palabras: %d\n", words);
  
  while((sep = strsep(&str, " ")) != NULL){        // Separar palabras
    printf("%s\n", sep);
  }
  
  
  return 0;
}
