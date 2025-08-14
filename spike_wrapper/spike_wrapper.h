#ifndef SPIKE_WRAPPER_H
#define SPIKE_WRAPPER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 从ELF文件路径运行spike调试命令
 * @param elf_file_path ELF文件的路径
 * @param debug_cmds_string 调试命令字符串
 * @param isa_string ISA字符串
 * @param output_buffer 输出缓冲区（由调用者分配）
 * @param buffer_size 输出缓冲区大小
 * @return 实际输出的字符数，-1表示错误
 */
int spike_debug_cmd_str_elf_file(const char* elf_file_path,
                         const char* debug_cmds_string,
                         const char* isa_string,
                         char* output_buffer,
                         size_t buffer_size);

/**
 * 从ELF文件路径运行spike调试命令
 * @param elf_file_path ELF文件的路径
 * @param debug_cmds_path 调试命令文件路径
 * @param isa_string ISA字符串
 * @param output_buffer 输出缓冲区（由调用者分配）
 * @param buffer_size 输出缓冲区大小
 * @return 实际输出的字符数，-1表示错误
 */
int spike_debug_cmd_file_elf_file(const char* elf_file_path,
                         const char* debug_cmds_path,
                         const char* isa_string,
                         char* output_buffer,
                         size_t buffer_size);

/**
 * 获取最后一次错误的描述
 * @return 错误描述字符串
 */
const char* spike_get_last_error(void);

#ifdef __cplusplus
}
#endif

#endif // SPIKE_WRAPPER_H
