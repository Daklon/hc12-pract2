/*
 * Jose Ramón Casero Fuentes
 * Daniel Fraga Viera
 * Noviembre - 2019
 * Sistemas Empotrados
 *
 * */

#include <pwm.h>

uint8_t canal = 0; ///variable que almacena el canal en el que se trabaja.


/*
* @brief Funcion que cambia el factor de division para generar la fuente de
*     reloj A.
* @param factor Variable de tipo uint8_t que indica el factor a modificar.
*/
void fijaFactorA(uint8_t factor)
{
  if(factor > 7){
#ifdef PWM_DEBUG
    serial_print("\nERROR - Frecuencia no válida");
#endif // PWM_DEBUG
  }
  else{
    uint8_t frecuencia_actual = _io_ports[M6812_PWCLK];

#ifdef PWM_DEBUG
    serial_print("\nFrecuencia_A (PWCLK) original: ");
    serial_printbinbyte(frecuencia_actual);

    serial_print("\nFactor introducido: ");
    serial_printbinbyte(factor);
#endif // PWM_DEBUG

    factor = factor << 3;
    frecuencia_actual = (frecuencia_actual &
      ~((M6812B_PCKA0 | M6812B_PCKA1 | M6812B_PCKA2))) | factor;

#ifdef PWM_DEBUG
    serial_print("\nFrecuencia_A (PWCLK) nueva: ");
    serial_printbinbyte(frecuencia_actual);
#endif // PWM_DEBUG

    _io_ports[M6812_PWCLK] = frecuencia_actual;
  }
}

/*
* @brief Funcion que cambia el factor de division para generar la fuente de
*     reloj B.
* @param factor Variable de tipo uint8_t que indica el factor a modificar.
*/
void fijaFactorB(uint8_t factor)
{
  if(factor > 7){
#ifdef PWM_DEBUG
    serial_print("\nERROR - Frecuencia no válida");
#endif // PWM_DEBUG
  }
  else{
    uint8_t frec = _io_ports[M6812_PWCLK];

#ifdef PWM_DEBUG
    serial_print("\nFrecuencia_B (PWCLK) original: ");
    serial_printbinbyte(frec);

    serial_print("\nFactor: ");
    serial_printbinbyte(factor);
#endif // PWM_DEBUG

    frec = (frec & ~((M6812B_PCKB0 | M6812B_PCKB1 | M6812B_PCKB2))) | factor;

#ifdef PWM_DEBUG
    serial_print("\nFrecuencia_B (PWCLK) nueva: ");
    serial_printbinbyte(frec);
#endif // PWM_DEBUG

    _io_ports[M6812_PWCLK] = frec;
  }
}

/*
* @brief Funcion que cambia la polaridad del ciclo de trabajo del canal.
*     Si es 0, ciclo de trabajo en baja. Si es 1, ciclo de trabajo en alta.
* @param polaridad Variable de tipo uint8_t que indica la polaridad a cambiar.
*     Variable de tipo uint8_t que indica la posicion a desplazar.
*/
void cambiaPolaridad(uint8_t polaridad)
{
#ifdef PWM_DEBUG
  serial_print("\nPolaridad original: ");
  serial_printbinbyte(_io_ports[M6812_PWPOL]);
#endif // PWM_DEBUG

  if(canal <= 3){
    if(polaridad == 1){
      polaridad = polaridad << canal;
      _io_ports[M6812_PWPOL] = _io_ports[M6812_PWPOL] | polaridad;
    }
    else if(polaridad == 0){
      polaridad = 1;
      polaridad = polaridad << canal;
      polaridad = ~polaridad;
      _io_ports[M6812_PWPOL] = _io_ports[M6812_PWPOL] & polaridad;
    }

#ifdef PWM_DEBUG
    serial_print("\nPolaridad modificada: ");
    serial_printbinbyte(_io_ports[M6812_PWPOL]);
#endif // PWM_DEBUG

  }
}

