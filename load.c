/*
Nicolas Cifuentes Barriga - Juan Sebastian Carrillo Quiñones - Juan Camilo Peña Espinosa
Sistemas embebidos
Laboratorio 1: Equalizador WAV
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct wav {
    unsigned char texts[4];
    uint32_t file_size;
    uint32_t chunk;
    uint16_t format_type;
    uint16_t channels;
    uint32_t sample_frequency;
    uint32_t bytes_sec;
    uint16_t block_alignment;
    uint16_t bits_Sample;
    uint32_t data_chunk;
};

char* data;
char* name;

long int size(char name[]) {

    FILE* file = fopen(name,"rb");
    
    if (file == NULL) {
        printf("Not file\n");
        return -1;
    }

    fseek(file,0,SEEK_END);
    unsigned long int size = ftell(file);

    fclose(file);

    if (size != -1) {
        printf("Bytes: %ld\n",size);
    } else {
        exit(-1);
    }

    return size;
}

void print_bytes() {
    FILE* file = fopen(name,"rb");
    struct wav wav;

    fread(&wav.texts,4,1,file);
    printf("Riff: %c%c%c%c\n",wav.texts[0],wav.texts[1],wav.texts[2],wav.texts[3]);
    fread(&wav.file_size,4,1,file);
    printf("Size of file: %d\n",wav.file_size);
    fread(&wav.texts,4,1,file);
    printf("Wave: %c%c%c%c\n",wav.texts[0],wav.texts[1],wav.texts[2],wav.texts[3]);
    fread(&wav.texts,4,1,file);
    printf("Fmt: %c%c%c%c\n",wav.texts[0],wav.texts[1],wav.texts[2],wav.texts[3]);
    fread(&wav.chunk,4,1,file);
    printf("Chunk: %d\n",wav.chunk);
    fread(&wav.format_type,2,1,file);
    printf("Format type: %d\n",wav.format_type);
    fread(&wav.channels,2,1,file);
    printf("Channel: %d\n",wav.channels);
    fread(&wav.sample_frequency,4,1,file);
    printf("Sample frequency: %d\n",wav.sample_frequency);
    fread(&wav.bytes_sec,4,1,file);
    printf("Bytes per second: %d\n",wav.bytes_sec);
    fread(&wav.block_alignment,2,1,file);
    printf("Block alingment: %d\n",wav.block_alignment);
    fread(&wav.bits_Sample,2,1,file);
    printf("Bits per sample: %d\n",wav.bits_Sample);
    fread(&wav.data_chunk,4,1,file);
    printf("Data chunk: %d\n",wav.data_chunk);

    fclose(file);
}

void menu () {
    int option;
    while (option != 3) {
        printf("--------------------\nIngrese una opcion:\n1 - Adelantar\n2 - Atrasar\n3 - Salir\n");
        scanf("%d",&option);
        switch (option){
        case 1:
            printf("AVANZAR\n");
            break;
        case 2:
            printf("RETROCEDER\n");
            break;
        case 3:
            printf("HASTA LUEGO!\n");
            break;
        default:
            printf("Opcion invalida\n");
        }
    }
    exit(-1);
}

int main (int argc, char* argv[]) {

    name = argv[1];
    printf("%s\n",name);

    long int bytes_size = size(name);
    print_bytes();

    data = malloc(bytes_size);

    //menu();

    return 0;
}