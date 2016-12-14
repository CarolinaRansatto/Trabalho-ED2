#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Curriculo {
    // carga horária total, nº máximo de períodos, tempo nominal do curso
    int cht, ntotper, tnc;
} TC;

typedef struct Aluno {
    // matrícula, carga horária cursada com sucesso,
    // nº de períodos na universidade, nº de trancamentos
    int mat, chcs, npu, ntran;
    char nome[51];
    float cr;
    TC* cur; // ponteiro pro currículo
} TA;

typedef struct Arvore {
    // se é folha, folha = 1
    int folha, *mats, nmats; // *mats pros nós internos...
    TA** alunos; // ...e **alunos pras folhas?
    struct Arvore **filhos, *prox, *ant;
    // por que usar duas estruturas diferentes:
    // se usar só alunos, quando liberar um ponteiro vai dar ruim,
    // porque os nós podem ter infos que não existem
} TABM;

TABM* inicializa();
TABM* cria_no();
TABM* insere(TABM* a, TA* aln);
TABM* retira(TABM* a, int mat);
TA* busca(TABM* a, int mat);
void libera(TABM* a);
void imprime(TABM* a);
void altera_chcs(TABM* a, int mat, int chcs);
void altera_cr(TABM* a, int mat, float cr);
void altera_npu(TABM* a, int mat, int npu);
void altera_ntran(TABM* a, int mat, int ntran);

TABM* retira_formandos(TABM* a);
TABM* retira_alunos_tnc(TABM* a);
TABM* retira_alunos_ntotper(TABM* a);
