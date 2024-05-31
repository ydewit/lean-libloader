#include <lean/lean.h>

#ifdef LEAN_WINDOWS
#include <windows.h>
#else
#include <dlfcn.h>
#endif

void load_dynlib(std::string path) {
#ifdef LEAN_WINDOWS
    HMODULE h = LoadLibrary(path.c_str());
    if (!h) {
        throw exception(sstream() << "error loading library " << path << ": " << GetLastError());
    }
#else
    void *handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (!handle) {
        throw exception(sstream() << "error loading library, " << dlerror());
    }
#endif
    // NOTE: we never unload libraries
}

/* loadDynlib : System.FilePath -> IO Unit */
extern "C" LEAN_EXPORT obj_res lean_load_dynlib(b_obj_arg path, obj_arg) {
    try {
        load_dynlib(string_cstr(path));
        return io_result_mk_ok(box(0));
    } catch (exception & ex) {
        return io_result_mk_error(ex.what());
    }
}
