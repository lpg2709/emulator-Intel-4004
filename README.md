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

> Development in progress

Simple emulator of the Intel 4004 chip.

The Intel 4004 was the first computer that I made an emulator, unfortunately I
lost the code. Since then I have been studying and assembling small emulators
and studying the architecture of computers and embedded devices. After a long
time I am going back to the emulators, and the first one that I will focus on
development is the Intel 4004, because it is a simple device with a few
instructions (46 instructions).

## Intel 4004

Intel 4004 is a 4-bit CPU released by Intel in 1971, which was the first
commercial programmable microprocessor.

## Compile

Inside the root dir of the project.

```sh
mkdir build && cd build
cmake ..
make
```

That create the executable ```4004_emulator```.

## References

If you don't know anything about intel 4004, your [wikipedia](https://en.wikipedia.org/wiki/Intel_4004) page is a good place to start.

Brief history of the Intel 4004. [Intel's First Microprocessor](https://www.intel.com/content/www/us/en/history/museum-story-of-intel-4004.html)

Recommended online emulator: [Emulator - Assembler - Disassembler](http://e4004.szyc.org/)

[4004 Datasheet](./doc/4004\ Datasheet.pdf)*

[MCS-4 Assembly Language Programming Manual - December 1973](./doc/MCS-4\ Assembly\ Language\ Programming\ Manual.pdf)*


## License

[MIT](./LICENSE)
