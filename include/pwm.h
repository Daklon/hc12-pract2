/*! @defgroup pwm Librería gestión generadores PWM

 * Jose Ramón Casero Fuentes
 * Daniel Fraga Viera
 * Noviembre - 2019
 * Sistemas Empotrados

 */
 /*@{*/

#include <sys/param.h>
#include <sys/ports.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <imath.h>
#include <types.h>


/**
* @brief Funcion que cambia el factor de division para generar la fuente de
*     reloj A.
* @param factor Variable de tipo uint8_t que indica el factor a modificar.
*/
void fijaFactorA(uint8_t factor);

/**
* @brief Funcion que cambia el factor de division para generar la fuente de
*     reloj B.
* @param factor Variable de tipo uint8_t que indica el factor a modificar.
*/
void fijaFactorB(uint8_t factor);

/**
* @brief Funcion que cambia la polaridad del ciclo de trabajo del canal.
*     Si es 0, ciclo de trabajo en baja. Si es 1, ciclo de trabajo en alta.
* @param polaridad Variable de tipo uint8_t que indica la polaridad a cambiar.
*     Variable de tipo uint8_t que indica la posicion a desplazar.
*/
void cambiaPolaridad(uint8_t polaridad);

/**
* @brief Funcion que cambia el alineamiento de la señal de salida para el canal.
* @param alineamiento Variable de tipo uint8_t que indica el alineamiento a modificar.
*     0, alineamiento izquierdo. 1, alineamiento centrado.
*/
void cambiaAlineamiento(uint8_t alineamiento);

/**
* @brief Funcion que modifica el ciclo de trabajo del canal de forma directa.
* @param duty Variable de tipo uint8_t que indica el duty a modificar.
*/
void cambiaPWDTY(uint8_t duty);

/**
* @brief Funcion que modifica el ciclo de trabajo del canal de forma porcentual.
* @param porcentaje Variable de tipo uint8_t que indica el porcentaje a modificar.
*/
void cambiaPWDTYporciento(uint8_t porcentaje);

/**
* @brief Funcion que modifica el periodo asociado al canal. Además calcula el
*     nuevo porcentaje proporcional.
* @param periodo Variable de tipo uint8_t que indica el periodo a modificar.
*/
void cambiaPWPER(uint8_t periodo);


/**
* @brief Funcion que modifica el canal de forma directa.
* @param canal_nuevo Variable de tipo uint8_t que indica el canal a modificar.
*/
void cambiaCanal(uint8_t canal_nuevo);

/**
* @brief Funcion que resetea todos los canales.
*/
inline void deshabilitaCanales();

/**
* @brief Funcion que crea un ejemplo de PWM con los parámetros que se le pasen.
* @param Múltiples variables de tipo uint8_t con las que usar las funciones
*     anteriores.
*/
void pwmBase(uint8_t pos, uint8_t factor_a, uint8_t factor_b, uint8_t pol,
  uint8_t pwper, uint8_t porcentaje, uint8_t alineamiento);

/**
* @brief Funcion que muestra el ejemplo de PWM construido.
*/
void muestraPWM();

/*@}*/
