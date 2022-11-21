//여기서 추후 코드 개발할 것!
<<<<<<< HEAD
#include <stdio.h>
#include <Windows.h>
//현재 함수부 전반적으로 정상 작동 하지 않고 있어 사용 중단
void MENUE(int *n) {
=======
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#define MAX 1000

int MENUE(int *n) {
>>>>>>> fce90095d7538d16a64315153eff9a9ff591d27f
	printf("Welcome to ENIGMA!\n\n");
	printf("MENU\n\n");
	printf("1. Encript File(File will be compressed first!)\n");
	printf("2. Encript Text\n");
<<<<<<< HEAD
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
=======
	printf("3. Encription Key management\n");
	printf("Enter the number of menue( 1 - 3 ): ");
	scanf("%d",&n);
	return n;

}
//void REISSUE(){	} 이 함수가 키 발급 관련 함수
//void TXTENCRIPT(*TEXT){	} 이 함수가 문자열 암호화 해줄 함수
//void FILEENCRIPT(*FNAME) {	} 이 함수가 파일 암호화 해줄 함수, file 주소가 입력될 것
void EF() {
	char FNAME[MAX];
	printf("You choosed Encriping file\n");
	printf("Put the file in same location with program and enter the name of file \n(Ex : file.txt )\n >>> ");
	scanf("%s", &FNAME);
	printf("%s", FNAME);

	//FILEENCRIP(LOCATION)
	printf("파일 암호화 결과"); //암호화 된 파일의 위치를 보여줄 것, 이 프로그램이 저장되어 있는 file 안에 저장될거임
	Sleep(10000);
}
void ET() {
	char TEXT[MAX];
	printf("You choosed Encripting text\n");
	printf("Enter the text : ");
	scanf("%s", &TEXT);
	printf("%s", TEXT);

	//TXTENCRIPT(TEXT);
	printf("텍스트 암호화 결과"); //실시간으로 출력해주지 않고 TXT 파일 형태로 제공할 것
	Sleep(10000);
}
void KEY() {
	int KEY;
	char answer;
	FILE* PKEY = NULL;
	fopen_s(&PKEY, "publickey.txt", "r");
	if (PKEY == NULL) {
		printf("You dont have public key! Issue it first!");
		Sleep(10000);
		system("cls");
		return -1;
	}
	fscanf_s(PKEY, "%d", &KEY);
	printf("This is Your Public Key : %d\n",KEY);
	printf("Do you wnat to reissue your public and private key? \n");
	printf("YOUR CURRENT KEY WILL BE DELETED PERMENENTLY! [Y/N]\n");
	scanf("%s", &answer);
	if (answer == 'Y') {
		system("cls");
		printf("공개 키 재발급");
		//REISSUE();
		Sleep(10000);
		system("cls");
		return 0;
	}
}
int CHOOSE(int *n){
	int flag = 1;
	int temp = n;
	do{
		if (temp == 1 || temp == 2 || temp ==3) {
			flag++;
		}
		switch (temp) {
		case 1: {system("cls"); EF(); break; }
		case 2: {system("cls"); ET(); break; }
		case 3: {system("cls"); KEY(); break; }
		default: printf("PLZ reenter the number of the menue : ");
			scanf("%d", &temp); break;

		}

	} while (flag == 1);
}
int main() {
	int n = 0;
	n=MENUE(n);
	CHOOSE(n);

>>>>>>> fce90095d7538d16a64315153eff9a9ff591d27f
	system("cls");
	printf("THX for your usage!\n");
	printf("If you faced any bug, PLZ report : robinjh0314@gmail.com");

	Sleep(1000);

	system("cls");

<<<<<<< HEAD
=======

>>>>>>> fce90095d7538d16a64315153eff9a9ff591d27f
	return 0;
}