/*
* @brief Funcion que cambia el alineamiento de la señal de salida para el canal.
* @param alineamiento Variable de tipo uint8_t que indica el alineamiento a modificar.
*     0, alineamiento izquierdo. 1, alineamiento centrado.
*/
void cambiaAlineamiento(uint8_t alineamiento){

#ifdef PWM_DEBUG
  serial_print("\nAlineamiento (PWCTL) anterior: ");
  serial_printbinbyte(_io_ports[M6812_PWCTL]);
#endif // PWM_DEBUG

  if(alineamiento == 1) {
    alineamiento = alineamiento << 3;
    _io_ports[M6812_PWCTL] = _io_ports[M6812_PWCTL] | alineamiento;
  } else if(alineamiento == 0) {
    alineamiento = 1;
    alineamiento = alineamiento << 3;
    alineamiento = ~alineamiento;
    _io_ports[M6812_PWCTL] = _io_ports[M6812_PWCTL] & alineamiento;
  }

#ifdef PWM_DEBUG
  serial_print("\nAlineamiento (PWCTL) nuevo: ");
  serial_printbinbyte(_io_ports[M6812_PWCTL]);
#endif // PWM_DEBUG
}

/*
* @brief Funcion que modifica el ciclo de trabajo del canal de forma directa.
* @param duty Variable de tipo uint8_t que indica el duty a modificar.
*/
void cambiaPWDTY(uint8_t duty)
{
#ifdef PWM_DEBUG
  serial_print("\nDUTY anterior: ");
  serial_printdecbyte(_io_ports[M6812_PWDTY0 + canal]);
#endif // PWM_DEBUG

  _io_ports[M6812_PWDTY0 + canal] = duty;

#ifdef PWM_DEBUG
  serial_print("\nDUTY nuevo: ");
  serial_printdecbyte(_io_ports[M6812_PWDTY0 + canal]);
#endif // PWM_DEBUG
}

/*
* @brief Funcion que modifica el ciclo de trabajo del canal de forma porcentual.
* @param porcentaje Variable de tipo uint8_t que indica el porcentaje a modificar.
*/
void cambiaPWDTYporciento(uint8_t porcentaje)
{
  uint8_t pwdty;
  pwdty = _io_ports[M6812_PWPER0 + canal] * porcentaje / 100;
  cambiaPWDTY(pwdty);
}

/*
* @brief Funcion que modifica el periodo asociado al canal. Además calcula el
*     nuevo porcentaje proporcional.
* @param periodo Variable de tipo uint8_t que indica el periodo a modificar.
*/
void cambiaPWPER(uint8_t periodo)
{
  uint8_t porcentaje_actual = (_io_ports[M6812_PWDTY0 + canal]  * 100 /
    _io_ports[M6812_PWPER0 + canal]);

#ifdef PWM_DEBUG
  serial_print("\nPorcentaje actual: ");
  serial_printdecbyte(porcentaje_actual);

  serial_print("\nPWPER anterior: ");
  serial_printdecbyte(_io_ports[M6812_PWPER0 + canal]);
#endif // PWM_DEBUG

  _io_ports[M6812_PWPER0 + canal] = periodo;

#ifdef PWM_DEBUG
  serial_print("\nPWPER nuevo: ");
  serial_printdecbyte(_io_ports[M6812_PWPER0 + canal]);
#endif // PWM_DEBUG

  cambiaPWDTYporciento(porcentaje_actual);
}


