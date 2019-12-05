#include <types.h>
#include <es.h>
#include <timer.h>

uint8_t display_digit = 0;
uint8_t number[4];

void sieteSeg_init(){
    //incializamos el puerto de salida G
    e_s_total('G',255);
}

void sieteSeg_digitos(uint8_t* value){
    uint8_t i;
    uint8_t mask = (1 << 7);//10000000
    for (i=0;i<4;i++){
        number[i] |= mask;
        mask = mask >> 1;
    }
}

void sieteSeg_valor(uint16_t value){
    int16_t i = 3;
    uint8_t mask = (1 << 7);
    while(i >= 0){
        number[i] = value % 10;
        serial_print("\n");
        serial_printdecbyte(i);
        serial_print("-");
        serial_printbinbyte(number[i]);
        number[i] |= mask;
        serial_print("-");
        serial_printbinbyte(number[i]);
        value /= 10;
        mask = mask >> 1;
        i--;
    }
    sieteSeg_digitos(number);
}

void update_siete_seg(){
    if(display_digit > 3){
        display_digit = 0;
    }
    escribir_puerto('G',number[display_digit]);
    //serial_print("\n");
    //serial_printbinbyte(number[display_digit]);
    //serial_print("-");
    //serial_printdecbyte(display_digit);
    display_digit++;
}

int main(){
    uint16_t i = 0;
    uint8_t mynumber[4];
    serial_init();
    serial_print("\nInicializado");
    serial_recv();
    sieteSeg_init();
    initialize(); //initializes timer
    mynumber[0] = 1;
    mynumber[1] = 2;
    mynumber[2] = 3;
    mynumber[3] = 4;
    serial_printbinbyte(mynumber[0]);
    serial_print("\n");
    serial_printbinbyte(mynumber[1]);
    serial_print("\n");
    serial_printbinbyte(mynumber[2]);
    serial_print("\n");
    serial_printbinbyte(mynumber[3]);
    serial_print("\n");
    periodic_f(&update_siete_seg,250);
    sieteSeg_digitos(mynumber);
    while(1){
        //sieteSeg_valor(i);
        delayms(500);
        i++;
    }
}
