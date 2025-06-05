
#include "util.h"

kern_return_t (*io_hideventsystem_open)(mach_port_t, mach_port_t, mach_port_t *) = NULL;
kern_return_t (*io_hideventsystem_clear_service_cache)(mach_port_t) = NULL;
kern_return_t (*io_hideventsystem_copy_matching_services)(mach_port_t, void *, int, mach_vm_address_t *, mach_vm_size_t *, mach_vm_address_t *, mach_vm_size_t *) = NULL;
kern_return_t (*io_hideventsystem_queue_create)(mach_port_t, mach_port_t, int, mach_port_t *) = NULL;
kern_return_t (*io_hideventsystem_queue_start)(mach_port_t) = NULL;
kern_return_t (*io_hideventsystem_queue_stop)(mach_port_t) = NULL;
io_service_t (*IOServiceGetMatchingService)(mach_port_t mainPort, CFDictionaryRef matching) = NULL;
CFMutableDictionaryRef (*IOServiceMatching)(const char *name) = NULL;
io_object_t (*IOIteratorNext)(uint32_t iterator) = NULL;
kern_return_t (*IOObjectRelease)(io_object_t) = NULL;
kern_return_t (*IOConnectMapMemory)(io_connect_t, uint32_t, task_port_t, vm_address_t *, vm_size_t *, uint32_t) = NULL;
kern_return_t (*IOConnectSetNotificationPort)(io_connect_t, uint32_t, mach_port_t, uintptr_t) = NULL;
kern_return_t (*IOConnectCallMethod)(mach_port_t, uint32_t, uint64_t *, uint32_t, void *, size_t, uint64_t *, uint32_t *, void *, size_t *) = NULL;
IOHIDEventRef (*IOHIDEventCreateKeyboardEvent)(CFAllocatorRef, uint64_t, uint16_t, uint16_t, Boolean, uint32_t flags) = NULL;
IOHIDEventSystemClientRef (*IOHIDEventSystemClientCreate)(CFAllocatorRef) = NULL;
void (*IOHIDEventSetSenderID)(IOHIDEventRef, uint64_t) = NULL;
void (*IOHIDEventSystemClientDispatchEvent)(IOHIDEventSystemClientRef, IOHIDEventRef) = NULL;
IOHIDEventRef (*IOHIDEventCreateAccelerometerEvent)(CFAllocatorRef, uint64_t, float, float, float, IOOptionBits options) = NULL;
kern_return_t (*IORegistryEntryGetProperty)(io_registry_entry_t, const io_name_t, io_struct_inband_t, uint32_t *) = NULL;
kern_return_t (*IOServiceClose)(io_connect_t) = NULL;
kern_return_t (*io_service_open_extended)(mach_port_t, task_t, uint32_t, NDR_record_t, io_buf_ptr_t, mach_msg_type_number_t, kern_return_t *, mach_port_t *) = NULL;
kern_return_t (*IORegistryEntryCreateIterator)(io_registry_entry_t, const io_name_t, IOOptionBits, mach_port_t *) = NULL;
kern_return_t (*IOServiceOpen)(io_service_t, task_port_t, uint32_t, io_connect_t *) = NULL;
CFTypeRef (*IORegistryEntryCreateCFProperty)(io_registry_entry_t entry, CFStringRef key, CFAllocatorRef allocator, IOOptionBits options) = NULL;
kern_return_t (*IOServiceGetMatchingServices)(mach_port_t mainPort, CFDictionaryRef matching, mach_port_t *existing) = NULL;
kern_return_t (*IOConnectSetCFProperty)(io_connect_t connect, CFStringRef propertyName, CFTypeRef property) = NULL;
kern_return_t (*IORegistryEntryCreateCFProperties)(io_registry_entry_t entry, CFMutableDictionaryRef *properties, CFAllocatorRef allocator, IOOptionBits options);

static uint32_t spin_gadget = 0;

