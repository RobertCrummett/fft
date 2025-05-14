#include <stdio.h>
#include <stdlib.h>

#define FOURIER_IMPLEMENTATION
#include "fourier.h"

#define COMPLEX_IMPLEMENTATION
#include "complex.h"

#include <math.h>
#include <stdio.h>

#define ASSERT(message, test) do { \
    if (!(test)) { \
        printf("\x1b[31mTEST FAILED:\x1b[0m %s\n", message); \
        return message; \
    } \
} while (0)

#define TEST(test) do { \
    printf("Running %s...\n", #test); \
    char *message = test(); \
    UNIT_TEST_COUNT++; \
    if (message) return message; \
} while (0)

int UNIT_TEST_COUNT = 0;

static char *test_fft(void);
static char *test_ifft(void);

static char *test_all(void) {
    TEST(test_fft);
    printf("\n");
    TEST(test_ifft);
    return 0;
}

int main(void) {
    char *result = test_all();
    if (result != 0)
        printf("%s\n", result);
    else
        printf("\x1b[32mALL TESTS PASSED\x1b[0m\n");

    printf("%d tests run\n", UNIT_TEST_COUNT);
    
    return result != 0;
}

static complex_t *auxillary_space(size_t size) {
    size_t larger_size = 1;
    while (larger_size >> 1 < size) larger_size <<= 1;
    
    complex_t* aux = calloc(size + 2 * larger_size + larger_size / 2, sizeof(*aux));
    if (aux == NULL) {
        fprintf(stderr, "No more room!\n");
        exit(-1);
    }
    return aux;
}

typedef struct test_group {
    complex_t *input;
    complex_t *expected;
    complex_t *auxillary;
    size_t size;
    size_t stride;
} test_group;

static int close(double a, double b) {
    return fabs(a-b) < 1e-8; 
}

static int complex_close(complex_t a, complex_t b) {
    return close(complex_real(a), complex_real(b)) && close(complex_imag(a), complex_imag(b));
}

static int group_close(test_group *group) {
    for (size_t i = 0; i < group->size; i++) {
        printf("    index [%zu]: input (%.3lf, %.3lf), expected (%.3lf, %.3lf)\n", i, complex_real(group->input[i]), complex_imag(group->input[i]), complex_real(group->expected[i]), complex_imag(group->expected[i]));
        if (!complex_close(group->expected[i], group->input[i]))
            return 0;
    }
    return 1;
}

static void group_free(test_group *group) {
    if (group == NULL)
        return;

    if (group->input != NULL)
        free(group->input);
    if (group->expected != NULL)
        free(group->expected);
    if (group->auxillary != NULL)
        free(group->auxillary);
}

static void setup_test_group_one(test_group *group);
static void setup_test_group_two(test_group *group);
static void setup_test_group_three(test_group *group);
static void setup_test_group_four(test_group *group);

static char *test_fft(void) {
    test_group group = {0};

    printf("GROUP 1:\n");
    setup_test_group_one(&group);
    fourier_fft(group.input, group.auxillary, group.size, group.stride);
    ASSERT("error, input != expected", group_close(&group));
    group_free(&group);

    printf("GROUP 2:\n");
    setup_test_group_two(&group);
    fourier_fft(group.input, group.auxillary, group.size, group.stride);
    ASSERT("error, input != expected", group_close(&group));
    group_free(&group);

    printf("GROUP 3:\n");
    setup_test_group_three(&group);
    fourier_fft(group.input, group.auxillary, group.size, group.stride);
    ASSERT("error, input != expected", group_close(&group));
    group_free(&group);

    printf("GROUP 4:\n");
    setup_test_group_four(&group);
    fourier_fft(group.input, group.auxillary, group.size, group.stride);
    ASSERT("error, input != expected", group_close(&group));
    group_free(&group);

    return 0;
}

