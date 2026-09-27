#ifndef RC522_H
#define RC522_H

int rc522_init(void);
int rc522_read_uid(uint8_t *uid, uint8_t *len);

#endif // RC522_H
