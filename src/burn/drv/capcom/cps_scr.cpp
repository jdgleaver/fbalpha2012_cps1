#include "cps.h"

// CPS Scroll (Background Layers)

// Base = 0x4000 long tile map
// sx=Scroll X value, sy=Scroll Y value,
INT32 Ghouls=0;
INT32 Ssf2t=0;
INT32 Xmcota=0;

INT32 Scroll1TileMask = 0;
INT32 Scroll2TileMask = 0;
INT32 Scroll3TileMask = 0;

INT32 Cps1Scr1Draw(UINT8 *Base,INT32 sx,INT32 sy)
{
   INT32 x,y;
   INT32 nKnowBlank=-1; // The tile we know is blank
   INT32 ix=(sx>>3)+1;
   INT32 iy=(sy>>3)+1;
   sx&=7; sy&=7; sx=8-sx; sy=8-sy;

   for (y=-1; y<28; y++)
   {
      for (x=-1; x<48; x++)
      {
         INT32 t,a;
         UINT16 *pst;
         INT32 fx=ix+x;
         INT32 fy=iy+y; // fx/fy= 0 to 63
         // Find tile address
         INT32 p=((fy&0x20)<<8) | ((fx&0x3f)<<7) | ((fy&0x1f)<<2);

         p &= 0x3fff;
         pst=(UINT16 *)(Base + p);

         t = BURN_ENDIAN_SWAP_INT16(pst[0]);

         if (Scroll1TileMask) t &= Scroll1TileMask;

         t = GfxRomBankMapper(GFXTYPE_SCROLL1, t);
         if (t == -1)
            continue;

         t<<=6; // Get real tile address

         t+=nCpsGfxScroll[1]; // add on offset to scroll tiles
         if (t==nKnowBlank) continue; // Don't draw: we know it's blank

         a = BURN_ENDIAN_SWAP_INT16(pst[1]);

         CpstSetPal(0x20 | (a&0x1f));

         // Don't need to clip except around the border
         if (x<0 || x>=48-1 || y<0 || y>=28-1)
            nCpstType=CTT_8X8 | CTT_CARE;
         else
            nCpstType=CTT_8X8;

         nCpstX=sx+(x<<3); nCpstY=sy+(y<<3);
         nCpstTile=t; nCpstFlip=(a>>5)&3;

         if (nBgHi)
            CpstPmsk = BURN_ENDIAN_SWAP_INT16(*(UINT16*)(CpsSaveReg[0] + MaskAddr[(a & 0x180) >> 7]));

         if(CpstOneDoX[nBgHi]())
            nKnowBlank=t;
      }
   }

   return 0;
}

INT32 Cps1Scr3Draw(UINT8 *Base,INT32 sx,INT32 sy)
{
   INT32 x,y;
   INT32 nKnowBlank=-1; // The tile we know is blank
   INT32 ix=(sx>>5)+1;
   INT32 iy=(sy>>5)+1;
   sx&=31; sy&=31; sx=32-sx; sy=32-sy;

   for (y=-1; y<7; y++)
   {
      for (x=-1; x<12; x++)
      {
         INT32 t,a;
         UINT16 *pst;
         INT32 fx=ix+x;
         INT32 fy=iy+y; // fx/fy= 0 to 63

         // Find tile address
         INT32 p=((fy&0x38)<<8) | ((fx&0x3f)<<5) | ((fy&0x07)<<2);
         p&=0x3fff;
         pst=(UINT16 *)(Base + p);

         t = BURN_ENDIAN_SWAP_INT16(pst[0]);

         if (Scroll3TileMask) t &= Scroll3TileMask;

         t = GfxRomBankMapper(GFXTYPE_SCROLL3, t);
         if (t == -1) continue;

         t<<=9; // Get real tile address
         t+=nCpsGfxScroll[3]; // add on offset to scroll tiles

         if (t==nKnowBlank) continue; // Don't draw: we know it's blank

         a = BURN_ENDIAN_SWAP_INT16(pst[1]);

         CpstSetPal(0x60 | (a&0x1f));
         nCpstType=CTT_32X32;

         // Don't need to clip except around the border
         if (x<0 || x>=12-1 || y<0 || y>=7-1)
            nCpstType |= CTT_CARE;

         nCpstX=sx+(x<<5); nCpstY=sy+(y<<5);
         nCpstTile=t; nCpstFlip=(a>>5)&3;

         if (nBgHi)
            CpstPmsk = BURN_ENDIAN_SWAP_INT16(*(UINT16*)(CpsSaveReg[0] + MaskAddr[(a & 0x180) >> 7]));

         if(CpstOneDoX[nBgHi]())
            nKnowBlank=t;
      }
   }

   return 0;
}