static char *test_ifft(void) {
    test_group group = {0};

    printf("GROUP 1:\n");
    setup_test_group_one(&group);
    fourier_ifft(group.expected, group.auxillary, group.size, group.stride);
    for (size_t i = 0; i < group.size; i++)
        group.expected[i] = complex_div(group.expected[i], (complex_t){group.size, 0.0});
    ASSERT("error, input != expected", group_close(&group));
    group_free(&group);

    printf("GROUP 2:\n");
    setup_test_group_two(&group);
    fourier_ifft(group.expected, group.auxillary, group.size, group.stride);
    for (size_t i = 0; i < group.size; i++)
        group.expected[i] = complex_div(group.expected[i], (complex_t){group.size, 0.0});
    ASSERT("error, input != expected", group_close(&group));
    group_free(&group);

    printf("GROUP 3:\n");
    setup_test_group_three(&group);
    fourier_ifft(group.expected, group.auxillary, group.size, group.stride);
    for (size_t i = 0; i < group.size; i++)
        group.expected[i] = complex_div(group.expected[i], (complex_t){group.size, 0.0});
    ASSERT("error, input != expected", group_close(&group));
    group_free(&group);

    printf("GROUP 4:\n");
    setup_test_group_four(&group);
    fourier_ifft(group.expected, group.auxillary, group.size, group.stride);
    for (size_t i = 0; i < group.size; i++)
        group.expected[i] = complex_div(group.expected[i], (complex_t){group.size, 0.0});
    ASSERT("error, input != expected", group_close(&group));
    group_free(&group);

    return 0;
}

static void setup_test_group_one(test_group *group) {
    group->size = 8;
    group->stride = 1;

    group->input = malloc(group->size * sizeof *group->input);
    if (group->input == NULL) {
        fprintf(stderr, "No more room!\n");
        exit(-1);
    }
    group->input[0] = (complex_t){1.0, 0.0};
    group->input[1] = (complex_t){2.0, 0.0};
    group->input[2] = (complex_t){3.0, 0.0};
    group->input[3] = (complex_t){4.0, 0.0};
    group->input[4] = (complex_t){5.0, 0.0};
    group->input[5] = (complex_t){6.0, 0.0};
    group->input[6] = (complex_t){7.0, 0.0};
    group->input[7] = (complex_t){8.0, 0.0};

    group->expected = malloc(group->size * sizeof *group->expected);
    if (group->expected == NULL) {
        fprintf(stderr, "No more room!\n");
        exit(-1);
    }
    group->expected[0] = (complex_t){36.0, 0.0};
    group->expected[1] = (complex_t){-4.0, 9.6568542494923};
    group->expected[2] = (complex_t){-4.0, 4.0};
    group->expected[3] = (complex_t){-4.0, 1.6568542494923};
    group->expected[4] = (complex_t){-4.0, 0.0};
    group->expected[5] = (complex_t){-4.0, -1.6568542494923};
    group->expected[6] = (complex_t){-4.0, -4.0};
    group->expected[7] = (complex_t){-4.0, -9.6568542494923};

    group->auxillary = auxillary_space(group->size);
}

static void setup_test_group_two(test_group *group) {
    group->size = 8;
    group->stride = 1;

    group->input = malloc(group->size * sizeof *group->input);
    if (group->input == NULL) {
        fprintf(stderr, "No more room!\n");
        exit(-1);
    }
    group->input[0] = (complex_t){1.544211895503951, -0.192418510588264};
    group->input[1] = (complex_t){0.085931133175425, 0.888610425420721};
    group->input[2] = (complex_t){-1.491590310637609, -0.764849236567874};
    group->input[3] = (complex_t){-0.742301837259857, -1.402268969338759};
    group->input[4] = (complex_t){-1.061581733319986, -1.422375925091496};
    group->input[5] = (complex_t){2.350457224002042, 0.488193909859941};
    group->input[6] = (complex_t){-0.615601881466894, -0.177375156618825};
    group->input[7] = (complex_t){0.748076783703985, -0.196053487807333};

    group->expected = malloc(group->size * sizeof *group->expected);
    if (group->expected == NULL) {
        fprintf(stderr, "No more room!\n");
        exit(-1);
    }
    group->expected[0] = (complex_t){0.817601273701057, -2.778536950731890};
    group->expected[1] = (complex_t){0.901128710185560, 5.897124808105485};
    group->expected[2] = (complex_t){5.564949146715222, -3.103183453226400};
    group->expected[3] = (complex_t){3.170886721240411, 1.873027189733686};
    group->expected[4] = (complex_t){-4.066725333542135, -2.335500707001029};
    group->expected[5] = (complex_t){3.135510387564217, -1.685233120757589};
    group->expected[6] = (complex_t){-0.385304438138284, 1.758043368240278};
    group->expected[7] = (complex_t){3.215648696305561, -1.165089219068650};

    group->auxillary = auxillary_space(group->size);
}

