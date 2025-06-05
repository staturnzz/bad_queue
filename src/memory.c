#include "common.h"
#include "util.h"
#include "exploit.h"

size_t kread_buf(uint32_t addr, void *buf, size_t size) {
    return task_read(kinfo->tfp0, addr, buf, size);
}

size_t kwrite_buf(uint32_t addr, void *buf, size_t size) {
    return task_write(kinfo->tfp0, addr, buf, size);
}

uint64_t kread64(uint32_t addr) {
    uint64_t val = 0;
    kread_buf(addr, &val, 8);
    return val;
}

uint32_t kread32(uint32_t addr) {
    uint32_t val = 0;
    kread_buf(addr, &val, 4);
    return val;
}

uint16_t kread16(uint32_t addr) {
    uint32_t val = 0;
    kread_buf(addr, &val, 2);
    return val;
}

uint8_t kread8(uint32_t addr) {
    uint32_t val = 0;
    kread_buf(addr, &val, 1);
    return val;
}

void kwrite64(uint32_t addr, uint64_t val) {
    kwrite_buf(addr, &val, 8);
}

void kwrite32(uint32_t addr, uint32_t val) {
    kwrite_buf(addr, &val, 4);
}

void kwrite16(uint32_t addr, uint16_t val) {
    kwrite_buf(addr, &val, 2);
}

void kwrite8(uint32_t addr, uint8_t val) {
    kwrite_buf(addr, &val, 1);
}

uint32_t kalloc(size_t size) {
    return task_alloc(kinfo->tfp0, size);
}

uint32_t kfree(uint32_t addr, size_t size) {
    return task_free(kinfo->tfp0, addr, size);
}
