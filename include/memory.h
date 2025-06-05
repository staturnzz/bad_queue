#ifndef memory_h
#define memory_h

#include "common.h"

size_t kread_buf(uint32_t addr, void *buf, size_t size);
size_t kwrite_buf(uint32_t addr, void *buf, size_t size);
uint64_t kread64(uint32_t addr);
uint32_t kread32(uint32_t addr);
uint16_t kread16(uint32_t addr);
uint8_t kread8(uint32_t addr);
void kwrite64(uint32_t addr, uint64_t val);
void kwrite32(uint32_t addr, uint32_t val);
void kwrite16(uint32_t addr, uint16_t val);
void kwrite8(uint32_t addr, uint8_t val);
uint32_t kalloc(size_t size);
uint32_t kfree(uint32_t addr, size_t size);

#endif /* memory_h */
