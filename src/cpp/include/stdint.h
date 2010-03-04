#ifndef __TEOS_STDINT_H__
#define __TEOS_STDINT_H__

#ifdef __GNUC__

typedef unsigned char 	uint8_t;
typedef signed char 	int8_t;
typedef unsigned short 	uint16_t;
typedef signed short 	int16_t;
typedef unsigned int 	uint32_t;
typedef signed int 		int32_t;
typedef unsigned long	uint64_t;
typedef signed long 	int64_t;
typedef unsigned long	uintptr_t;
typedef signed long		intptr_t;
typedef unsigned int	uint_fast8_t;
typedef unsigned int	uint_fast16_t;
typedef unsigned int	uint_fast32_t;
typedef long			uint_fast64_t;
typedef int				int_fast8_t;
typedef int				int_fast16_t;
typedef int				int_fast32_t;
typedef long			int_fast64_t;
typedef uint8_t			uint_least8_t;
typedef uint16_t		uint_least16_t;
typedef uint32_t		uint_least32_t;
typedef uint64_t		uint_least64_t;
typedef int8_t			int_least8_t;
typedef int16_t			int_least16_t;
typedef int32_t			int_least32_t;
typedef int64_t			int_least64_t;
typedef int64_t			intmax_t;
typedef uint64_t		uintmax_t;

#endif

#endif
