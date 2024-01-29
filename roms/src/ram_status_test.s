; From: Leonardo Padilha Guarezi
; Test the write values into status character of DATA RAM

init,
	FIM R0R1, 0    ; initialize R0=R1=0
	FIM R2R3, 0    ; initialize R2=R3=0
	LDM 12         ; load 12 to accumulator
	XCH R2         ; initialize R2=12
loop,
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
	ISZ R2, loop  ; loop 4 times