static void setup_test_group_three(test_group *group) {
    group->size = 10;
    group->stride = 1;

    group->input = malloc(group->size * sizeof *group->input);
    if (group->input == NULL) {
        fprintf(stderr, "No more room!\n");
        exit(-1);
    }
    group->input[0] = (complex_t){ 1.419310150642549,-1.147952778898594};
    group->input[1] = (complex_t){ 0.291584373984183, 0.104874716016494};
    group->input[2] = (complex_t){ 0.197811053464361, 0.722254032225002};
    group->input[3] = (complex_t){ 1.587699089974059, 2.585491252616241};
    group->input[4] = (complex_t){-0.804465956349547,-0.666890670701386};
    group->input[5] = (complex_t){ 0.696624415849607, 0.187331024578940};
    group->input[6] = (complex_t){ 0.835088165072682,-0.082494425370955};
    group->input[7] = (complex_t){-0.243715140377952,-1.933022917850987};
    group->input[8] = (complex_t){ 0.215670086403744,-0.438966153934773};
    group->input[9] = (complex_t){-1.165843931482049,-1.794678841455123};

    group->expected = malloc(group->size * sizeof *group->expected);
    if (group->expected == NULL) {
        fprintf(stderr, "No more room!\n");
        exit(-1);
    }

    group->expected[0] = (complex_t){ 3.029762307181637, -2.464054762775142};
    group->expected[1] = (complex_t){ 5.877858269717315, -3.827924434589892};
    group->expected[2] = (complex_t){ 0.822425918752895, -4.329845121538964};
    group->expected[3] = (complex_t){-0.332572995260294, +0.493187865017670};
    group->expected[4] = (complex_t){ 7.994543480821431, -2.277235633792666};
    group->expected[5] = (complex_t){ 0.697064691285941, -0.764045230586272};
    group->expected[6] = (complex_t){-1.311467284010494, +4.881012335402681};
    group->expected[7] = (complex_t){ 3.842777324937977, -1.985198021598392};
    group->expected[8] = (complex_t){ 0.044408409715314, -0.612985588894181};
    group->expected[9] = (complex_t){-6.471698616716228, -0.592439195630783};

    group->auxillary = auxillary_space(group->size);
}

static void setup_test_group_four(test_group *group) {
    group->size = 11;
    group->stride = 1;

    group->input = malloc(group->size * sizeof *group->input);
    if (group->input == NULL) {
        fprintf(stderr, "No more room!\n");
        exit(-1);
    }
    group->input[0] = (complex_t){ 1.419310150642549,-1.147952778898594};
    group->input[1] = (complex_t){ 0.291584373984183, 0.104874716016494};
    group->input[2] = (complex_t){ 0.197811053464361, 0.722254032225002};
    group->input[3] = (complex_t){ 1.587699089974059, 2.585491252616241};
    group->input[4] = (complex_t){-0.804465956349547,-0.666890670701386};
    group->input[5] = (complex_t){ 0.696624415849607, 0.187331024578940};
    group->input[6] = (complex_t){ 0.835088165072682,-0.082494425370955};
    group->input[7] = (complex_t){-0.243715140377952,-1.933022917850987};
    group->input[8] = (complex_t){ 0.215670086403744,-0.438966153934773};
    group->input[9] = (complex_t){-1.165843931482049,-1.794678841455123};
    group->input[10] = (complex_t){1.0, 2.0};

    group->expected = malloc(group->size * sizeof *group->expected);
    if (group->expected == NULL) {
        fprintf(stderr, "No more room!\n");
        exit(-1);
    }

    group->expected[0] = (complex_t){ 4.029762307181637, - 0.464054762775142};
    group->expected[1] = (complex_t){ 6.355294569413763, - 0.278904477557993};
    group->expected[2] = (complex_t){ 0.224283438736181, - 1.802183883781256};
    group->expected[3] = (complex_t){-3.419016130292236, + 0.958576434771673};
    group->expected[4] = (complex_t){ 1.693236876381317, + 3.145606296850946};
    group->expected[5] = (complex_t){-2.962266441009343, - 7.043485920886038};
    group->expected[6] = (complex_t){-1.975110187255135, - 6.097282841362100};
    group->expected[7] = (complex_t){ 6.047517507123803, + 0.794775149177677};
    group->expected[8] = (complex_t){ 5.476258986843832, - 4.523905887686367};
    group->expected[9] = (complex_t){ 4.370097962890453, - 0.543101380704051};
    group->expected[10] = (complex_t){-4.227647232946229, + 3.226480706068112};

    group->auxillary = auxillary_space(group->size);
}
