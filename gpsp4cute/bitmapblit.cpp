/* gpsp4cute
 *
 * Copyright (C) 2010 Summeli <summeli@summeli.fi>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <e32cmn.h>

#include "bitmapblit.h"


inline void copyPixel16MU( TUint32* bitmap, TUint16* screen, TUint32 red,
					TUint32 green, TUint32 blue )
	{
	red = (*screen >> 11) & 0x1F;
	green = (*screen  >> 5) & 0x3F;
	blue = (*screen  & 0x001F);  
	
	red = (red << 3) | (red >> 2);
	green = (green << 2) | (green >> 4);
	blue = (blue << 3) | (blue >> 2);

	*bitmap = (red << 16) | (green << 8) | (blue);
	}

void Blitkeepratio( TUint8* aScreen, TUint8* aBitmap )
	{
	TUint32* bitmap = (TUint32*) aBitmap;
	TUint16* screen = (TUint16*) aScreen;
	TUint32* screen_temp;
	
	TUint32 red = 0;
	TUint32 green = 0;
	TUint32 blue = 0;
	for(TInt j=0; j<160;j++)
	 {
	 for( TInt i=0; i<240; i++)
	   {
	   copyPixel16MU( bitmap, screen, red, green, blue );
	   bitmap++;
	   copyPixel16MU( bitmap, screen, red, green, blue );
	   screen++;
	   bitmap++;
	   }
	 screen_temp = bitmap - 480;
	 memcpy(bitmap, screen_temp, 4*480);
	 bitmap += 480;
	 }
	
	}

void BlitWidgetDSA( TUint8* aScreen, TUint8* aBitmap )
	{
	TUint32* bitmap = (TUint32*) aBitmap;
	TUint16* screen = (TUint16*) aScreen;
	TUint32* screen_temp;
	
	TUint32 red = 0;
	TUint32 green = 0;
	TUint32 blue = 0;
	
	TUint32 extinterp = 0;
	for(TInt j=0; j<160;j++)
	 {
	 for( TInt i=0; i<240; i++)
	   {
	   copyPixel16MU( bitmap, screen, red, green, blue );
	   bitmap++;
	   copyPixel16MU( bitmap, screen, red, green, blue );
	   screen++;
	   bitmap++;
	   }
	 extinterp++;
	 if( extinterp == 4 )
		 {
		 screen_temp = bitmap - 480;
		 memcpy(bitmap, screen_temp, 4*480);
		 bitmap += 480;
		 extinterp = 0;
		 }
	 screen_temp = bitmap - 480;
	 memcpy(bitmap, screen_temp, 4*480);
	 bitmap += 480;
	 }
	
	}

void BlitWidgetFullScreen( TUint8* aScreen, TUint8* aBitmap )
	{
	TUint32* bitmap = (TUint32*) aBitmap;
	TUint16* screen = (TUint16*) aScreen;
	TUint32* screen_temp;
	
	TUint32 red = 0;
	TUint32 green = 0;
	TUint32 blue = 0;
	
	TUint32 extinterp = 0;
	TUint32 extin = 0;
	for(TInt j=0; j<160;j++)
	 {
	 for( TInt i=0; i<240; i++)
	   {
	   copyPixel16MU( bitmap, screen, red, green, blue );
	   bitmap++;
	   copyPixel16MU( bitmap, screen, red, green, blue );
	   screen++;
	   bitmap++;
	   //TODO!
	   /*
	   if( extin == 3 )
		 {
		 copyPixel16MU( bitmap, screen, red, green, blue );
		 bitmap++;
		 extin = 0;
		 }
	   extin++;*/
	   }
	 extinterp++;
	 if( extinterp == 4 )
		 {
		 screen_temp = bitmap - 640;
		 memcpy(bitmap, screen_temp, 4*640);
		 bitmap += 640;
		 extinterp = 0;
		 }
	 screen_temp = bitmap - 640;
	 memcpy(bitmap, screen_temp, 4*640);
	 bitmap += 640;
	 }
	}
