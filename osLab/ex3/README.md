# Linux File System Implementation - OS Lab Exercise 3

## Project Overview
This lab focuses on understanding and implementing core components of the ext2 file system through two main parts:
1. Analysis of existing ext2 file systems
2. Kernel-level implementation of a simplified ext2-lite file system

## Part 1: ext2 File System Analysis
### Objectives
- Explore ext2 disk images using both high-level tools and low-level hex analysis
- Understand fundamental file system structures:
  - Superblocks
  - Block groups
  - Inodes and directory entries
  - Allocation bitmaps

### Methodology
1. **Tools Approach**:
   - Mount disk images
   - Use standard Linux utilities (`ls`, `stat`, `debugfs`)
   - Analyze file system metadata

2. **Hex Analysis**:
   - Direct byte-level examination of disk structures
   - Manual structure parsing
   - Corruption detection/repair

## Part 2: ext2-lite Kernel Implementation
### Key Components to Implement
1. **File System Registration** (`super.c`):
   - Module initialization/cleanup
   - Superblock operations

2. **Directory Operations** (`dir.c`):
   - Directory entry lookup
   - Folio management

3. **Inode Handling** (`inode.c`):
   - Disk inode location
   - VFS inode creation
   - Operation method assignment

4. **Block Allocation** (`balloc.c`):
   - Contiguous block allocation
   - Bitmap manipulation
   - Group descriptor interaction

### Development Environment
- **Kernel Version**: Target version specified in lab materials
- **Virtual Machine**: QEMU/KVM with provided disk images
- **Tools**: Standard kernel development tools

## Testing Requirements
- Verify module loading/unloading
- Test mounting/unmounting
- Validate basic file operations:
  - File creation/deletion
  - Directory operations
  - Data read/write

## Deliverables
- Complete kernel module implementation
- Analysis report covering:
  - Part 1 investigation results
  - Part 2 implementation approach
  - Testing methodology and results

## Resources
- Kernel documentation on ext2
- File system development guides
- Provided reference materials

## References
- Linux kernel source code [ext2](https://elixir.bootlin.com/linux/v6.11/source/fs/ext2)