/*
Nicolas Cifuentes Barriga - Juan Sebastian Carrillo Quiñones - Juan Camilo Peña Espinosa
Sistemas embebidos
Laboratorio 1: Equalizador WAV
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define PI 3.14159265358979323846

typedef struct{
    unsigned char riff[4];
    uint32_t file_size;
    unsigned char wave[4];
    unsigned char fmt[4];
    uint32_t wav_chunk;
    uint16_t format_type;
    uint16_t channels;
    uint32_t sample_frequency;
    uint32_t bytes_sec;
    uint16_t block_alignment;
    uint16_t bits_Sample;
    unsigned char data[4];
    uint32_t data_chunk;
    uint16_t *data_sound;
} WAV;

typedef struct{
    unsigned int real;
    unsigned int imaginary;    
} COMPLEX;

char* name;
WAV *wav;
unsigned int *sound_info;
COMPLEX *sound_signal;
int seconds = 0;

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

void print_bytes(long int size_file) {
    
    FILE* file = fopen(name,"rb");
    uint8_t *full_file;
    full_file = malloc(size_file);
    fread(full_file,sizeof(uint8_t),size_file,file);
    wav = (WAV*) full_file;
    
    printf("Riff: %c%c%c%c\n",wav->riff[0],wav->riff[1],wav->riff[2],wav->riff[3]);
    printf("Size of file: %d\n",wav->file_size);
    printf("Wave: %c%c%c%c\n",wav->wave[0],wav->wave[1],wav->wave[2],wav->wave[3]);
    printf("Fmt: %c%c%c%c\n",wav->fmt[0],wav->fmt[1],wav->fmt[2],wav->fmt[3]);
    printf("Chunk: %d\n",wav->wav_chunk);
    printf("Format type: %d\n",wav->format_type);
    printf("Channel: %d\n",wav->channels);
    printf("Sample frequency: %d\n",wav->sample_frequency);
    printf("Bytes per second: %d\n",wav->bytes_sec);
    printf("Block alingment: %d\n",wav->block_alignment);
    printf("Bits per sample: %d\n",wav->bits_Sample);
    printf("Data: %c%c%c%c\n",wav->data[0],wav->data[1],wav->data[2],wav->data[3]);
    printf("Data chunk: %d\n",wav->data_chunk);

    fclose(file);
}

void load_data() {
    FILE* file = fopen(name,"rb");
    uint16_t data_sound;
    fseek(file,44,SEEK_CUR);
    sound_signal = malloc(wav->data_chunk);

    for (int i = 0 ; i < wav->data_chunk ; i++) {
        data_sound = fgetc(file);
        sound_info[i] = data_sound;
        //printf("Data of bytes %d: %d\n",i,sound_info[i]);
    }
}

void fft(COMPLEX *sound, int size) {
    
    if(size == 1) return;
    COMPLEX even[size/2],odd[size/2];

    for (int i = 0 ; i < size/2 ; i++) {
        even[i] = sound[2*i];
        odd[i] = sound[2*i+1];
    }

    fft(even,size/2);
    fft(odd,size/2);

    for (int i = 0 ; i < size ; i++) {
        double t_real = cos(-2*PI*i/size) * odd[i].real - sin(-2*PI*i/size) * odd[i].imaginary;
        double t_imaginary = sin(-2*PI*i/size) * odd[i].real - cos(-2*PI*i/size) * odd[i].imaginary;
        sound[i].real = even[i].real - (int) t_real;
        sound[i].imaginary = even[i].imaginary - (int) t_imaginary;
        sound[i+size/2].real = even[i].real - (int) t_real;
        sound[i+size/2].imaginary = even[i].imaginary - (int) t_imaginary;
    }
}

void results(COMPLEX *moment){
    
}

void second(){
    COMPLEX *temporal;
    temporal = malloc(wav->bytes_sec);
    int begin = seconds * wav->bytes_sec;
    int end = (seconds + 1) * wav->bytes_sec;

    for (int i = begin ; i < end ; i++) {
        temporal[i].real = sound_signal[i].real;
        temporal[i].imaginary = sound_signal[i].imaginary;
        printf("Complejo: %d + %di\n",temporal[i].real,temporal[i].imaginary);
    }
    /*
    fft(temporal,wav->bytes_sec);
    for (int i = 0; i < wav->bytes_sec; i++) {
        double magnitude = (int) sqrt(temporal[i].real * temporal[i].real + temporal[i].imaginary * temporal[i].imaginary);
        printf("bin %d: %.2f (frequency %.2f Hz)\n", i, magnitude, i * (1.0 * wav->bytes_sec / 2) / wav->bytes_sec);
    }
    */
}

void menu () {
    int option;
    while (option != 3) {
        printf("--------------------\nIngrese una opcion:\n1 - Adelantar\n2 - Atrasar\n3 - Salir\n");
        scanf("%d",&option);
        switch (option){
        case 1:
            printf("AVANZAR\n");
            second();
            seconds++;
            break;
        case 2:
            printf("RETROCEDER\n");
            second();
            seconds--;
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

void create_complex() {
    sound_signal = malloc(wav->data_chunk);
    for (int i = 0 ; i < wav->data_chunk ; i++) {
        sound_signal[i].real = sound_info[i];
        sound_signal[i].imaginary = 0;
        //printf("Complejo: %d + %di\n",sound_signal[i].real,sound_signal[i].imaginary);
    }
}

int main (int argc, char* argv[]) {

    name = argv[1];
    printf("Nombre del archivo: %s\n",name);

    long int bytes_size = size(name);
    print_bytes(bytes_size);

    wav->data_sound = malloc(wav->data_chunk);
    sound_info = malloc(wav->data_chunk);

    load_data();
    
    create_complex();
    
    menu();

    return 0;
}