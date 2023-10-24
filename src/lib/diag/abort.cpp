/*
 * Copyright (c) Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "abort.hpp"
#include "nx/kernel/svc.h"
#include "pe/Util/Log.h"

#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstring>

namespace exl::diag {

void NOINLINE AbortImpl(const AbortCtx& ctx)
{
    uintptr_t to = 0x6969696969696969;
    *reinterpret_cast<u64*>(to) = ctx.m_Result;
}

#define ABORT_WITH_VALUE(v)                                \
    {                                                      \
        exl::diag::AbortCtx ctx { .m_Result = (Result)v }; \
        AbortImpl(ctx);                                    \
    }

/* TODO: better assert/abort support. */
void NOINLINE AssertionFailureImpl(const char* file, int line, const char* func, const char* expr, u64 value, const char* format, ...)
{
    pe::err("AssertionFailure: %s:%d %s %s %zu", file, line, func, expr, value);
    ABORT_WITH_VALUE(value)
}
void NOINLINE AssertionFailureImpl(const char* file, int line, const char* func, const char* expr, u64 value)
{
    pe::err("AssertionFailure: %s:%d %s %s %zu", file, line, func, expr, value);
    ABORT_WITH_VALUE(value)
}
void NOINLINE AbortImpl(const char* file, int line, const char* func, const char* expr, u64 value, const char* format, ...)
{
    pe::err("Abort: %s:%d %s %s %zu", file, line, func, expr, value);
    ABORT_WITH_VALUE(value)
}
void NOINLINE AbortImpl(const char* file, int line, const char* func, const char* expr, u64 value)
{
    pe::err("Abort: %s:%d %s %s %zu", file, line, func, expr, value);
    ABORT_WITH_VALUE(value)
}

};

/* C shim for libnx */
extern "C" NORETURN void exl_abort(Result r)
    ABORT_WITH_VALUE(r)