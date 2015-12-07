#include "destools.h"

#include <iostream>

int pc1_table[] = { 57, 49, 41, 33, 25, 17,  9,  1,
                    58, 50, 42, 34, 26, 18, 10,  2,
                    59, 51, 43, 35, 27, 19, 11,  3,
                    60, 52, 44, 36, 63, 55, 47, 39,
                    31, 23, 15,  7, 62, 54, 46, 38,
                    30, 22, 14,  6, 61, 53, 45, 37,
                    29, 21, 13,  5, 28, 20, 12,  4,
                     8, 16, 24, 32, 40, 48, 56, 64};

int initial_permutation_table[] = { 58, 50, 42, 34, 26, 18, 10,  2,
                                    60, 52, 44, 36, 28, 20, 12,  4,
                                    62, 54, 46, 38, 30, 22, 14,  6,
                                    64, 56, 48, 40, 32, 24, 16,  8,
                                    57, 49, 41, 33, 25, 17,  9,  1,
                                    59, 51, 43, 35, 27, 19, 11,  3,
                                    61, 53, 45, 37, 29, 21, 13,  5,
                                    63, 55, 47, 39, 31, 23, 15,  7};

int final_permutation_table[] = {   40, 8, 48, 16, 56, 24, 64, 32,
                                    39, 7, 47, 15, 55, 23, 63, 31,
                                    38, 6, 46, 14, 54, 22, 62, 30,
                                    37, 5, 45, 13, 53, 21, 61, 29,
                                    36, 4, 44, 12, 52, 20, 60, 28,
                                    35, 3, 43, 11, 51, 19, 59, 27,
                                    34, 2, 42, 10, 50, 18, 58, 26,
                                    33, 1, 41,  9, 49, 17, 57, 25};

uint64_t permute(uint64_t input, int* permutation_table, unsigned output_length){
    uint64_t result = 0;
    for (int i = 0; i < output_length; ++i){
        uint64_t plaintext_bit = (input >> (permutation_table[i] - 1) ) & 0x1;
        result |= plaintext_bit << i;
    }
    return result;
}

uint64_t reverse_permute(uint64_t input, int* permutation_table, unsigned output_length){
    uint64_t result = 0;
    for (int i = 0; i < output_length; ++i){
        uint64_t current_bit = (input >> i) & 0x1;
        result |= current_bit << (permutation_table[i] - 1);
    }
    return result;
}

uint64_t initial_permutation(uint64_t plaintext){
    return permute(plaintext, initial_permutation_table, 64);
}

uint64_t initial_permutation_reverse(uint64_t intermediate_text){
    return reverse_permute(intermediate_text, initial_permutation_table, 64);
}

uint64_t final_permutation(uint64_t intermediate_text){
    return permute(intermediate_text, final_permutation_table, 64);
}

uint64_t final_permutation_reverse(uint64_t ciphertext){
    return reverse_permute(ciphertext, final_permutation_table, 64);
}

uint64_t key_initial_permutation(uint64_t key){
    return permute(key, pc1_table, 56);
}

uint64_t key_initial_permutation_reverse(uint64_t key_without_paritybits){
    uint64_t permute_result = reverse_permute(key_without_paritybits, pc1_table, 64);
    printf("before setting the parity bits: %016llx\n", permute_result);
    return key_set_parity_bits(permute_result);
}

uint64_t key_set_parity_bits(uint64_t key_with_wrong_paritybits){
    uint64_t result = key_with_wrong_paritybits & 0xFEFEFEFEFEFEFEFE;
    uint64_t parity_bit = 0;
    for (int i = 63; i > 0; --i){
        if(i % 8 == 0){
            result |= parity_bit << i;
            parity_bit = 0;
        } else {
            parity_bit ^= (key_with_wrong_paritybits >> i) & 0x1;
        }
    }
    return result;
}

int main(){
    printf("%016llx\n", initial_permutation(0xAAAAAAAAAAAAAAAA));
    printf("%016llx\n", initial_permutation_reverse(initial_permutation(0xAAAAAAAAAAAAAAAA)));
    printf("%016llx\n", final_permutation_reverse(final_permutation(0xAAAAAAAAAAAAAAAA)));
    printf("%016llx\n", key_initial_permutation_reverse(key_initial_permutation(0xF0F0F0F0F0F0F0F0)));
    printf("key initial permutation: %016llx\n", key_initial_permutation(0xA0B1C0D1E1F0F0F0));
    printf("key with wrong parity bits: %016lx\nkey with corrected parity bits: %016llx\n", 0xA1B1C1D1E1F1F0F1, key_set_parity_bits(0xA1B1C1D1E1F1F0F1));
    printf("%016llx\n", key_initial_permutation_reverse(key_initial_permutation(0xA0B1C0D1E1F0F0F0)));
}
