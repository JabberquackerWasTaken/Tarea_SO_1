#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

void crear_dir(){
    // Se crean los directorios de los jugadores, el mazo y la ultima carta.
    mkdir("Jugador1", 0700);
    mkdir("Jugador2", 0700);
    mkdir("Jugador3", 0700);
    mkdir("Jugador4", 0700);
    mkdir("Mazo", 0700);
    mkdir("Last", 0700);
}
void crear_txt(char *nombre){
    // Esta funcion crea la carta en forma de archivo txt.
    FILE *carta;
    carta = fopen(nombre, "w");
    fclose(carta);
}

void crear_mazo(){
    chdir("Mazo");
    char colores[4][20] = {"rojo", "verde", "azul", "amarillo"};
    int color, num;
    char nombre[20];
    // Creacion de los numeros del 1 al 9 en los cuatro colores.
    for(color = 0; color < 4; color++){
        for(num = 1; num < 10; num++){
            sprintf(nombre, "%d %s.txt", num, colores[color]);
            crear_txt(nombre);
            sprintf(nombre, "%d %s (2).txt", num, colores[color]);
            crear_txt(nombre);
        }
    }
    // Creacion de las cartas 0's.
    for(color = 0; color < 4; color++){
        sprintf(nombre, "0 %s.txt", colores[color]);
        crear_txt(nombre);
    }
    // Creacion de las cartas especiales.
    for(color = 0; color < 4; color++){
        sprintf(nombre, "+2 %s.txt", colores[color]);
        crear_txt(nombre);
        sprintf(nombre, "+2 %s (2).txt", colores[color]);
        crear_txt(nombre);

        sprintf(nombre, "R %s.txt", colores[color]);
        crear_txt(nombre);
        sprintf(nombre, "R %s (2).txt", colores[color]);
        crear_txt(nombre);

        sprintf(nombre, "S %s.txt", colores[color]);
        crear_txt(nombre);
        sprintf(nombre, "S %s (2).txt", colores[color]);
        crear_txt(nombre);

        sprintf(nombre, "+2 %s.txt", colores[color]);
        crear_txt(nombre);
        sprintf(nombre, "+2 %s (2).txt", colores[color]);
        crear_txt(nombre);

        sprintf(nombre, "+4 (%d).txt", color+1);
        crear_txt(nombre);

        sprintf(nombre, "Color (%d).txt", color+1);
        crear_txt(nombre);
    }
    chdir(".."); // Se sale de la carpeta "Mazo".
}

char* getCard(int cont){
    // Esta funcion retorna una carta de la carpeta "Mazo".
    DIR* dir;
    struct dirent *carpeta;
    int i = 0;
    static char carta[20];
    dir = opendir("Mazo");
    if(dir){
        while((carpeta = readdir(dir)) != NULL){
            if(carpeta->d_type == DT_REG){ // Verificacion de archivo regular.
                if(i==0){
                    strcpy(carta, carpeta->d_name);
                    chdir("Mazo");
                    remove(carta); // Se remueve la carta que se movera de la carpeta "Mazo".
                    chdir("..");
                    break;
                }
                i++;
            }
        }
        closedir(dir);
    }else{printf("No se pudo abrir\n");}
    return carta;
}
void repartir(){
    // Esta funcion reparte las cartas iniciales a cada jugador y deja una en el la carpeta "Last".
    int aleatorio, i, j, cont = 0;
    char* carta;
    srand(time(0));
    char jugador[10];
    // Se reparten 7 cartas a cada jugador.
    for(i=1; i < 5; i++){
        sprintf(jugador, "Jugador%d", i);
        for(j=0;j<7;j++){
            aleatorio = rand()%(110-cont)+1;
            carta=getCard(aleatorio); // Se busca la carta en el mazo.
            chdir(jugador);
            crear_txt(carta); // Se crea la carta en el jugadorX.
            chdir("..");
            cont++;
        }
    }
    // Se mueve una carta del mazo a la carpeta "Last".
    aleatorio = rand()%(110-cont)+1;
    carta = getCard(aleatorio);
    chdir("Last");
    crear_txt(carta);
    chdir("..");
 }

int main(){
    crear_dir();
    crear_mazo();
    repartir();
    return 0;
}