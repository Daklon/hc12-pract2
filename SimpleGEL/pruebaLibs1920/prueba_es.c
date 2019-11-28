#include <types.h>
#include <es.h>


int main(){

  char port;
  uint8_t pin;
  uint8_t e_s;
  uint8_t aux;
  uint8_t leido;
  uint8_t valor;

  while(1){
    serial_print("\n-----------------------------------------------");
    serial_print("\n1.-Para configurar pin como entrada/salida pulse ");
    serial_print("\n2.-Para activar/Desactivar pull up ");
    serial_print("\n3.-Para leer valor de un pin ");
    serial_print("\n4.-Para escribir el valor de un pin ");
    serial_print("\n6.-Para configurar TODO EL PUERTO como entrada/salida pulse ");
    serial_print("\n7.-Para leer el valor de un puerto ");
    serial_print("\n8.-Para escribir el valor de un puerto ");
    serial_print("\n-----------------------------------------------\n");
    serial_print("==>");
    aux = serial_getdecbyte();

    switch(aux){
      case 1:
        serial_print("\nIntroduce el puerto a cambiar [A, B, E, H, G, S, T]: ");
        port = serial_recv();
        serial_send(port);
        serial_print("\nIntroduce el bit a cambiar: ");
        pin = serial_getdecbyte();
        serial_print("\nSi desea cambiar a salida [1], si quiere a entrada pulse [0]: ");
        e_s = serial_getdecbyte();

        entrada_salida(port,pin,e_s);

      break;

      case 2:
        serial_print("\nIntroduce el puerto a cambiar el pull-up [A, B, E, G, H, P, T]: ");
        port = serial_recv();
        serial_send(port);
        serial_print("\nActivar [1] - Desactivar [0]");
        e_s = serial_getdecbyte();

        ad_pullup(port,e_s);

      break;

      case 3:
        serial_print("\nIntroduce el puerto del que quieres leer [A, B, E, G, H, S, T]: ");
        port = serial_recv();
        serial_send(port);
        serial_print("\nIntroduce el pin que quieres leer: ");
        pin = serial_getdecbyte();

        leido = leer_pin(port, pin);
        if(leido)
          serial_print("\n El valor es = 1");
        else
          serial_print("\n El valor es = 0");

      break;

      case 4:
        serial_print("\nIntroduce el puerto en el que quieres escribir [A, B, E, G, H, S, T]: ");
        port = serial_recv();
        serial_send(port);
        serial_print("\nIntroduce el pin a escribir: ");
        pin = serial_getdecbyte();
        serial_print("\n¿Que valor desea escribir?");
        valor = serial_getdecbyte();

        escribir_pin(port, pin, valor);
      break;

      case 5:
        serial_print("\n");

      break;
      case 6:
        serial_print("\nIntroduce el puerto a cambiar [A, B, E, H, G, S, T]: ");
        port = serial_recv();
        serial_send(port);
        serial_print("\nIntroduce la configuracion completa a cambiar en el puerto [Ej:01001001]: ");
        pin = serial_getbinbyte();

        e_s_total(port,pin);

      break;
      case 7:
        serial_print("\nIntroduce el puerto que quieres leer: [A, B, E, G, H, S, T]: ");
        port = serial_recv();
        serial_send(port);

        leido = leer_puerto(port);
        serial_print("\nEl valor leido es: ");
        serial_printbinbyte(leido);
      break;
      case 8:
        serial_print("\nIntroduce el puerto en el que quieres escribir [A, B, E, G, H, S, T]: ");
        port = serial_recv();
        serial_send(port);
        serial_print("\nIntroduce la configuracion completa a escribir en el puerto [Ej:01001001]: ");
        valor = serial_getbinbyte();

        escribir_puerto(port, valor);
      break;
    }

serial_print("\nDDRG = ");
serial_printbinbyte(_io_ports[M6812_DDRG]);
serial_print("\nPORTG = ");
serial_printbinbyte(_io_ports[M6812_PORTG]);

serial_print("\nDDRH = ");
serial_printbinbyte(_io_ports[M6812_DDRH]);
serial_print("\nPORTH = ");
serial_printbinbyte(_io_ports[M6812_PORTH]);

serial_print("\nDDRS = ");
serial_printbinbyte(_io_ports[M6812_DDRS]);
serial_print("\nPORTS = ");
serial_printbinbyte(_io_ports[M6812_PORTS]);

serial_print("\nDDRT = ");
serial_printbinbyte(_io_ports[M6812_DDRT]);
serial_print("\nPORTT = ");
serial_printbinbyte(_io_ports[M6812_PORTT]);

serial_print("\n\nPUCR = ");
serial_printbinbyte(_io_ports[M6812_PUCR]);


  }
}
