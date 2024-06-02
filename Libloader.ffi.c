// #include <stdio.h>
// #include <stdlib.h>
#ifdef LEAN_WINDOWS
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <lean/lean.h>

int load_dynlib(const char *path) {
#ifdef LEAN_WINDOWS
    HMODULE h = LoadLibrary(path);
    if (!h) {
        // fprintf(stderr, "Error loading library %s: %lu\n", path, GetLastError());
        return 1;
    }
#else
    void *handle = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
    if (!handle) {
        // fprintf(stderr, "Error loading library %s: %s\n", path, dlerror());
        return 1;
    }
#endif
    // NOTE: we never unload libraries
    return 0;
}

/* loadDynlib : System.FilePath -> IO Unit */
LEAN_EXPORT lean_obj_res lean_load_dynlib(b_lean_obj_arg path, lean_obj_arg _) {
    if ( load_dynlib(lean_string_cstr(path)) == 0 ){
        return lean_io_result_mk_ok(lean_box(0));
    } else {
        return lean_io_result_mk_error(lean_mk_string(dlerror()));
    }
}
