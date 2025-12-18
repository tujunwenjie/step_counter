#include "S_UART.h"	
void UART_printf(UART_HandleTypeDef *uartbase, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	int length;
	char buffer[128];
	length = vsnprintf(buffer, 128, fmt, ap);
	HAL_UART_Transmit(uartbase, (uint8_t *)buffer, length, 0xffff);
}