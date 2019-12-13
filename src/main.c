/*! \file main.c
    \brief Working implementation of the Embedded Systems project

	Features:

	\* Seven-segment display support, allowing to display integer numeric values in it.

	\* Potentiometer support with up to 16 bit precision.

	\* Numeric keyboard support.

	\* Main program that connects the functionality for testing purposes
*/

/*! @defgroup hc12-lib  hc12-lib

  Embedded Systems project library made in class

  \date 2019-12-13

 */
 /*@{*/
#include <types.h>
#include <es.h>
#include <timer.h>
#include <atd_lib.h>
#include <pwm.h>

#define COLUMNA_UNO 4
#define COLUMNA_DOS 6
#define COLUMNA_TRES 2

#define FILA_UNO 5
#define FILA_DOS 0
#define FILA_TRES 1
#define FILA_CUATRO 3
#define NINGUNA_TECLA_PULSADA 4

/** Selects which digit from the number array is going to 
 * be displayed in the seven-segment. */
uint8_t display_digit = 0;
/** Contains the digit to display in the seven-segment. */
uint8_t number[4];
/** Represents the numeric keyboard. */
char matrix_teclado[3][4];
/** Represents the timeout state for the 
 * teclado_getch_timeout function. */
uint8_t boolean_timeout = 0;

/** Port where the display is connected to. */
char display_port = 'G';
/** Port where the numeric keyboard is connected to. */
char teclado_port = 'H';

/** Initialises the seven-segment module configuring
 *  display_port as an output interface. */
void sieteSeg_init(){
    //incializamos el puerto de salida display_port
    e_s_total(display_port,255);
}

/** Displays a 4-byte numeric array integer values in the seven-segment.
 *
 * It is important that the array passed to this function as an argument is at least 4 bytes wide.
 *
 * @param value Pointer to the 8-bit integer array.
*/
void sieteSeg_digitos(uint8_t* value){
    uint8_t i;
    uint8_t mask = (1 << 7);//10000000
    for (i=0;i<4;i++){
        number[i] = value[i];
        number[i] |= mask;
        mask = mask >> 1;
    }
}

/** Displays a value in the seven-segment.
 *
 * @param value 16-bit unsigned integer value to display.
*/
void sieteSeg_valor(uint16_t value){
    int8_t i = 3;
    uint8_t mask = (1 << 4);
    while(i >= 0){
        number[i] = value % 10;
        number[i] |= mask;
        value /= 10;
        mask = mask << 1;
        i--;
    }
}

/** Updates the values displayed in the seven-segment module. */
void update_siete_seg(){
    if(display_digit > 3){
        display_digit = 0;
    }
    escribir_puerto(display_port,number[display_digit]);
    display_digit++;
}

/** Initialises the potentiometer. */
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

/** Retrieves a 16-bit unsigned integer value from the potentiometer output port. 
 *
 * @return 16-bit, unsigned integer value that represents the potentiometer position.
 *
*/
uint16_t get_potenciometro() {
	uint8_t port = 0;
	uint16_t poten[4];
	uint16_t ret = 0;

	if (atd_devuelveValores(port, poten, 4) == 1) {
		// Convert to uint16_t
		for (uint8_t i = 0; i < 4; i++) {
			//serial_printbinword(poten[i]);
			ret += poten[i] >> 6;
		}
		ret >>= 2;
	}
	return ret;
}

