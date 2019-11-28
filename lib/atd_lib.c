
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

uint8_t ATDxCTL3[2] = {0 , 0};
uint8_t ATDxCTL4[2] = {7 , 7};  //Bajamos frecuencia al máximo
uint8_t ATDxCTL5[2] = {0 , 0};

void (*manejadora0)(void) = 0;
void (*manejadora1)(void) = 0;


/* Fija el número de bits de la conversión en 8 o 10 bits.
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1
 *  @param size debe ser 8 o 10

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_setBits(uint8_t atdNum, uint8_t size) {
  if (atdNum > 1) return 0;
  if (size == 8) {
    ATDxCTL4[atdNum] &= ~M6812B_RES10;
  } else if (size == 10) {
    ATDxCTL4[atdNum] |= M6812B_RES10;
  } else
    return 0;
  return 1;
}

/* Fija el número ciclos de muestreo.
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1
 *  @param ciclos de ciclos: 2, 4, 8, 16

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_setCiclos(uint8_t atdNum, uint8_t ciclos) {
  if (atdNum > 1) return 0;
  uint8_t bits = 0;
  switch (ciclos) {
    case 2: bits = 0; break;
    case 4: bits = 1; break;
    case 8: bits = 2; break;
    case 16: bits = 3; break;
    default: return 0;
  }
  ATDxCTL4[atdNum] &= ~(M6812B_SMP0 | M6812B_SMP1);
  ATDxCTL4[atdNum] |= bits << M6812B_SMP0_POS;
  return 1;
}

/* Fija el número de conversiones sucesivas
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1
 *  @param numSucesivas número de conversiones sucesivas: 1, 4, 8

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_setNumConversiones(uint8_t atdNum, uint8_t numSucesivas) {
  if (atdNum > 1) return 0;
  switch (numSucesivas) {
    case 1:
      ATDxCTL5[atdNum] &= ~(M6812B_S8C);
      ATDxCTL3[atdNum] |= M6812B_S1C;
      break;
    case 4:
      ATDxCTL5[atdNum] &= ~M6812B_S8C;
      ATDxCTL3[atdNum] &= ~M6812B_S1C;
      break;
    case 8:
      ATDxCTL5[atdNum] |= M6812B_S8C;
      break;
    default: return 0;
  }
  return 1;
}

/* Fija pin para conversión para única o de inicio para multiple
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1
 *  @param inicio pin de inicio, debe ser entr 0 y 7

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_setPin(uint8_t atdNum, uint8_t inicio) {
  if (atdNum > 1) return 0;
  if (inicio > 7) return 0;
  ATDxCTL5[atdNum] &= ~(M6812B_CC | M6812B_CB | M6812B_CA);
  ATDxCTL5[atdNum] |= (inicio << M6812B_CA_POS);
  return 1;
}

/* Fija conversión múltiple o única
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1
 *  @param multiple si != 0 fija multiple, si 0 fija simple

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_setMultiple(uint8_t atdNum, uint8_t multiple) {
  if (atdNum > 1) return 0;
  if (multiple)
    ATDxCTL5[atdNum] |= M6812B_SCAN;
  else
    ATDxCTL5[atdNum] &= ~M6812B_SCAN;
  return 1;
}

/* Activa, desactiva modo SCAN
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1
 *  @param scan si != 0 fija SCAN, si 0 desactiva SCAN

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_setScan(uint8_t atdNum, uint8_t scan) {
  if (atdNum > 1) return 0;
  if (scan)
    ATDxCTL5[atdNum] |= M6812B_SCAN;
  else
    ATDxCTL5[atdNum] &= ~M6812B_SCAN;
  return 1;
}

/* Inicia la secuencia de conversiones.
 *
 * Activa el conversor y escribe la configuración de los distintos
 * registros.
 * Aborta la secuencia en curso (si la hay).
 *
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_iniciaConversion(uint8_t atdNum) {
  if (atdNum > 1) return 0;
  uint16_t desp = atdNum ? M6812_ATD1CTL0 - M6812_ATD0CTL0 : 0;

  _io_ports[M6812_ATD0CTL2 + desp] |= M6812B_ADPU; // encendemos

  // cargamos la configuración preparada
  _io_ports[M6812_ATD0CTL3 + desp] = ATDxCTL3[atdNum];
  _io_ports[M6812_ATD0CTL4 + desp] = ATDxCTL4[atdNum];
  _io_ports[M6812_ATD0CTL5 + desp] = ATDxCTL5[atdNum];

  return 1;
}

/* Espera a que termine conversión.
 *
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_esperaFinConversion(uint8_t atdNum) {
  if (atdNum > 1) return 0;
  uint16_t desp = atdNum ? M6812_ATD1CTL0 - M6812_ATD0CTL0 : 0;

  while ( !(_io_ports[M6812_ATD0STAT0 + desp] & M6812B_SCF));

  return 1;
}


/* Copia en `buffer` el contenido de las `n` primeras conversiones.
 *
 * El código llamante es responsable de que haya suficiente espacio
 * en el `buffer` para los `n` valores pedidos.
 *
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1
 *  @param buffer puntero a donde depositar los valores
 *  @param n número de valores solicitados.

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_devuelveValores(uint8_t atdNum, uint16_t* buffer, uint8_t n) {
  if (atdNum > 1) return 0;
  if (n > 8) return 0;
  uint16_t desp = atdNum ? M6812_ATD1CTL0 - M6812_ATD0CTL0 : 0;

  for(uint8_t i = 0; i < n; i++, buffer++)
    *buffer  = _IO_PORTS_W(M6812_ADR00H + desp + 2 * i);

  return 1;
}

/* Instala función manejadorea de conversión complete.
 *
 *  @param atdNum cuál unidad ATD se refiere, debe ser 0 o 1
 *  @param ptfun puntero a la función manejadora

    @return 1 si correcto, 0 si algún problema
*/
uint8_t atd_instalaManejadora(uint8_t atdNum, void (*ptFun)(void) ) {
  if (atdNum > 1) return 0;

  if (atdNum)
    manejadora1 = ptFun;
  else
    manejadora0 = ptFun;

  // habilitamos las interrupciones de la unidad
  // y usamos puesta a 0 rápida del flag
  uint16_t desp = atdNum ? M6812_ATD1CTL0 - M6812_ATD0CTL0 : 0;
  _io_ports[M6812_ATD0CTL2 + desp] |= (M6812B_ASCIE | M6812B_AFFC);

  return 1;
}

void __attribute__((interrupt)) vi_atd(void) {
  // Puede haber interrumpida cualquiera de las unidades
  //   tiene que estar su ASCIF y ASCIE activos

  if ((_io_ports[M6812_ATD0CTL2] & M6812B_ASCIF)
      && (_io_ports[M6812_ATD0CTL2] & M6812B_ASCIE)) {
    //Accedemos a un registro de resultados para bajar flag
    _IO_PORTS_W(M6812_ADR00H);

    //serial_send(_io_ports[M6812_ATD0CTL2] & M6812B_ASCIF ? '+' : '-');
    // Llammos manejadora si está definido (es != 0)
    if (manejadora0)
      (*manejadora0)();
  }

  if ((_io_ports[M6812_ATD1CTL2] & M6812B_ASCIF)
      && (_io_ports[M6812_ATD1CTL2] & M6812B_ASCIE)) {
    //Accedemos a un registro de resultados para bajar flag
    _IO_PORTS_W(M6812_ADR10H);

    // Llammos manejadora si está definido (es != 0)
    if (manejadora1)
      (*manejadora1)();
  }
}

/*@}*/
