typedef struct Symbol {	/* symbol table entry */
	char	*name;
	short	type;	/* NAME, NUM or STRING  */
	union {
		int	val;	/* if NAME or NUM */
		char	*str;	/* if STRING */
	} u;
	struct Symbol	*next;	/* to link to another */
} Symbol;
Symbol	*install(char *, int, int, char *), *lookup(char *);

typedef union Datum {	/* interpreter stack type */
	int	 val;
	char    *str;
	Symbol	*sym;
} Datum;
extern	Datum pop();

typedef int (*Inst)();	/* machine instruction */
#define	STOP	(Inst) 0

extern  Inst *code(Inst);
extern	Inst prog[];
extern	int eval();
extern  int add();
extern  int negate(); 
extern  int power();
extern	int assign();
extern  int bltin();
extern  int varpush(); 
extern  int constpush();
extern  int strpush();
extern  int print();

extern void initcode();
extern void execute(Inst *);