/** Permutes the column values based on the received input.
 *
 * @param pin Value that multiplexes the control flow. To
 * make effect, it has to be between 0 and 3.
*/
void set_teclado_scan_out(uint8_t pin){
    switch (pin){
        case 0:
            escribir_pin(teclado_port,COLUMNA_UNO,0);   
            escribir_pin(teclado_port,COLUMNA_DOS,1);
            escribir_pin(teclado_port,COLUMNA_TRES,1);
            break;
        case 1:
            escribir_pin(teclado_port,COLUMNA_UNO,1);   
            escribir_pin(teclado_port,COLUMNA_DOS,0);
            escribir_pin(teclado_port,COLUMNA_TRES,1);
            break;
        case 2:
            escribir_pin(teclado_port,COLUMNA_UNO,1);   
            escribir_pin(teclado_port,COLUMNA_DOS,1);
            escribir_pin(teclado_port,COLUMNA_TRES,0);
            break;
        case 3:
            escribir_pin(teclado_port,COLUMNA_UNO,0);   
            escribir_pin(teclado_port,COLUMNA_DOS,0);
            escribir_pin(teclado_port,COLUMNA_TRES,0);
            break;
    }
}

/** Initialises the numeric keyboard. */
void teclado_init(){
    //configuramos puerto T como s,s,e,s,e,s,e,e
    e_s_total(teclado_port,212);
    //configuramos pullups
    ad_pullup(teclado_port,1);
    //ponemos a 0 los bits de salida
    set_teclado_scan_out(3);
    //mapeamos teclado
    //1   2   3
    //4   5   6
    //7   8   9
    //*   0   #
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

/** Gets which row is being pressed at the time.
 *
 * @return Unsigned integer value representing that row.
*/
uint8_t get_teclado_inputs(){
    if (leer_pin(teclado_port,FILA_UNO) == 0){
        return 0;
    }else if(leer_pin(teclado_port,FILA_DOS) == 0){
        return 1;
    }else if(leer_pin(teclado_port,FILA_TRES) == 0){
        return 2;
    }else if(leer_pin(teclado_port,FILA_CUATRO) == 0){
        return 3;
    }
    return NINGUNA_TECLA_PULSADA; //sin tecla pulsada
}

/** Gets which key is being pressed at the time.
 *
 * @return char with the key value.
*/
char teclado_getch(){
    uint8_t column,row;
    //comprobamos si ya hay una tecla pulsada, si es así esperamos a que se suelte
    set_teclado_scan_out(3);
    while (get_teclado_inputs() != NINGUNA_TECLA_PULSADA);
    delayms(20);
    //comprobamos el teclado hasta que haya una pulsación
    do{
        column = get_teclado_inputs();
    }while (column == NINGUNA_TECLA_PULSADA);
    delayms(20);
    column = get_teclado_inputs();
    for(int i = 0;i<3;i++){
        set_teclado_scan_out(i);
        if (get_teclado_inputs() != NINGUNA_TECLA_PULSADA){
            row = i;
            break;
        }
    }
    set_teclado_scan_out(3); //devolvemos todas las columnas a 0 para poder detectar nuevas pulsaciones
    return matrix_teclado[row][column];
}

/** Sets the boolean timeout for the teclado_getch_timeout function. */
void timeout(){
    boolean_timeout = 1;
}

/** Gets which key is being pressed at the time. If it takes too long, it
 * times out, sending the character 'T'.
 *
 * @param milis 32-bit unsigned integer that contains the milliseconds before timeout.
 *
 * @return char with the key value, or 'T' if it times out.
*/
char teclado_getch_timeout(uint32_t milis){
    future_f(&timeout,milis*1000);
    uint8_t column,row; 
    //comprobamos si ya hay una tecla pulsada, si es así esperamos a que se suelte
    set_teclado_scan_out(3);
    while (get_teclado_inputs() != NINGUNA_TECLA_PULSADA || boolean_timeout);
    if (boolean_timeout){
        return 'T';
    }
    delayms(20);         
    //comprobamos el teclado hasta que haya una pulsación
    do{                  
        column = get_teclado_inputs();
    }while (column == NINGUNA_TECLA_PULSADA || boolean_timeout);
    if (boolean_timeout){
        return 'T';
    }
    delayms(20);         
    column = get_teclado_inputs();
    for(int i = 0;i<3;i++){
        set_teclado_scan_out(i);
        if (get_teclado_inputs() != NINGUNA_TECLA_PULSADA){
            row = i;  
            break;       
        }                
    }                    
    set_teclado_scan_out(3); //devolvemos todas las columnas a 0 para poder detectar nuevas pulsaciones
    return matrix_teclado [row][column];
}

/** Converts a number stored as an numeric array into a 8-bit unsigned integer.
 *
 * Example: [1, 2, 3] --> 123
 *
 * @param value Pointer to array to be processed.
 *
 * @return 8-bit unsigned integer obtained from processing the array.
*/
uint8_t array_to_uint(uint8_t* value){
    uint8_t temp = 0;
    uint8_t multiplier = 1;
    for (uint8_t i = 0;i<4;i++){
        temp += value[i] * multiplier;
        multiplier *= 10;
    }
    return temp;
}

/** Right-shifts 4-byte array elements and assigns a new value to
 * the less significant element.
 *
 * @param value Pointer to array to be processed.
 *
 * @param new_value char value to set in the less significant element.
 *
*/
void set_shifted_value(uint8_t* value,char new_value){
    for(uint8_t i = 3;i>0;i--){
        value[i] = value[i-1];
    }
    value[0] = new_value- '0';
}

/** Initialises the motor or fan. */
void motor_init(){
    pwmBase(0, 0, 0, 1, 100, 15, 0);
}

/** Establishes the motor speed through the pwm interface. 
 *
 * @value speed 8-bit unsigned integer to assign as the new speed value.
 *
*/ 
void set_motor_speed(uint8_t speed){
    cambiaPWDTYporciento(speed);
}

/** Copies a 4-byte array.
 *
 * @value to Pointer to copy array.
 * @value from Pointer to array to be copied.
 *
*/ 
void copyto(uint8_t* to, uint8_t* from){
    for (uint8_t i = 0;i < 4;i++){
       to[i] = from[i]; 
    }
}

/** Main program loop.
 *
 * Reads a number from keyboard-based input and displays it in a seven-segment array, causing a small fan to rotate with a speed value proportional to the keyboard input.
 *
 * @return Returns an error code (more than 0) if the program does not exit gracefully. 
*/
int main(){
    uint16_t i = 0;
    uint8_t keyboard_input[4],temp[4];
    char value;
    keyboard_input[3] = 0;
    for (uint8_t j = 0; j<4;j++){
         temp[j] = 0;
         keyboard_input[j] = 0;
    }
    serial_init();
    serial_print("\nInicializado");
    sieteSeg_init();
	teclado_init();
    initialize(); //initializes timer
    periodic_f(&update_siete_seg,2500);
    motor_init();
    sieteSeg_digitos(temp);
    while(1){
        value = teclado_getch();
        //si se empieza a recibir un valor por teclado
        //ponemos temp a 0 para poder llenarlo con el 
        //valor que recibimos
        if(i == 0){
            for(uint8_t j = 0;j<4;j++){
                temp[j] = 0;
            }
        }
        if (value == '#'){//cancelar
            copyto(temp,keyboard_input);//devolvemos el display a su valor original
            i = 0;
        }else if (value == '*'){//aceptar
            copyto(keyboard_input,temp);//hacemos un "backup" del valor del display
            //ponemos el motor a esa velocidad
            set_motor_speed(array_to_uint(temp));
            i = 0;
        } else if (i>3){//demasiados caracteres
            i = 0;
            copyto(temp,keyboard_input);//devolvemos el display a su valor original
        }else if ((array_to_uint(temp)*10) +(value -'0') > 100){//comprobar si el valor está entre 0 y 100
            i = 0;
            copyto(temp,keyboard_input);//devolvemos el display a su valor original
        }else{
            set_shifted_value(temp,value);//desplazamos los caracteres a la izquierda y ponemos el dígito introducido por teclado en el menos significativo
            i++;
        }
		sieteSeg_valor(array_to_uint(temp));//imprimimos temp
    }
}

 /*@}*/
