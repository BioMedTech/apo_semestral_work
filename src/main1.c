#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int main(){
    FILE *f=fopen("sound1.wav", "rb");
    uint16_t s[80];

    fread(&s, sizeof(uint16_t), 80, f);
   
    for (int i = 0; i<80; i++){
        printf("%X ", s[i]);
    }
    fclose(f);
    return 1;
}