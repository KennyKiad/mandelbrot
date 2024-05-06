.code

Asm_Draw proc
; extern "C" void Asm_Draw(char *video_buff, SSize size)
; RCX - video_buff
; RDX - size

	mov rdi,rcx
	mov eax,0ffffffffh

	mov rcx,100000

	rep stosd

	ret
Asm_Draw endp

Get_Address proc 
; Получаем адресс буфера экрана по координатам XY
;
; RDX - start_point
; R9 - buf_color
; R11 - video_buff

	push rax
	push rbx
	push r10

	movzx r10,r9w	; r10 = buf_color.Buf_Size.Width

	mov rax,rdx
	shr rax,16		; rax = start_point.Y

	imul rax,r10	
	mov bx,dx
	add rax,rbx		; rax = (buf_color.Buf_Size.Width * start_point.Y) + start_point.X
	shl rax,2

	mov rdi,r11
	add rdi,rax


	pop r10
	pop rbx
	pop rax

	ret
Get_Address endp

Asm_Draw_Line proc
; extern "C" void Asm_Draw_Line(char* video_buff, SPoint start_point, SPoint end_point, SBuf_Color buf_color);
; RCX - video_buff
; RDX - start_point
; R8 - end_point
; R9 - buf_color
	
	push rbx
	push rsi
	push rdi
	push r11
	push r12
	push r13
	push r14
	push r15


	mov r11,rcx

	; Проверяем, что координата по Х начальной точки больше начальной
	cmp r8w,dx
	jbe _ret

	; Проверяем, что координата по Y начальной точки больше начальной
	mov eax,edx
	shr eax,16	; Получаем значение Y начальной точки

	mov ebx,r8d
	shr ebx,16	; Получаем значение Y конечной точки

	cmp bx,ax
	jbe _ret
	;-------------------------------------------------------------------

	call Get_Address	; RDI - Адресс пикселя по координатам start_point


	; Проверяем куда направлена линия: по вертикали или горизонтали
	mov ax,r8w	; end_point.X
	sub ax,dx	; end_point.X - start_point.X
	inc ax
	mov r12w,ax

	mov ebx,r8d
	shr ebx,16	; end_point.Y

	mov ecx,edx
	shr ecx,16	; start_point.Y

	sub bx,cx	;end_point.Y - start_point.Y
	inc bx
	mov r13w,bx

	cmp ax,bx
	jle _drawVertical
_drawHorizon:
	
	xor r14w,r14w

	movzx rcx,r13w	; delta_y

	movzx r15,r9w	; buf_color.Buf_Size.Width
	shl r15,2		; r15 * 4 Значение настоящего адреса

	mov rax,r9
	shr rax,32		; rax = buf_color.color

_drawHorPx:
	stosd

	add r14w,r13w	; Добавили delta_Y
	cmp r14w,r12w	; Сравниваем, что не больше delta_X
	jl _drawHorPx

	sub r14w,r12w

	add rdi,r15

	loop _drawHorPx	; Переходит на метку столько раз, сколько в rcx(delta_y)

	jmp _ret

_drawVertical:

	xor r14w,r14w

	movzx rcx,r12w	; delta_x

	movzx r15,r9w	; buf_color.Buf_Size.Width
	dec r15
	shl r15,2		; r15 * 4 Значение настоящего адреса

	mov rax,r9
	shr rax,32		; rax = buf_color.color

_drawVerPx:
	stosd

	add rdi,r15

	add r14w,r12w	; Добавили delta_X
	cmp r14w,r13w	; Сравниваем, что не больше delta_Y
	jl _drawVerPx	

	sub r14w,r13w

	add rdi,4
	loop _drawVerPx	; Переходит на метку столько раз, сколько в rcx(delta_x)


_ret:
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop rdi
	pop rsi
	pop rbx

	ret
Asm_Draw_Line endp

Asm_Draw_Hor_Line proc
; extern "C" void Asm_Draw_Hor_Line(char* video_buff, SPoint start_point, int len, SBuf_Color buf_color);
;
; RCX - video_buff
; RDX - start_point
; R8 - length
; R9 - buf_color
	
	push rbx
	push rsi
	push rdi
	push r11

	mov r11,rcx

	call Get_Address	; RDI - Адресс пикселя по координатам start_point

	mov rax,r9
	shr rax,32		; rax = buf_color.color

	mov rcx,r8		; Rcx - Длина линии

	rep stosd

	pop r11
	pop rdi
	pop rsi
	pop rbx

	ret
