#ifndef AHT21_H
#define AHT21_H

int aht21_init(void);
int aht21_read(float *temp, float *humi);

#endif // AHT21_H
