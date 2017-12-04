; Considere um retangulo como abaixo
;   (x0,y0).____.
;          |    |
;          |____|
;               (x1,y1)
; Esse programa obtem as coordenadas x0, y0, x1, y1, nesta ordem, e calcula sua largura e altura
T_C: BEGIN
SECTION TEXT
	PUBLIC T_C
	PUBLIC VEC2W
	PUBLIC VEC2H
	T_D: EXTERN
	INPUT VEC2
	INPUT VEC2 + 1
	INPUT VEC2 + 2
	INPUT VEC2 + 3
	LOAD VEC2 + 2
	SUB VEC2
	STORE VEC2W
	LOAD VEC2 + 3
	SUB VEC2 + 1
	STORE VEC2H
	JMP T_D ; Calcula a area dos retangulos
SECTION DATA
	VEC2: SPACE 4
	VEC2W: SPACE
	VEC2H: SPACE
END