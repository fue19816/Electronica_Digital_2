#include <stdint.h>
#include <stdbool.h>
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

/**
 * Hello world
 * IE-3027
 * Diego Morales
 */

uint32_t ui32Period;

int main(void)
{
    //uint8_t BOTON;
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);     //Configuración del oscilador
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);                                                //Se habilita el reloj para el puerto F
    //GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);                                        //Se habilita el pin4 del puerto F como entrada
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);               //Se habilita los pines de F para salidas
    //Activar para parte 1 del lab:
    //GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);    //Se habilita el pin 4 con Pull up
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);                                               //Se habilita el reloj para el temporizador
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);                                            //Se configura el timer0 como temporizador períodico
    ui32Period = (SysCtlClockGet())/2;                                                         //Se realiza el cálculo del período para el temporizador (1seg)
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period-1);                                          //Se establece el período del temporizador
    IntEnable(INT_TIMER0A);                                                                    //Se habilita la interrupción por el TIMER0A
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);                                           //Se establece que exista la interrupción por Timeout
    IntMasterEnable();
    TimerEnable(TIMER0_BASE, TIMER_A);

    while(1){
    //Activar para parte 1 del lab:
    /*if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)==0)
            {
                BOTON=1;
            }
            if(BOTON==1)
            {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08);
                SysCtlDelay(30000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
                SysCtlDelay(5000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08);
                SysCtlDelay(5000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
                SysCtlDelay(5000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08);
                SysCtlDelay(5000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x0A);
                SysCtlDelay(20000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x02);
                BOTON=0;
            }*/
}
}

void Timer0IntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);                 //Se limpia la interrupción del timer

    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3) == 0)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x0E);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
    }
}
