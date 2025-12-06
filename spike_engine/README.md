# Spike Engine

Lightweight RISC-V execution engine with checkpoint/restore for fuzzing.

## Features

- Mixed-length instructions (16-bit compressed + 32-bit standard)
- Lightweight checkpointing (registers + index only)
- Dynamic address allocation
- Python bindings

## Usage

```python
import spike_engine

engine = spike_engine.SpikeEngine("template.elf", "rv64gc", 1000)
engine.initialize()

# Detect instruction size
size = spike_engine.SpikeEngine.get_instruction_size(0x003100b3)  # 4 bytes
size = spike_engine.SpikeEngine.get_instruction_size(0x00008522)  # 2 bytes

# Execute with checkpoint
engine.set_checkpoint()
xor_val = engine.execute_instruction(machine_code, source_regs, immediate)
if collision:
    engine.restore_checkpoint()
```

## Checkpoint Design

Optimized for single-checkpoint rollback pattern:

```cpp
struct Checkpoint {
    vector<uint64_t> xpr, fpr;  // Registers
    uint64_t pc;
    size_t instr_index;          // Index only, no instruction backup!
    uint64_t next_instruction_addr;
};
```

Restore uses `executed_instructions_.resize(index)` instead of memory writes.

**Result**: ~50% less memory, ~100x faster restore.

## Build

```bash
make clean && make
```
