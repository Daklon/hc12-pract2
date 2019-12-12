#include <types.h>
#include <es.h>
#include <timer.h>
#include <atd_lib.h>

#define COLUMNA_UNO 4
#define COLUMNA_DOS 6
#define COLUMNA_TRES 2

#define FILA_UNO 5
#define FILA_DOS 0
#define FILA_TRES 1
#define FILA_CUATRO 3

uint8_t display_digit = 0;
uint8_t number[4];
char matrix_teclado[3][4];

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
    if (
        atd_setBits(0, 10) &&
        atd_setPin(0, 0) &&
        atd_setMultiple(0, 0) &&
        atd_setCiclos(0, 16) &&
        atd_setNumConversiones(0, 8) &&
        atd_setScan(0,1) &&
        atd_iniciaConversion(0)
        ){
            serial_print("\nConfiguración correcta\n");
        } else {
            serial_print("\nERROR Configuración. PARAMOS\n");
            while(1);
        }
}

uint16_t get_potenciometro() {
	uint8_t port = 0;
	uint16_t poten[4];
	uint16_t ret = 0;

	if (atd_devuelveValores(port, poten, 4) == 1) {
		// Convert to uint16_t
		serial_print("\nPoten:\n");
		for (uint8_t i = 0; i < 4; i++) {
			//serial_printbinword(poten[i]);
			ret += poten[i] >> 6;
		}
		ret >>= 2;
	}
	return ret;
}

void teclado_init(){
    //configuramos puerto T como s,s,e,s,e,s,e,e
    e_s_total('H',212);
    //configuramos pullups
    ad_pullup('H',1);
    //ponemos a 0 los bits de salida
    escribir_puerto('H',0);
    //mapeamos teclado
    matrix_teclado[0][0] = '1';
    matrix_teclado[0][1] = '4';
    matrix_teclado[0][2] = '7';
    matrix_teclado[0][3] = '*';
    
    matrix_teclado[1][0] = '2';
    matrix_teclado[1][1] = '5';
    matrix_teclado[1][2] = '8';
    matrix_teclado[1][3] = '0';
    
    matrix_teclado[2][0] = '3';
    matrix_teclado[2][1] = '6';
    matrix_teclado[2][2] = '9';
    matrix_teclado[2][3] = '#';
}

void set_teclado_scan_out(uint8_t pin){
    switch (pin){
        case 0:
            escribir_pin('H',COLUMNA_UNO,1);   
            escribir_pin('H',COLUMNA_DOS,0);
            escribir_pin('H',COLUMNA_TRES,0);
        case 1:
            escribir_pin('H',COLUMNA_UNO,0);   
            escribir_pin('H',COLUMNA_DOS,1);
            escribir_pin('H',COLUMNA_TRES,0);
        case 2:
            escribir_pin('H',COLUMNA_UNO,0);   
            escribir_pin('H',COLUMNA_DOS,0);
            escribir_pin('H',COLUMNA_TRES,1);
        case 3:
            escribir_pin('H',COLUMNA_UNO,0);   
            escribir_pin('H',COLUMNA_DOS,0);
            escribir_pin('H',COLUMNA_TRES,0);
    }
}

uint8_t get_teclado_inputs(){
    if (leer_pin('H',FILA_UNO) == 0){
        return 0;
    }else if(leer_pin('H',FILA_DOS) == 0){
        return 1;
    }else if(leer_pin('H',FILA_TRES) == 0){
        return 2;
    }else if(leer_pin('H',FILA_CUATRO) == 0){
        return 3;
    }
    return 4; //error
}

char teclado_getch(){
    uint8_t mask,readed,column,row;
    //comprobamos el teclado hasta que haya una pulsación
    do{
        readed = get_teclado_inputs();
    }while (readed == 4);
    
    for(int i = 0;i<3;i++){
        set_teclado_scan_out(i);
        delayms(1);//esperamos un poco para que el valor se estabilice
        if (get_teclado_inputs() > 0){
            column = i;
            row = get_teclado_inputs();
        }
    }
    set_teclado_scan_out(3); //devolvemos todas las columnas a 0 para poder detectar nuevas pulsaciones
    serial_print("\nvalues:");
    serial_printdecbyte(column);
    serial_printdecbyte(row);
    return matrix_teclado [column][row];
}

int main(){
    uint16_t i = 0, potval = 0;
	uint8_t tecladoval = 0;
    serial_init();
    serial_print("\nInicializado");
    serial_recv();
    sieteSeg_init();
	potenciometro_init();
	teclado_init();
    initialize(); //initializes timer
    periodic_f(&update_siete_seg,2500);
    while(1){
		// Potenciometro
		/*
		potval = get_potenciometro();
		serial_print("\nPotenciometro: \n");
		serial_printdecword(potval);
		sieteSeg_valor(potval);
		delayms(200);
		*/

		serial_print("\nTECLADO:\n");
		while ((tecladoval = teclado_getch()) != '#') {
		    if(tecladoval == '1'){
		        serial_print('1');
		    }else{
			    serial_print(tecladoval + '0');
            }
		}
    }
}
