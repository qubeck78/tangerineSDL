
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

#define _RV_CSR_MSTATUS       0x300
#define _RV_CSR_MISA          0x301
#define _RV_CSR_MIE           0x304
#define _RV_CSR_MTVEC         0x305
#define _RV_CSR_MSTATUSH      0x310

#define _RV_CSR_MSCRATCH      0x340
#define _RV_CSR_MEPC          0x341
#define _RV_CSR_MCAUSE        0x342
#define _RV_CSR_MTVAL         0x343
#define _RV_CSR_MIP           0x344

#define _RV_CSR_MHARTID       0xf14

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

   //csr regs

   //0x300
   uint32_t  mstatus;
   //0x304
   uint32_t  mie;
   //0x305
   uint32_t  mtvec;
   //0x310
   uint32_t  mstatush;
   
   //0x340
   uint32_t  mscratch;
   //0x341
   uint32_t  mepc;
   //0x342
   uint32_t  mcause;
   //0x343
   uint32_t  mtval;
   //0x344
   uint32_t  mip;

}emContext_t;


uint32_t rvReadCSR( emContext_t *ctx, uint32_t csr );
uint32_t rvWriteCSR( emContext_t *ctx, uint32_t csr, uint32_t value );

uint32_t rvReset( emContext_t *ctx );

uint32_t rvStep( emContext_t *ctx );

uint32_t rvTriggerMtimeIRQ( emContext_t *ctx );

#endif
