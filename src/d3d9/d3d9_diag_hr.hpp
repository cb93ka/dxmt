#pragma once
// MADEIRA diag: DXMT_D9_DIAG_HR=1 logs every failing HRESULT the D3D9
// frontend hands back to the application (first 300, then one summary line
// per 4096 failures), to catch an engine falling back after a refused call.
#include "log/log.hpp"
#include "util_env.hpp"
#include "util_string.hpp"
#include <atomic>
#include <cstdio>

static inline void d3d9_diag_hr(const char *what, long hr) {
  static const bool enabled = dxmt::env::getEnvVar("DXMT_D9_DIAG_HR") == "1";
  if (!enabled)
    return;
  static std::atomic<uint32_t> count{0};
  uint32_t n = count.fetch_add(1) + 1;
  if (n <= 300 || (n & 0xFFF) == 0) {
    char code[16];
    std::snprintf(code, sizeof(code), "0x%08lx", (unsigned long)hr);
    dxmt::Logger::warn(dxmt::str::format("[diag-hr] #", n, " ", what, " -> ", code));
  }
}
