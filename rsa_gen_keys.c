//키 발급 함수

void rsa_gen_keys(struct public_key_class *pub, struct private_key_class *priv, char *PRIME_SOURCE_FILE)
{
  FILE *primes_list;
  if(!(primes_list = fopen(PRIME_SOURCE_FILE, "r"))){
    fprintf(stderr, "Problem reading %s\n", PRIME_SOURCE_FILE);
    exit(1);
  }

  // count number of primes in the list
  long long prime_count = 0;
  do{
    int bytes_read = fread(buffer,1,sizeof(buffer)-1, primes_list);
    buffer[bytes_read] = '\0';
    for (i=0 ; buffer[i]; i++){
      if (buffer[i] == '\n'){
	prime_count++;
      }
    }
  }
  while(feof(primes_list) == 0);


  // choose random primes from the list, store them as p,q

  long long p = 0;
  long long q = 0;

//values of e should be sufficiently large to protect against naive attacks
  long long e = (2 << 16) +1;
  long long d = 0;
  char prime_buffer[MAX_DIGITS];
  long long max = 0;
  long long phi_max = 0;

  srand(time(NULL));

  do{
    // a and b are the positions of p and q in the list
    int a =  (double)rand() * (prime_count+1) / (RAND_MAX+1.0);
    int b =  (double)rand() * (prime_count+1) / (RAND_MAX+1.0);

    // here we find the prime at position a, store it as p
    rewind(primes_list);
    for(i=0; i < a + 1; i++){
    //  for(j=0; j < MAX_DIGITS; j++){
    //	prime_buffer[j] = 0;
    //  }
      fgets(prime_buffer,sizeof(prime_buffer)-1, primes_list);
    }
    p = atol(prime_buffer);

    // here we find the prime at position b, store it as q
    rewind(primes_list);
    for(i=0; i < b + 1; i++){
      for(j=0; j < MAX_DIGITS; j++){
	prime_buffer[j] = 0;
      }
      fgets(prime_buffer,sizeof(prime_buffer)-1, primes_list);
    }
    q = atol(prime_buffer);

    max = p*q;
    phi_max = (p-1)*(q-1);
  }
  while(!(p && q) || (p == q) || (gcd(phi_max, e) != 1));

  // Next, we need to choose a,b, so that a*max+b*e = gcd(max,e). We actually only need b
  // here, and in keeping with the usual notation of RSA we'll call it d. We'd also like
  // to make sure we get a representation of d as positive, hence the while loop.
  d = ExtEuclid(phi_max,e);
  while(d < 0){
    d = d+phi_max;
  }

  //printf("primes are %lld and %lld\n",(long long)p, (long long )q);
  // We now store the public / private keys in the appropriate structs
  pub->modulus = max;
  pub->exponent = e;

  priv->modulus = max;
  priv->exponent = d;
}
