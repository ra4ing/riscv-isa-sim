#include "spike_wrapper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdexcept>
#include <sstream>
#include <memory>
#include <fstream>

// 全局错误信息存储
static std::string g_last_error;

// 从文件路径运行spike调试 - 使用--debug-cmd-from-string避免创建调试命令临时文件
static std::string run_spike_debug_cmd_str_elf_file(const std::string& elf_file_path,
                                           const std::string& debug_cmds_string,
                                           const std::string& isa_string) {
    try {
        // 构建spike命令 - 使用--debug-cmd-from-string选项直接传递调试命令
        char spike_cmd[4096];
        snprintf(spike_cmd, sizeof(spike_cmd),
                "spike -d --isa=%s --debug-cmd-from-string='%s' %s 2>&1",
                isa_string.c_str(), debug_cmds_string.c_str(), elf_file_path.c_str());

        // 执行spike命令并捕获输出
        FILE* pipe = popen(spike_cmd, "r");
        if (!pipe) {
            throw std::runtime_error("Failed to execute spike command");
        }

        // 读取输出
        std::string result;
        char buffer[4096];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }

        pclose(pipe);
        return result;

    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Spike execution failed: ") + e.what());
    }
}

static std::string run_spike_debug_cmd_file_elf_file(const std::string& elf_file_path,
                                           const std::string& debug_cmds_path,
                                           const std::string& isa_string) {
    try {
        // 构建spike命令 - 使用--debug-cmd选项直接传递调试命令
        char spike_cmd[4096];
        snprintf(spike_cmd, sizeof(spike_cmd),
                "spike -d --isa=%s --debug-cmd='%s' %s 2>&1",
                isa_string.c_str(), debug_cmds_path.c_str(), elf_file_path.c_str());

        // 执行spike命令并捕获输出
        FILE* pipe = popen(spike_cmd, "r");
        if (!pipe) {
            throw std::runtime_error("Failed to execute spike command");
        }

        // 读取输出
        std::string result;
        char buffer[4096];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }

        pclose(pipe);
        return result;

    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Spike execution failed: ") + e.what());
    }
}



// C接口实现
extern "C" {

int spike_debug_cmd_str_elf_file(const char* elf_file_path,
                         const char* debug_cmds_string,
                         const char* isa_string,
                         char* output_buffer,
                         size_t buffer_size) {
    try {
        if (!elf_file_path || !debug_cmds_string || !isa_string || !output_buffer) {
            g_last_error = "Invalid parameters";
            return -1;
        }

        std::string result = run_spike_debug_cmd_str_elf_file(
            std::string(elf_file_path),
            std::string(debug_cmds_string),
            std::string(isa_string)
        );

        size_t copy_size = std::min(result.length(), buffer_size - 1);
        std::memcpy(output_buffer, result.c_str(), copy_size);
        output_buffer[copy_size] = '\0';

        return static_cast<int>(copy_size);

    } catch (const std::exception& e) {
        g_last_error = e.what();
        return -1;
    }
}

int spike_debug_cmd_file_elf_file(const char* elf_file_path,
                         const char* debug_cmds_path,
                         const char* isa_string,
                         char* output_buffer,
                         size_t buffer_size) {
    try {
        if (!elf_file_path || !debug_cmds_path || !isa_string || !output_buffer) {
            g_last_error = "Invalid parameters";
            return -1;
        }

        std::string result = run_spike_debug_cmd_file_elf_file(
            std::string(elf_file_path),
            std::string(debug_cmds_path),
            std::string(isa_string)
        );

        size_t copy_size = std::min(result.length(), buffer_size - 1);
        std::memcpy(output_buffer, result.c_str(), copy_size);
        output_buffer[copy_size] = '\0';

        return static_cast<int>(copy_size);

    } catch (const std::exception& e) {
        g_last_error = e.what();
        return -1;
    }
}



const char* spike_get_last_error(void) {
    return g_last_error.c_str();
}

}
