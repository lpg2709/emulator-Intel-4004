/ ram_test.asm
/ test the Intel 4002
init,
	FIM 0P, 0    / initialize R0=R1=0
	FIM 1P, 0    / initialize R2=R3=0
	LDM 12       / load 12 to accumulator
	XCH R2       / initialize R2=12
lp1,
	SRC 0P       / select register & address
	WRM          / write accumulator to memory
	IAC          / increment accumulator
	ISZ R1, lp1  / loop 16 times
	INC R0       / increment R0
	ISZ R2, lp1  / loop 4 times
	FIM 0P, 0    / initialize R0=R1=0
	FIM 1P, 0    / initialize R2=R3=0
	LDM 12       / load 12 to accumulator
	XCH R2       / initialize R2=12
lp2,
	SRC 0P       / select register & address
	WR0          / write status character 0
	IAC          / increment accumulator
	WR1          / write status character 1
	IAC          / increment accumulator
	WR2          / write status character 2
	IAC          / increment accumulator
	WR3          / write status character 3
	IAC          / increment accumulator
	INC R0       / increment R0
	ISZ R2, lp2  / loop 4 times
DONE,
	JUN DONE     / endless loop as end of program
