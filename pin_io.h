#ifndef PIN_IO_H
#define PIN_IO_H

#define PERIPHS_GPIO_BASEADDR 0x60000300

#define WRITE_PERI_REG(addr, val) (*((volatile unsigned int*)ETS_UNCACHED_ADDR(addr))) = (unsigned int)(val)
#define GPIO_REG_WRITE(reg, val) WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + reg, val)

#endif /* PIN_IO_H */