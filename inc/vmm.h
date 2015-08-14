#ifndef __VMM_H
#define __VMM_H

#include <type.h>
#include <isr.h>

/* CPU */
#define CRO_PG  0x80000000 

/* pde&pdt attribute */
#define PTE_P   0x1     // Present
#define PTE_R   0x2     // Read/Write, can be read&write when set
#define PTE_U   0x4     // User / Kern
#define PTE_K   0x0     // User / Kern #define PTE_W   0x8     // Write through
#define PTE_D   0x10    // Cache disable
#define PTE_A   0x20    // Accessed
#define PTE_S   0x40    // Page size, 0 for 4kb pre page
#define PTE_G   0x80    // Ignored

/**/
#define USER_BASE 0xc0000000

/* 对于一个32位虚拟地址（virtual address）
 * 32-22: 页目录号 21-12: 页表号 11-0: 页内偏移 */

/* 4k per page */
#define PAGE_SIZE 4096 

/* 页掩码，取高20位 */
#define PAGE_MASK 0xfffff000

/* addr align */
#define PAGE_ALIGN_DOWN(x) ((x) & PAGE_MASK)
#define PAGE_ALIGN_UP(x) (((x) + PAGE_SIZE - 1) & PAGE_MASK)

/* get specific index of virtual address */
#define PDE_INDEX(x) (((x) >> 22) & 0x3ff)  // 获得地址x对应的页目录号
#define PTE_INDEX(x) (((x) >> 12) & 0x3ff)  // 获得页表号
#define OFFSET_INDEX(x) ((x) & 0xfff)       // 获得页内偏移

/* page global directory */
typedef uint32_t pde_t;
/* page talbe entry */
typedef uint32_t pte_t;

/* size of page global dirctory */
#define PDE_SIZE (PAGE_SIZE/sizeof(pte_t))
/* size of page talbe entry */
#define PTE_SIZE (PAGE_SIZE/sizeof(uint32_t))

/* 128个页表 每个PAGE_SIZE（4M）大小, 
 * 4M * 128 = 512M */
#define PTE_COUNT 1024

/* export page dirctroy */
extern pde_t pde_kern[PDE_SIZE];

void vmm_init();
void vmm_switch_pde(uint32_t pgdir);
void vmm_map(pde_t *pgdir, uint32_t va, uint32_t pa, uint32_t flags);
void vmm_unmap(pde_t *pgdir, uint32_t va);
int vmm_get_mapping(pde_t *pgdir, uint32_t va, uint32_t *pa);
void vmm_test();

/* isr 14 */
void page_fault(struct regs_s *regs);
#endif
