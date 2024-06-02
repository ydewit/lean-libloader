import «Libloader»

def main : IO Unit := do
  let a ← loadLibrary "libhello.so"
  IO.println s!"Hello!"
