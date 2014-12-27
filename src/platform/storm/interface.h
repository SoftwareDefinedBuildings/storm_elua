
#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <stdint.h>

#define __syscall_body(code) asm volatile (\
    "push {r4-r11}\n\t"\
    "svc %[immediate]\n\t"\
    "pop {r4-r11}\n\t"\
    "bx lr"::[immediate] "I" (code):"memory", "r0")

#define __syscall(code) asm volatile (\
    "push {r4-r11}\n\t"\
    "svc %[immediate]\n\t"\
    "pop {r4-r11}\n\t"\
    ::[immediate] "I" (code):"memory", "r0")

#define KABI_RESUME_PROCESS 0x80
#define KABI_EJECT          0x81

/**
 * Callback signature for a void function taking a single uint32_t argument
 */
typedef void (*cb_u32_t) (void *r, uint32_t);

/**
 * Callback signature for a void function taking a single int32_t argument
 */
typedef void (*cb_i32_t) (void *r, int32_t);


/**
 * Get the version of the kernel on the system.
 *
 * The octets are MAJ.MINOR.SUBMINOR.BUILD.
 * @return The currently loaded kernel's version
 */
uint32_t k_get_kernel_version();
#define ABI_ID_GET_KERNEL_VERSION 1

/**
 *
 * Write to a file descriptor. The kernel should probably provide stdout (1) as the system default UART,
 * which may have some form of framing for process disambiguation. stderr (2) could probably the SWO TRACE
 * output. The flash chip interface might use this too.
 * @param     fd   The file descriptor number
 * @param[in] src  The buffer to read from
 * @param     size The number of bytes to write
 * @return The number of bytes written, or -1 if there was an error.
 */
int32_t k_write(uint32_t fd, uint8_t const *src, uint32_t size);
#define ABI_ID_WRITE 2

/**
 *
 * Set a timeslice function that gets called every N ticks. 1 tick is
 * approx 61 microseconds
 * @param   ticks   The interval in ticks
 * @param   oneshot If 1, this will only occur once.
 * @param   cb      The callback to invoke
 */
//void request_timeslice(uint32_t ticks, uint8_t oneshot, void (*cb)())
//#define ABI_ID_REQUEST_TIMESLICE 3

/**
 *
 * Yield execution from payload to kernel. This has no affect on the payload other than to
 * be friendly in long sections of syscall-free code. Kernel will resume payload execution as soon
 * as the kernel task queue is empty. This call will no longer be necessary if
 * kernel gains time based preemption.
 */
void k_yield();
#define ABI_ID_YIELD 3

/**
 *
 * Read bytes from kernel, synchronously
 * @param      fd   The file descriptor number
 * @param[out] dst  The buffer to write to
 * @param      size The number of bytes to read
 * @return The number of bytes read (which may be less than size), or -1 if there was an error.
           0 denotes EOF
 */
int32_t k_read(uint32_t fd, uint8_t *dst, uint32_t size);
#define ABI_ID_READ 4

/**
 *
 * Read bytes from kernel, asynchronously. Callback happens when there is more than zero bytes
 * available to read. The callback gets deregistered upon invocation. Only one read callback
 * can be registered at a time. The dst buffer must remain available until the
 * callback is invoked.
 * @param      fd   The file descriptor number (only 0 is implemented)
 * @param[out] dst  The buffer to write to
 * @param      size The maximum number of bytes to read
 * @param      cb   The callback(int32_t) to invoke. The parameter value is the same as the return value
                    for k_read()
 * @return 0 on successful registration, EBUSY if there was already a callback registered.
 */
int32_t k_read_async(uint32_t fd, uint8_t *dst, uint32_t size, cb_i32_t cb, void* r);
#define ABI_ID_READ_ASYNC 5

/**
 *
 * Run a callback in the callback queues. If the callback queues are empty, this behaves like
 * k_yield(), keeping the payload process in the runnable state, returning zero.
 */
uint8_t k_run_callback();
#define ABI_ID_RUN_CALLBACK 6

/**
 *
 * Run all callbacks in the callback queues. If the callback queues are empty, wait until there
 * is a callback to run, then run it. This can be used after an asynchronous call to make it
 * synchronous.
 */
void k_wait_callback();
#define ABI_ID_WAIT_CALLBACK 7


#endif
