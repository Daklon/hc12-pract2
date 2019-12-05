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
        mas = mask >> 1;
    }
}

void sieteSeg_valor(uint16_t value){
    int16_t i = 3;
    uint8_t mask = (1 << 7);
    while(i >= 0){
        number[i] = value % 10;
        number[i] |= mask;
        value /= 10;
        i--;
    }
    sieteSeg_digitos(number);
}

void update_siete_seg(){
    if(display_digit > 3){
        display_digit = 0;
    }
    escribir_puerto('G',number[display_digit]);
    display_digit++;
}

void potenciometro_init() {
	// Utilizaremos el puerto de salida 'T' en modo input
	e_s_total('T', 0);
}

uint8_t get_potenciometro() {
	/*
	uint8_t valor = leer_puerto('G');
	uint8_t *ret[4]; 
	for (uint8_t i = 0; i < 4; i++) {
		ret[i] = valor >>= 8;
	}
	return ret;
	*/
	return leer_puerto('T');
}

int main(){
    uint16_t i = 0;
	uint8_t potval = 0;
    serial_init();
    serial_print("\nInicializado");
    serial_recv();
    sieteSeg_init();
	potenciometro_init();
    initialize(); //initializes timer
    periodic_f(&update_siete_seg,2500);
    while(1){
        sietesSeg_valor(i);
        delayms(500);
        i++;

		// Potenciometro
		potval = get_potenciometro();
		serial_print("\nPotenciometro: \n");
		serial_printdecbyte(potval);
		sieteSeg_valor(potval);
        delayms(500);
    }
}
