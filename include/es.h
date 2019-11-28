/*! @defgroup es Librería entrada/salida

 */
/*@{*/

#include <types.h>
#include <sys/param.h>
#include <sys/sio.h>
#include <sys/ports.h>


uint16_t get_ddr(char dport);

uint16_t get_port(char port);


void e_s_total(char dport,uint8_t pin);


void entrada_salida(char dport,uint8_t pin,uint8_t es);

void ad_pullup(char port, uint8_t ad);

uint8_t leer_pin(char port, uint8_t pin);

void escribir_pin(char port, uint8_t pin, uint8_t val);

uint8_t leer_puerto(char port);

void escribir_puerto(char port, uint8_t val);

/*@}*/
