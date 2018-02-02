
#pragma once

#include <stdbool.h>

void uart_init(void);
void uart_write_str(char *str);
char uart_get_char(bool peek);
uint8_t uart_available(void);