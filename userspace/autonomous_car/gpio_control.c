/* blink.c
 *
 * Raspberry Pi GPIO example using sysfs interface.
 * Guillermo A. Amaral B. <g@maral.me>
 *
 * This file blinks GPIO 4 (P1-07) while reading GPIO 24 (P1_18).
 */

#include "GpioContoller.h"

// Establece las constantes a utilizar
#define PWM_A_Pin 4  /* P1-07 */
#define PWM_B_Pin 5  /* P1-29 */
#define Motor_A_Pin1 17 /* P1-11 */
#define Motor_A_Pin2 27 /* P1-13 */
#define Motor_B_Pin1 6  /* P1-31 */
#define Motor_B_Pin2 13 /* P1-33 */
#define IRSensor_Input 26 /* P1-37 */

char* velocidad;
int pasos;
int objeto_detectado;

int setup()
{
	int retVal = 0;

	retVal |= autonomous::GpioController::GPIOExport(PWM_A_Pin);
	retVal |= autonomous::GpioController::GPIODirection(PWM_A_Pin, OUT);
	retVal |= autonomous::GpioController::GPIOExport(PWM_B_Pin);
	retVal |= autonomous::GpioController::GPIODirection(PWM_B_Pin, OUT);
	retVal |= autonomous::GpioController::GPIOExport(Motor_A_Pin1);
	retVal |= autonomous::GpioController::GPIODirection(Motor_A_Pin1, OUT);
	retVal |= autonomous::GpioController::GPIOExport(Motor_A_Pin2);
	retVal |= autonomous::GpioController::GPIODirection(Motor_A_Pin2, OUT);
	retVal |= autonomous::GpioController::GPIOExport(Motor_B_Pin1);
	retVal |= autonomous::GpioController::GPIODirection(Motor_B_Pin1, OUT);
	retVal |= autonomous::GpioController::GPIOExport(Motor_B_Pin2);
	retVal |= autonomous::GpioController::GPIODirection(Motor_B_Pin2, OUT);
	
	return retVal;
}

int close()
{
	autonomous::GpioController::GPIOUnexport(PWM_A_Pin);
	autonomous::GpioController::GPIOUnexport(PWM_B_Pin);
	autonomous::GpioController::GPIOUnexport(Motor_A_Pin1);
	autonomous::GpioController::GPIOUnexport(Motor_A_Pin2);
	autonomous::GpioController::GPIOUnexport(Motor_B_Pin1);
	autonomous::GpioController::GPIOUnexport(Motor_B_Pin2);
}

void paro(int pasos)
{
	autonomous::GpioController::GPIOWrite(Motor_A_Pin1, LOW, 1);
	autonomous::GpioController::GPIOWrite(Motor_A_Pin2, LOW, 1);
	autonomous::GpioController::GPIOWrite(Motor_B_Pin1, LOW, 1);
	autonomous::GpioController::GPIOWrite(Motor_B_Pin2, LOW, 1);
	usleep(pasos * 1000);
}

void marchaAdelante(int pasos, const char* velocidad, int length) 
{
	autonomous::GpioController::GPIOWrite(PWM_A_Pin, velocidad, length);
	autonomous::GpioController::GPIOWrite(Motor_A_Pin1, LOW, 1);
	autonomous::GpioController::GPIOWrite(Motor_A_Pin2, HIGH, 1);
	autonomous::GpioController::GPIOWrite(PWM_B_Pin, velocidad, length);
	autonomous::GpioController::GPIOWrite(Motor_B_Pin1, LOW, 1);
	autonomous::GpioController::GPIOWrite(Motor_B_Pin2, HIGH, 1);
	usleep(pasos * 1000);
}

void marchaAtras(int pasos, const char* velocidad, int length) 
{
	autonomous::GpioController::GPIOWrite(PWM_A_Pin, velocidad, length);
	autonomous::GpioController::GPIOWrite(Motor_A_Pin1, HIGH, 1);
	autonomous::GpioController::GPIOWrite(Motor_A_Pin2, LOW, 1);
	autonomous::GpioController::GPIOWrite(PWM_B_Pin, velocidad, length);
	autonomous::GpioController::GPIOWrite(Motor_B_Pin1, HIGH, 1);
	autonomous::GpioController::GPIOWrite(Motor_B_Pin2, LOW, 1);
	usleep(pasos * 1000);
}

void vueltaIzquierda(int pasos, const char* velocidad, int length) 
{
	autonomous::GpioController::GPIOWrite(PWM_A_Pin, velocidad, length);
	autonomous::GpioController::GPIOWrite(Motor_A_Pin1, HIGH, 1);
	autonomous::GpioController::GPIOWrite(Motor_A_Pin2, LOW, 1);
	autonomous::GpioController::GPIOWrite(PWM_B_Pin, velocidad, length);
	autonomous::GpioController::GPIOWrite(Motor_B_Pin1, LOW, 1);
	autonomous::GpioController::GPIOWrite(Motor_B_Pin2, HIGH, 1);
	usleep(pasos * 1000);
}


void vueltaDerecha(int pasos, const char* velocidad, int length) 
{
	autonomous::GpioController::GPIOWrite(PWM_A_Pin, velocidad, length);
	autonomous::GpioController::GPIOWrite(Motor_A_Pin1, LOW, 1);
	autonomous::GpioController::GPIOWrite(Motor_A_Pin2, HIGH, 1);
	autonomous::GpioController::GPIOWrite(PWM_B_Pin, velocidad, length);
	autonomous::GpioController::GPIOWrite(Motor_B_Pin1, HIGH, 1);
	autonomous::GpioController::GPIOWrite(Motor_B_Pin2, LOW, 1);
	usleep(pasos * 1000);
}

int main(int argc, char *argv[])
{
	int repeat = 1000;

	autonomous::GpioController gpioController();

	if (setup() != 0) {
		return (1);
	}

	do {
		if( access("/home/pi/direction", F_OK ) != -1 ) {
			marchaAdelante(3000,"1",1); 
			paro(500);
			marchaAtras(3000,"1", 1);
			paro(500);
		} else {
			vueltaDerecha(3000,"1", 1);
			paro(500);
			vueltaIzquierda(3000,"1", 1);
			paro(500);
		}
	}
	while (repeat--);

	close();

	return(0);
}