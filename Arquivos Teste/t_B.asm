; Considere um retangulo como abaixo
;   (x0,y0).____.
;          |    |
;          |____|
;               (x1,y1)
; Esse programa obtem as coordenadas x0, y0, x1, y1, nesta ordem, e calcula sua largura e altura
T_B: BEGIN
SECTION TEXT
	PUBLIC T_B
	PUBLIC VEC1W
	PUBLIC VEC1H
	T_C: EXTERN
	INPUT VEC1
	INPUT VEC1 + 1
	INPUT VEC1 + 2
	INPUT VEC1 + 3
	LOAD VEC1 + 2
	SUB VEC1
	STORE VEC1W
	LOAD VEC1 + 3
	SUB VEC1 + 1
	STORE VEC1H
	JMP T_C ; Obtem o segundo retangulo e calcula suas areas
SECTION DATA
	VEC1: SPACE 4
	VEC1W: SPACE
	VEC1H: SPACE
END