#ifndef _ASM_X86_RMWcc
#define _ASM_X86_RMWcc

#include <linux/asan.h>

#ifdef CC_HAVE_ASM_GOTO

#define __GEN_RMWcc(fullop, var, cc, ...)				\
do {									\
	asan_check(&(var), sizeof(var), true);				\
	asm_volatile_goto (fullop "; j" cc " %l[cc_label]"		\
			: : "m" (var), ## __VA_ARGS__ 			\
			: "memory" : cc_label);				\
	return 0;							\
cc_label:								\
	return 1;							\
} while (0)

#define GEN_UNARY_RMWcc(op, var, arg0, cc) 				\
	__GEN_RMWcc(op " " arg0, var, cc)

#define GEN_BINARY_RMWcc(op, var, vcon, val, arg0, cc)			\
	__GEN_RMWcc(op " %1, " arg0, var, cc, vcon (val))

#else /* !CC_HAVE_ASM_GOTO */

#define __GEN_RMWcc(fullop, var, cc, ...)				\
do {									\
	char c;								\
	asan_check(&(var), sizeof(var), true);				\
	asm volatile (fullop "; set" cc " %1"				\
			: "+m" (var), "=qm" (c)				\
			: __VA_ARGS__ : "memory");			\
	return c != 0;							\
} while (0)

#define GEN_UNARY_RMWcc(op, var, arg0, cc)				\
	__GEN_RMWcc(op " " arg0, var, cc)

#define GEN_BINARY_RMWcc(op, var, vcon, val, arg0, cc)			\
	__GEN_RMWcc(op " %2, " arg0, var, cc, vcon (val))

#endif /* CC_HAVE_ASM_GOTO */

#endif /* _ASM_X86_RMWcc */
