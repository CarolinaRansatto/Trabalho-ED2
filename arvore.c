#include "arvore.h"

TABM* inicializa() {
    return NULL;
}

TABM* cria_no(int t) {
    TABM* a = (TABM*) malloc(sizeof(TABM));
    a->alunos = (TA**) malloc(sizeof(TA*) * (2 * t - 1)); // deveria deixar nulo quando virar nó interno?
    a->ant = NULL;
    a->filhos = (TABM**) malloc(sizeof(TABM*) * (2 * t));
    a->folha = 1;
    a->mats = (int*) malloc(sizeof(int) * (2 * t - 1));
    a->nmats = 0;
    a->prox = NULL;
    return a;
}

TABM* insere(TABM* a, TA* aln, int t) {
    if (!a) {
        a = cria_no(t);
        a->alunos[0] = aln;
        ++a->nmats;
        return a;
    }
}
TABM* retira(TABM* a, int mat, int t);

TA* busca(TABM* a, int mat) {
    // no método que ela deu retornava a folha em que deveria estar.
    // foi uma boa ideia mudar?
    if (!a) return NULL;
    int i = 0;
    while ((i < a->nmats) && (mat > a->mats[i])) i++;
    if ((i < a->nmats) && (a->folha) && (mat == a->alunos[i]->mat))
        return a->alunos[i];
    if (a->folha) return NULL;
    if (a->mats[i] == mat) i++;
    return busca(a->filhos[i], mat);
}

void libera(TABM* a) {
    if (a) {
        int i;
        if (!a->folha) {
            for (i = 0; i <= a->nmats; i++)
                libera(a->filhos[i]);
        }
        for (i = 0; i < a->nmats; i++)
            libera(a->alunos[i]);
        free(a->alunos);
        free(a->filhos);
        free(a->mats);
        free(a);
    }
}

void imprime(TABM* a);

void altera_chcs(TABM* a, int mat, int chcs) {
    TA* aln = busca(a, mat);
    if (aln) aln->chcs = chcs;
    // devíamos retornar um int pra saber se foi bem sucedido ou não?
}

void altera_cr(TABM* a, int mat, float cr) {
    TA* aln = busca(a, mat);
    if (aln) aln->cr = cr;
}

void altera_npu(TABM* a, int mat, int npu) {
    TA* aln = busca(a, mat);
    if (aln) aln->npu = npu;
}

void altera_ntran(TABM* a, int mat, int ntran) {
    TA* aln = busca(a, mat);
    if (aln) aln->ntran = ntran;
    // devíamos fazer um método pra só somar um a ntran?
}

TABM* retira_formandos(TABM* a);
TABM* retira_alunos_tnc(TABM* a);
TABM* retira_alunos_ntotper(TABM* a);