Asm_Draw_Hor_Line endp
;-------------------------------------------------------------------
Asm_Set_Pixel proc 
;Asm_Set_Pixel(char* video_buff, SPoint point, SBuf_Color buf_color);
;
;RCX = video_buff
;RDX = point
;R8 = buf_color

	push rdi
	push r9
	push r10
	push r11

	mov r9,r8
	mov r11,rcx

	call Get_Address	; RDI - Адресс пикселя по координатам point

	shr r9,32			; r9 = buf_color.Color

	mov r10d,r9d
	shr r10d,16
	mov al,r9b
	shl eax,16
	mov ax,r9w
	mov al,r10b

	mov dword ptr[rdi],eax

	pop r11
	pop r10
	pop r9
	pop rdi

	ret
Asm_Set_Pixel endp
;-------------------------------------------------------------------
Asm_GetMandelbrotIndex proc
;int Asm_GetMandelbrotIndex(char* video_buff, long double x_0, long double y_0, int colors_cnt);

;RCX = video_buf
;XMM1 = x_0
;XMM2 = y_0
;R9 = colors_cnt
;EAX - return
	
	push r9

	mov rax,4
	cvtsi2sd xmm0,rax	; xmm0 = 4.0 

	mov rcx,r9
;	x_n = 0.0;
;	y_n = 0.0;
	xorpd xmm3,xmm3		; xmm3 = x_n
	xorpd xmm4,xmm4		; xmm4 = y_n


