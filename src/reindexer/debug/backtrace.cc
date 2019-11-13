#include "backtrace.h"
#ifndef WIN32
#include <signal.h>
#include <sstream>
#include "estl/span.h"
#include "resolver.h"
// There are 3 backtrace methods are available:
// 1. stangalone libunwind ( https://github.com/libunwind/libunwind )
// 2. libgcc's/llvm built in unwind
// 3. GNU's execinfo backtrace() call

#if REINDEX_WITH_LIBUNWIND
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#endif

#if REINDEX_WITH_UNWIND
#include <unwind.h>
#endif

#if REINDEX_WITH_EXECINFO
#include <execinfo.h>
#endif

namespace reindexer {

}  // namespace reindexer

#else
namespace reindexer {
namespace debug {
void backtrace_init() {}
void backtrace_set_writer(std::function<void(string_view out)>) {}
int backtrace_internal(void **, size_t, void *, string_view &) { return 0; }

}  // namespace debug
}  // namespace reindexer

#endif
