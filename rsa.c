#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>


char buffer[1024];
const int MAX_DIGITS = 50;
int i,j = 0;

struct public_key_class{
  long long modulus;
  long long exponent;
};

struct private_key_class{
  long long modulus;
  long long exponent;
};


long long gcd(long long a, long long b)
{
  long long c;
  while ( a != 0 ) {
    c = a; a = b%a;  b = c;
  }
  return b;
}


long long ExtEuclid(long long a, long long b)
{
 long long x = 0, y = 1, u = 1, v = 0, gcd = b, m, n, q, r;
 while (a!=0) {
   q = gcd/a; r = gcd % a;
   m = x-u*q; n = y-v*q;
   gcd = a; a = r; x = u; y = v; u = m; v = n;
   }
   return y;
}
static inline long long modmult(long long a,long long b,long long mod)
{
   
   if (a == 0 ){
         return 0;
   }
   register long long product = a * b;
    
   if (product / a == b){
          return product % mod;
   }
   
   if ( a & 1 ) {
         product = modmult((a>>1), b, mod);
         if ((product << 1) > product ){
         return ((( product << 1 ) % mod ) + b) % mod;
      }
   }
   
   product = modmult((a >> 1), b, mod);
   if ((product << 1) > product){
         return (product << 1) % mod ;
         }
  
    long long sum;
    sum = 0;
    while(b>0)
    {
        if(b&1)
            sum = (sum + a) % mod;
        a = (2*a) % mod;
        b>>=1;
    }
    return sum;
}
long long rsa_modExp(long long b, long long e, long long m)
{
      long long product;
      product = 1;
      if (b < 0 || e < 0 || m <= 0){
            return -1;
      }
      b = b % m;
      while ( e > 0){
            if (e & 1){
                  product = modmult(product, b, m);
            }
            b = modmult(b, b, m);
            e >>= 1;
      }
      return product;
}


long long *rsa_encrypt(const char *message, const unsigned long message_size,
                     const struct public_key_class *pub)
{
  long long *encrypted = malloc(sizeof(long long)*message_size);
  if(encrypted == NULL){
    fprintf(stderr,
     "Error: Heap allocation failed.\n");
    return NULL;
  }
  long long i = 0;
  for(i=0; i < message_size; i++){
    if ((encrypted[i] = rsa_modExp(message[i], pub->exponent, pub->modulus)) == -1)
    return NULL;
  }
  return encrypted;
}


char *rsa_decrypt(const long long *message,
                  const unsigned long message_size,
                  const struct private_key_class *priv)
{
  if(message_size % sizeof(long long) != 0){
    fprintf(stderr,
     "Error: message_size is not divisible by %d, so cannot be output of rsa_encrypt\n", (int)sizeof(long long));
     return NULL;
  }
  
 
  char *decrypted = malloc(message_size/sizeof(long long));
  char *temp = malloc(message_size);
  if((decrypted == NULL) || (temp == NULL)){
    fprintf(stderr,
     "Error: Heap allocation failed.\n");
    return NULL;
  }
  
  long long i = 0;
  for(i=0; i < message_size/8; i++){
    if ((temp[i] = rsa_modExp(message[i], priv->exponent, priv->modulus)) == -1){
          free(temp);
          return NULL;
      }
  }
  
  for(i=0; i < message_size/8; i++){
    decrypted[i] = temp[i];
  }
  free(temp);
  return decrypted;
}