_for:
;	for (i = 0; i < Frame_DC.colors_cnt; i++)
;		{
;			x_n1 = x_n * x_n - y_n * y_n + x_0;
	movapd xmm5,xmm3	; xmm5 = x_n
	movapd xmm6,xmm4	; xmm6 = y_n

	mulsd xmm5,xmm5		; xmm5 = x_n * x_n
	mulsd xmm6,xmm6		; xmm6 = y_n * y_n

	subsd xmm5,xmm6		; xmm5 = x_n * x_n - y_n * y_n

	addsd xmm5,xmm1		; xmm5 = x_n1 = x_n * x_n - y_n * y_n + x_0;

;			y_n1 = 2.0 * x_n * y_n + y_0;
	movapd xmm6,xmm3	; xmm6 = x_n

	mulsd xmm6,xmm4		; xmm6 = x_n * y_n
	addsd xmm6,xmm6		; xmm6 = 2 * x_n * y_n

	addsd xmm6,xmm2		; xmm6 = y_n1 = 2.0 * x_n * y_n + y_0
;
;			distance = x_n1 * x_n1 + y_n1 * y_n1;
	movapd xmm7,xmm5	; xmm7 = x_n1
	movapd xmm8,xmm6	; xmm8 = y_n1

	mulsd xmm7,xmm7		; xmm7 = x_n1 * x_n1
	mulsd xmm8,xmm8		; xmm8 = y_n1 * y_n1

	addsd xmm7,xmm8		; xmm7 = distance


;			if (distance > 4.0)
;				break;
	
	cmpnlesd xmm7,xmm0	; xmm7 > xmm0 
	movmskpd eax,xmm7
	bt eax,0
	jc _ret


;			x_n = x_n1;
;			y_n = y_n1;
	movapd xmm3,xmm5
	movapd xmm4,xmm6

	loop _for

_ret:
	mov rax,r9
	sub rax,rcx		; index

	pop r9

	ret
Asm_GetMandelbrotIndex endp
;-------------------------------------------------------------------

Asm_Set_Mandelbrot_Point proc
;extern "C" int Asm_Set_Mandelbrot_Point(char* video_buff, SPoint_LDouble *xy_0, int *palette_rgb, int colors_cnt);

;RCX = video_buf
;RDX = xy_0
;R8 = palette_rgb
;R9 = colors_cnt
;EAX - return
	
	push rcx
	push rbx
	push r9
	push r10

	mov rax,4
	cvtsi2sd xmm0,rax	; xmm0 = 4.0 

	mov r10,rcx		; R10 - video_buff

	mov rcx,r9		; RCX = colors_cnt

	movupd xmm1,[rdx]		; XMM1 = x_0
	movupd xmm2,[rdx+8]		; XMM2 = y_0

;	x_n = 0.0;
;	y_n = 0.0;
	xorpd xmm3,xmm3		; xmm3 = x_n
	xorpd xmm4,xmm4		; xmm4 = y_n


_for:
;	for (i = 0; i < Frame_DC.colors_cnt; i++)
;		{
;			x_n1 = x_n * x_n - y_n * y_n + x_0;
	movapd xmm5,xmm3	; xmm5 = x_n
	movapd xmm6,xmm4	; xmm6 = y_n

	mulsd xmm5,xmm5		; xmm5 = x_n * x_n
	mulsd xmm6,xmm6		; xmm6 = y_n * y_n

	subsd xmm5,xmm6		; xmm5 = x_n * x_n - y_n * y_n

	addsd xmm5,xmm1		; xmm5 = x_n1 = x_n * x_n - y_n * y_n + x_0;

;			y_n1 = 2.0 * x_n * y_n + y_0;
	movapd xmm6,xmm3	; xmm6 = x_n

	mulsd xmm6,xmm4		; xmm6 = x_n * y_n
	addsd xmm6,xmm6		; xmm6 = 2 * x_n * y_n

	addsd xmm6,xmm2		; xmm6 = y_n1 = 2.0 * x_n * y_n + y_0
;
;			distance = x_n1 * x_n1 + y_n1 * y_n1;
	movapd xmm7,xmm5	; xmm7 = x_n1
	movapd xmm8,xmm6	; xmm8 = y_n1

	mulsd xmm7,xmm7		; xmm7 = x_n1 * x_n1
	mulsd xmm8,xmm8		; xmm8 = y_n1 * y_n1

	addsd xmm7,xmm8		; xmm7 = distance


;			if (distance > 4.0)
;				break;
	
	cmpnlesd xmm7,xmm0	; xmm7 > xmm0 
	movmskpd eax,xmm7
	bt eax,0
	jc _ret


;			x_n = x_n1;
;			y_n = y_n1;
	movapd xmm3,xmm5
	movapd xmm4,xmm6

	loop _for

_ret:
	mov rbx,r9
	sub rbx,rcx		; index

	; Если RCX == 0, то чёрный цвет


	xor eax,eax		; Обнуляем EAX, чтобы сравнить его с ECX, и если EAX == 0, то в EAX будет чёрный цвет
	cmp ecx,eax
	cmovne eax,[r8 + rbx * 4] ; EAX = palette_rgb[color_index]	; перемещаем в EAX значение, если ECX != 0

	mov [r10],eax

	pop r10
	pop r9
	pop rbx
	pop rcx

	ret
Asm_Set_Mandelbrot_Point endp
;-------------------------------------------------------------------

Asm_Set_Mandelbrot_2_Points proc
;extern "C" int Asm_Set_Mandelbrot_2_Points(char* video_buff, SPacked_XY* xy_0, int* palette_rgb, int colors_cnt);

;RCX = video_buf
;RDX = xy_0
;R8 = palette_rgb
;R9 = colors_cnt
;EAX - return
	
	push rdx
	push rcx
	push rbx
	push r9
	push r10
	push r11
	push r12

	mov r11,11b		; R11[1..0] - Маска значений, для которых уже посчитан индекс (0 - да, 1 - нет)

	mov rax,4
	cvtsi2sd xmm0,rax	; xmm0 = 4.0 

	pshufd xmm0,xmm0,01000100b	; xmm0 = {4.0; 4.0}

	mov r10,rcx		; R10 - video_buff

	mov rcx,r9		; RCX = colors_cnt

	movupd xmm2,[rdx]		; XMM2 = y_0
	pshufd xmm2,xmm2,01000100b	; xmm2 = {y_0; y_0}


	movupd xmm1,[rdx+8]		; XMM1 = {x0_1, x0_2}

;	x_n = 0.0;
;	y_n = 0.0;

	xorpd xmm3,xmm3		; xmm3 = {x1_n; x2_n} = {0.0; 0.0}
	xorpd xmm4,xmm4		; xmm4 = {y1_n; y2_n} = {0.0; 0.0}


_for:
;	for (i = 0; i < Frame_DC.colors_cnt; i++)
;		{
;			x_n1 = x_n * x_n - y_n * y_n + x_0;
	movapd xmm5,xmm3	; xmm5 = {x1_n; x2_n}
	movapd xmm6,xmm4	; xmm6 = {y1_n; y2_n}

	mulpd xmm5,xmm5		; xmm5 = {x1_n * x1_n; x2_n * x2_n}
	mulpd xmm6,xmm6		; xmm6 = {y1_n * y1_n; y2_n * y2_n}

	subpd xmm5,xmm6		; xmm5 = {x1_n * x1_n - y1_n * y1_n; x2_n * x2_n - y2_n * y2_n}

	addpd xmm5,xmm1		; xmm5 = {x1_n1 = x1_n * x1_n - y1_n * y1_n + x_0; x_n2 = x2_n1 * x2_n - y2_n * y2_n + x_0};

;			y_n1 = 2.0 * x_n * y_n + y_0;
	movapd xmm6,xmm3	; xmm6 = {x1_n; x2_n}

	mulpd xmm6,xmm4		; xmm6 = {x1_n * y1_n; x2_n * y2_n}
	addpd xmm6,xmm6		; xmm6 = {2 * x1_n * y1_n; 2 * x2_n * y2_n}

	addpd xmm6,xmm2		; xmm6 = {y1_n1 = 2.0 * x1_n * y1_n + y_0; y2_n1 = 2.0 * x2_n * y2_n + y_0}
;
;			distance = x_n1 * x_n1 + y_n1 * y_n1;
	movapd xmm7,xmm5	; xmm7 = {x1_n1; x2_n1}
	movapd xmm8,xmm6	; xmm8 = {y1_n1; y2_n1}

	mulpd xmm7,xmm7		; xmm7 = {x1_n1 * x1_n1; x2_n1 * x2_n1}
	mulpd xmm8,xmm8		; xmm8 = {y1_n1 * y1_n1; y2_n1 * y2_n1}

	addpd xmm7,xmm8		; xmm7 = {distance_1; distance_2}
	

;			x_n = x_n1;
;			y_n = y_n1;
	movapd xmm3,xmm5
	movapd xmm4,xmm6

;			if (distance > 4.0)
;				break;
	
	cmpnlepd xmm7,xmm0	; xmm7 > xmm0 

	movmskpd eax,xmm7
	and rax,r11			; Накладываем маску, обнуляя биты, для которых уже был посчитан индекс

	cmp rax,0
	jne _check_bits

;	bt eax,0
;	jc _ret


	loop _for		; Продолжаем, т.к нет новых индексов

	; Дойдём сюда, когда закончатся все итерации, при этом в RAX - результат последнего сравнения
	; Надо к этому результату применить результат, накопленный в маске

	or rax,r11		; Принудительно отмечаем оставшееся биты как те, для которых надо посчитать индекс

_check_bits:
	xor edx,edx		; Начнём проверять EAX с 0-го бита

_check_one_bit:
	bt eax,edx
	jnc _check_next
	
	mov r12,r9
	sub r12,rcx		; index

	; Если RCX == 0, то чёрный цвет

	xor ebx,ebx		; Обнуляем EBX, чтобы сравнить его с ECX, и если EBX == 0, то в EBX будет чёрный цвет
	cmp ecx,ebx
	cmovne ebx,[r8 + r12 * 4] ; EBX = palette_rgb[color_index]	; перемещаем в EBX значение, если ECX != 0

	mov [r10+rdx*4],ebx

	btr r11d,edx ; Сбросим бит маски, чтобы не проверять этот индекс


_check_next:
	inc edx
	cmp edx,2
	jl _check_one_bit

	cmp r11d,0
	
	je _exit
	dec rcx
	cmp rcx,0
	jg _for

_exit:
	pop r12
	pop r11
	pop r10
	pop r9
	pop rbx
	pop rcx
	pop rdx

	ret
Asm_Set_Mandelbrot_2_Points endp
;-------------------------------------------------------------------

Asm_Set_Mandelbrot_4_Points proc
;extern "C" int Asm_Set_Mandelbrot_4_Points(char* video_buff, SPacked_XY_4* packed_xy, int* palette_rgb, int colors_cnt);

;RCX = video_buf
;RDX = packed_xy
;R8 = palette_rgb
;R9 = colors_cnt
;EAX - return
	
	push rdx
	push rcx
	push rbx
	push r9
	push r10
	push r11
	push r12

	mov r11,1111b		; R11[3..0] - Маска значений, для которых уже посчитан индекс (0 - да, 1 - нет)

	;mov rax,4
	;cvtsi2sd xmm0,rax	; xmm0 = 4.0 
	
	;pshufd xmm0,xmm0,01000100b	; xmm0 = {4.0; 4.0}

	vmovupd ymm0,[rdx]			; ymm0 = {4.0; 4.0; 4.0; 4.0}

	mov r10,rcx		; R10 - video_buff

	mov rcx,r9		; RCX = colors_cnt

	vmovupd ymm1,[rdx+8*4]		; YMM1 = {x0_1, x0_2, x0_3, x0_4}
	vmovupd ymm2,[rdx+8*8]		; YMM1 = {y0_1, y0_2, y0_3, y0_4}

;	x_n = 0.0;
;	y_n = 0.0;

	vxorpd ymm3,ymm3,ymm3		; ymm3 = {x1_n; x2_n; x3_n; x4_n} = {0.0; 0.0; 0.0; 0.0}
	vxorpd ymm4,ymm4,ymm4		; ymm4 = {y1_n; y2_n; y3_n; y4_n} = {0.0; 0.0; 0.0; 0.0}


_for:
;	for (i = 0; i < Frame_DC.colors_cnt; i++)
;		{
;			x_n1 = x_n * x_n - y_n * y_n + x_0;
	;vmovapd ymm5,ymm3	; ymm5 = {x1_n; x2_n; x3_n; x4_n}
	;vmovapd ymm6,ymm4	; ymm6 = {y1_n; y2_n; y3_n; y4_n}

	vmulpd ymm5,ymm3,ymm3		; ymm5 = {x1_n * x1_n;.. x4_n * x4_n}
	vmulpd ymm6,ymm4,ymm4		; ymm6 = {y1_n * y1_n;.. y4_n * y4_n}

	vsubpd ymm5,ymm5,ymm6		; ymm5 = {x1_n * x1_n - y1_n * y1_n;.. x4_n * x4_n - y4_n * y4_n}

	vaddpd ymm5,ymm5,ymm1		; ymm5 = {x1_n1 = x1_n * x1_n - y1_n * y1_n + x_0;.. x3_n1 = x4_n1 * x4_n - y4_n * y4_n + x_0};

;			y_n1 = 2.0 * x_n * y_n + y_0;
	;movapd xmm6,xmm3	; xmm6 = {x1_n; x2_n}

	vmulpd ymm6,ymm4,ymm3		; ymm6 = {x1_n * y1_n;.. x4_n * y4_n}
	vaddpd ymm6,ymm6,ymm6			; ymm6 = {2 * x1_n * y1_n;.. 2 * x4_n * y4_n}

	vaddpd ymm6,ymm6,ymm2			; ymm6 = {y1_n1 = 2.0 * x1_n * y1_n + y_0;.. y4_n1 = 2.0 * x4_n * y4_n + y_0}
;
;			distance = x_n1 * x_n1 + y_n1 * y_n1;
	;movapd xmm7,xmm5	; xmm7 = {x1_n1; x2_n1}
	;movapd xmm8,xmm6	; xmm8 = {y1_n1; y2_n1}

	vmulpd ymm7,ymm5,ymm5		; ymm7 = {x1_n1 * x1_n1;.. x4_n1 * x4_n1}
	vmulpd ymm8,ymm6,ymm6		; ymm8 = {y1_n1 * y1_n1;.. y4_n1 * y4_n1}

	vaddpd ymm7,ymm7,ymm8		; ymm7 = {distance_1;.. distance_4}
	

;			x_n = x_n1;
;			y_n = y_n1;
	vmovapd ymm3,ymm5
	vmovapd ymm4,ymm6

;			if (distance > 4.0)
;				break;
	
	vcmpnlepd ymm7,ymm7,ymm0	; ymm7 > ymm0 

	vmovmskpd eax,ymm7
	and rax,r11			; Накладываем маску, обнуляя биты, для которых уже был посчитан индекс

	cmp rax,0
	jne _check_bits


	loop _for		; Продолжаем, т.к нет новых индексов

	; Дойдём сюда, когда закончатся все итерации, при этом в RAX - результат последнего сравнения
	; Надо к этому результату применить результат, накопленный в маске

	or rax,r11		; Принудительно отмечаем оставшееся биты как те, для которых надо посчитать индекс

_check_bits:
	xor edx,edx		; Начнём проверять EAX с 0-го бита

_check_one_bit:
	bt eax,edx
	jnc _check_next
	
	mov r12,r9
	sub r12,rcx		; index

	; Если RCX == 0, то чёрный цвет

	xor ebx,ebx		; Обнуляем EBX, чтобы сравнить его с ECX, и если EBX == 0, то в EBX будет чёрный цвет
	cmp ecx,ebx
	je @F
	mov ebx,[r8 + r12 * 4] ; EBX = palette_rgb[color_index]	; перемещаем в EBX значение, если ECX != 0
	
@@:
	mov [r10+rdx*4],ebx

	btr r11d,edx ; Сбросим бит маски, чтобы не проверять этот индекс


_check_next:
	inc edx
	cmp edx,4
	jl _check_one_bit

	cmp r11d,0
	
	je _exit
	dec rcx
	cmp rcx,0
	jg _for

_exit:
	pop r12
	pop r11
	pop r10
	pop r9
	pop rbx
	pop rcx
	pop rdx

	ret
Asm_Set_Mandelbrot_4_Points endp
;-------------------------------------------------------------------
end