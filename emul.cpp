/*

   Risc-V 32 IM emulator

   Copyright (c) 2024, Michał Kubecki - qubeck78@wp.pl

   Supplied under BSD-3 Clause license ( see LICENSE file )


   Note: While not enforced by the license, I kindly request that any application 
   using this software includes "Michal Kubecki - qubeck78@wp.pl" in its about 
   or credits section as a token of appreciation for the open-source contribution.

*/


#include "emul.h"
#include <cstdio>

uint32_t rvReadCSR( emContext_t *ctx, uint32_t csr )
{
   switch( csr )
   {
      case _RV_CSR_MSTATUS:

         return ctx->mstatus;
         break;

      case _RV_CSR_MISA:

         //rv-32 IM

         return 0b01000000000000000001000100000000;
         break;

      case _RV_CSR_MIE:

         return ctx->mie;
         break;

      case _RV_CSR_MTVEC:

         return ctx->mtvec;
         break;

      case _RV_CSR_MSTATUSH:

         return ctx->mstatush;
         break;

      case _RV_CSR_MSCRATCH:

         return ctx->mscratch;
         break;

      case _RV_CSR_MEPC:

         return ctx->mepc;
         break;

      case _RV_CSR_MCAUSE:

         return ctx->mcause;
         break;

      case _RV_CSR_MTVAL:

         return ctx->mtval;
         break;

      case _RV_CSR_MIP:

         return ctx->mip;
         break;

      case _RV_CSR_MHARTID:

         return 0;
         break;

      default:

         printf( "warning: unknown CSR read: 0x%x\n", csr );
         return 0;
         break;

   }

   return 0;
}

uint32_t rvWriteCSR( emContext_t *ctx, uint32_t csr, uint32_t value )
{
   switch( csr )
   {
      case _RV_CSR_MSTATUS:

         ctx->mstatus = value;
         return 0;
         break;

      case _RV_CSR_MISA:

         printf( "warning: csr write to misa: 0x%x\n", value );
         return 1;
         break;

      case _RV_CSR_MIE:

         ctx->mie = value;
         return 0;
         break;

      case _RV_CSR_MTVEC:

         ctx->mtvec  = value & 0xfffffffc;

         if( value & 3 )
         {
            printf( "warning: csr write to mtvec configures vectored interrupt ( unsupported )\n" );
            return 1;
         }

         return 0;
         break;

      case _RV_CSR_MSTATUSH:

         ctx->mstatush = value;
         return 0;
         break;

      case _RV_CSR_MSCRATCH:

         ctx->mscratch = value;
         return 0;
         break;

      case _RV_CSR_MEPC:

         ctx->mepc = value;
         break;

      case _RV_CSR_MCAUSE:

         ctx->mcause = value;
         break;

      case _RV_CSR_MTVAL:

         ctx->mtval = value;
         break;

      case _RV_CSR_MIP:

         ctx->mip = value;
         break;

      case _RV_CSR_MHARTID:


         printf( "warning: csr write to mhartid: 0x%x\n", value );
         return 1;
         break;

      default:

         printf( "warning: unknown CSR write: 0x%x, 0x%x\n", csr, value );
         return 0;
         break;

   }

   return 0;
}


uint32_t rvReset( emContext_t *ctx )
{
   if( ctx == NULL )
   {
      return 1;
   }

   ctx->sregs = ( int32_t* ) &ctx->regs[0];

   ctx->pc        = 0;
   ctx->regs[0]   = 0;
   ctx->regs[2]   = 0x5a80;

   ctx->instrCounter = 0;

   ctx->mstatus   = 0;
   ctx->mie       = 0;
   ctx->mtvec     = 0;
   ctx->mstatush  = 0;
   
   ctx->mscratch  = 0;
   ctx->mepc      = 0;
   ctx->mcause    = 0;
   ctx->mtval     = 0;
   ctx->mip       = 0;


   return 0;
}


