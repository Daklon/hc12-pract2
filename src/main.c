#include <types.h>
#include <es.h>

void sieteSeg_init(){
    //incializamos el puerto de salida G
    e_s_total('G','255');
}

void sieteSeg_digitos(uint8_t* number){
    uint8_t i = 0;
    uint8_t value = 128;//10000000
    uint8_t mask = 15;//00001111
    for (i;i<4;i++){
        number[i] &= ~mask;//nos aseguramos de que la parte mas significativa está a 0
        value |= number[i]; //ponemos la parte mas significativa, para encender el display adecuado y la parte menos significativa contiene el valor
    }
}

int main(){
    uint16_t i;
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
