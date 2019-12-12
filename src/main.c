#include <types.h>
#include <es.h>
#include <timer.h>
#include <atd_lib.h>
#include <pwm.h>

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

uint8_t array_to_uint(uint8_t* value){
    uint8_t temp = 0;
    uint8_t multiplier = 1;
    for (uint8_t i = 0;i<4;i++){
        temp += value[i] * multiplier;
        multiplier *= 10;
    }
    return temp;
}

uint8_t* set_shifted_value(uint8_t* value,uint8_t new_value,uint8_t shifts){
    for(uint8_t i = 3;i>0;i--){
        value[i] = value[i-1];
    }
    value[0] = new_value;
}

void motor_init(){
    pwmBase(0, 0, 0, 1, 100, 15, 0);
}

void set_motor_speed(uint8_t speed){
    cambiaPWDTYporciento(speed);
}

int main(){
    uint16_t i = 0, potval = 0,;
    char keyboard_input[4],temp[0],value;
    keyboard_input[3] = 0;
    temp[0] = 0;
    serial_init();
    serial_print("\nInicializado");
    serial_recv();
    sieteSeg_init();
	teclado_init();
    initialize(); //initializes timer
    periodic_f(&update_siete_seg,2500);
    motor_init();
    while(1){
        value = teclado_getch();
        if (value == '#'){//cancelar
            temp = keyboard_input;
            i = 0;
        }else if (value == '*'){//aceptar
            //comprobar si el valor está entre 0 y 100
            if (array_to_uint(temp)>= 100 && array_to_uint(temp) <= 0){
                keyboard_input = temp;
                //ponemos el motor a esa velocidad
                set_motor_speed(array_to_uint(temp));
            } else {
                temp = keyboard_input;
            }
            i = 0;
        } else if (i>3){//demasiados caracteres
            i = 0;
            temp = keyboard_input;
        }else if (array_to_uint(temp) >= 100 && array_to_uint(temp) <= 0){//comprobar si el valor está entre 0 y 100
            i = 0;
            temp = keyboard_input;
        }else{
            if(i == 0){
                for(uint8_t j = 0;j<4;j++){
                    temp[0] = 0;
                }
            }
            temp = set_shifted_value(temp,value,1);
            i++;
        }
		sieteSeg_digitos(temp);
    }
}
