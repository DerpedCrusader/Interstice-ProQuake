/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// common.h  -- general definitions

#ifndef COMMON_H
#define COMMON_H



#if !defined BYTE_DEFINED
typedef unsigned char 		byte;
#define BYTE_DEFINED 1
#endif

#undef true
#undef false

#if defined(PSP) && defined(__cplusplus)
typedef enum {qfalse, qtrue}	qboolean;
#else
typedef enum {false, true}	qboolean;
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif


#ifndef QMIN
#define QMIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef QMAX
#define QMAX(a, b) ((a) > (b) ? (a) : (b))
#endif

// by joe
#define bound(a, b, c) ((a) >= (c) ? (a) : (b) < (a) ? (a) : (b) > (c) ? (c) : (b))

//============================================================================

typedef struct sizebuf_s
{
	qboolean	allowoverflow;	// if false, do a Sys_Error
	qboolean	overflowed;		// set to true if the buffer size failed
	byte	*data;
	int		maxsize;
	int		cursize;
} sizebuf_t;

void SZ_Alloc (sizebuf_t *buf, int startsize);
void SZ_Free (sizebuf_t *buf);
void SZ_Clear (sizebuf_t *buf);
void *SZ_GetSpace (sizebuf_t *buf, int length);
void SZ_Write (sizebuf_t *buf, void *data, int length);
void SZ_Print (sizebuf_t *buf, char *data);	// strcats onto the sizebuf

//============================================================================

typedef struct link_s
{
	struct link_s	*prev, *next;
} link_t;

// Baker: this stuff is static in sv_world.c
//void ClearLink (link_t *l);
//void RemoveLink (link_t *l);
//void InsertLinkBefore (link_t *l, link_t *before);
//void InsertLinkAfter (link_t *l, link_t *after);

// (type *)STRUCT_FROM_LINK(link_t *link, type, member)
// ent = STRUCT_FROM_LINK(link,entity_t,order)
// FIXME: remove this mess!
#define	STRUCT_FROM_LINK(l,t,m) ((t *)((byte *)l - (int)&(((t *)0)->m)))

//============================================================================

#define Q_MAXCHAR ((char)0x7f)
#define Q_MAXSHORT ((short)0x7fff)
#define Q_MAXINT	((int)0x7fffffff)
#define Q_MAXLONG ((int)0x7fffffff)
#define Q_MAXFLOAT ((int)0x7fffffff)

#define Q_MINCHAR ((char)0x80)
#define Q_MINSHORT ((short)0x8000)
#define Q_MININT 	((int)0x80000000)
#define Q_MINLONG ((int)0x80000000)
#define Q_MINFLOAT ((int)0x7fffffff)

//============================================================================

extern	qboolean		bigendien;

extern	short	(*BigShort) (short l);
extern	int	(*BigLong) (int l);
extern	float	(*BigFloat) (float l);

#define LittleLong(l) l
#define LittleShort(l) l
#define LittleFloat(l) l

//============================================================================

void MSG_WriteChar (sizebuf_t *sb, int c);
void MSG_WriteByte (sizebuf_t *sb, int c);
void MSG_WriteShort (sizebuf_t *sb, int c);
void MSG_WriteLong (sizebuf_t *sb, int c);
void MSG_WriteFloat (sizebuf_t *sb, float f);
void MSG_WriteString (sizebuf_t *sb, char *s);
void MSG_WriteCoord (sizebuf_t *sb, float f);
void MSG_WriteAngle (sizebuf_t *sb, float f);
void MSG_WritePreciseAngle (sizebuf_t *sb, float f); // JPG - precise aim!!
#ifdef FITZQUAKE_PROTOCOL
void MSG_WriteAngle16 (sizebuf_t *sb, float f); //johnfitz
#endif

extern	int			msg_readcount;
extern	qboolean	msg_badread;		// set if a read goes beyond end of message

void MSG_BeginReading (void);
int MSG_ReadChar (void);
int MSG_ReadByte (void);
#ifdef PROQUAKE_EXTENSION
int MSG_PeekByte (void); // JPG - need this to check for ProQuake messages
#endif
int MSG_ReadShort (void);
int MSG_ReadLong (void);
float MSG_ReadFloat (void);
char *MSG_ReadString (void);

