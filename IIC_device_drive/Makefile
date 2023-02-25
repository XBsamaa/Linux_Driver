KERN_DIR = /home/xbsama/100ask_imx6ull-sdk/Linux-4.9.88
CC = $(CROSS_COMPILE)gcc

all:
	make -C $(KERN_DIR) M=`pwd` modules
	$(CC) -o pgmAp3216 pgmAp3216.c

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -rf pgmAp3216


obj-m	+= IIC_driver_ap3216.o