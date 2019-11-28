/*
 * Jose Ramón Casero Fuentes
 * Daniel Fraga Viera
 * Noviembre - 2019
 * Sistemas Empotrados
 *
 * */

#include <pwm.h>

int main()
{
  uint8_t alin = 0;
  uint8_t canal = 0;
  uint8_t dato = 0;
  uint8_t porcentaje = 15;
  uint8_t factor_a = 2;
  uint8_t factor_b = 6;
  uint8_t polaridad = 1;
  uint8_t pwper = 100;

  serial_init();

  pwmBase(canal, factor_a, factor_b, polaridad, pwper, porcentaje, alin);

  muestraPWM();

  while(1){
    serial_print("\n\nElija una opción\n");
    serial_print("\n1 - Cambiar Período");
    serial_print("\n2 - Cambiar Duty");
    serial_print("\n3 - Cambiar Duty en porcentaje");
    serial_print("\n4 - Cambiar Canal");
    serial_print("\n5 - Cambiar Alineamiento");
    serial_print("\n6 - Cambiar Polaridad");
    serial_print("\n7 - Cambiar Frecuencia (Factor A)");
    serial_print("\n8 - Cambiar Frecuencia (Factor B)");
    serial_print("\n9 - Mostrar PWM\n\n");

    char opcion;
    opcion = serial_recv();
    serial_send(opcion);

    switch(opcion){
      case('1'):
        serial_print("\nIntroduzca el periodo a cambiar: ");
        dato = serial_getdecbyte();
        cambiaPWPER(dato);
        break;

      case('2'):
        serial_print("\nIntroduzca el duty a cambiar: ");
        dato = serial_getdecbyte();
        cambiaPWDTY(dato);
        break;

      case('3'):
        serial_print("\nIntroduzca el porcentaje de duty a cambiar: ");
        dato = serial_getdecbyte();
        cambiaPWDTYporciento(dato);
        break;

      case('4'):
        // serial_print("\nResetando canales...");
        // deshabilitaCanales();

        serial_print("\nIntroduzca canal a utilizar (de 0 a 3): ");
        dato = serial_getdecbyte();

        while(dato != 0 && dato != 1 && dato != 2 && dato != 3){
          serial_print("\nOpción incorrecta, introduzca de 0 a 3\n");
          dato = serial_getdecbyte();
        }
        cambiaCanal(dato);
        break;

      case('5'):
        serial_print("\n0 para alineamiento izquierdo");
        serial_print("\n1 para alineamiento centrado: ");
        dato = serial_getdecbyte();

        while(dato != 0 && dato != 1){
          serial_print("\nOpción incorrecta, introduzca 0 ó 1\n");
          dato = serial_getdecbyte();
        }
        cambiaAlineamiento(dato);
        break;

      case('6'):
        serial_print("\n0 para baja-alta polaridad");
        serial_print("\n1 para el caso contrario: ");
        dato = serial_getdecbyte();

        while(dato != 0 && dato != 1){
          serial_print("\nOpción incorrecta, introduzca 0 ó 1\n");
          dato = serial_getdecbyte();
        }
        cambiaPolaridad(dato);
        break;

      case('7'):
        serial_print("\nIntroduzca el factor para cambiar frecuencia: ");
        dato = serial_getdecbyte();
        fijaFactorA(dato);
        break;

      case('8'):
        serial_print("\nIntroduzca el factor para cambiar frecuencia: ");
        dato = serial_getdecbyte();
        fijaFactorB(dato);
        break;

      case('9'):
        muestraPWM();
        break;

      default:
        serial_print("\nOpción incorrecta\n");
        break;
    }
  }
}
