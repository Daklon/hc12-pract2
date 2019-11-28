#include <types.h>
#include <es.h>

int main(){
    uint16_t i;

    serial_init();
    serial_print("\nInicializado");
    port = serial_recv();
    while(1){
        for(i=0;i<8;i++){
            entrada_salida('G',i,1);
        }
        for(i=0;i<8;i++){
            escribir_pin('G', i, 1);
        }
    }
}
