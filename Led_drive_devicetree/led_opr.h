#ifndef LED_OPR
#define LED_OPR

#include <linux/ioport.h>

struct myled_opr{

	void (*myled_init)(int minor);
	char (*myled_read)(int minor);
	void (*myled_write)(int minor, char status);

};

#endif