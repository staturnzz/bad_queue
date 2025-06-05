#ifndef util_h
#define util_h

#include "common.h"

#define find_io_sym(name) if ((name = dlsym(handle, #name)) == NULL) return -1;

extern char **environ;
extern kern_return_t (*io_hideventsystem_open)(mach_port_t, mach_port_t, mach_port_t *);
extern kern_return_t (*io_hideventsystem_clear_service_cache)(mach_port_t);
extern kern_return_t (*io_hideventsystem_copy_matching_services)(mach_port_t, void *, int, mach_vm_address_t *, mach_vm_size_t *, mach_vm_address_t *, mach_vm_size_t *);
extern kern_return_t (*io_hideventsystem_queue_create)(mach_port_t, mach_port_t, int, mach_port_t *);
extern kern_return_t (*io_hideventsystem_queue_start)(mach_port_t);
extern kern_return_t (*io_hideventsystem_queue_stop)(mach_port_t);
extern io_service_t (*IOServiceGetMatchingService)(mach_port_t mainPort, CFDictionaryRef matching);
extern CFMutableDictionaryRef (*IOServiceMatching)(const char *name);
extern io_object_t (*IOIteratorNext)(uint32_t iterator);
extern kern_return_t (*IOObjectRelease)(io_object_t);
extern kern_return_t (*IOConnectMapMemory)(io_connect_t, uint32_t, task_port_t, vm_address_t *, vm_size_t *, uint32_t);
extern kern_return_t (*IOConnectSetNotificationPort)(io_connect_t, uint32_t, mach_port_t, uintptr_t);
extern kern_return_t (*IOConnectCallMethod)(mach_port_t, uint32_t, uint64_t *, uint32_t, void *, size_t, uint64_t *, uint32_t *, void *, size_t *);
extern IOHIDEventRef (*IOHIDEventCreateKeyboardEvent)(CFAllocatorRef, uint64_t, uint16_t, uint16_t, Boolean, uint32_t flags);
extern IOHIDEventSystemClientRef (*IOHIDEventSystemClientCreate)(CFAllocatorRef);
extern void (*IOHIDEventSetSenderID)(IOHIDEventRef, uint64_t);
extern void (*IOHIDEventSystemClientDispatchEvent)(IOHIDEventSystemClientRef, IOHIDEventRef);
extern IOHIDEventRef (*IOHIDEventCreateAccelerometerEvent)(CFAllocatorRef, uint64_t, float, float, float, IOOptionBits options);
extern kern_return_t (*IORegistryEntryGetProperty)(io_registry_entry_t, const io_name_t, io_struct_inband_t, uint32_t *);
extern kern_return_t (*IOServiceClose)(io_connect_t);
extern kern_return_t (*io_service_open_extended)(mach_port_t, task_t, uint32_t, NDR_record_t, io_buf_ptr_t, mach_msg_type_number_t, kern_return_t *, mach_port_t *);
extern kern_return_t (*IORegistryEntryCreateIterator)(io_registry_entry_t, const io_name_t, IOOptionBits, mach_port_t *);
extern kern_return_t (*IOServiceOpen)(io_service_t, task_port_t, uint32_t, io_connect_t *);
extern CFTypeRef (*IORegistryEntryCreateCFProperty)(io_registry_entry_t entry, CFStringRef key, CFAllocatorRef allocator, IOOptionBits options);
extern kern_return_t (*IOServiceGetMatchingServices)(mach_port_t mainPort, CFDictionaryRef matching, mach_port_t *existing);
extern kern_return_t (*IOConnectSetCFProperty)(io_connect_t connect, CFStringRef propertyName, CFTypeRef property);
extern kern_return_t (*IORegistryEntryCreateCFProperties)(io_registry_entry_t entry, CFMutableDictionaryRef *properties, CFAllocatorRef allocator, IOOptionBits options);

int iosurface_init(void);
size_t task_read(mach_port_t task, uint32_t addr, void *buf, size_t size);
size_t task_write(mach_port_t task, uint32_t addr, void *buf, size_t size);
uint32_t task_alloc(mach_port_t task, uint32_t size);
uint32_t task_free(mach_port_t task, uint32_t addr, uint32_t size);
kern_return_t io_connect_set_notification_port_copy_send(mach_port_t connect, uint32_t type, mach_port_t port, uint32_t ref);

#endif /* util_h */
