#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#else
#include <sys/mman.h>
#include <dirent.h>
#include <fcntl.h>
#endif

#define CHUNK_SIZE 4096
#define PATTERN_MAX 64

typedef struct {
    uintptr_t address;
    unsigned char data[256];
    size_t size;
} MemChunk;

void stealth_sleep(int ms) {
    #ifdef _WIN32
    Sleep(ms);
    #else
    usleep(ms * 1000);
    #endif
}

void* get_base_address(const char* proc_name) {
    #ifdef _WIN32
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 entry = { sizeof(PROCESSENTRY32) };
    if (Process32First(snap, &entry)) {
        do {
            if (strcmp(entry.szExeFile, proc_name) == 0) {
                CloseHandle(snap);
                return (void*)entry.th32ProcessID;
            }
        } while (Process32Next(snap, &entry));
    }
    CloseHandle(snap);
    return NULL;
    #else
    // For Linux/Mac: returns PID
    DIR* dir = opendir("/proc");
    struct dirent* ent;
    while ((ent = readdir(dir)) != NULL) {
        int pid = atoi(ent->d_name);
        if (pid > 0) {
            char path[256];
            snprintf(path, sizeof(path), "/proc/%d/comm", pid);
            FILE* f = fopen(path, "r");
            if (f) {
                char name[64];
                if (fgets(name, sizeof(name), f) && strstr(name, proc_name)) {
                    fclose(f);
                    closedir(dir);
                    return (void*)(uintptr_t)pid;
                }
                fclose(f);
            }
        }
    }
    closedir(dir);
    return NULL;
    #endif
}

int scan_memory(void* handle, const unsigned char* pattern, size_t pat_len, MemChunk* result) {
    #ifdef _WIN32
    HANDLE hProc = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, (DWORD)(uintptr_t)handle);
    if (!hProc) return -1;
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    uintptr_t start = (uintptr_t)sysInfo.lpMinimumApplicationAddress;
    uintptr_t end = (uintptr_t)sysInfo.lpMaximumApplicationAddress;
    unsigned char buffer[CHUNK_SIZE];
    for (uintptr_t addr = start; addr < end; addr += CHUNK_SIZE) {
        SIZE_T read;
        if (ReadProcessMemory(hProc, (LPCVOID)addr, buffer, CHUNK_SIZE, &read)) {
            for (size_t i = 0; i <= read - pat_len; i++) {
                if (memcmp(buffer + i, pattern, pat_len) == 0) {
                    result->address = addr + i;
                    memcpy(result->data, buffer + i, read - i);
                    result->size = read - i;
                    CloseHandle(hProc);
                    return 1;
                }
            }
        }
        stealth_sleep(1);
    }
    CloseHandle(hProc);
    #else
    // Linux memory scanning via /proc/pid/mem
    char mem_path[256];
    snprintf(mem_path, sizeof(mem_path), "/proc/%d/mem", (int)(uintptr_t)handle);
    int fd = open(mem_path, O_RDONLY);
    if (fd < 0) return -1;
    unsigned char buffer[CHUNK_SIZE];
    off_t offset = 0;
    while (1) {
        ssize_t bytes = pread(fd, buffer, CHUNK_SIZE, offset);
        if (bytes <= 0) break;
        for (size_t i = 0; i <= (size_t)bytes - pat_len; i++) {
            if (memcmp(buffer + i, pattern, pat_len) == 0) {
                result->address = (uintptr_t)offset + i;
                memcpy(result->data, buffer + i, bytes - i);
                result->size = bytes - i;
                close(fd);
                return 1;
            }
        }
        offset += CHUNK_SIZE;
        stealth_sleep(1);
    }
    close(fd);
    #endif
    return 0;
}

int main() {
    printf("[SHADOW] Initiated silent memory scanner.\n");
    const char* target = "notepad.exe"; // <- Change this to your target
    unsigned char pattern[] = { 0x48, 0x8B, 0x05, 0x00, 0x00, 0x00, 0x00 }; // Example: x86 mov opcode
    size_t pat_len = sizeof(pattern);

    void* handle = get_base_address(target);
    if (!handle) {
        fprintf(stderr, "[!] Process not found.\n");
        return 1;
    }

    MemChunk found = {0};
    int result = scan_memory(handle, pattern, pat_len, &found);
    if (result == 1) {
        printf("[+] Pattern found at: 0x%lx\n", found.address);
        printf("[+] Data preview: ");
        for (size_t i = 0; i < (found.size > 16 ? 16 : found.size); i++) {
            printf("%02X ", found.data[i]);
        }
        printf("\n");
    } else if (result == -1) {
        fprintf(stderr, "[!] Access denied. Try running as root/admin.\n");
    } else {
        printf("[-] Pattern not found in target memory.\n");
    }

    return 0;
}
