#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int p, q;   //소수 2개
int n;      //n=p*q
int Euler;
int e;      //공개키e
int d = 1;  //비밀키d

void Menu()

{

   int choice;

   for (;;)

   {

      printf("\n1. 파일 암호화\n2. 파일 복호화\n3. 종료\n\n");

      printf("입력 : ");

      scanf_s("%d", &choice); 

      if (choice <= 3)

      {

         break;

      }

      else

      {

         continue;

      }

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

            File = pow_(File, e, n);

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

            File = pow_(File, d, n);

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
