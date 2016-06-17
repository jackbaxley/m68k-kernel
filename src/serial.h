#ifndef __serial_h
#define __serial_h

void serial_clear();
char serial_get_c();
char serial_check_c();
void serial_write_c(char c);
void serial_write_s(char *s);
void serial_init();
int get_time();


#endif