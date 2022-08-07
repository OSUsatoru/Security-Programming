#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <openssl/sha.h>

const int TRIAL = 100;

const int WORD_LEN = 7;

// hex = 16 bits = 2 bytes
//

// 24 bits = 3 bytes = 6 hex charactors . or
// 24 bits = 3 bytes = 3 hex charactors .
const int HASH_LEN = 6;
char char_set[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; //62 + 1 (0 to 61)


void hash_function(char *text, char *str_hash);
int one_way_collision();
int collision_free();

void rand_text(char *text) {

    for (int i = 0; i < WORD_LEN; i++) {
        text[i] = char_set[rand() % strlen(char_set)];
    }
    text[WORD_LEN] = '\0';
}

int main(void){
    srand((unsigned int)time(NULL));

    //printf("%s\n", random_text);
    //printf("%d", strlen(random_text));

    /*
	unsigned char digest[SHA256_DIGEST_LENGTH];

	SHA256_CTX sha_ctx;
	SHA256_Init(&sha_ctx);
	SHA256_Update(&sha_ctx, message, sizeof(message));
	SHA256_Final(digest, &sha_ctx);

	for (int i = 0; i < sizeof(digest); ++i) {
		printf("%x", digest[i]);
	}
	printf("\n");
    */
    printf("One way-----------\n");
    float one_way_average = 0;
    for(int i = 0; i < TRIAL; ++i){
        printf("Trial: %d\n",i+1);
        one_way_average+=one_way_collision();
        if(i!=0)
            one_way_average*=.5;
    }


    printf("collision free---------------\n");
    float collision_average = 0;
    for(int i = 0; i < TRIAL; ++i){
        printf("Trial: %d\n",i+1);
        collision_average+=collision_free();
        if(i!=0)
            collision_average*=.5;
    }
    printf("one way average: %10.2lf\n", one_way_average);
    printf("collision free average: %10.2lf\n", collision_average);

}
void hash_function(char *text, char *str_hash){

    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha_ctx;
	SHA256_Init(&sha_ctx);
	SHA256_Update(&sha_ctx, text, sizeof(text));
	SHA256_Final(digest, &sha_ctx);

    for (int i = 0; i < HASH_LEN/2; ++i) {
        sprintf(&str_hash[i*2], "%02x", digest[i]);
    }
    //printf("digest: %s\n", str_hash);
    return;
}
int one_way_collision(){

    char message[] = "This is main massage";
    unsigned char main_hash[64];
    unsigned char second_hash[64];
    hash_function(message, main_hash);

    char random_text[WORD_LEN + 1];

    int cnt = 0;
    do{
        rand_text(random_text);
        //printf("%s", random_text);
        hash_function(random_text, second_hash);
        ++cnt;
    }while(strcmp(main_hash, second_hash)!=0);

    //printf("main: %s\n", main_hash);
    //printf("second: %s\n", second_hash);
    printf("count: %d\n", cnt);
    return cnt;
}
int collision_free(){
    char first_rand[WORD_LEN + 1];
    unsigned char first_hash[64];
    unsigned char second_hash[64];
    hash_function(first_rand, first_hash);

    char second_rand[WORD_LEN + 1];

    int cnt = 0;
    do{
        do{
            rand_text(first_rand);
            rand_text(second_rand);
        }while(strcmp(first_rand,second_rand) == 0);

        hash_function(first_rand, first_hash);
        hash_function(second_rand, second_hash);
        ++cnt;
    }while(strcmp(first_hash, second_hash)!=0);

    //printf("first: %s\n", first_hash);
    //printf("second: %s\n", second_hash);
    printf("count: %d\n", cnt);
    return cnt;
}

