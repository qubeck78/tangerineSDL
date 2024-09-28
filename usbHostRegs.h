#ifndef _USBHOSTREGS_H
#define _USBHOSTREGS_H

#include "gfTypes.h"

#include "usbHost.h"

typedef struct _tgUsbHostRegs_t
{
    //usb HID host
    volatile uint32_t id;
    volatile uint32_t version;

    volatile uint32_t usbHidKeyboardStatus;
    volatile uint32_t usbHidKeyboardData;


}tgUsbHostRegs_t;


uint32_t usbHostRegsInit( tgUsbHostRegs_t *regs );
uint32_t usbHostRegsReadReg(  tgUsbHostRegs_t *regs, usbhContext_t *ctx, uint16_t addr );
uint32_t usbHostRegsWriteReg(  tgUsbHostRegs_t *regs, usbhContext_t *ctx, uint16_t addr, uint32_t value );


#endif
