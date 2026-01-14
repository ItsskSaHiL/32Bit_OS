# Complete Mental Model — 32Bit_OS

Purpose: Minimal 32-bit hobby OS (bootloader + kernel + drivers + FAT filesystem).

Runtime flow (high level):
- Bootloader -> Kernel entry -> Memory (paging + heap) -> IDT/interrupts -> Device I/O -> Filesystem -> userland/tools

Project tree (ASCII, logical):

Root
├─ build.sh
├─ Makefile
├─ README.md
├─ LICENSE
├─ bin/                     # final images / bootable outputs
├─ build/                   # per-subsystem build artifacts
│  ├─ disk/
│  ├─ fs/
│  ├─ idt/
│  ├─ io/
│  ├─ memory/
│  │  ├─ heap/
│  │  └─ paging/
│  └─ string/
├─ src/                     # primary source tree
│  ├─ config.h
│  ├─ kernel.asm            # low-level kernel entry / startup glue
│  ├─ kernel.c              # kernel init & runtime logic
│  ├─ kernel.h
│  ├─ linker.ld             # memory layout for linking the kernel image
│  ├─ status.h
│  ├─ boot/
│  │  └─ boot.asm           # bootloader (loads kernel)
│  ├─ disk/
│  │  ├─ disk.c             # block device abstraction
│  │  ├─ disk.h
│  │  ├─ streamer.c         # streaming read/write helpers
│  │  └─ streamer.h
│  ├─ fs/
│  │  ├─ fat.c              # FAT parsing + helpers
│  │  ├─ fat.h
│  │  ├─ file.c             # file-level API (open/read/seek)
│  │  ├─ file.h
│  │  ├─ pparser.c          # path parser
│  │  └─ pparser.h
│  ├─ idt/
│  │  ├─ idt.asm            # interrupt stubs
│  │  ├─ idt.c              # IDT setup and handlers
│  │  └─ idt.h
│  ├─ io/
│  │  ├─ io.asm             # low-level port I/O
│  │  └─ io.h
│  ├─ memory/
│  │  ├─ memory.c           # memory manager front-end
│  │  ├─ memory.h
│  │  ├─ heap/
│  │  │  ├─ heap.c
│  │  │  ├─ heap.h
│  │  │  ├─ kheap.c         # kernel heap allocator
│  │  │  └─ kheap.h
│  │  └─ paging/
│  │     ├─ paging.asm
│  │     ├─ paging.c
│  │     └─ paging.h
│  └─ string/
│     ├─ string.c
│     └─ string.h
├─ hello.txt                # example/test content

Responsibilities (concise):
- `boot.asm`: transition CPU state, load the kernel image at addresses in `linker.ld`.
- `kernel.asm` / `kernel.c`: initialize subsystems (memory, IDT, drivers), provide runtime services.
- Memory subsystem: set up paging, provide `kheap` for allocations used across drivers and FS.
- IDT: register handlers early so interrupts/exceptions are managed safely.
- IO/disk: low-level port I/O and block access; FS depends on this.
- FS (FAT): parse on-disk structures, provide file operations used by kernel and tools.
- Build system: `Makefile` / `build.sh` compiles assembly/C, links with `linker.ld`, emits artifacts in `build/` and `bin/`.

Key notes for contributors:
- The assembly modules provide privileged operations and small stubs called by C modules.
- Order of initialization matters: paging & heap before subsystems that allocate memory; IDT early for safe hardware handling.
- `linker.ld` dictates where symbols and the kernel image must be loaded by the bootloader — mismatches break boot.
- To run quickly, test images in an emulator (QEMU/Bochs) rather than hardware.

Next steps (optional):
- Add sequence diagrams (boot → kernel → fs) or a component dependency graph.
- Add a short README section describing how to run the image in QEMU.

----
Generated: 2026-01-14
