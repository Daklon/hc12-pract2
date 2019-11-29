#include <types.h>
#include <es.h>


void sieteSeg_init(){
    //incializamos el puerto de salida G
    e_s_total('G',255);
}

void sieteSeg_digitos(uint8_t* number){
    uint8_t i;
    uint8_t value = 128;//10000000
    for (i=0;i<4;i++){
        value |= number[i]; //ponemos la parte mas significativa, para encender el display adecuado y la parte menos significativa contiene el valor
        escribir_puerto('G',value);
        value = value >> 1;
    }
}

int main(){
    uint8_t number[4];
    number[0] = 0;
    number[1] = 1;
    number[2] = 2;
    number[3] = 3;
    serial_init();
    serial_print("\nInicializado");
    serial_recv();
    sieteSeg_init();
    while(1){
        sieteSeg_digitos(number);
    }
}
