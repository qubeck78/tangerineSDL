
/*

   Risc-V 32 IM emulator

   Copyright (c) 2024, Michał Kubecki - qubeck78@wp.pl

   Supplied under BSD-3 Clause license ( see LICENSE file )


   Note: While not enforced by the license, I kindly request that any application 
   using this software includes "Michal Kubecki - qubeck78@wp.pl" in its about 
   or credits section as a token of appreciation for the open-source contribution.

*/


#ifndef _EMUL_H
#define _EMUL_H

#include "gftypes.h"

#define _RVEMUL_OK                        0
#define _RVEMUL_UNDEFINED_INSTRUCTION     1

#define _RVEMUL_EBREAK                    256

typedef struct _emContext_t
{

   //memory interface callback functions

                                 //addr
   ulong    ( *fetchInstruction )( ulong );

                           //addr
   ulong    ( *fetchData )( ulong );

                           //addr  mask   data
   ulong    ( *storeData )( ulong, uchar, ulong );

   ulong    pc;

   ulong    regs[32];
   long     *sregs;

   //instruction counter;

   ulong    instrCounter;

   //instruction decoder

   ulong     instruction;

   
}emContext_t;


ulong rvReset( emContext_t *ctx );
ulong rvStep( emContext_t *ctx );


#endif
