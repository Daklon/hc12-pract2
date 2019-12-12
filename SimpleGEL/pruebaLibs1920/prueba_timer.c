#include <types.h>
#include <sys/sio.h>
#include <sys/param.h>
#include <sys/locks.h>
#include <timer.h>

/** Serves as test for the futures */
void dummyfunction(){
  serial_print("\nFuncionaAA!");
}

void invierteLed() {
  /*Invertimos el led*/
  _io_ports[M6812_PORTG] ^= M6812B_PG7;
}


int main(){
  //inicializamos
  initialize();
  //inicializamos el serial
  serial_init();
  serial_print("\nEjemplo de librería de temporización");

  //inicializamos preescaler
  serial_print("\nTMSK2: ");
  serial_printbinbyte(_io_ports[M6812_TMSK2]);

  serial_print("\ngeticks: ");
  serial_printdeclong(geticks());

  serial_print("\ngetmicros: ");
  serial_printdeclong(getmicros());


  uint32_t msDelay = 1000;

  serial_print("\ngetmilis: ");
  serial_printdeclong(getmilis());
  serial_print("\n msDelay: ");
  serial_printdeclong(msDelay);
  delayms(msDelay);
  serial_print("\ngetmilis: ");
  serial_printdeclong(getmilis());

  serial_print("\ngetmilis: ");
  serial_printdeclong(getmilis());
  serial_print("\ndelay: ");
  serial_printdeclong( msDelay);
  delayms( msDelay);
  serial_print("\ngetmilis: ");
  serial_printdeclong(getmilis());

  msDelay = 5000;
  serial_print("\ngetmilis: ");
  serial_printdeclong(getmilis());
  serial_print("\ndelay: ");
  serial_printdeclong(msDelay);
  delayms(msDelay);
  serial_print("\ngetmilis: ");
  serial_printdeclong(getmilis());

  uint32_t usDelay = 1000000;
  serial_print("\n\ngetmicros: ");
  serial_printdeclong(getmicros());
  serial_print("\nusDelay: ");
  serial_printdeclong(usDelay);
  delayus(usDelay);
  serial_print("\ngetmicros: ");
  serial_printdeclong(getmicros());

  serial_print("\nsDelay: ");
  serial_printdeclong(usDelay);
  future_f(&dummyfunction, usDelay);
  serial_print("\nEsperando future");

  delayus(usDelay + 1000000);

  serial_print("\nDebe haber pasado future");

  usDelay = 70100;
  serial_print("\nPasamos a periodica usDelay: ");
  serial_printdeclong(usDelay);

  /*Encendemos led*/
  _io_ports[M6812_DDRG] |= M6812B_PG7;
  _io_ports[M6812_PORTG] |= M6812B_PG7;

  periodic_f(&invierteLed, usDelay);

  delayms(30000);
  serial_print("\nParamos a periodica");
  periodic_f(&invierteLed, 0);

  serial_print("\nFIN");
  while(1) {
  }
}

