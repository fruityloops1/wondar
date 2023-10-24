#pragma once

#include <cstdio>
#include <sead/container/seadPtrArray.h>
#include <sead/heap/seadHeap.h>

namespace pe {

enum class LogType : u8 {
    Log,
    Warning,
    Error
};

void initializeLog(sead::Heap* parent);
sead::PtrArray<LogType>& getLogLines();
char* addLog(LogType type, size_t len);
bool& shouldLogWindowScrollDown();

#define PE_UTIL_LOG_TEMPLATE(NAME, TYPE)                  \
    template <typename... Args>                           \
    void NAME(const char* fmt, Args... args)              \
    {                                                     \
        size_t size = snprintf(nullptr, 0, fmt, args...); \
        char* msg = addLog(LogType::TYPE, size);          \
        snprintf(msg, size + 1, fmt, args...);            \
        shouldLogWindowScrollDown() = true;               \
    }

void log(const char* fmt, ...);
void warn(const char* fmt, ...);
void err(const char* fmt, ...);

#undef PE_UTIL_LOG_TEMPLATE

} // namespace pe
