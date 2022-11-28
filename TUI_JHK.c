//여기서 추후 코드 개발할 것!
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAX 10000
int PRM1, PRM2;   //소수 2개
int mul;      //소수 곱
int Euler;
int PubKEY;      //공개키e
int ScrKEY = 1;  //비밀키d
void CHOOSE();

void select_PRMs(int* PRM1, int* PRM2) {         //소수 2개를 선택하는 함수
	int i, j;
	srand((unsigned)time(NULL));

	i = (rand() % 80) + 1;
	for (i; 1; i++) {
		for (j = 2; j <= i; j++) {
			if (i % j == 0)
				break;
		}
		if (j == i) {
			*PRM1 = i;
			break;
		}
	}
	i = (rand() % 80) + 1;
	for (i; 1; i++) {
		for (j = 2; j <= i; j++) {
			if (i % j == 0)
				break;
		}
		if (j == i) {
			*PRM2 = i;
			break;
		}
	}

}

//값, 키를 받아 RSA암호화를 진행하는 함수

long pow_(long i, long j, long k) {
	double l, temp, p = 1;
	for (temp = 0; temp < j; temp++) {
		p = (p * ((double)i));
		l = (long)(p / k);
		p = p - (l * k);
	}
	return (long)p;
}

void FileEncoding()

{
	FILE* FileCreat = NULL;
	int File;
	char Type[27];
	system("cls");
	printf("※ Must be entered including file extension.\n");
	getchar();
	for (;;)
	{
		printf("Please enter the file name to encrypt : ");
		gets_s(Type,sizeof(Type));
		FileCreat = fopen(Type, "rb");
		if (FileCreat == NULL)
		{
			printf("\nERROR! FILE DOES NOT EXIST!\n");
			continue;
		}
		else
		{
			FILE* CreatFile;
			char NameType[27];
			printf("\nEnter the name of the file to be created(.txt) : ");
			gets_s(NameType,sizeof(NameType));
			CreatFile = fopen(NameType, "w");
			while ((File = fgetc(FileCreat)) != EOF)
			{
				File = pow_(File, PubKEY, mul);
				printf(".");
				fprintf(CreatFile, "%d ", File);
			}
			fclose(CreatFile);
			system("cls");
			printf("\n\nEncryption Finished! \nGoing back to File Encrypting/Decrypting menue\n");
			Sleep(2500);
			system("cls");
			break;
		}
	}
	fclose(FileCreat);
}

void FileDecoding()
{
	FILE* DeCodFileCreat = NULL;
	int File;
	char DeType[27];
	system("cls");
	printf("※ Must be entered including file extension.\n");
	getchar();
	for (;;)
	{
		printf("Please enter the file name to decrypt(.txt) : ");
		gets_s(DeType,sizeof(DeType));
		DeCodFileCreat = fopen(DeType, "r");
		if (DeCodFileCreat == NULL)
		{
			printf("\nERROR! FILE DOES NOT EXIST!\n");
			continue;
		}
		else
		{
			FILE* DeCodCreatFile;
			char DeCodNameType[27];
			printf("\nEnter the name of the file to be created : ");
			gets_s(DeCodNameType,sizeof(DeCodNameType));
			DeCodCreatFile = fopen(DeCodNameType, "wb");
			while (fscanf(DeCodFileCreat, "%d", &File) != EOF)
			{
				File = pow_(File, ScrKEY, mul);
				printf(".");
				fwrite(&File, sizeof(char), 1, DeCodCreatFile);
			}
			fclose(DeCodCreatFile);
			system("cls");
			printf("\n\nEncryption Finished! \nGoing back to File Encrypting/Decrypting menue\n");
			Sleep(2500);
			system("cls");
			break;
		}
	}
	fclose(DeCodFileCreat);
}

void MENUE() { //실행 시 처음으로 나오는 메뉴를 보여주는 함수
	printf("Welcome to ENIGMA!\n\n");
	printf("MENU\n\n");
	printf("1. Encript / Decript File(File will be compressed first!)\n");
	printf("2. Encript / Decript Text\n");
	printf("3. Encription Key management\n");
	printf("4. End program\n");
	printf("Enter the number of menue( 1 - 4 ): ");
}

