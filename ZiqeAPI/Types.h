#ifndef ZIQEAPI_TYPES_H
#define ZIQEAPI_TYPES_H

#include "Macros.h"

#ifdef __x86_64__
typedef unsigned long ZqRegisterType;
typedef ZqRegisterType ZqSystemCallIDType;

typedef struct {
    /*
     * C ABI says these regs are callee-preserved. They aren't saved on kernel entry
     * unless syscall needs a complete, fully filled "struct pt_regs".
     */
    unsigned long r15;
    unsigned long r14;
    unsigned long r13;
    unsigned long r12;
    unsigned long bp;
    unsigned long bx;
    /* These regs are callee-clobbered. Always saved on kernel entry. */
    unsigned long r11;
    unsigned long r10;
    unsigned long r9;
    unsigned long r8;
    unsigned long ax;
    unsigned long cx;
    unsigned long dx;
    unsigned long si;
    unsigned long di;
    /*
     * On syscall entry, this is syscall#. On CPU exception, this is error code.
     * On hw interrupt, it's IRQ number:
     */
    unsigned long orig_ax;
    /* Return frame for iretq */
    unsigned long ip;
    unsigned long cs;
    unsigned long flags;
    unsigned long sp;
    unsigned long ss;
    /* top of stack page */
} ZqThreadRegisters;

# define ZQ_PAGE_SIZE (4096)
# define ZQ_PAGE_ALIGN(addr) (addr % ZQ_PAGE_SIZE)
#elif defined  (__i386__)
typedef unsigned long RegisterType;
typedef RegisterType SystemCallIDType;

typedef struct {
    unsigned long bx;
    unsigned long cx;
    unsigned long dx;
    unsigned long si;
    unsigned long di;
    unsigned long bp;
    unsigned long ax;
    unsigned long ds;
    unsigned long es;
    unsigned long fs;
    unsigned long gs;
    unsigned long orig_ax;
    unsigned long ip;
    unsigned long cs;
    unsigned long flags;
    unsigned long sp;
    unsigned long ss;
} ZqThreadRegisters;
#else
# error "Unsupported machine."
#endif

#ifdef __cplusplus
# include <cstdint>
# include <cstddef>
#else
# include <linux/types.h>
typedef u8 uint8_t;
typedef u16 uint16_t;
typedef u32 uint32_t;
typedef u64 uint64_t;

typedef s8 int8_t;
typedef s16 int16_t;
typedef s32 int32_t;
typedef s64 int64_t;
#endif

typedef char ZqBool;
#define ZQ_FALSE (0)
#define ZQ_TRUE (!ZQ_FALSE)

typedef uint64_t ZqSizeType;
typedef int64_t  ZqDifferenceType;

typedef struct {
  ZqThreadRegisters registers;

  ZqBool isRunning;
} ZqThreadStatus;

#endif
