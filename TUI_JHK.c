//여기서 추후 코드 개발할 것!
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#define MAX 1000

int MENUE(int* n) { //실행 시 처음으로 나오는 메뉴를 보여주는 함수
	printf("Welcome to ENIGMA!\n\n");
	printf("MENU\n\n");
	printf("1. Encript / Decript File(File will be compressed first!)\n");
	printf("2. Encript / Decript Text\n");
	printf("3. Encription Key management\n");
	printf("Enter the number of menue( 1 - 3 ): ");
	scanf("%d", &n);
	return n;
}

void FILEENCRIPT() {	//실질적으로 파일 암호화가 되서 돌아갈 부분, 이름 받아올거임
	char FNAME[MAX];
	printf("You choosed Encriping file\n");
	printf("Put the file in same location with program and enter the name of file \n(Ex : file.txt )\n >>>   ");
	scanf("%s", &FNAME);

	FILE* FOPEN = fopen( FNAME, "r");
	if (FOPEN == NULL) {
		printf("There is no file! PLZ check again!");
		Sleep(1000);
		system("cls");
	}
	fclose(FOPEN);
	printf("File Encripted! Check RESULT.txt");
	FILE* RESULT = fopen( "RESULT.txt", "w");
	fprintf(RESULT, "%d ", FNAME);
	fclose(RESULT);
	printf("%s", FNAME);
	Sleep(10000);
	system("cls");
}

void FILEDECRIPT() { // 파일 복호화 해주는 기능을 할 함수
	char FNAME[MAX];
	printf("You choosed Decriping file\n");
	printf("Put the file in same location with program and enter the name of file \n(Ex : file.txt )\n >>>   ");
	scanf("%s", &FNAME);
	FILE* FOPEN =fopen( FNAME, "r");
	if (FOPEN == NULL) {
		printf("There is no file! PLZ check again!");
		Sleep(5000);
		system("cls");
	}
	printf("%s", FNAME);//이거는 복호화 한 파일이 나가는 자리가 될것
	fclose(FOPEN);
}

void EDF() { //file 관련 처리 담당할 함수
	char CHECK;
	int temp = 0;
	printf("Encription or Decription? [E / D]\n>>>>   ");
	scanf("%s", &CHECK);
	int flag = 1;
	do {
		if (CHECK == 'E' || CHECK == 'e' || CHECK == 'D' || CHECK == 'd')
		{
			flag++;
			if (CHECK == 'E' || CHECK == 'e')
			{
				temp = 1;
			}
			if (CHECK == 'D' || CHECK == 'd')
			{
				temp = 2;
			}
		}
		switch (temp) {
		case 1: {system("cls"); FILEENCRIPT(); break; }
		case 2: {system("cls"); FILEDECRIPT(); break; }
		default: printf("PLZ reenter the menue : ");
			scanf("%s", &CHECK); break;
		}

	} while (flag == 1);
}

void TXTENCRIPT() { // 이 함수가 문자열 암호화 해줄 함수
	char TEXT[MAX];
	printf("You choosed Encriping text\n");
	printf("Type text!\n >>>   ");
	scanf("%s", &TEXT);
	printf("Encripting finished! The result is %s\n", TEXT);
	printf("You can also find result at RESULT.txt");
	FILE* RESULT =fopen("RESULT.txt", "w");
	fprintf(RESULT, "%d ", TEXT);
	fclose(RESULT);
	Sleep(10000);
	system("cls");
}


void TXTDECRIPT() { // 이 함수가 문자열 복호화 해 줄 함수
	char TEXT[MAX];
	printf("You choosed Decriping text\n");
	printf("Type text!\n >>>   ");
	scanf("%s", &TEXT);
	printf("Encripting finished! The result is &s\n", TEXT);

	printf("You can also find result at RESULT.txt");
	FILE* RESULT = fopen("RESULT.txt", "w");
	fprintf(RESULT, "%d ", TEXT);
	fclose(RESULT);
	printf("%s", TEXT);
	Sleep(10000);
	system("cls");
}

void EDT() { //텍스트 관련 처리 담당할 함수
	char CHECK2;
	printf("Encription or Decription? [E / D]\n>>>>   ");
	scanf("%c", &CHECK2);
	int flag = 1;
	int temp = 0;
	do {
		if (CHECK2 == 'E' || CHECK2 == 'e' || CHECK2 == 'D' || CHECK2 == 'd')
		{
			flag++;
			if (CHECK2 == 'E' || CHECK2 == 'e')
			{
				temp = 1;
			}
			if (CHECK2 == 'D' || CHECK2 == 'd')
			{
				temp = 2;
			}
		}
		switch (temp) {
		case 1: {system("cls"); TXTENCRIPT(); break; }
		case 2: {system("cls"); TXTDECRIPT(); break; }
		default: printf("PLZ reenter the menue : ");
			scanf("%s", &CHECK2); break;
		}

	} while (flag == 1);
}

void REISSUE() { // 이 함수가 키 재발급 해줄 함수임
	FILE* PKEY = fopen("publickey.txt", "w");
	//fprintf();
	fclose(&PKEY);
	printf("Sucsessfully reissued!");
	Sleep(2500);
	system("cls");

}

void KEY() {
	int KEY;
	char answer;
	FILE* PKEY =fopen("publickey.txt", "r");
	if (PKEY == NULL) {
		printf("You dont have public key! Issue it first!");
		Sleep(10000);
		system("cls");
		return -1;
	}
	fscanf_s(PKEY, "%d", &KEY);
	fclose(PKEY);
	printf("This is Your Public Key : %d\n", KEY);
	printf("Do you wnat to reissue your public and private key? \n");
	printf("YOUR CURRENT KEY WILL BE DELETED PERMENENTLY! [Y/N]\n");
	scanf("%s", &answer);
	if (answer == 'Y') {
		system("cls");
		printf("공개 키 재발급");
		REISSUE();
		Sleep(10000);
		system("cls");
		return 0;
	}
}

int CHOOSE(int* n) {
	int flag = 1;
	int temp = n;
	do {
		if (temp == 1 || temp == 2 || temp == 3) {
			flag++;
		}
		switch (temp) {
		case 1: {system("cls"); EDF(); break; }
		case 2: {system("cls"); EDT(); break; }
		case 3: {system("cls"); KEY(); break; }
		default: printf("PLZ reenter the number of the menue : ");
			scanf("%d", &temp); break;

		}

	} while (flag == 1);
}
int main() {

	FILE* PKEY = NULL;
	fopen_s(&PKEY, "publickey.txt", "r");
	if (PKEY == NULL) {
		fopen_s(&PKEY, "publickey.txt", "w");
		//fprintf();
		fclose(&PKEY);
	}

	int n = 0;
	n = MENUE(n);
	CHOOSE(n);

	system("cls");
	printf("THX for your usage!\n");
	printf("If you faced any bug, PLZ report : robinjh0314@gmail.com");

	Sleep(1000);

	system("cls");


	return 0;
}
