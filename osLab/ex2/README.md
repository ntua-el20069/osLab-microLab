# Linux Sensor Device Driver - OS Lab Exercise 2

## Project Overview
Implementation of a Linux character device driver that exposes sensor measurements through dedicated device files. The driver interfaces with a wireless sensor network protocol stack to provide structured access to sensor data.

## System Architecture
- **Data Acquisition Layer**: Receives raw sensor measurements
- **Protocol Layer**: Processes and categorizes sensor data
- **Driver Layer**: Presents data through character devices
  - `/dev/lunix0-batt` (Voltage)
  - `/dev/lunix0-temp` (Temperature)
  - `/dev/lunix0-light` (Light)

## Key Features
- Implements standard file operations (`open`, `read`, `release`)
- Supports multiple concurrent readers
- Implements blocking I/O with wait queues
- Optional memory-mapped access (`mmap`)

## Development Requirements
1. **Basic Driver**:
   - Create character devices
   - Implement file operations skeleton
   - Return placeholder values

2. **Full Implementation**:
   - Integrate with sensor data buffers
   - Implement blocking/non-blocking I/O
   - Add proper synchronization

## Technical Specifications
- **Kernel Version**: 6.11
- **Development Environment**: QEMU/KVM VM
- **Synchronization**: Uses kernel mutexes/spinlocks
- **Memory Management**: Kernel-space buffers

## Testing Methodology
User-space test program should verify:
- Correct device file operations
- Data consistency
- Concurrent access patterns
- Blocking behavior

## Constraints
- Must maintain existing protocol interface
- No modification to upper layers
- Kernel-safe implementation required

## References
- Some files are not included in the repository inorder to preserve the rights of the NTUA operating systems lab authors.