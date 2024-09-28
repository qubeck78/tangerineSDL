
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

#include "gfTypes.h"

#define _RVEMUL_OK                        0
#define _RVEMUL_UNDEFINED_INSTRUCTION     1

#define _RVEMUL_EBREAK                    256

typedef struct _emContext_t
{

   //memory interface callback functions

                                 //addr
   uint32_t  ( *fetchInstruction )( uint32_t );

                           //addr
   uint32_t  ( *fetchData )( uint32_t );

                           //addr  mask   data
   uint32_t  ( *storeData )( uint32_t, uint8_t, uint32_t );

   uint32_t  pc;

   uint32_t  regs[32];
   int32_t  *sregs;

   //instruction counter;

   uint32_t  instrCounter;

   //instruction decoder

   uint32_t  instruction;

   
}emContext_t;


uint32_t rvReset( emContext_t *ctx );
uint32_t rvStep( emContext_t *ctx );


#endif
