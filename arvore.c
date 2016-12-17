#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

TABM* inicializa() {
    return NULL;
}

TABM* cria_no(int t) {
    TABM* a = (TABM*) malloc(sizeof(TABM));
    a->alunos = (TA**) malloc(sizeof(TA*) * (2 * t - 1));
    int i;
    for (i = 0; i < 2 * t - 1; i++)
        a->alunos[i] = NULL;
    a->ant = NULL;
    a->filhos = (TABM**) malloc(sizeof(TABM*) * (2 * t));
    for (i = 0; i < 2 * t; i++)
        a->filhos[i] = NULL;
    a->folha = 1;
    a->mats = (int*) malloc(sizeof(int) * (2 * t - 1));
    a->nmats = 0;
    a->prox = NULL;
    return a;
}

TC* cria_curriculo(int cht, int ntotper, int tnc) {
    TC* cur = (TC*) malloc(sizeof(TC));
    cur->cht = cht;
    cur->ntotper = ntotper;
    cur->tnc = tnc;
    return cur;
}

TA* cria_aluno(int mat, int chcs, int npu, int ntran, char *nome, float cr, TC* curriculo){
    TA* aluno = (TA*) malloc(sizeof(TA));
    aluno->mat = mat;
    aluno->chcs = chcs;
    aluno->npu = npu;
    aluno->ntran = ntran;
    strcpy(aluno->nome, nome);
    aluno->cr = cr;
    aluno->cur = curriculo;
    return aluno;
}

TABM* divisao(TABM* x, int i, TABM* y, int t) {
    TABM* z = cria_no(t);
    z->folha = y->folha;
    int j;

    if (z->folha) {
        z->nmats = t; // vai ter mais chaves no n� da direita
        for (j = 0; j < t; j++) {
            z->mats[j] = y->mats[j + t - 1];
            z->alunos[j] = y->alunos[j + t - 1];
        }
        z->prox = y->prox;
        y->prox = z;
        z->ant = y;
    }
    else { // pra n� interno funciona como �rvore b
        z->nmats = t - 1;
        for (j = 0; j < t - 1; j++)
            z->mats[j] = y->mats[j + t];
        for (j = 0; j < t; j++) {
            z->filhos[j] = y->filhos[j + t];
            y->filhos[j + t] = NULL;
        }
    }

    y->nmats = t - 1;
    for (j = x->nmats; j >= i; j--) {
        x->filhos[j + 1] = x->filhos[j];
        x->mats[j] = x->mats[j - 1];
    }
    x->filhos[i] = z;
    x->mats[i - 1] = y->mats[t - 1]; // igual � primeira matr�cula de z se z for folha, "promovida" se z for n� interno
    ++x->nmats;
    return x;
}

TABM* insere_nao_completo(TABM* a, TA* aln, int t) {
    int i = a->nmats - 1;
    if (a->folha) {
        while ((i >= 0) && (aln->mat < a->mats[i])) {
            a->mats[i + 1] = a->mats[i];
            a->alunos[i + 1] = a->alunos[i];
            --i;
        }
        a->mats[i + 1] = aln->mat;
        a->alunos[i + 1] = aln;
        ++a->nmats;
    }
    else {
        while ((i >= 0) && (aln->mat < a->mats[i])) i--;
        ++i;
        if (a->filhos[i]->nmats == 2 * t - 1) {
            a = divisao(a, i + 1, a->filhos[i], t);
            if (aln->mat > a->mats[i]) ++i;
        }
        a->filhos[i] = insere_nao_completo(a->filhos[i], aln, t);
    }
    return a;
}

TABM* insere(TABM* a, TA* aln, int t) {
    if (!a) {
        a = cria_no(t);
        a->alunos[0] = aln;
        a->mats[0] = aln->mat;
        a->prox = NULL;
        a->ant = NULL;
        ++a->nmats;
        return a;
    }
    if (a->nmats == (2 * t - 1)) {
        TABM* s = cria_no(t);
        s->folha = 0;
        s->filhos[0] = a;
        s = divisao(s, 1, a, t);
        s = insere_nao_completo(s, aln, t);
        return s;
    }
    a = insere_nao_completo(a, aln, t);
    return a;
}

TABM* retira(TABM* a, int mat, int t);

TA* busca(TABM* a, int mat) {
    // no m�todo que ela deu retornava a folha em que deveria estar.
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
            free(a->alunos[i]);
        free(a->alunos);
        free(a->filhos);
        free(a->mats);
        free(a);
    }
}

void imprime(TABM *a, int andar){
  if(a){
    int i, j;
    for(i = 0; i <= a->nmats-1; i++){
      imprime(a->filhos[i], andar + 1);
      for(j = 0; j <= andar; j++) printf("   ");
      printf("%d\n", a->mats[i]);
    }
    imprime(a->filhos[i], andar+1);
  }
}

void altera_chcs(TABM* a, int mat, int chcs) {
    TA* aln = busca(a, mat);
    if (aln) aln->chcs = chcs;
    // dev�amos retornar um int pra saber se foi bem sucedido ou n�o?
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
    // dev�amos fazer um m�todo pra s� somar um a ntran?
}

TABM* retira_formandos(TABM* a);
TABM* retira_alunos_tnc(TABM* a);
TABM* retira_alunos_ntotper(TABM* a);
