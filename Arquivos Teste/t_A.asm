; Esse codigo compara a area de dois retangulos e
; retorna se o segundo eh maior, igual ou menor que o primeiro
T_A: BEGIN
SECTION TEXT
	PUBLIC FIM
	T_B: EXTERN
	VEC1A: EXTERN
	VEC2A: EXTERN
	JMP T_B ; Obtem os retangulos e calcula suas areas
	FIM: LOAD VEC2A
	SUB VEC1A
	JMPP MAIOR
	JMPZ IGUAL
	LOAD NEGUM
	STORE R
	JMP OUT
	IGUAL: LOAD ZERO
	STORE R
	JMP OUT
	MAIOR: LOAD UM
	STORE R
	OUT: OUTPUT R
	STOP
SECTION DATA
	R: SPACE
	NEGUM: CONST -1
	ZERO: CONST 0
	UM: CONST 1
END