T_D: BEGIN
SECTION TEXT
	PUBLIC T_D
	PUBLIC VEC1A
	PUBLIC VEC2A
	VEC1W: EXTERN
	VEC1H: EXTERN
	VEC2W: EXTERN
	VEC2H: EXTERN
	FIM: EXTERN
	LOAD VEC1W
	MULT VEC1H
	STORE VEC1A
	LOAD VEC2W
	MULT VEC2H
	STORE VEC2A
	JMP FIM
SECTION DATA
	VEC1A: SPACE
	VEC2A: SPACE
END