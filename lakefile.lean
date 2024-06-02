import Lake
open Lake DSL

package «libloader» where
  -- add package configuration options here

module_facet ffi.o mod : FilePath := do
  let ffiCFile := mod.leanFile.withExtension "ffi.c"
  if ← ffiCFile.pathExists then
    let ffiOFile := mod.leanLibPath "ffi.o"
    -- use leanc to build, but we cant find libSystem includes
    let extra := #[
      "-I/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include"
    ]
    buildLeanO ffiOFile (pure ffiCFile) mod.weakLeancArgs (mod.leancArgs ++ extra) -- build shim.o files
    -- -- use clang to build, but we cant find lean/lean.h
    -- buildO ffiOFile (pure ffiCFile) mod.weakLeancArgs (mod.leancArgs ++ #["-v"]) -- build shim.o files
  else
    logError s!"{ffiCFile} file for module '{mod.name}' not found"
    none

lean_lib «Libloader» where
  moreLinkArgs := #[
    "-L/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib",
    "-ldl"]
  nativeFacets := fun _ => #[
    Module.oFacet,          -- builds module .o files
    `ffi.o                  -- builds module .ffi.o files
  ]
  -- defaultFacets := #[
  --   LeanLib.sharedFacet, -- builds .lake/build/lib/libMyLibrary.dylib
  --   LeanLib.staticFacet  -- builds .lake/build/lib/libMyLibrary.a
  -- ]

@[default_target]
lean_exe «libloader» where
  root := `Main
