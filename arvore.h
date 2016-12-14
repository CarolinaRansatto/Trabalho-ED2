#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Curriculo {
    // carga hor�ria total, n� m�ximo de per�odos, tempo nominal do curso
    int cht, ntotper, tnc;
} TC;

typedef struct Aluno {
    // matr�cula, carga hor�ria cursada com sucesso,
    // n� de per�odos na universidade, n� de trancamentos
    int mat, chcs, npu, ntran;
    char nome[51];
    float cr;
    TC* cur; // ponteiro pro curr�culo
} TA;

typedef struct Arvore {
    // se � folha, folha = 1
    int folha, *mats, nmats; // *mats pros n�s internos...
    TA** alunos; // ...e **alunos pras folhas?
    struct Arvore **filhos, *prox, *ant;
    // por que usar duas estruturas diferentes:
    // se usar s� alunos, quando liberar um ponteiro vai dar ruim,
    // porque os n�s podem ter infos que n�o existem
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
