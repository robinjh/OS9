//여기서 추후 코드 개발할 것!
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
void newkeymaking();

int i, n, X, Y;
long int e[50], d[50], temp[50], j, m[50], en[50];
char msg[MAX];
void encrypt();
void decrypt();

void EDT();

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
	char Type[MAX];
	system("cls");
	printf("※ Must be entered including file extension.\n");
	getchar();
	for (;;)
	{
		printf("Please enter the file name to encrypt : ");
		gets_s(Type, sizeof(Type));
		FileCreat = fopen(Type, "rb");
		if (FileCreat == NULL)
		{
			printf("\nERROR! FILE DOES NOT EXIST!\n");
			continue;
		}
		else
		{

			int encount = 0;
			int enfilecount = 1;

			FILE* CreatFile;
			char NameType[MAX];
			printf("\nEnter the name of the file to be created(.txt) : ");
			gets_s(NameType, sizeof(NameType));
			CreatFile = fopen(NameType, "w");
			while ((File = fgetc(FileCreat)) != EOF)
			{
				encount++;
				File = pow_(File, PubKEY, mul);

				fprintf(CreatFile, "%d ", File);

				if (encount % 1000000 == 0) {
					fclose(CreatFile);
					CreatFile = fopen(NameType, "a");
					enfilecount++;
					printf("\n%d", enfilecount);
				}
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
	char DeType[MAX];
	system("cls");
	printf("※ Must be entered including file extension.\n");
	getchar();
	for (;;)
	{
		printf("Please enter the file name to decrypt(.txt) : ");
		gets_s(DeType, sizeof(DeType));
		DeCodFileCreat = fopen(DeType, "r");
		if (DeCodFileCreat == NULL)
		{
			printf("\nERROR! FILE DOES NOT EXIST!\n");
			continue;
		}
		else
		{

			int defilecount = 1;
			int decount = 0;
			FILE* DeCodCreatFile;
			char DeCodNameType[MAX];
			printf("\nEnter the name of the file to be created : ");
			gets_s(DeCodNameType, sizeof(DeCodNameType));
			DeCodCreatFile = fopen(DeCodNameType, "wb");
			while (fscanf(DeCodFileCreat, "%d", &File) != EOF)
			{
				decount++;
				File = pow_(File, ScrKEY, mul);

				fwrite(&File, sizeof(char), 1, DeCodCreatFile);

				if (decount % 100000 == 0) {
					defilecount++;
					printf("현재 블록 : %d", defilecount);
				}
			}
			fclose(DeCodCreatFile);
			system("cls");
			printf("\n\nDecryption Finished! \nGoing back to File Encrypting/Decrypting menue\n");
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
////////////////////////////////////////////////////////////////////////////////////////////////

//function to encrypt the message
void encrypt()
{
	long int pt, ct, key = e[0], k, len;
	i = 0;
	len = strlen(msg);
	while (i != len)
	{
		pt = m[i];
		pt = pt - 96;
		k = 1;
		for (j = 0; j < key; j++)
		{
			k = k * pt;
			k = k % n;
		}
		temp[i] = k;
		ct = k + 96;
		en[i] = ct;
		i++;
	}
	en[i] = -1;
	printf("\n\nTHE ENCRYPTED MESSAGE IS\n");
	for (i = 0; en[i] != -1; i++)
		printf("%c", en[i]);
}

//function to decrypt the message
void decrypt()
{
	long int pt, ct, key = d[0], k;
	i = 0;
	while (en[i] != -1)
	{
		ct = temp[i];
		k = 1;
		for (j = 0; j < key; j++)
		{
			k = k * ct;
			k = k % n;
		}
		pt = k + 96;
		m[i] = pt;
		i++;
	}
	m[i] = -1;
	printf("\n\nTHE DECRYPTED MESSAGE IS\n");
	for (i = 0; m[i] != -1; i++)
		printf("%c", m[i]);
	printf("\n");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void EDT() {
	{
		int choice;
		for (;;)
		{
			printf("\n1. Text Encrypt\n2. Text Decrypt\n3. Exit to main menue\n\n");
			printf("Type : ");
			scanf_s("%d", &choice);
			system("cls");
			if (choice < 4) { break; }
			else { continue; }
		}
		switch (choice)
		{
		case 1:
			printf("\nENTER MESSAGE OR STRING TO ENCRYPT\n");
			getchar();
			gets(msg);
			for (i = 0; msg[i] != NULL; i++)
				m[i] = msg[i];
			encrypt();
			printf("\nSave the result by copying text(Ctrl+c)");
			Sleep(5000);
			system("CLS");
			EDT();
			break;

		case 2:
			printf("\nENTER MESSAGE OR STRING TO ENCRYPT\n");
			getchar();
			gets(msg);
			for (i = 0; msg[i] != NULL; i++)
				m[i] = msg[i];
			decrypt();
			printf("\nSave the result by copying text(Ctrl+c)");
			Sleep(5000);
			system("CLS");
			EDT();
			break;

		case 3:
			MENUE();
			CHOOSE();
			break;
		}

	}
}

void keymaking() {
	ScrKEY = 1;
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

void newkeymaking() {                   // [추가한 부분] 3자리 수 이상으로 만들어질 때까지 반복*************
	while (1) {

		keymaking();
		if (PubKEY >= 100 && ScrKEY >= 100 && PubKEY <= 10000 && ScrKEY <= 10000) {		//비밀키가 과도하게 크게 생성되는 문제가 있어 만 이하의 수로 결정되도록 조건 추가함

			break;
		}
	}
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
		newkeymaking();
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
		if (temp == 1 || temp == 2 || temp == 3 || temp == 4) {
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
		newkeymaking();
	}
	else {
		fscanf(PKEYOPEN, "%d %d %d", &PubKEY, &ScrKEY, &mul);
		fclose(PKEYOPEN);
	}

	e[0] = PubKEY;
	d[0] = ScrKEY;
	n = mul;

	MENUE();

	CHOOSE();

	system("cls");
	printf("THX for your usage!\n");
	printf("If you faced any bug, PLZ report : robinjh0314@gmail.com");

	Sleep(2500);

	system("cls");


	return 0;
}
