/* stackmachine.c */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"code.h"

#define MAXFILENAME 255
#define MAXPROGRAM  255
#define MAXSTACK    255

struct {
	 int operation,operand;
	}    code[MAXPROGRAM];
int s[MAXSTACK];
int ps,pc,sp,bp;

void initialize();
void read_object_program();
void report_status();

/*  トップレベルループ */
main()
{
  int opr,opd;

  initialize();

  while(ps == 0){
    report_status();

    opr = code[pc].operation;
    opd = code[pc].operand;
    pc ++;

    do_operation(opr,opd);
    if(pc < 0 || pc>MAXPROGRAM)
      ps = 2;
  }
  report_status();
}

/*  初期化 */
void initialize()
{
  read_object_program();

  ps = bp = pc = 0;
  s[0] = 0;
  s[1] = -1;
  sp = 1;
}

/*  スタックプログラムの読みこみ */
void read_object_program()
{
  int ca;
  char source[MAXFILENAME];
  FILE *fp;

  printf("Program Name -->");
  scanf("%s",source);
  if((fp = fopen(source,"r")) == NULL) {
    printf("ファイルがありません!! (%s)\n",source);
    exit(1);
  }
  for( ca=0;
       fscanf(fp,"%*s %d %d",&code[ca].operation,&code[ca].operand) != EOF;
       ca++);
  fclose(fp);
}

/* 命令の実行 */
do_operation(int opr,int opd)
{
  switch (opr){
  case LDADR :
    s[++sp] = bp+opd;
    break;
  case LDVAL :
    s[++sp] = opd;
    break;
  case LDIND :
    s[sp] = s[s[sp]];
    break;
  case INCSP :
    sp += opd;
    break;
  case DECSP :
    sp -= opd;
    break;
  case STORE :
    s[s[sp-1]] = s[sp];
    sp -= 2;
    break;
  case JUMP  :
    pc = opd;
    break;
  case JUMPZ :
    if(s[sp--] == 0) pc = opd;
    break;
  case CALL  :
    s[sp+1] = bp; s[sp+2] = pc;
    pc = opd; bp = sp+1;
    break;
  case EXITP :
    sp = bp-1; pc = s[bp+1]; bp =s[bp];
    break;
  case EXITF :
    { int tmp; tmp = sp; sp = bp; pc = s[bp+1];
	       bp = s[bp]; s[sp] = s[tmp];  }
    break;
  case HALT  :
    ps = 1;
    break;
  case RDINT :
    scanf("%d",&s[++sp]);
    break;
  case RDCHR :
    scanf("%c",&s[++sp]);
    break;
  case WTINT :
    printf("%d",s[sp--]);
    break;
  case WTCHR :
    printf("%c",s[sp--]);
    break;
  case NL    :
    printf("\n");
    break;
  case ADD   :
    sp--; s[sp] = s[sp] + s[sp+1];
    break;
  case SUB   :
    sp--; s[sp] = s[sp] - s[sp+1];
    break;
  case MLT   :
    sp--; s[sp] = s[sp] * s[sp+1];
    break;
  case DIV   :
    sp--; s[sp] = s[sp] / s[sp+1];
    break;
  case MOD   :
    sp--; s[sp] = s[sp] % s[sp+1];
    break;
  case NEG   :
    s[sp] = -s[sp];
    break;
  case EQ    :
    sp--; if(s[sp] == s[sp+1]) s[sp] = 1;
    else s[sp] = 0; break;
  case NE    :
    sp--; if(s[sp] != s[sp+1]) s[sp] = 1;
    else s[sp] = 0; break;
  case GT    :
    sp--; if(s[sp] > s[sp+1]) s[sp] = 1;
    else s[sp] = 0; break;
  case LT    :
    sp--; if(s[sp] < s[sp+1]) s[sp] = 1;
    else s[sp] = 0; break;
  case NOP   :
    break;
  default :
    ps = 2;
    break;
}}
/* 状態のモニタ用関数  */
void report_status()
{
  int i;
  printf("--- PS:%d PC:%2d SP:%2d BP:%2d CD:%3d %3d\n",
	      ps,pc,sp,bp,code[pc].operation,code[pc].operand);
  printf("    STK:");
  for(i=sp;i>=0;i--) printf("%4d ",s[i]);
  printf("\n");
}

/* end of stackmachine */
