//여기서 추후 코드 개발할 것!
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#define MAX 1000

int MENUE(int *n) {
	printf("Welcome to ENIGMA!\n\n");
	printf("MENU\n\n");
	printf("1. Encript File(File will be compressed first!)\n");
	printf("2. Encript Text\n");
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

	system("cls");
	printf("THX for your usage!\n");
	printf("If you faced any bug, PLZ report : robinjh0314@gmail.com");

	Sleep(1000);

	system("cls");


	return 0;
}