void TXTENCRIPT() { // 이 함수가 문자열 암호화 해줄 함수
	char TEXT[MAX];
	printf("You choosed Encriping text\n");
	printf("Type text!\n >>>   ");
	scanf("%s", &TEXT);
	printf("Encripting finished! The result is %s\n", TEXT);
	printf("You can also find result at RESULT.txt");
	FILE* RESULT = fopen("RESULT.txt", "w");
	fprintf(RESULT, "%d ", TEXT);
	fclose(RESULT);
	Sleep(5000);
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
	Sleep(5000);
	system("cls");
}

void EDT() { //텍스트 관련 처리 담당할 함수
	char CHECK2;
	printf("Encription or Decription? [E / D]\n>>>>   ");

	scanf(" %c", &CHECK2);

	int flag = 1;
	int temp = 0;

	while (flag == 1) {
		if (CHECK2 == 'E') { flag++; system("cls");  TXTENCRIPT(); break; }
		else if (CHECK2 == 'e') { flag++; system("cls"); TXTENCRIPT(); break; }
		else if (CHECK2 == 'D') { flag++; system("cls"); TXTDECRIPT(); break; }
		else if (CHECK2 == 'd') { flag++; system("cls"); TXTDECRIPT(); break; }
		else { printf("PLZ reenter the menue : "); scanf(" %c", &CHECK2); }
	}

}

void keymaking() {
	select_PRMs(&PRM1, &PRM2);
	mul = PRM1 * PRM2;
	Euler = (PRM1 - 1) * (PRM2 - 1);
	for (ScrKEY; 1; ScrKEY++) {
		PubKEY = (rand() % (Euler - 2)) + 2;  // 1 < e < Euler(n)
		int result;
		result = (ScrKEY * PubKEY) % Euler;
		if (result == 1) {
			break;
		}
	}
	FILE* PKEY = fopen("publickey.txt", "w+");
	fprintf(PKEY, "%d %d %d", PubKEY, ScrKEY, mul);
	fclose(PKEY);
}

void KEY() {
	char answer;
	FILE* PKEY = fopen("publickey.txt", "r");
	fscanf(PKEY, "%d %d %d", &PubKEY, &ScrKEY, &mul);
	fclose(PKEY);
	printf("This is Your Public Key : %d\n", PubKEY);
	printf("Do you wnat to reissue your public and private key? \n");
	printf("YOUR CURRENT KEY WILL BE DELETED PERMENENTLY! [Y/N]\n");
	scanf(" %c", &answer);
	if (answer == 'Y') {
		system("cls");
		printf("Reissued key!\n");
		keymaking();
		printf("Sucsessfully reissued!");
		printf("공개키(%d)  비밀키(%d)", PubKEY, ScrKEY);
		Sleep(5000);
		system("cls");
		printf("Going back to menue!");
	}
	system("cls");
	MENUE();
	CHOOSE();
}


void EDF()
{
	int choice;
	for (;;)
	{
		printf("\n1. File Encrypt\n2. File Decrypt\n3. Exit to main menue\n\n");
		printf("Type : ");
		scanf_s("%d", &choice);
		system("cls");
		if (choice < 4) { break; }
		else { continue; }
	}

	switch (choice)
	{
	case 1:
		FileEncoding();
		EDF();
		break;

	case 2:
		FileDecoding();
		EDF();
		break;

	case 3:
		MENUE();
		CHOOSE();
		break;
	}
}

void CHOOSE() {
	int flag = 1;
	int temp = 0;
	scanf("%d", &temp);
	do {
		if (temp == 1 || temp == 2 || temp == 3|| temp == 4) {
			flag++;
		}
		switch (temp) {
		case 1: {system("cls"); EDF(); break; }
		case 2: {system("cls"); EDT(); break; }
		case 3: {system("cls"); KEY(); break; }
		case 4: {exit; break; }
		default: printf("PLZ reenter the number of the menue : ");
			scanf("%d", &temp); break;

		}

	} while (flag == 1);
}


int main() {

	FILE* PKEYOPEN = fopen("publickey.txt", "r");
	if (PKEYOPEN == NULL) {
		keymaking();
	}
	else{
		fscanf(PKEYOPEN, "%d %d %d", &PubKEY, &ScrKEY, &mul);
		fclose(PKEYOPEN);
	}

	MENUE();

	CHOOSE();

	system("cls");
	printf("THX for your usage!\n");
	printf("If you faced any bug, PLZ report : robinjh0314@gmail.com");

	Sleep(2500);

	system("cls");


	return 0;
}
