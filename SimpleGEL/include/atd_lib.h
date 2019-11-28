
/*! @defgroup atd_lib Librería gestión conversor A/D

  Conjunto de funciones para la configuración y manejo de los
  conversires A/D

  \author Alberto Hamilton Castro
  \date 2019-11-21

 */
 /*@{*/

#include <types.h>
#include <sys/param.h>
#include <sys/ports.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>

#define M6812B_CA_POS (0)
#define M6812B_SMP0_POS (5)


/** Fija el número de bits de la conversión en 8 o 10 bits.
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1
 *  @param size debe ser 8 o 10

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_setBits(uint8_t atdNum, uint8_t size);

/** Fija el número ciclos de muestreo.
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1
 *  @param ciclos de ciclos: 2, 4, 8, 16

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_setCiclos(uint8_t atdNum, uint8_t ciclos);

/** Fija el número de conversiones sucesivas
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1
 *  @param numSucesivas número de conversiones sucesivas: 1, 4, 8

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_setNumConversiones(uint8_t atdNum, uint8_t numSucesivas);

/** Fija pin para conversión para única o de inicio para multiple
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1
 *  @param inicio pin de inicio, debe ser entr 0 y 7

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_setPin(uint8_t atdNum, uint8_t inicio);

/** Fija conversión múltiple o única
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1
 *  @param multiple si != 0 fija multiple, si 0 fija simple

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_setMultiple(uint8_t atdNum, uint8_t multiple);

/** Activa, desactiva modo SCAN
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1
 *  @param scan si != 0 fija SCAN, si 0 desactiva SCAN

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_setScan(uint8_t atdNum, uint8_t scan);

/** Inicia la secuencia de conversiones.
 *
 * Activa el conversor y escribe la configuración de los distintos
 * registros.
 * Aborta la secuencia en curso (si la hay).
 *
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_iniciaConversion(uint8_t atdNum);

/** Espera a que termine conversión.
 *
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_esperaFinConversion(uint8_t atdNum);

/** Copia en `buffer` el contenido de las `n` primeras conversiones.
 *
 * El código llamante es responsable de que haya suficiente espacio
 * en el `buffer` para los `n` valores pedidos.
 *
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1
 *  @param buffer puntero a donde depositar los valores
 *  @param n número de valores solicitados.

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_devuelveValores(uint8_t atdNum, uint16_t* buffer, uint8_t n);

/** Instala función manejadorea de conversión complete.
 *
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1
 *  @param ptfun puntero a la función manejadora

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_instalaManejadora(uint8_t atdNum, void (*ptFun)(void) );

/*@}*/
