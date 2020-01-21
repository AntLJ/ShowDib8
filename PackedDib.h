/*-------------------------------------------------------------------------
 PACKEDIB.H -- Header file for PACKEDIB.C
 (c) Charles Petzold, 1998
--------------------------------------------------------------------------*/

BITMAPINFO * PackedDibLoad(PTSTR szFileName);
int PackedDibGetWidth(BITMAPINFO * pPackedDib);
int PackedDibGetHeight(BITMAPINFO * pPackedDib);
int PackedDibGetBitCount(BITMAPINFO * pPackedDib);
int PackedDibGetRowLength(BITMAPINFO * pPackedDib);
int PackedDibGetInfoHeaderSize(BITMAPINFO * pPackedDib);
int PackedDibGetColorsUsed(BITMAPINFO * pPackedDib);
int PackedDibGetNumColors(BITMAPINFO * pPackedDib);
int PackedDibGetColorTableSize(BITMAPINFO * pPackedDib);
RGBQUAD * PackedDibGetColorTablePtr(BITMAPINFO * pPackedDib);
RGBQUAD * PackedDibGetColorTableEntry(BITMAPINFO * pPackedDib, int i);
BYTE * PackedDibGetBitsPtr(BITMAPINFO * pPackedDib);
int PackedDibGetBitsSize(BITMAPINFO * pPackedDib);
HPALETTE PackedDibCreatePalette(BITMAPINFO * pPackedDib);
