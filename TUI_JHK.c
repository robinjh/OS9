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
int mul;      //p*q
int Euler;
int PubKEY;      //공개키e
int ScrKEY = 1;  //비밀키d

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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RSA의 공개키, 비공개키를 만들어 보여주는 함수

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
    printf("공개키e, n (%d, %d)\n비밀키d, n (%d, %d)", PubKEY, mul, ScrKEY, mul);

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
/////////////////////////////////////////////////////////////////////////////////////////////////////////이 사이가 키 관리 함수에 포함되야 될 것

void EDF()
{
	int choice;
   for (;;)
   {
      printf("\n1. 파일 암호화\n2. 파일 복호화\n5. 종료\n\n");
	  printf("입력 : ");
	  scanf_s("%d", &choice);
	  if (choice < 3) { break; }
	  else { continue; }
   }

   switch (choice)
   {
   case 1:
      FileEncoding();
      break;

   case 2:
      FileDecoding();
      break;

   case 3:
      exit(1);
   }
}

void FileEncoding()

{

   FILE* FileCreat = NULL;

   int File;

   char Type[27];

   system("cls");

   printf("※ 확장자 까지 입력하여 주셔야 합니다.\n");

   getchar();

   for (;;)

   {

      printf("암호화 할 파일을 입력 해 주십시오 : ");

      gets_s(Type);

      FileCreat = fopen(Type, "rb");

      if (FileCreat == NULL)

      {

         printf("\n파일이 존재하지 않습니다.\n");

         continue;

      }

      else

      {

         FILE* CreatFile;

         char NameType[27];

         printf("\n생성 될 파일명을 적어주세요(.txt) : ");

         gets_s(NameType);

         CreatFile = fopen(NameType, "w");

         while ((File = fgetc(FileCreat)) != EOF)

         {

            File = pow_(File, PubKEY, mul);

            printf(".");

            fprintf(CreatFile, "%d ", File);

         }

         fclose(CreatFile);

         system("cls");
         printf("\n\n암호화 완료! 시작 화면으로 돌아갑니다\n");

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

   printf("※ 확장자 까지 입력하여 주셔야 합니다.\n");

   getchar();

   for (;;)

   {

      printf("복호화 할 파일을 입력 해 주십시오(.txt) : ");

      gets_s(DeType);

      DeCodFileCreat = fopen(DeType, "r");

      if (DeCodFileCreat == NULL)

      {

         printf("\n파일이 존재하지 않습니다.\n");

         continue;

      }

      else

      {

         FILE* DeCodCreatFile;

         char DeCodNameType[27];

         printf("\n생성 될 파일명을 적어주세요 : ");

         gets_s(DeCodNameType);

         DeCodCreatFile = fopen(DeCodNameType, "wb");

         while (fscanf(DeCodFileCreat, "%d", &File) != EOF)

         {

            File = pow_(File, ScrKEY, mul);

            printf(".");

            fwrite(&File, sizeof(char), 1, DeCodCreatFile);

         }

         fclose(DeCodCreatFile);

         system("cls");
         printf("\n\n복호화 완료! 시작 화면으로 돌아갑니다\n");

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
	printf("Enter the number of menue( 1 - 3 ): ");

}

void FILEENCRIPT() {	//실질적으로 파일 암호화가 되서 돌아갈 부분, 이름 받아올거임
	char FNAME[MAX];
	char CONTENT[MAX];
	printf("You choosed Encriping file\n");
	printf("Put the file in same location with program and enter the name of file \n(Ex : file.txt )\n >>>   ");
	scanf("%s", &FNAME);

	FILE* FOPEN = fopen( FNAME, "r");
	if (FOPEN == NULL) {
		printf("There is no file! PLZ check again!");
		Sleep(1000);
		system("cls");
	}
	fscanf(FNAME, "%S", &CONTENT);
	//fclose(FOPEN);
	//여기에서 암호화 진행하는거 삽입
	printf("File Encripted! Check RESULT.txt");
	FILE* RESULT = fopen( "RESULT.txt", "w");
	fprintf(RESULT, "%d ", CONTENT);
	fclose(RESULT);
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
		Sleep(1000);
		system("cls");
	}
	//이거는 복호화 한 파일이 나가는 자리가 될것
	//fclose(FOPEN);
}

void EDFold() { //file 관련 처리 담당할 함수
	char CHECK;
	int temp = 0;
	printf("Encription or Decription? [E / D]\n>>>>   ");
	scanf(" %c", &CHECK);
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
			scanf(" %c", &CHECK); break;
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

	scanf(" %c",&CHECK2);

	int flag = 1;
	int temp = 0;

	while (flag == 1){
		if (CHECK2 == 'E') { flag++; system("cls");  TXTENCRIPT(); break; }
		else if (CHECK2 == 'e') { flag++; system("cls"); TXTENCRIPT(); break; }
		else if (CHECK2 == 'D') { flag++; system("cls"); TXTDECRIPT(); break; }
		else if (CHECK2 == 'd') { flag++; system("cls"); TXTDECRIPT(); break; }
		else { printf("PLZ reenter the menue : ");scanf(" %c", &CHECK2); }
	}

}


void REISSUE() { // 이 함수가 키 재발급 해줄 함수임
	FILE* PKEY = fopen("publickey.txt", "w");
	//fprintf();
	fclose(PKEY);
	printf("Sucsessfully reissued!");
	Sleep(2500);
	system("cls");

}

void KEY() {
	int KEY;
	char answer;
	FILE* PKEY = fopen("publickey.txt", "r");
	if (PKEY == NULL) {
		printf("You dont have public key! Issue it first!");
		Sleep(10000);
		system("cls");
	}
	fscanf(PKEY, "%d", &KEY);
	fclose(PKEY);
	printf("This is Your Public Key : %d\n", KEY);
	printf("Do you wnat to reissue your public and private key? \n");
	printf("YOUR CURRENT KEY WILL BE DELETED PERMENENTLY! [Y/N]\n");
	scanf(" %c", &answer);
	if (answer == 'Y') {
		system("cls");
		printf("공개 키 재발급");
		REISSUE();
		Sleep(10000);
		system("cls");
	}
}

void CHOOSE() {
	int flag = 1;
	int temp = 0;
	scanf("%d", &temp);
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

	FILE* PKEYOPEN = fopen( "publickey.txt", "r");
	if (PKEYOPEN == NULL) {
		fopen("publickey.txt", "w");
		//fprintf();
		fclose(PKEYOPEN);
	}


	MENUE();

	CHOOSE();

	system("cls");
	printf("THX for your usage!\n");
	printf("If you faced any bug, PLZ report : robinjh0314@gmail.com");

	Sleep(1000);

	system("cls");


	return 0;
}
