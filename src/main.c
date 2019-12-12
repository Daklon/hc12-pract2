#include <types.h>
#include <es.h>
#include <timer.h>
#include <atd_lib.h>

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
        number[i] = value[i];
        number[i] |= mask;
        mask = mask >> 1;
    }
}

void sieteSeg_valor(uint16_t value){
    int16_t i = 3;
    uint8_t mask = (1 << 4);
    while(i >= 0){
        number[i] = value % 10;
        number[i] |= mask;
        value /= 10;
        mask = mask << 1;
        i--;
    }
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
	entrada_salida('A', 0, 0);
}

uint16_t get_potenciometro() {
	uint8_t port = leer_puerto('T');
	uint16_t poten[4];
	uint16_t ret = 0;

	if (atd_devuelveValores(port, poten, 4) == 1 || 1 == 1) {
		// Convert to uint16_t
		serial_print("\nPoten:\n");
		for (uint8_t i = 0; i < 4; i++) {
			serial_printbinword(poten[i]);
			serial_print("-");
			ret += poten[i] << i;
			serial_printbinword(ret);
			serial_print("\n");
		}
	}
	return ret;
}

int main(){
    uint16_t i = 0, potval = 0;
    serial_init();
    serial_print("\nInicializado");
    serial_recv();
    sieteSeg_init();
	potenciometro_init();
    initialize(); //initializes timer
    periodic_f(&update_siete_seg,2500);
    while(1){
        sieteSeg_valor(i);
        delayms(1500);
        i++;

		// Potenciometro
		potval = get_potenciometro();
		serial_print("\nPotenciometro: \n");
		serial_printdecbyte(potval);
		sieteSeg_valor(potval);
        delayms(500);
    }
}
