#ifdef __x86_64__
typedef unsigned long ZqRegisterType;
typedef ZqRegisterType ZqSystemCallIDType;
#else
typedef int RegisterType;
typedef RegisterType SystemCallIDType;
#endif

typedef char ZqBool;
#define ZQ_TRUE (1)
#define ZQ_FALSE (0)
