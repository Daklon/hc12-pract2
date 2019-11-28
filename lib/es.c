
#include <es.h>

uint16_t get_ddr(char dport){

  switch(dport){

    case 'A':
      return M6812_DDRA;
    break;

    case 'B':
      return M6812_DDRB;
    break;

    case 'E':
      return M6812_DDRE;
    break;

    case 'G':
      return M6812_DDRG;
    break;
    case 'H':
      return M6812_DDRH;
    break;

    case 'S':
      return M6812_DDRS;
    break;

    case 'T':
      return M6812_DDRT;
    break;

    default:
      serial_print("Puerto erroneo,revise el nombre\n");
      return 0;
  }

}

uint16_t get_port(char port){

  switch(port){

    case 'A':
      return M6812_PORTA;
    break;

    case 'B':
      return M6812_PORTB;
    break;

    case 'E':
      return M6812_PORTE;
    break;

    case 'G':
      return M6812_PORTG;
    break;
    case 'H':
      return M6812_PORTH;
    break;

    case 'S':
      return M6812_PORTS;
    break;

    case 'T':
      return M6812_PORTT;
    break;
    default:
      serial_print("Puerto erroneo,revise el nombre\n");
      return 0;
  }
}


void e_s_total(char dport,uint8_t pin){

  uint16_t dir=M6812_DDRA;

  dir = get_ddr(dport);

  _io_ports[dir] = pin;

}



void entrada_salida(char dport,uint8_t pin,uint8_t es){

  uint8_t mascara = 0;
  mascara = (1 << pin);
  uint16_t dir=M6812_DDRA;

  dir = get_ddr(dport);

  if(es){
    _io_ports[dir] = _io_ports[dir] | mascara;
  }else{
    _io_ports[dir] = _io_ports[dir] & ~mascara;
  }

}

void ad_pullup(char port, uint8_t ad){

  uint16_t dir = M6812_PUCR;
  uint16_t pin = 0;

  switch(port){

    case 'A':
      pin = M6812B_PUPA;
    break;

    case 'B':
      pin = M6812B_PUPB;
    break;

    case 'E':
      pin = M6812B_PUPE;
    break;

    case 'G':
      pin = M6812B_PUPG;
    break;
    case 'H':
      pin = M6812B_PUPH;
    break;

    case 'P':
      dir = M6812_PWCTL;
      pin = M6812B_PUPP;
    break;

    case 'T':
      dir = M6812_TMSK2;
      pin = M6812B_PUPT;
    break;

    default:
      serial_print("\nPuerto erroneo,revise el nombre");

  }

  if(ad)
    _io_ports[dir] |= pin;
  else
    _io_ports[dir] &= ~pin;

}

uint8_t leer_pin(char port, uint8_t pin){

  uint8_t mascara = 0;
  mascara = (1 << pin);
  uint16_t dir;

  dir = get_port(port);

  return _io_ports[dir] & mascara;

}

void escribir_pin(char port, uint8_t pin, uint8_t val){

  uint8_t mascara = 0;
  mascara = (1 << pin);
  uint16_t dir=M6812_PORTA;

    dir = get_port(port);

  if(val)
    _io_ports[dir] = _io_ports[dir] | mascara;
  else
    _io_ports[dir] = _io_ports[dir] & ~mascara;
}

uint8_t leer_puerto(char port){
  uint16_t dir;
  dir = get_port(port);
  return _io_ports[dir];
}


void escribir_puerto(char port, uint8_t val){

  uint16_t dir;
  dir = get_port(port);

  _io_ports[dir] = val;

}

