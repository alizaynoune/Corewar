.name "sli"
.comment ""
ld 	%2, r2
ld 	%2, r2
all: ld 	:all, r2
ld	%2, r2
ld 	%2, r2
ali:	sti r2, 2, %2
	zjmp %:all
	ldi r2,r3 , r0
	st r3, :ali
loop:
        sti r1, %:live, %1
live:
        live %0
        ld %0, r2
        zjmp %:loop
