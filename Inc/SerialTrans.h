#ifndef __SERIALTRANS_H
#define __SERIALTRANS_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "stdint.h"

extern void SerialPrint(char * str);
void Cmd_Interface(void);
extern uint8_t target_mode;
extern uint8_t new_stage;


#ifdef __cplusplus
}
#endif
  
#endif /* SERIALTRANS_H */