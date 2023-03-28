///////////////////////////////////////////////////////////
//// NOTE(Elias) Dependencies
//// include <stdint.h>, <math.h>

#ifndef BASE_LAYER_H

///////////////////////////////////////////////////////////
//// NOTE(Elias): Helper Macros

#define Stmnt(S) do{ S }while(0)

#define AssertBreak() (*(int *)0 = 0)

#if ENABLE_ASSERT
# define assert(c) Stmnt( if (!(c)){ AssertBreak(); })
#else
# define assert(c)
#endif

#if ENABLE_DEBUG_MESSAGES
#define evalprintS64(x)    (printf("%s = %ld\n", #x, (S64)(x)))
#define evalprintF64(x)    (printf("%s = %f\n", #x, (F64)(x)))
#define evalprintch(x)     (printf("%s = %c\n", #x, (char)(x)))
#define logstr(x)          (printf(x"\n"))
#define logerrstr(s)       (printf("%s, %d, %s:   %s\n",\
                                   __FILE__, __LINE__, __PRETTY_FUNCTION__, s))
#else
#define evalprintS64(x)
#define evalprintF64(x)
#define evalprintch(x)
#define logstr(x)
#define logerrstr(s)
#endif

#define arraycount(a)    (sizeof(a)/sizeof(*(a)))
#define min(a,b)         (((a)<(b))?(a):(b))
#define max(a,b)         (((a)>(b))?(a):(b))
#define clamp(l,x,u)     (((x)<(l))?(l):((u)<(x))?(u):(x))
#define clampbot(l,u)    (max((l),(u)))
#define clamptop(l,u)    (min((l),(u)))
#define inbetween(x,l,u) ((x >= l && x <= u)? 1 : 0)
#define abs(a)           (((a)<0)?(-(a)):(a))

#define internal static 
#define local_persist static
#define global_variable static 

#define true 1
#define false 0

///////////////////////////////////////////////////////////
//// NOTE(Elias): Basic Types 
  
typedef int8_t   S8;
typedef int16_t  S16;
typedef int32_t  S32;
typedef int64_t  S64;
typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef float    F32;
typedef double   F64;
typedef S8       B8;
typedef S16      B16;
typedef S32      B32;
typedef S64      B64;

///////////////////////////////////////////////////////////
//// NOTE(Elias): Basic Constants 

global_variable S8  min_S8  = (S8)  0x80;
global_variable S16 min_S16 = (S16) 0x8000;
global_variable S32 min_S32 = (S32) 0x80000000;
global_variable S64 min_S64 = (S64) 0x8000000000000000llu;

global_variable S8  max_S8  = (S8)  0x7f;
global_variable S16 max_S16 = (S16) 0x7fff;
global_variable S32 max_S32 = (S32) 0x7fffffff;
global_variable S64 max_S64 = (S64) 0x7fffffffffffffffllu;

global_variable U8  max_U8  = (U8)  0xff;
global_variable U16 max_U16 = (U16) 0xffff;
global_variable U32 max_U32 = (U32) 0xffffffff;
global_variable U64 max_U64 = (U64) 0xffffffffffffffffllu;

global_variable F32 pi_F32         = 3.14159265359f;
global_variable F32 tau_F32        = 6.28318530718f;
global_variable F32 e_F32          = 2.71828182846f;
global_variable F32 gold_big_F32   = 1.61803398875f;
global_variable F32 gold_small_F32 = 0.61803398875f;

global_variable F64 pi_F64         = 3.14159265359;
global_variable F64 tau_F64        = 6.28318530718;
global_variable F64 e_F64          = 2.71828182846;
global_variable F64 gold_big_F64   = 1.61803398875;
global_variable F64 gold_small_F64 = 0.61803398875;

///////////////////////////////////////////////////////////
//// NOTE(Elias): Float Constant Functions

internal F32 inf_F32(void);
internal F32 neg_inf_F32(void);
internal F64 inf_F64(void);
internal F64 neg_inf_F64(void);

///////////////////////////////////////////////////////////
//// NOTE(Elias): Math functions 

internal F32 abs_F32(F32 x);
internal F64 abs_F64(F64 x);

#define BASE_LAYER_H
#endif 
