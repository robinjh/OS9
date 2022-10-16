#include <stdio.h>

void LSMprofile() {
    printf("이름 : dydtmddl\n");
    printf("이메일 : runring11@gmail.com\n");
    printf("학과 : 컴퓨터공학과\n");
    printf("학년 : 1\n");

}
void INOUT_KJH() {
	int a, b, c;
	int add = 0;
	int sub = 0;
	printf("type three numbers, A B C \n(like 1 2 3)\n");
	scanf("%d %d %d", &a, &b, &c);
	add = a + b + c;
	sub = a - b - c;
	printf("added result is %d\n", add);
	printf("subtracted result is %d\n", sub);
	printf("written by KJH(robinjh0314@gmail.com, Computer Science and Engineering, 1st grade)THX!\n");
}
void CJW()
{
  printf("NAME : choi jin woo\n");
  printf("Student ID : 2022040018\n");
}
int main () {
printf("\n");
LSMprofile();
printf("\n");
INOUT_KJH();
printf("\n");
CJW();
printf("\n");
	kyt();
  return 0;
}

void kyt()
{
	printf("kim yun tae");
}
