#include <stdio.h>
#include <Windows.h>
//현재 함수부 전반적으로 정상 작동 하지 않고 있어 사용 중단
void MENUE(int *n) {
	printf("Welcome to ENIGMA!\n\n");
	printf("MENU\n\n");
	printf("1. Encript File(File will be compressed first!)\n");
	printf("2. Encript Text\n");
	printf("Enter the number of menue(1 or 2): ");
	scanf("%d",&n);
}
void EF(int n) {
  //이 함수가 추후 File 암호화로 진행될 것
	printf("파일 암호화 결과");
	Sleep(1000);
}
void ET(int n) {
  //이 함수가 추후 문자열 암호화로 진행될 것
	printf("텍스트 암호화 결과");
	Sleep(1000);
}
void CHOOSE(int n, int flag) {
	switch (n) {
	case 1: EF(n); break;
	case 2: ET(n); break;
	default: printf("PLZ reenter the number of the menue : ");
		printf("%d", n);
		scanf("%d", &n);
		if (n == 1 || n == 2) {
			flag++;
		}
		break;
	}
}
int main() {
	int n = 0;
	int flag = 1;
	//MENUE(n);//현재 에러 발생으로 사용 중지
	printf("Welcome to ENIGMA!\n\n");
	printf("MENU\n\n");
	printf("1. Encript File(File will be compressed first!)\n");
	printf("2. Encript Text\n");
	printf("Enter the number of menue(1 or 2): ");
	scanf("%d", &n);
	while (flag == 1)
	{
		if (n == 1 || n == 2) {
			flag++;
		}
		switch (n) {
		case 1: {system("cls"); EF(n); break; }
		case 2: {system("cls"); ET(n); break; }
		default: printf("PLZ reenter the number of the menue : ");
			scanf("%d", &n); break;

		}

	}
	system("cls");
	printf("THX for your usage!\n");
	printf("If you faced any bug, PLZ report : robinjh0314@gmail.com");

	Sleep(1000);

	system("cls");

	return 0;
}
