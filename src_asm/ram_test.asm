; ram_test.asm
; test the Intel 4002
init
	FIM R0R1, 0    ; initialize R0=R1=0
	FIM R2R3, 0    ; initialize R2=R3=0
	LDM 12         ; load 12 to accumulator
	XCH R2         ; initialize R2=12
loop1
	SRC R0R1       ; select register & address
	WRM            ; write accumulator to memory
	IAC            ; increment accumulator
	ISZ R1, loop1  ; loop 16 times
	INC R0         ; increment R0
	ISZ R2, loop1  ; loop 4 times

	FIM R0R1, 0    ; initialize R0=R1=0
	FIM R2R3, 0    ; initialize R2=R3=0
	LDM 12         ; load 12 to accumulator
	XCH R2         ; initialize R2=12
loop2
	SRC R0R1       ; select register & address
	WR0            ; write status character 0
	IAC            ; increment accumulator
	WR1            ; write status character 1
	IAC            ; increment accumulator
	WR2            ; write status character 2
	IAC            ; increment accumulator
	WR3            ; write status character 3
	IAC            ; increment accumulator
	INC R0         ; increment R0
	ISZ R2, loop2  ; loop 4 times

DONE
	JUN DONE       ; endless loop as end of program
