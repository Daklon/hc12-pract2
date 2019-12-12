/** Programa de prueba librería `atd_lib` utilizando pool
 *
  \author Alberto Hamilton Castro
  \date 2019-12-01

 */

#include <types.h>
#include <atd_lib.h>


// para indicar que se llamó a la manejadora
// Debe ser volatile para que el compilador no optimize las comprobaciones
volatile uint8_t termino = 0;

void atiendeFin() {
  // serial_send('T');
  termino = 1;
}

int main () {

  /* Deshabilitamos interrupciones */
  lock ();

  /*Encendemos led*/
  _io_ports[M6812_DDRG] |= M6812B_PG7;
  _io_ports[M6812_PORTG] |= M6812B_PG7;


  serial_init();
  serial_print("\nprueba_atd_lib_manejadora ===========\n");

  unlock();  // Habilitamos las interrupciones


  while(1) {
    // Quitamos posible pulsación pendiente
    if (serial_receive_pending()) serial_recv();
    /* Elección del puerto */
    serial_print("\nPuerto conversor a utilizar (0 - 1)?:");
    char c;
    while((c = serial_recv()) != '0' && c != '1');
    serial_send(c); /* a modo de confirmación*/

    uint8_t unidad = c - '0';

    serial_print("\nUsando unidad ");
    serial_printdecbyte(unidad);

    /* Elección del pin dentro del puerto */
    serial_print("\nPin del puerto a utilizar (0 - 7)?:");
    while((c = serial_recv()) < '0' && c > '7');
    serial_send(c); /* a modo de confirmación*/
    uint8_t pin = c - '0';
    serial_print("\nUsando PIN ");
    serial_printdecbyte(pin);


    /*Pasamos a configurar AD correspondiente*/
    if (
      atd_setBits(unidad, 10) &&
      atd_setPin(unidad, pin) &&
      atd_setMultiple(unidad, 0) &&
      atd_setCiclos(unidad, 16) &&
      atd_setNumConversiones(unidad, 8) &&
      atd_setScan(unidad,1) &&
      atd_instalaManejadora(unidad, &atiendeFin) &&
      atd_iniciaConversion(unidad)

    ) {
      serial_print("\nConfiguración correcta\n");
      serial_print("\nRegistros 0 (del 2 al 5):\n");
      serial_printbinbyte(_io_ports[M6812_ATD0CTL2]);
      serial_send('\n');
      serial_printbinbyte(_io_ports[M6812_ATD0CTL3]);
      serial_send('\n');
      serial_printbinbyte(_io_ports[M6812_ATD0CTL4]);
      serial_send('\n');
      serial_printbinbyte(_io_ports[M6812_ATD0CTL5]);
      serial_send('\n');

      serial_print("\nRegistros 1 (del 2 al 5):\n");
      serial_printbinbyte(_io_ports[M6812_ATD1CTL2]);
      serial_send('\n');
      serial_printbinbyte(_io_ports[M6812_ATD1CTL3]);
      serial_send('\n');
      serial_printbinbyte(_io_ports[M6812_ATD1CTL4]);
      serial_send('\n');
      serial_printbinbyte(_io_ports[M6812_ATD1CTL5]);
      serial_send('\n');
    } else {
      serial_print("\nERROR Configuración. PARAMOS\n");
      while(1);
    }

    serial_print("\nConvirtiendo (pulsa para salir)\n");

    char simbolo[] = "/|\\-*";
    uint16_t itera = 0;
#define ITERA_CAMBIO (5000)

    uint16_t resultados[8]; // Array para recoger resultados
    uint16_t resultadoAnterior = 0;
    while(!serial_receive_pending()) {

      if (!(itera % ITERA_CAMBIO)) {
        uint8_t simAct = itera/ITERA_CAMBIO;
        if(!simbolo[simAct]) {
          itera = 0;
          simAct = 0;
        }
        serial_send(simbolo[simAct]);
        serial_send('\b');
      }
      itera++;

      /* Esperamos a que se termine la conversión */
      while (!termino);
      termino = 0;

      /*Invertimos el led*/
      _io_ports[M6812_PORTG] ^= M6812B_PG7;

      /*Vemos si los 8 resultados son iguales */
      atd_devuelveValores(unidad, resultados, 8);

      uint16_t resultado = resultados[0];
      uint8_t iguales = 1;
      for(uint8_t i = 0; iguales && i < 8; i++)
        iguales = (resultado == resultados[i]);
      if(! iguales)
        continue;
      if (resultado == resultadoAnterior)
        continue;

      /* Los 8 resultados son iguales y distintos a lo que teníamos antes*/
      serial_print("Nuevo valor = ");
      serial_printdecword(resultado);
      serial_send('\n');
      resultadoAnterior = resultado;
    }
  }
}
