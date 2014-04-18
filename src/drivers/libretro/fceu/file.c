/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "fceu-types.h"
#include "file.h"
#include "fceu-endian.h"
#include "fceu-memory.h"
#include "driver.h"
#include "general.h"

typedef struct {
	uint8 *data;
	uint32 size;
	uint32 location;
} MEMWRAP;

void ApplyIPS(FILE *ips, MEMWRAP *dest)
{
}

static MEMWRAP *MakeMemWrap(void *tz, int type)
{
	MEMWRAP *tmp;

	if (!(tmp = (MEMWRAP*)FCEU_malloc(sizeof(MEMWRAP))))
		goto doret;
	tmp->location = 0;

   fseek((FILE*)tz, 0, SEEK_END);
   tmp->size = ftell((FILE*)tz);
   fseek((FILE*)tz, 0, SEEK_SET);
   if (!(tmp->data = (uint8*)FCEU_malloc(tmp->size)))
   {
      free(tmp);
      tmp = 0;
      goto doret;
   }
   fread(tmp->data, 1, tmp->size, (FILE*)tz);

 doret:
	if (type == 0)
		fclose((FILE*)tz);
	return tmp;
}

#ifndef __GNUC__
 #define strcasecmp strcmp
#endif

FCEUFILE * FCEU_fopen(const char *path, const char *ipsfn, char *mode, char *ext)
{
	FCEUFILE *fceufp;
	void *t;

	fceufp = (FCEUFILE*)malloc(sizeof(FCEUFILE));

	if ((t = FCEUD_UTF8fopen(path, mode)))
   {
		fseek((FILE*)t, 0, SEEK_SET);
		fceufp->type = 0;
		fceufp->fp = t;
		return fceufp;
	}

	free(fceufp);
	return 0;
}

int FCEU_fclose(FCEUFILE *fp)
{
   fclose((FILE*)fp->fp);
	free(fp);
	fp = 0;
	return 1;
}

uint64 FCEU_fread(void *ptr, size_t size, size_t nmemb, FCEUFILE *fp)
{
   return fread(ptr, size, nmemb, (FILE*)fp->fp);
}

uint64 FCEU_fwrite(void *ptr, size_t size, size_t nmemb, FCEUFILE *fp)
{
	return fwrite(ptr, size, nmemb, (FILE*)fp->fp);
}

int FCEU_fseek(FCEUFILE *fp, long offset, int whence)
{
	return fseek((FILE*)fp->fp, offset, whence);
}

uint64 FCEU_ftell(FCEUFILE *fp)
{
	return ftell((FILE*)fp->fp);
}

void FCEU_rewind(FCEUFILE *fp)
{
	fseek(fp->fp, 0, SEEK_SET);
}

int FCEU_read16le(uint16 *val, FCEUFILE *fp)
{
	uint8 t[2];

   if (fread(t, 1, 2, (FILE*)fp->fp) != 2)
      return(0);
	*val = t[0] | (t[1] << 8);
	return(1);
}

int FCEU_read32le(uint32 *Bufo, FCEUFILE *fp)
{
   return read32le(Bufo, (FILE*)fp->fp);
}

int FCEU_fgetc(FCEUFILE *fp)
{
	return fgetc((FILE*)fp->fp);
}

uint64 FCEU_fgetsize(FCEUFILE *fp)
{
   long t, r;
   t = ftell((FILE*)fp->fp);
   fseek((FILE*)fp->fp, 0, SEEK_END);
   r = ftell((FILE*)fp->fp);
   fseek((FILE*)fp->fp, t, SEEK_SET);
   return r;
}

int FCEU_fisarchive(FCEUFILE *fp)
{
	return 0;
}
