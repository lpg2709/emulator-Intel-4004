# Intel 4004 emulator
```text
                            I/O   I/O   I/O   I/O   Vdd    CM   TEST  RESET
                             --    --    --    --    --    --    --    --
                            |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
                         ---    --    --    --    --    --    --    --    ---
                        |    16    15    14    13    12    11    10     9    |
                        |                                                    |
                        |           ##      ###         ###           ##     |
                        |          ###    ##   ##     ##   ##        ###     |
                         --       # ##   ##     ##   ##     ##      # ##     |
                           |     #  ##   ##     ##   ##     ##     #  ##     |
                           |    #   ##   ##     ##   ##     ##    #   ##     |
                         --    #######   ##     ##   ##     ##   #######     |
                        |           ##    ##   ##     ##   ##         ##     |
                        |           ##      ###         ###           ##     |
                        |                                                    |
                        |     1     2     3     4     5     6     7     8    |
                         ---    --    --    --    --    --    --    --    ---
                            |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
                             --    --    --    --    --    --    --    --
                             D0    D1    D2    D3    Vss   Ph1   Ph2  SYNC
```

> Development in progress. (Check dev branch)

Simple emulator of the Intel 4004 chip.

The Intel 4004 was the first computer that I made an emulator, unfortunately I
lost the code. Since then I have been studying and assembling small emulators
and studying the architecture of computers and embedded devices. After a long
time I am going back to the emulators, and the first one that I will focus on
development is the Intel 4004, because it is a simple device with a few
instructions (46 instructions).


This program has an emulator, assembly and disassembler integrated for the
Intel 4004. Its objective is to be faithful, as far as possible, to the original
hardware, following the specifications of the documents of its time.

## Intel 4004

Intel 4004 is a 4-bit CPU released by Intel in 1971, which was the first
commercial programmable microprocessor.

### References

If you don't know anything about intel 4004, your
[wikipedia](https://en.wikipedia.org/wiki/Intel_4004) page is a good place to
start.

Brief history of the Intel 4004. [Intel's First Microprocessor](https://www.intel.com/content/www/us/en/history/museum-story-of-intel-4004.html)

Recommended online emulator: [Emulator - Assembler - Disassembler](http://e4004.szyc.org/)

> This emulator does not strictly follow the specifications of the original
4004, having some facilities and conveniences not present in the documentation.

[4004 Datasheet](./doc/4004\ Datasheet.pdf)*

[MCS-4 Assembly Language Programming Manual - December 1973](./doc/MCS-4\ Assembly\ Language\ Programming\ Manual.pdf)*

## Usage

After download the executable for your system, open a terminal (or promt) and
execute it. You have some modes:

You can use `--help` to see the options in more detail.

### Emulator -e (Default option)

Execute a 4004 program instruction by instruction, view the registers and memory
on a CLI ASCII view.

### Assembler -a

Compile a program to Intel 4004 bytes.

### Disassembler -d

Decompile a binary file to a text file with the assembley instruction.

## Compile

You have to options to compile.

## 1. Using CMake

Inside the root dir of the project.

```sh
mkdir build && cd build
cmake ..
make
```

That create the executable ```4004_emulator```.

## 2. Using Zig

Inside the root dir of the project to compile for you machine.

```sh
zig build
```

> Need to be Zig 0.13.0

The zig build system and C compiler is used to gennerate the release
executables.


## License

[MIT](./LICENSE)
