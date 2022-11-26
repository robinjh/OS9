//전역변수로 설정

int p, q;   //소수 2개
int n;      //p*q
int Euler;
int e;      //공개키e
int d = 1;  //비밀키d



void select_pq(int* p, int* q) {         //소수 2개를 선택하는 함수
    int i, j;
    srand((unsigned)time(NULL));

    i = (rand() % 80) + 1;               
    for (i; 1; i++) {
        for (j = 2; j <= i; j++) {
            if (i % j == 0)
                break;
        }
        if (j == i) {
            *p = i;
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
            *q = i;
            break;
        }
    }

}



//RSA의 공개키, 비공개키를 만들어 보여주는 함수


void keymaking() {



    select_pq(&p, &q);
    //printf("%d, %d", p, q);
    n = p * q;
    Euler = (p - 1) * (q - 1);

    for (d; 1; d++) {
        e = (rand() % (Euler - 2)) + 2;  // 1 < e < Euler(n)
        int result;
        result = (d * e) % Euler;
        if (result == 1) {
            break;
        }
    }
    //printf("%d %d", e, d);
    printf("공개키e, n (%d, %d)\n비밀키d, n (%d, %d)", e, n, d, n);

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