uint32_t rvStep( emContext_t *ctx )
{
   //Instruction decoder

   //common
   uint32_t opcode;
   uint32_t rd;
   uint32_t funct3;
   uint32_t rs1;
   uint32_t rs2;
   uint32_t rtFunct7;

   //I-type
   uint32_t itImm;
   int32_t  itsImm;

   //S-type
   uint32_t stImm;
   int32_t  stsImm;

   //B-type
   uint32_t btImm;
   int32_t  btsImm;

   //U-type
   uint32_t utImm;

   //J-type
   uint32_t jtImm;
   int32_t  jtsImm;

   uint32_t iDecAux;
   uint32_t instruction;
   uint32_t i;
   uint32_t j;

   uint32_t rv;
   
   //fetch instruction

   ctx->instruction  = ctx->fetchInstruction( ctx->pc );
   
   instruction       = ctx->instruction;

   ctx->instrCounter++;

   //decode instruction

   //common

   opcode = instruction & 0b1111111;
   rd     = ( instruction >> 7 ) & 0b11111;
   funct3 = ( instruction >> 12 ) & 0b111;
   rs1    = ( instruction >> 15 ) & 0b11111;
   rs2    = ( instruction >> 20 ) & 0b11111;


   //decode opcodes


   rv = _RVEMUL_UNDEFINED_INSTRUCTION;

   switch( opcode )
   {

      //RV32I

      //R - type
      case 0b0110011:

         //R-type

         rtFunct7  = ( instruction >> 25 ) & 0b1111111;


         //RV32M

         //https://tomverbeure.github.io/rtl/2018/08/12/Multipliers.html#risc-v-multiply-instructions

         if( rtFunct7 == 0x01 )
         {
            switch( funct3 )
            {

            case 0x0:

//             mul   rd, rs1, rs2

               rv = _RVEMUL_OK;

               if( rd )
               {
                  ctx->regs[ rd ] = ctx->regs[rs1] * ctx->regs[rs2];
               }

               break;

            case 0x1:

//             mulh  rd, rs1, rs2

               rv = _RVEMUL_OK;

               if( rd )
               {
                  ctx->regs[ rd ] = (int64_t)( ( (int64_t)ctx->sregs[rs1] * (int64_t)ctx->sregs[rs2] ) ) >> 32;
               }
               break;


            case 0x2:

//             mulsu rd, rs1, rs2

               rv = _RVEMUL_OK;

               if( rd )
               {
                  ctx->regs[ rd ] = (int64_t)( ( (int64_t)ctx->sregs[rs1] * (uint64_t)ctx->regs[rs2] ) ) >> 32;
               }
               break;
 
            case 0x3:

//             mulu  rd, rs1, rs2

               rv = _RVEMUL_OK;

               if( rd )
               {
                  ctx->regs[ rd ] = (uint64_t)( ( (uint64_t)ctx->regs[rs1] * (uint64_t)ctx->regs[rs2] ) ) >> 32;
               }
               break;

            case 0x4:

//             div   rd, rs1, rs2

               rv = _RVEMUL_OK;

               if( rd )
               {
                  if( ctx->regs[rs2] )
                  {
                     ctx->sregs[ rd ] = ctx->sregs[rs1] / ctx->sregs[rs2];
                  }
                  else
                  {
                     ctx->regs[ rd ] = 0;                     
                  }
               }
               break;

            case 0x5:

//             divu  rd, rs1, rs2

               rv = _RVEMUL_OK;

               if( rd )
               {
                  if( ctx->regs[rs2] )
                  {
                     ctx->regs[ rd ] = ctx->regs[rs1] / ctx->regs[rs2];
                  }
                  else
                  {
                     ctx->regs[ rd ] = 0;
                  }

               }
               break;

            case 0x6:

//             rem   rd, rs1, rs2

               rv = _RVEMUL_OK;

               if( rd )
               {
                  if( ctx->regs[rs2] )
                  {
                     ctx->sregs[ rd ] = ctx->sregs[rs1] % ctx->sregs[rs2];
                  }
                  else
                  {
                     ctx->sregs[ rd ] = 0;
                  }
               }
               break;
 
             case 0x7:

//             remu  rd, rs1, rs2

               rv = _RVEMUL_OK;

               if( rd )
               {
                  if( ctx->regs[rs2] )
                  {
                     ctx->regs[ rd ] = ctx->regs[rs1] % ctx->regs[rs2];
                  }
                  else
                  {
                     ctx->regs[ rd ] = 0;
                  }
               }
               break;

           }
         }
         else
         {
            //RV32I
            
            switch( funct3 )
            {
               case 0x0:

                  if( rtFunct7 == 0x00 )
                  {


//                   add   rd, rs1, rs2

                     rv = _RVEMUL_OK;

                     if( rd )
                     {
                        ctx->regs[rd] = ctx->regs[rs1] + ctx->regs[rs2];
                     }

                  }else if( rtFunct7 == 0x20 )
                  {

//                   sub   rd, rs1, rs2

                     rv = _RVEMUL_OK;

                     if( rd )
                     {
                        ctx->regs[rd] = ctx->regs[rs1] - ctx->regs[rs2];
                     }
                  }

                  break;

               case 0x4:

                  if( rtFunct7 == 0x00 )
                  {

//                   xor   rd, rs1, rs2

                     rv = _RVEMUL_OK;

                     if( rd )
                     {
                        ctx->regs[rd] = ctx->regs[rs1] ^ ctx->regs[rs2];
                     }
                  }

                  break;

               case 0x6:

                  if( rtFunct7 == 0x00 )
                  {

//                   or    rd, rs1, rs2

                     rv = _RVEMUL_OK;

                     if( rd )
                     {
                        ctx->regs[rd] = ctx->regs[rs1] | ctx->regs[rs2];
                     }

                  }

                  break;

               case 0x7:

                  if( rtFunct7 == 0x00 )
                  {

//                   and   rd, rs1, rs2

                     rv = _RVEMUL_OK;

                     if( rd )
                     {
                        ctx->regs[rd] = ctx->regs[rs1] & ctx->regs[rs2];
                     }
                  }

                  break;

               case 0x1:

                  if( rtFunct7 == 0x00 )
                  {

//                   sll   rd, rs1, rs2
                     rv = _RVEMUL_OK;

                     if( rd )
                     {
                        ctx->regs[rd] = ctx->regs[rs1] << ( ctx->regs[rs2] & 0x1f );
                     }

                  }

                  break;

               case 0x5:

                  if( rtFunct7 == 0x00 )
                  {

//                   srl   rd, rs1, rs2

                     rv = _RVEMUL_OK;

                     if( rd )
                     {
                        ctx->regs[rd] = ctx->regs[rs1] >> ( ctx->regs[rs2] & 0x1f );
                     }

                  }else if( rtFunct7 == 0x20 )
                  {

//                   sra   rd, rs1, rs2

                     rv = _RVEMUL_OK;

                     if( rd )
                     {
                        if( ctx->regs[rs1] & 0x80000000 )
                        {
                           //extend sign bit
                           ctx->regs[rd] = ctx->regs[rs1];

                           for( i = 0; i < ( ctx->regs[rs2] & 0x1f ); i++ )
                           {
                              ctx->regs[rd] >>=  1;
                              ctx->regs[rd] |=   0x80000000;
                           }
                        }
                        else
                        {
                           ctx->regs[rd] = ctx->regs[rs1] >> ctx->regs[rs2];
                        }
                     }
                  }

                  break;

               case 0x2:

                  if( rtFunct7 == 0x00 )
                  {

//                   slt   rd, rs1, rs2

                     rv = _RVEMUL_OK;

                     if( rd )
                     {

                        if( ctx->sregs[rs1] < ctx->sregs[rs2] )
                        {
                           ctx->regs[rd] = 1;
                        }
                        else
                        {
                           ctx->regs[rd] = 0;
                        }
                     }

                  }

                  break;

               case 0x3:

                  if( rtFunct7 == 0x00 )
                  {

//                   sltu  rd, rs1, rs2

                     rv = _RVEMUL_OK;

                     if( rd )
                     {

                        if( ctx->regs[rs1] < ctx->regs[rs2] )
                        {
                           ctx->regs[rd] = 1;
                        }
                        else
                        {
                           ctx->regs[rd] = 0;
                        }
                     }
                  }
            
                  break;

            }
         }
         break;

      //I - type

      case 0b0010011:
        
         //I-type

         itImm     = ( instruction >> 20 ) & 0b111111111111;
         itsImm = itImm;      

         if( itImm & 0b100000000000 )
         {
            itsImm |= 0b11111111111111111111100000000000;
         }
 
         switch( funct3 )
         {
            case 0x0:

//             addi  rd, rs1, imm
               
               rv = _RVEMUL_OK;

               if( rd )
               {
                  ctx->regs[rd] = ctx->regs[rs1] + itsImm;
               }

               break;

            case 0x4:

//             xori  rd, rs1, imm

               rv = _RVEMUL_OK;

               if( rd )
               {
                  ctx->regs[rd] = ctx->regs[rs1] ^ itsImm;
               }

               break;

            case 0x6:

//             ori   rd, rs1, imm

               rv = _RVEMUL_OK;

               if( rd )
               {
                  ctx->regs[rd] = ctx->regs[rs1] | itsImm;
               }

               break;

            case 0x7:

//             andi  rd, rs1, imm

               rv = _RVEMUL_OK;

               if( rd )
               {
                  ctx->regs[rd] = ctx->regs[rs1] & itsImm;
               }

               break;

            case 0x1:

               //todo: check if itImm(11 downto 5) are zeroes

//             slli  rd, rs1, imm

               rv = _RVEMUL_OK;

               if( rd )
               {
                  ctx->regs[rd] = ctx->regs[rs1] << ( itImm & 0x1f );
               }

               break;

            case 0x5:

                  //todo: do a proper check
                  if( itImm & 0xffe0 )
                  {

//                   srai  rd, rs1, imm

                     rv = _RVEMUL_OK;

                     if( rd )
                     {
                        if( ctx->regs[rs1] & 0x80000000 )
                        {
                           //extend sign bit
                           ctx->regs[rd] = ctx->regs[rs1];

                           for( i = 0; i < ( itImm & 0x1f ); i++ )
                           {
                              ctx->regs[rd] >>=  1;
                              ctx->regs[rd] |=   0x80000000;
                           }
                        }
                        else
                        {
                           ctx->regs[rd] = ctx->regs[rs1] >> ( itImm & 0x1f );
                        }
                     }

                  }
                  else
                  {

//                   srli  rd, rs1, imm
 
                     rv = _RVEMUL_OK;

                     if( rd )
                     {
                        ctx->regs[rd] = ctx->regs[rs1] >> ( itImm & 0x1f );
                     }
                  }

               break;

            case 0x2:

//             slti  rd, rs1, imm

               rv = _RVEMUL_OK;

               if( rd )
               {

                  if( ctx->sregs[rs1] < itsImm )
                  {
                     ctx->regs[rd] = 1;
                  }
                  else
                  {
                     ctx->regs[rd] = 0;
                  }
               }

               break;

            case 0x3:

//             sltiu rd, rs1, imm

               rv = _RVEMUL_OK;

               if( rd )
               {

                  if( ctx->regs[rs1] < itImm )
                  {
                     ctx->regs[rd] = 1;
                  }
                  else
                  {
                     ctx->regs[rd] = 0;
                  }
               }

               break;
         }

      break;

      //I - type

      case 0b0000011:
        

         //I-type

         itImm     = ( instruction >> 20 ) & 0b111111111111;
         itsImm = itImm;      

         if( itImm & 0b100000000000 )
         {
            itsImm |= 0b11111111111111111111100000000000;
         }

         switch( funct3 )
         {

            case 0x0:

//             lb    rd, imm( rs1 )

               rv = _RVEMUL_OK;

               if( rd )
               {

                  i = ctx->regs[ rs1 ];
                  i += itsImm;

                  j = ctx->fetchData( i & 0xfffffffc );

                  switch( i & 3 )
                  {
                     case 0:  // 0 - 7

                        j  &= 0xff;

                        if( j & 0x80 )
                        {
                           j |= 0xffffff80;
                        }

                        ctx->regs[ rd ]  = j;

                        break;

                     case 1: // 8 - 15

                        j  >>= 8;
                        j  &= 0xff;

                        if( j & 0x80 )
                        {
                           j |= 0xffffff80;
                        }

                        ctx->regs[ rd ]  = j;

                        break;

                     case 2: // 16 - 23

                        j  >>= 16;
                        j  &= 0xff;

                        if( j & 0x80 )
                        {
                           j |= 0xffffff80;
                        }

                        ctx->regs[ rd ]  = j;

                        break;

                     case 3: // 24 - 31

                        j  >>= 24;
                        j  &= 0xff;

                        if( j & 0x80 )
                        {
                           j |= 0xffffff80;
                        }

                        ctx->regs[ rd ]  = j;

                        break;
                  }
               }

               break;

            case 0x1:

//             lh    rd, imm( rs1 )

               rv = _RVEMUL_OK;

               if( rd )
               {

                  i = ctx->regs[ rs1 ];
                  i += itsImm;

                  j = ctx->fetchData( i & 0xfffffffc );

                  if( i & 2 )
                  {

                     j  >>= 16;
                     j  &= 0xffff;

                     if( j & 0x8000 )
                     {
                        j |= 0xffff8000;
                     }

                     ctx->regs[ rd ]  = j;
                  }
                  else
                  {

                     j  &= 0xffff;

                     if( j & 0x8000 )
                     {
                        j |= 0xffff8000;
                     }

                     ctx->regs[ rd ]  = j;
                  }
               }
               break;

            case 0x2:

//             lw    rd, imm( rs1 )

               rv = _RVEMUL_OK;

               if( rd )
               {

                  i = ctx->regs[ rs1 ];
                  i += itsImm;

                  ctx->regs[ rd ] = ctx->fetchData( i );

               }

               break;

            case 0x4:

//             lbu   rd, imm( rs1 )

               rv = _RVEMUL_OK;

               if( rd )
               {

                  i = ctx->regs[ rs1 ];
                  i += itsImm;

                  j = ctx->fetchData( i & 0xfffffffc );

                  switch( i & 3 )
                  {
                     case 0:  // 0 - 7

                        j  &= 0xff;


                        ctx->regs[ rd ]  = j;

                        break;

                     case 1: // 8 - 15

                        j  >>= 8;
                        j  &= 0xff;


                        ctx->regs[ rd ]  = j;

                        break;

                     case 2: // 16 - 23

                        j  >>= 16;
                        j  &= 0xff;


                        ctx->regs[ rd ]  = j;

                        break;

                     case 3: // 24 - 31

                        j  >>= 24;
                        j  &= 0xff;

                        ctx->regs[ rd ]  = j;

                        break;
                  }
               }
               break;

            case 0x5:

//             lhu   rd, imm( rs1 )

               rv = _RVEMUL_OK;

               if( rd )
               {

                  i = ctx->regs[ rs1 ];
                  i += itsImm;

                  j = ctx->fetchData( i & 0xfffffffc );

                  if( i & 2 )
                  {

                     j  >>= 16;
                     j  &= 0xffff;

                     ctx->regs[ rd ]  = j;
                  }
                  else
                  {

                     j  &= 0xffff;

                     ctx->regs[ rd ]  = j;
                  }
               }
               break;
         }
         break;

      //S - type

      case 0b0100011:
        
         //S-type

         stImm     = ( instruction >> 7 ) & 0b11111;
         iDecAux   = ( instruction >> 25 ) & 0b1111111;
         stImm     |= iDecAux << 5;
         stsImm    = stImm;

         if( stImm & 0b100000000000 )
         {
            stsImm |= 0b11111111111111111111100000000000;
         }

         switch( funct3 )
         {

            case 0x0:

//             sb    rs2, imm( rs1 )

               rv = _RVEMUL_OK;

               i = ctx->regs[ rs1 ];
               i += stsImm;

               switch( i & 0b11 )
               {
                  case 0:  //0 - 7

                     ctx->storeData( i & 0xfffffffc, 0b0001, ctx->regs[rs2] & 0xff );

                     break;

                  case 1:  //8 - 15

                     ctx->storeData( i & 0xfffffffc, 0b0010, ( ctx->regs[rs2] & 0xff ) << 8 );

                     break;

                  case 2:  //16 - 23

                     ctx->storeData( i & 0xfffffffc, 0b0100, ( ctx->regs[rs2] & 0xff ) << 16 );

                     break;

                  case 3:  //24 - 31

                     ctx->storeData( i & 0xfffffffc, 0b1000, ( ctx->regs[rs2] & 0xff ) << 24 );

                     break;
               }

               break;

            case 0x1:

//             sh    rs2, imm( rs1 )

               rv = _RVEMUL_OK;

               i = ctx->regs[ rs1 ];
               i += stsImm;

               if( i & 2 )
               {
                  ctx->storeData( i & 0xfffffffc, 0b1100, ( ctx->regs[rs2] & 0xffff ) << 16 );
               }
               else
               {
                  ctx->storeData( i & 0xfffffffc, 0b0011, ctx->regs[rs2] & 0xffff );
               }

               break;

            case 0x2:

//             sw    rs2, imm( rs1 )

               rv = _RVEMUL_OK;

               i = ctx->regs[ rs1 ];
               i += stsImm;

               ctx->storeData( i, 0b1111, ctx->regs[rs2] );

               break;
         }

         break; 

      //B - type
      case 0b1100011:
        
         //B-type

         iDecAux   = ( instruction >> 7 ) & 0b1;
         btImm     = iDecAux << 11;
         iDecAux   = ( instruction >> 8 ) & 0b1111;
         btImm     |= iDecAux << 1;
         iDecAux   = ( instruction >> 25 ) & 0b111111;
         btImm     |= iDecAux << 5;
         iDecAux   = ( instruction >> 31 ) & 0b1;
         btImm     |= iDecAux << 12;
         btsImm    = btImm;

         if( btImm & 0b1000000000000 )
         {
            btsImm |= 0b11111111111111111111000000000000;
         }

         switch( funct3 )
         {

            case 0x0:

//             beq   rs1, rs2, imm

               rv = _RVEMUL_OK;

               if( ctx->regs[rs1] == ctx->regs[rs2] )
               {

                  ctx->pc += btsImm;
                  ctx->pc -= 4; 
               }

               break;

            case 0x1:

  //           bne   rs1, rs2, imm

               rv = _RVEMUL_OK;

               if( ctx->regs[rs1] != ctx->regs[rs2] )
               {

                  ctx->pc += btsImm;
                  ctx->pc -= 4; 
               }

               break;

            case 0x4:

//             blt   rs1, rs2, imm

               rv = _RVEMUL_OK;

               if( ctx->sregs[rs1] < ctx->sregs[rs2] )
               {

                  ctx->pc += btsImm;
                  ctx->pc -= 4; 
               }

               break;

            case 0x5:

//             bge   rs1, rs2, imm

               rv = _RVEMUL_OK;

               if( ctx->sregs[rs1] >= ctx->sregs[rs2] )
               {

                  ctx->pc += btsImm;
                  ctx->pc -= 4; 
               }

               break;

            case 0x6:

//             bltu  rs1, rs2, imm

               rv = _RVEMUL_OK;

               if( ctx->regs[rs1] < ctx->regs[rs2] )
               {

                  ctx->pc += btsImm;
                  ctx->pc -= 4; 
               }

               break;

            case 0x7:

//             bgeu  rs1, rs2, imm

               rv = _RVEMUL_OK;

               if( ctx->regs[rs1] >= ctx->regs[rs2] )
               {

                  ctx->pc += btsImm;
                  ctx->pc -= 4; 
               }
               break;
         }

         break;

      //J - type
      case 0b1101111:

         //J-type

         //33222222222211111111110000000000
         //10987654321098765432109876543210
         //21000000000111111111rrrrrooooooo
         //00987654321198765432dddddccccccc
        
         iDecAux   = ( instruction >> 12 ) & 0b11111111;
         jtImm     = iDecAux << 12;

         iDecAux   = ( instruction >> 20 ) & 0b1;
         jtImm     |= iDecAux << 11;

         iDecAux   = ( instruction >> 21 ) & 0b1111111111;
         jtImm     |= iDecAux << 1;
         
         iDecAux   = ( instruction >> 31 ) & 0b1;
         jtImm     |= iDecAux << 20;
         jtsImm    = jtImm;

         if( jtImm & 0b100000000000000000000 )
         {
            jtsImm |= 0b11111111111100000000000000000000;
         }   


//       jal   rd, imm

         rv = _RVEMUL_OK;

         if( rd )
         {
            ctx->regs[rd] = ctx->pc + 4;
         }

         ctx->pc += jtsImm;
         ctx->pc -= 4; 

         break;

      //I - type

      case 0b1100111:


         //I-type

         itImm     = ( instruction >> 20 ) & 0b111111111111;
         itsImm = itImm;      

         if( itImm & 0b100000000000 )
         {
            itsImm |= 0b11111111111111111111100000000000;
         }

         if( funct3 == 0x0 )
         {
         
//          jalr  rd, rs1, imm

            rv = _RVEMUL_OK;

            if( rd )
            {
               ctx->regs[rd] = ctx->pc + 4;
            }

            ctx->pc = ctx->regs[ rs1 ];
            ctx->pc += itsImm;

            ctx->pc -= 4; 

         }
         break;

      //U - type
      case 0b0110111:

         //U-type

         utImm = instruction & 0b11111111111111111111000000000000;
  
//       lui   rd, imm

         rv = _RVEMUL_OK;

         if( rd )
         {

            ctx->regs[ rd ] = utImm;

         }
         break;

      //U - type
      case 0b0010111:

         //U-type

         utImm = instruction & 0b11111111111111111111000000000000;

//       auipc rd, imm

         rv = _RVEMUL_OK;

         if( rd )
         {

            ctx->regs[ rd ] = ctx->pc + utImm;

         }

         break;

      //I - type
      case 0b1110011:

         //I-type

         itImm     = ( instruction >> 20 ) & 0b111111111111;
         itsImm = itImm;      

         if( itImm & 0b100000000000 )
         {
            itsImm |= 0b11111111111111111111100000000000;
         }

         switch( funct3 )
         {

            case 0x0:

               switch( itImm )
               {
                  
                  case 0x0:

                     rv = _RVEMUL_OK;

                     //ecall
                     ctx->mepc   = ctx->pc;
                     ctx->pc     = ( ctx->mtvec & 0xfffffffc ) - 4;

                     ctx->mcause = 11;       //Environment call from M-mode
                     break;

                  case 0x1:
                     
                     //ebreak ( call builtin debugger )                  
                     rv = _RVEMUL_EBREAK;


                     /*
                     //ebreak ( call debugger in emulated mode)
                     ctx->mepc   = ctx->pc;
                     ctx->pc     = ( ctx->mtvec & 0xfffffffc ) - 4;
                     ctx->mcause = 3;     //Breakpoint
                     */

                     break;
                  
                  case 0b001100000010:

                     rv = _RVEMUL_OK;

                     //mret
 
                     ctx->pc = ctx->mepc - 4;
                     break;               

               }

               break;

            case 0x1:

               rv = _RVEMUL_OK;

               //csrrw %s, %d, %s

               if( rd )
               {
                  ctx->regs[ rd ] = rvReadCSR( ctx, itImm );
               }

               rvWriteCSR( ctx, itImm, ctx->regs[ rs1 ] );
               
               break;

            case 0x2:

               rv = _RVEMUL_OK;

               //csrrs %s, %d, %s", regDName, ctx->itImm, regS1Name );

               i = rvReadCSR( ctx, itImm );

               if( rd )
               {
                  ctx->regs[ rd ] = i;
               }

               i |= ctx->regs[ rs1 ];

               rvWriteCSR( ctx, itImm, i );

               break;

            case 0x3:

               rv = _RVEMUL_OK;

               //csrrc %s, %d, %s", regDName, ctx->itImm, regS1Name );

               i = rvReadCSR( ctx, itImm );

               if( rd )
               {
                  ctx->regs[ rd ] = i;
               }

               i &= ctx->regs[ rs1 ] ^ 0xffffffff;

               rvWriteCSR( ctx, itImm, i );

               break;

            case 0x5:

               rv = _RVEMUL_OK;

               //csrrwi %s, %d, %d", regDName, ctx->itImm, ctx->rs1 );

               if( rd )
               {
                  ctx->regs[ rd ] = rvReadCSR( ctx, itImm );
               }

               rvWriteCSR( ctx, itImm, rs1 );

               break;

            case 0x6:

               rv = _RVEMUL_OK;

               //csrrsi %s, %d, %d", regDName, ctx->itImm, ctx->rs1 );

               i = rvReadCSR( ctx, itImm );

               if( rd )
               {
                  ctx->regs[ rd ] = i;
               }

               i |= rs1;

               rvWriteCSR( ctx, itImm, i );

               break;

            case 0x7:

               rv = _RVEMUL_OK;

              //csrrci %s, %d, %d", regDName, ctx->itImm, ctx->rs1 );

               i = rvReadCSR( ctx, itImm );

               if( rd )
               {
                  ctx->regs[ rd ] = i;
               }

               i &= rs1 ^ 0xffffffff;

               rvWriteCSR( ctx, itImm, i );

               break;

         }

         break;
 
   }

   //advance pc
   ctx->pc += 4;

   return rv;
}

uint32_t rvTriggerMtimeIRQ( emContext_t *ctx )
{

   if( ctx->mstatus & 0x08 )
   {

      ctx->mepc   = ctx->pc;
      ctx->pc     = ctx->mtvec & 0xfffffffc;

   }

   return _RVEMUL_OK;
}