/*
* @brief Funcion que modifica el canal de forma directa.
* @param canal_nuevo Variable de tipo uint8_t que indica el canal a modificar.
*/
void cambiaCanal(uint8_t canal_nuevo)
{
  canal = canal_nuevo;

  if(canal == 0){
    _io_ports[M6812_PWCLK] = _io_ports[M6812_PWCLK] & (~M6812B_CON01);
    _io_ports[M6812_PWEN] = _io_ports[M6812_PWEN] | M6812B_PWEN0;
  }
  else if(canal == 1){
    _io_ports[M6812_PWCLK] = _io_ports[M6812_PWCLK] & (~M6812B_CON01);
    _io_ports[M6812_PWEN] = _io_ports[M6812_PWEN] | M6812B_PWEN1;
  }
  else if(canal == 2){
    _io_ports[M6812_PWCLK] = _io_ports[M6812_PWCLK] & (~M6812B_CON23);
    _io_ports[M6812_PWEN] = _io_ports[M6812_PWEN] | M6812B_PWEN2;
  }
  else if(canal == 3){
    _io_ports[M6812_PWCLK] = _io_ports[M6812_PWCLK] & (~M6812B_CON23);
    _io_ports[M6812_PWEN] = _io_ports[M6812_PWEN] | M6812B_PWEN3;
  }
  _io_ports[M6812_DDRP] = _io_ports[M6812_DDRP] | (M6812B_DDP0 << canal);
}

/*
* @brief Funcion que resetea todos los canales.
*/
inline void deshabilitaCanales()
{
  _io_ports[M6812_PWEN] = _io_ports[M6812_PWEN] & (~M6812B_PWEN0);
  _io_ports[M6812_PWEN] = _io_ports[M6812_PWEN] & (~M6812B_PWEN1);
  _io_ports[M6812_PWEN] = _io_ports[M6812_PWEN] & (~M6812B_PWEN2);
  _io_ports[M6812_PWEN] = _io_ports[M6812_PWEN] & (~M6812B_PWEN3);
}

/*
* @brief Funcion que crea un ejemplo de PWM con los parámetros que se le pasen.
* @param Múltiples variables de tipo uint8_t con las que usar las funciones
*     anteriores.
*/
void pwmBase(uint8_t pos, uint8_t factor_a, uint8_t factor_b, uint8_t pol,
  uint8_t pwper, uint8_t porcentaje, uint8_t alineamiento)
{
  if(factor_a > 7 || factor_b > 7){
#ifdef PWM_DEBUG
    serial_print("\nERROR - Frecuencia no válida");
#endif // PWM_DEBUG
  }
  else{
    serial_printdecbyte(pos);
    cambiaCanal(pos);
    serial_printdecbyte(canal);
    cambiaPWPER(pwper);
    cambiaPWDTYporciento(porcentaje);
    cambiaAlineamiento(alineamiento);
    fijaFactorA(factor_a);
    fijaFactorB(factor_b);
    cambiaPolaridad(pol);
  }
}

/*
* @brief Funcion que muestra el ejemplo de PWM construido.
*/
void muestraPWM()
{
  serial_print("\n\nParámetros:\n");
  serial_print("\n - Canal: ");
  serial_printdecbyte(canal);
  serial_print("\n - Frecuencia: ");
  serial_printdecbyte(_io_ports[M6812_PWCLK]);
  serial_print("\n - Factor A: ");
  serial_printdecbyte(_io_ports[M6812_PWCLK] >> 3);
  serial_print("\n - Factor B: ");
  serial_printdecbyte(_io_ports[M6812_PWCLK] & 7);
  serial_print("\n - Polaridad: ");
  serial_printdecbyte((_io_ports[M6812_PWPOL] >> canal) & 1);
  serial_print("\n - PWPER: ");
  serial_printdecbyte(_io_ports[M6812_PWPER0 + canal]);
  serial_print("\n - DUTY: ");
  serial_printdecbyte(_io_ports[M6812_PWDTY0 + canal]);
  serial_print("\n - DUTY (porcentaje): ");

  uint8_t porcentaje = (_io_ports[M6812_PWDTY0 + canal] * 100
    / _io_ports[M6812_PWPER0 + canal]);

  serial_printdecbyte(porcentaje);
  serial_print("\n - Alineamiento: ");

  if(_io_ports[M6812_PWCTL] == 0){
    serial_print("izquierdo");
  }
  else{
    serial_print("centrado");
  }
}
