/*! @defgroup es Librería entrada/salida

  \author Karmele Bencomo Martin
  \author David Mejia Sanchez
  \date 2019-11-28

 */
/*@{*/

#include <types.h>
#include <sys/param.h>
#include <sys/sio.h>
#include <sys/ports.h>

/** Función que cambia el puerto entero a entrada o salida.
 *  @param dport es el puerto a cambiar [A-B-E-H-G-S-T].
 *  @param pin debe ser la cadena entera del puerto 01100101
*/
void e_s_total(char dport,uint8_t pin);

/** Fija el pin de entrada/salida de un puerto como entrada/salida.
 *  @param dport es el puerto a cambiar [A-B-E-H-G-S-T].
 *  @param pin debe ser el número del pin a cambiar.
 *  @param es valor a 1 si se cambia a entrada y 0 a salida.
*/
void entrada_salida(char dport,uint8_t pin,uint8_t es);

/** Activa o desactiva el pin de pullup del puerto indicado
 *  @param dport es el puerto a cambiar [A-B-E-H-G-S-T].
 *  @param ad valor a 1 si se activa y 0 si se desactiva.
*/
void ad_pullup(char port, uint8_t ad);

/** Lee el pin indicado del puerto indicado.
 *  @param dport es el puerto a cambiar [A-B-E-H-G-S-T].
 *  @param pin debe ser el número del pin a leer.
 @return el valor del bit [0-1].
*/

uint8_t leer_pin(char port, uint8_t pin);
/** Escribe el pin indicado del puerto indicado.
 *  @param dport es el puerto a cambiar [A-B-E-H-G-S-T].
 *  @param pin debe ser el número del pin a escribir.
*/

void escribir_pin(char port, uint8_t pin, uint8_t val);

/** Lee el puerto completo indicado.
 *  @param dport es el puerto a cambiar [A-B-E-H-G-S-T].
 @return el valor del puerto, cadena de 0-1.
*/
uint8_t leer_puerto(char port);
/** Lee el pin indicado del puerto indicado.
 *  @param dport es el puerto a cambiar [A-B-E-H-G-S-T].
 *  @param val debe ser la cadena de 0-1 a escribir.
*/
void escribir_puerto(char port, uint8_t val);

/*@}*/
