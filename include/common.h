#ifndef common_h
#define common_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <device/device_types.h>
#include <CoreFoundation/CoreFoundation.h>
#include <unistd.h>
#include <dlfcn.h>

#define kIORegistryIterateRecursively 0x00000001

typedef uint32_t io_object_t;
typedef io_object_t io_connect_t;
typedef io_object_t io_service_t;
typedef io_object_t io_registry_entry_t;
typedef uint32_t IOSurfaceLockOptions;
typedef struct __IOSurface * IOSurfaceRef;
typedef uint32_t IOOptionBits;
typedef uint32_t IOHIDEventOptionBits;
typedef uint32_t IOHIDEventField;
typedef struct __IOHIDEventSystemClient * IOHIDEventSystemClientRef;
typedef struct __IOHIDEvent * IOHIDEventRef;
typedef char io_name_t[128];
typedef char io_struct_inband_t[4096];
typedef char *io_buf_ptr_t;

typedef struct __attribute__((packed, aligned(4))) {
    mach_msg_header_t Head;
    mach_msg_body_t msgh_body;
    mach_msg_port_descriptor_t port;
    NDR_record_t NDR;
    uint32_t notification_type;
    uint32_t reference;
} set_notification_request_t;

typedef struct __attribute__((packed, aligned(4))) {
    mach_msg_header_t Head;
    NDR_record_t NDR;
    kern_return_t RetCode;
    mach_msg_trailer_t trailer;
} set_notification_reply_t;

extern char **environ;
extern void notify_post(const char *name);
extern void mach_reply_port(void);
extern kern_return_t mach_vm_map(vm_map_t, mach_vm_address_t *, mach_vm_size_t, mach_vm_offset_t, int, mem_entry_name_port_t, memory_object_offset_t, boolean_t, vm_prot_t, vm_prot_t, vm_inherit_t);
extern kern_return_t mach_vm_deallocate(vm_map_t, mach_vm_address_t, mach_vm_size_t);
extern kern_return_t mach_vm_allocate(vm_map_t, mach_vm_address_t *, mach_vm_size_t, int);
extern kern_return_t mach_vm_copy(vm_map_t, mach_vm_address_t, mach_vm_size_t, mach_vm_address_t);
extern kern_return_t mach_vm_read_overwrite(vm_map_read_t, mach_vm_address_t, mach_vm_size_t, mach_vm_address_t, mach_vm_size_t *);
extern kern_return_t mach_vm_write(vm_map_t, mach_vm_address_t, vm_offset_t, mach_msg_type_number_t);
extern kern_return_t bootstrap_look_up(mach_port_t, char *, mach_port_t *);
extern void vsyslog(int, const char *, __darwin_va_list);
extern int host_get_io_master(mach_port_t, mach_port_t *);

#endif /* common_h */