float MSG_ReadCoord (void);
float MSG_ReadAngle (void);
float MSG_ReadPreciseAngle (void); // JPG - precise aim!!
#ifdef FITZQUAKE_PROTOCOL
float MSG_ReadAngle16 (void); //johnfitz
#endif

//============================================================================

#ifdef _WIN32

#undef snprintf
#undef vsnprintf
#define	vsnprintf _vsnprintf
#define snprintf _snprintf

// MSVC has a different name for several standard functions
# define strcasecmp stricmp
# define strncasecmp strnicmp
#endif

#ifdef BUILD_MP3_VERSION

int		va_snprintf (char *function, char *buffer, size_t buffersize, const char *format, ...);
int		va_vsnprintf (char *function, char *buffer, size_t buffersize, const char *format, va_list args);

#endif


int dpsnprintf (char *buffer, size_t buffersize, const char *format, ...);
int dpvsnprintf (char *buffer, size_t buffersize, const char *format, va_list args);



#if !defined(FLASH)

char *strltrim(char *s);

// strlcat and strlcpy, from OpenBSD
// Most (all?) BSDs already have them
#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__) || defined(MACOSX)
# define HAVE_STRLCAT 1
# define HAVE_STRLCPY 1
#endif

#ifndef HAVE_STRLCAT
/*
 * Appends src to string dst of size siz (unlike strncat, siz is the
 * full size of dst, not space left).  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz <= strlen(dst)).
 * Returns strlen(src) + MIN(siz, strlen(initial dst)).
 * If retval >= siz, truncation occurred.
 */
size_t strlcat(char *dst, const char *src, size_t siz);
#endif  // #ifndef HAVE_STRLCAT

#ifndef HAVE_STRLCPY
/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
size_t strlcpy(char *dst, const char *src, size_t siz);

#endif  // #ifndef HAVE_STRLCPY

#endif



//void Q_strcpy (char *dest, char *src);
//void Q_strncpy (char *dest, char *src, int count);


//int Q_strcasecmp (char *s1, char *s2);
//int Q_strncasecmp (char *s1, char *s2, int n);
//int	Q_atoi (char *str);
//float Q_atof (char *str);

//void Q_strncpyz (char *dest, char *src, size_t size);
//void Q_snprintfz (char *dest, size_t size, char *fmt, ...);
//============================================================================

extern	char		com_token[1024];
extern	qboolean	com_eof;

char *COM_Parse (char *data);


extern	int		com_argc;
extern	char	**com_argv;

int COM_CheckParm (char *parm);
void COM_Init (char *path);
void COM_InitArgv (int argc, char **argv);

char *COM_SkipPath (char *pathname);
void COM_StripExtension (char *in, char *out);
char *COM_FileExtension (char *in);
void COM_FileBase (char *in, char *out);
void COM_DefaultExtension (char *path, char *extension);

char	*va(char *format, ...);
// does a varargs printf into a temp buffer

char *CopyString (char *s);

//============================================================================

extern int com_filesize;
struct cache_user_s;

extern	char	com_gamedir[MAX_OSPATH];
extern char	com_basedir[MAX_OSPATH];

void COM_ForceExtension (char *path, char *extension);	// by joe

void COM_WriteFile (char *filename, void *data, int len);

void COM_CreatePath (char *path);
int COM_OpenFile (char *filename, int *hndl);
int COM_FOpenFile (char *filename, int *file);
void COM_CloseFile (int h);

byte *COM_LoadStackFile (char *path, void *buffer, int bufsize);
byte *COM_LoadTempFile (char *path);
byte *COM_LoadHunkFile (char *path);
void COM_LoadCacheFile (char *path, struct cache_user_s *cu);
#ifdef HTTP_DOWNLOAD
void COM_GetFolder (char *in, char *out);//R00k
#endif

char *COM_Quakebar (int len);


extern	struct cvar_s	registered;

extern qboolean		standard_quake, rogue, hipnotic;

#ifdef SUPPORTS_KUROK
extern qboolean		kurok;
#endif
extern qboolean		mod_deeplair, mod_conhide,  mod_nosoundwarn;

extern void COM_ToLowerString(char *in, char *out);

void R_PreMapLoad (char *mapname);

#endif // COMMON_H