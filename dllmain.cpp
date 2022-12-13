// file : c_module_main.cpp
#define _CRT_SECURE_NO_WARNINGS
#ifdef _MSC_VER                      // VC++
#define EXPORT __declspec(dllexport) // 윈도우용 dll export 지정자
#else
#define EXPORT                       // 리눅스의 경우 별다른 지정자가 필요치 않다
#endif

#include <vector>
#include <numeric>
#include <time.h>


extern "C"
{


    // 1. int 타입 인자를 받고, int 타입을 리턴하는 예
    EXPORT int add(int a, int b)
    {
        return a + b;
    }

    // 2. out 파라메터로 포인터를 사용하는 예
    EXPORT void sub(double a, double b, double* result)
    {
        *result = a - b;
    }

    // 3. 배열 파라메터를 사용하는 예
    EXPORT int accumulate(int* input, int size)
    {
        std::vector<int> v(input, input + size);
        int result = std::accumulate(v.begin(), v.end(), 0u);
        return result;
    }



    int PRM1, PRM2;   //소수 2개
    int mul;      //소수 곱
    int Euler;
    int PubKEY;      //공개키e
    int ScrKEY = 1;  //비밀키d
    #define MAX 1000


    int i, n, X, Y;
    long int e[50], d[50], temp[50], j, m[50], en[50];

    EXPORT void newkeymaking() {
        FILE* kp = fopen("key.txt", "r");
        if (kp != NULL) {
            fscanf(kp, "%d %d %d", &PubKEY, &ScrKEY, &mul);
            e[0] = PubKEY;
            d[0] = ScrKEY;
            n = mul;
            fclose(kp);


        }
        else {
            while (1) {

                int i, j;
                srand((unsigned)time(NULL));

                i = (rand() % 80) + 1;
                for (i; 1; i++) {
                    for (j = 2; j <= i; j++) {
                        if (i % j == 0)
                            break;
                    }
                    if (j == i) {
                        PRM1 = i;
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
                        PRM2 = i;
                        break;
                    }
                }


                ScrKEY = 1;
                mul = PRM1 * PRM2;
                Euler = (PRM1 - 1) * (PRM2 - 1);
                for (ScrKEY; 1; ScrKEY++) {
                    PubKEY = (rand() % (Euler - 2)) + 2;
                    int result;
                    result = (ScrKEY * PubKEY) % Euler;
                    if (result == 1) {
                        break;
                    }
                }


                if (PubKEY >= 100 && ScrKEY >= 100 && PubKEY <= 10000 && ScrKEY <= 10000) {
                    e[0] = PubKEY;
                    d[0] = ScrKEY;
                    n = mul;

                    FILE* fp = fopen("key.txt", "w");
                    fprintf(fp, "%d %d %d", PubKEY, ScrKEY, mul);
                    fclose(fp);
                    break;
                }
            }
        }

    }



    EXPORT int KEY() {
        return PubKEY;
    }
    EXPORT void rekey() {
        while (1) {

            int i, j;
            srand((unsigned)time(NULL));

            i = (rand() % 80) + 1;
            for (i; 1; i++) {
                for (j = 2; j <= i; j++) {
                    if (i % j == 0)
                        break;
                }
                if (j == i) {
                    PRM1 = i;
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
                    PRM2 = i;
                    break;
                }
            }


            ScrKEY = 1;
            mul = PRM1 * PRM2;
            Euler = (PRM1 - 1) * (PRM2 - 1);
            for (ScrKEY; 1; ScrKEY++) {
                PubKEY = (rand() % (Euler - 2)) + 2;
                int result;
                result = (ScrKEY * PubKEY) % Euler;
                if (result == 1) {
                    break;
                }
            }


            if (PubKEY >= 100 && ScrKEY >= 100 && PubKEY <= 10000 && ScrKEY <= 10000) {
                e[0] = PubKEY;
                d[0] = ScrKEY;
                n = mul;

                FILE* fp = fopen("key.txt", "w");
                fprintf(fp, "%d %d %d", PubKEY, ScrKEY, mul);
                fclose(fp);
                break;
            }
        }
    }
    EXPORT int FileEncoding(int File)
    {

        long i = File;
        long j = PubKEY;
        long k = mul;
        double l, temp, p = 1;
        for (temp = 0; temp < j; temp++) {
            p = (p * ((double)i));
            l = (long)(p / k);
            p = p - (l * k);
        }
        File = (long)p;
        return File;

    }

    EXPORT void insert(int data) {
        FILE* fp = fopen("encrypt.txt", "a");
        fprintf(fp, "%d ", data);
        fclose(fp);

    }

    EXPORT void fileencryption(char* filename, char* enfile)
    {

        int File;
        char Filename[100];
        char Enfile[100];
        strcpy(Filename, filename);
        strcpy(Enfile, enfile);

        FILE* FileCreat = fopen(Filename, "rb");

        FILE* CreatFile = fopen(Enfile, "w");
    }


    EXPORT void filedecrypt()
    {
        int File;

        FILE* DeCodFileCreat = fopen("encrypt.txt", "r");

        FILE* DeCodCreatFile = fopen("decrypt.zip", "wb");
        while (fscanf(DeCodFileCreat, "%d", &File) != EOF)
        {
            long i = File;
            long j = ScrKEY;
            long k = mul;
            double l, temp, p = 1;
            for (temp = 0; temp < j; temp++) {
                p = (p * ((double)i));
                l = (long)(p / k);
                p = p - (l * k);
            }
            File = (long)p;


            fwrite(&File, sizeof(char), 1, DeCodCreatFile);
        }
        fclose(DeCodCreatFile);

        fclose(DeCodFileCreat);
    }

    EXPORT void txtencrypt() {
        char msg[MAX];
        FILE* readfile = fopen("txtinput.txt", "r");
        FILE* CreatFile = fopen("txtout.txt", "w");
            fgets(msg, sizeof(msg), readfile);
        for (i = 0; msg[i] != NULL; i++)
            m[i] = msg[i];

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
            for (i = 0; en[i] != -1; i++)
                fprintf(CreatFile, "%c", en[i]);

        fclose(readfile);
        fclose(CreatFile);
    }

    EXPORT void txtdecrypt() {
        char msg[MAX];
        FILE* readfile = fopen("txtinput.txt", "r");
        FILE* CreatFile = fopen("txtout.txt", "w");
        fgets(msg, sizeof(msg), readfile);
        for (i = 0; msg[i] != NULL; i++)
           m[i] = msg[i];

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
            for (i = 0; m[i] != -1; i++)
                fprintf(CreatFile, "%c", m[i]);
        fclose(readfile);
        fclose(CreatFile);

    }



    struct Rect
    {
        int x;
        int y;
        int width;
        int height;
    };

}
