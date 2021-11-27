#ifndef _H_ZX_QEMU_HOOK_ADDR_H_
#define _H_ZX_QEMU_HOOK_ADDR_H_

#define IA32_HOOK_OPCODE    ".byte 0x0f,0x3c"
#define IA32_HOOK_START     1 
#define IA32_HOOK_INSERT    2 
#define IA32_HOOK_STOP      3 
#define IA32_HOOK_DELETE    4 

static inline void __start_qemu_hook(void)
{
    __asm__ __volatile__ (IA32_HOOK_OPCODE "\n" 
			  : 
			  : "a"(IA32_HOOK_START)
              );
}

static inline void __stop_qemu_hook(void)
{
    __asm__ __volatile__ (IA32_HOOK_OPCODE "\n" 
			  : 
			  : "a"(IA32_HOOK_STOP)
              );
}

static inline void __delete_hook_addr_list(unsigned long start_addr, unsigned long size)
{
    __asm__ __volatile__ (IA32_HOOK_OPCODE "\n" 
			  : 
			  : "a"(IA32_HOOK_DELETE),                
				"b"(start_addr),
			    "c"(size)
              );
}


static inline void __insert_hook_addr_list(unsigned long start_addr, unsigned long size)
{
    __asm__ __volatile__ (IA32_HOOK_OPCODE "\n" 
			  : 
			  : "a"(IA32_HOOK_INSERT),
                "b"(start_addr),
			    "c"(size)
              );
}

#endif