int iosurface_init(void) {
    void *handle = dlopen("/System/Library/Frameworks/IOSurface.framework/IOSurface", RTLD_NOW);
    if (handle == NULL) {
        handle = dlopen("/System/Library/PrivateFrameworks/IOSurface.framework/IOSurface", RTLD_NOW);
        if (handle == NULL) return -1;
    }
    
    find_io_sym(io_hideventsystem_open);
    find_io_sym(io_hideventsystem_clear_service_cache);
    find_io_sym(io_hideventsystem_copy_matching_services);
    find_io_sym(io_hideventsystem_queue_create);
    find_io_sym(io_hideventsystem_queue_start);
    find_io_sym(io_hideventsystem_queue_stop);
    find_io_sym(IOServiceGetMatchingService);
    find_io_sym(IOServiceMatching);
    find_io_sym(IOIteratorNext);
    find_io_sym(IOObjectRelease);
    find_io_sym(IOConnectMapMemory);
    find_io_sym(IOConnectSetNotificationPort);
    find_io_sym(IOConnectCallMethod);
    find_io_sym(IOHIDEventCreateKeyboardEvent);
    find_io_sym(IOHIDEventSystemClientCreate);
    find_io_sym(IOHIDEventSetSenderID);
    find_io_sym(IOHIDEventSystemClientDispatchEvent);
    find_io_sym(IOHIDEventCreateAccelerometerEvent);
    find_io_sym(IORegistryEntryGetProperty);
    find_io_sym(IOServiceClose);
    find_io_sym(io_service_open_extended);
    find_io_sym(IORegistryEntryCreateIterator);
    find_io_sym(IOServiceOpen);
    find_io_sym(IORegistryEntryCreateCFProperty);
    find_io_sym(IOServiceGetMatchingServices);
    find_io_sym(IOConnectSetCFProperty);
    find_io_sym(IORegistryEntryCreateCFProperties);
    return 0;
}

size_t task_read(mach_port_t task, uint32_t addr, void *buf, size_t size) {
    size_t off = 0;
    while (off < size) {
        mach_vm_size_t sz, chunk = 2048;
        if (chunk > size - off) chunk = size - off;
        mach_vm_read_overwrite(task, addr + off, chunk, (uint32_t)buf + off, &sz);
        off += sz;
    }
    return off;
}

size_t task_write(mach_port_t task, uint32_t addr, void *buf, size_t size) {
    size_t off = 0;
    while (off < size) {
        size_t chunk = 2048;
        if (chunk > size - off) chunk = size - off;
        mach_vm_write(task, addr + off, (uint32_t)buf + off, (int)chunk);
        off += chunk;
    }
    return off;
}

uint32_t task_alloc(mach_port_t task, uint32_t size) {
    mach_vm_address_t addr = 0;
    mach_vm_allocate(task, &addr, size, VM_FLAGS_ANYWHERE);
    return (uint32_t)addr;
}

uint32_t task_free(mach_port_t task, uint32_t addr, uint32_t size) {
    mach_vm_deallocate(task, addr, size);
    return (uint32_t)addr;
}

kern_return_t io_connect_set_notification_port_copy_send(mach_port_t connect, uint32_t type, mach_port_t port, uint32_t ref) {
    union {
        set_notification_request_t input;
        set_notification_reply_t output;
    } msg;
    
    msg.input.Head.msgh_bits = MACH_MSGH_BITS_COMPLEX | MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
    msg.input.Head.msgh_remote_port = connect;
    msg.input.Head.msgh_local_port = mig_get_reply_port();
    msg.input.Head.msgh_id = 2818;
    msg.input.Head.msgh_reserved = 0;
    msg.input.msgh_body.msgh_descriptor_count= 1;
    
    msg.input.port.name = port;
    msg.input.port.disposition = MACH_MSG_TYPE_COPY_SEND;
    msg.input.port.type = MACH_MSG_PORT_DESCRIPTOR;
    msg.input.NDR = NDR_record;
    msg.input.notification_type = type;
    msg.input.reference = ref;
    
    return mach_msg(&msg.input.Head, 3, 0x38, 0x2c, msg.input.Head.msgh_local_port, 0, 0);
}
