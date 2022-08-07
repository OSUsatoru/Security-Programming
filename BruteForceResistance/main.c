
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <openssl/evp.h>

char word_file[] = "english_words.txt";
char plaintext[] = "This is a top secret.";
char ciphertext[] = "8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9";
unsigned char iv[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/**
 * remove last '\n' charactor
 *********************************/
void lntrim(char *str) {
  char *p;
  p = strchr(str, '\n');
  if(p != NULL) {
    *p = '\0';
    //str[strlen(str)-2] = '\0';
  }
}



int Encrypt(const char* key,  unsigned char* encoded_text){

    int f_len = 0;
    int p_len = strlen(plaintext);
    int e_len;
    //memset(encoded_text, 0x00, e_len);
    int enc_len;

    EVP_CIPHER_CTX ctx;

    // I explained some weird issue on my paper.
    /*
    EVP_CIPHER_CTX_init(&ctx);
    EVP_EncryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, (unsigned char*)key, iv);
    EVP_EncryptUpdate(&ctx, (unsigned char*)encoded_text, &e_len, (unsigned char *)plaintext, p_len);
    enc_len = e_len;
    EVP_EncryptFinal_ex(&ctx, (unsigned char *)(encoded_text + e_len), &e_len);
    enc_len += e_len;
    EVP_CIPHER_CTX_cleanup(&ctx);
    */
    EVP_CIPHER_CTX_init(&ctx);
    EVP_EncryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, key, iv);
    EVP_EncryptUpdate(&ctx, encoded_text, &e_len, plaintext, p_len);
    enc_len = e_len;
    EVP_EncryptFinal_ex(&ctx, (encoded_text + e_len), &e_len);
    enc_len += e_len;
    EVP_CIPHER_CTX_cleanup(&ctx);

    return enc_len;
}

int main(int argc, char *argv[])
{

    FILE *english_words = fopen(word_file, "r");
    // this is key
    char key[126];
    int word_len = 0;
    /* to see if all words are read */
    int count = 0;

    //char *currentWord = NULL;
    //size_t len = 0;
    //ssize_t nread;

    unsigned char encoded_text[100];
    int enc_len;

    while(fscanf(english_words, "%s\n", key) != EOF){
    //while ((nread = getline(&currentWord, &len, english_words)) != -1){
        //strcpy(key, currentWord);
        lntrim(key);
        word_len = strlen(key);
        if((word_len)<=16){

            for(int i = word_len; i < 16; ++i){
                key[i] = ' ';
            }
            key[16] = '\0';

            unsigned char enc_hex[100];

            enc_len = Encrypt(key, encoded_text);


            for(int i = 0; i < enc_len; ++i){
                sprintf(enc_hex+i*2, "%02x", encoded_text[i]);
            }
            enc_hex[enc_len*2]='\0';

            if (strcmp(enc_hex, ciphertext) == 0){
                printf("Key: %s\n", key);
            }
            //printf("%s\n%d\n", key, strlen(key));


        }
        count++;
    }
    //printf("%d\n",count);

    return 0;
}

