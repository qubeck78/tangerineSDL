#ifndef _USBHOSTREGS_H
#define _USBHOSTREGS_H

#include "gftypes.h"

#include "usbHost.h"

typedef struct _tgUsbHostRegs_t
{
    //usb HID host
    volatile ulong id;
    volatile ulong version;

    volatile unsigned long usbHidKeyboardStatus;
    volatile unsigned long usbHidKeyboardData;


}tgUsbHostRegs_t;


ulong usbHostRegsInit( tgUsbHostRegs_t *regs );
ulong usbHostRegsReadReg(  tgUsbHostRegs_t *regs, usbhContext_t *ctx, ushort addr );
ulong usbHostRegsWriteReg(  tgUsbHostRegs_t *regs, usbhContext_t *ctx, ushort addr, ulong value );


#endif
