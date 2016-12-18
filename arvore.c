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

TC* cria_curriculo(int num, int cht, int ntotper, int tnc) {
    TC* cur = (TC*) malloc(sizeof(TC));
    cur->num = num;
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
        z->nmats = t; // vai ter mais chaves no nó da direita
        for (j = 0; j < t; j++) {
            z->mats[j] = y->mats[j + t - 1];
            z->alunos[j] = y->alunos[j + t - 1];
        }
        z->prox = y->prox;
        y->prox = z;
        z->ant = y;
    }
    else { // pra nó interno funciona como árvore b
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
    x->mats[i - 1] = y->mats[t - 1]; // igual à primeira matrícula de z se z for folha, "promovida" se z for nó interno
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

TABM* busca(TABM* a, int mat) {
    if (!a) return NULL;
    int i = 0;
    while ((i < a->nmats) && (mat > a->mats[i])) i++;
    if ((i < a->nmats) && (a->folha) && (mat == a->alunos[i]->mat))
        return a;
    if (a->folha) return NULL;
    if (a->mats[i] == mat) i++;
    return busca(a->filhos[i], mat);
}

TA* busca_aluno(TABM* a, int mat) {
    TABM* b = busca(a, mat);
    if (!b) return NULL;
	int i = 0;
	while ((i < b->nmats) && (mat > b->mats[i])) i++;
    return b->alunos[i];
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

// devíamos retornar um int pra saber se as alterações foram bem sucedidas ou não?

int altera_chcs(TABM* a, int mat, int chcs) {
    TA* aln = busca_aluno(a, mat);
    if (!aln) return 1;
    aln->chcs = chcs;
    return 0;
}

int altera_cr(TABM* a, int mat, float cr) {
    TA* aln = busca_aluno(a, mat);
    if (!aln) return 1;
    aln->cr = cr;
    return 0;
}

int altera_npu(TABM* a, int mat, int npu) {
    TA* aln = busca_aluno(a, mat);
    if (!aln) return 1;
    aln->npu = npu;
    return 0;
}

int altera_ntran(TABM* a, int mat, int ntran) {
    TA* aln = busca_aluno(a, mat);
    if (!aln) return 1;
    aln->ntran = ntran;
    return 0;
}

TL* ins_ini(TL* l, int info) {
	TL* novo = (TL*) malloc(sizeof(TL));
	novo->info = info;
	novo->prox = l;
	return novo;
}

void libera_lista(TL* l) {
	while (l) {
		TL* a = l;
		l = l->prox;
		free(a);
	}
}

TABM* remover(TABM* a, int mat, int t){
	if(!a) return NULL;
	int i = 0;

	printf("Removendo %d...\n", mat);

	while ((i < a->nmats) && (mat > a->mats[i])) i++;
	if(!a->folha){
            if(mat == a->mats[i]) i++;
	}

	//printf("%d\n", i);

	if((i<a->nmats)&&(a->mats[i]==mat)&&(a->folha)){ //CASO 1 - funciona
		printf("\nCASO 1\n");
		int j;
		free(a->alunos[i]);
		for(j=i;j<a->nmats-1;j++){
			a->mats[j] = a->mats[j+1];
			a->alunos[j] = a->alunos[j+1];
		}
		a->alunos[j] = NULL;
		a->nmats--;
		return a;
	}
	else if (a->folha) return a;

	//if (i == a->nmats) --i;
	int f = i;
	//if(mat==a->mats[i]) ++f; //esse é o índice do filho, se for igual tem que ser um a mais que o da mat
	TABM* y = a->filhos[f], *z = NULL;
	if(y->nmats == t-1){  //CASO 3A OU 3B
		if((f < a->nmats) && (a->filhos[f+1]->nmats >=t)){ // CASO 3A
			printf("\nCASO 3A: i menor que nmats\n");
			z = a->filhos[f+1];
			int j;

			if(!y->folha){
				y->mats[y->nmats] = a->mats[i];
				a->mats[i] = z->mats[0];
				y->nmats++;

				y->filhos[y->nmats] = z->filhos[0]; //ajusta o ponteiro do filho do novo elemento de y

				for(j=0; j<z->nmats-1; j++){ //ajustar lugar dos filhos de z
					z->filhos[j] = z->filhos[j+1];
					z->mats[j] = z->mats[j+1];
				}

				z->filhos[j] = z->filhos[j+1]; //o numero de filhos é o numero de mats + 1

				z->filhos[z->nmats] = NULL;
			}
			else{
				y->mats[y->nmats] = z->mats[0];
				y->alunos[y->nmats] = z->alunos[0];
				a->mats[i] = z->mats[1];
				y->nmats++;

				for(j=0; j<z->nmats-1; j++){ //rearranjar as informações dos alunos
					z->mats[j] = z->mats[j+1];
					z->alunos[j] = y->alunos[j+1];
				}

				z->alunos[z->nmats-1] = NULL;
			}
			z->nmats--;

			a->filhos[f] = remover(a->filhos[f],mat,t);
			return a;
		}

		if((f > 0) && (a->filhos[f-1]->nmats >=t)){ //CASO 3A
			printf("\nCASO 3A: i igual a nmats\n"); //não mais
			z = a->filhos[f-1];
			int j;
			for(j=y->nmats; j>0; j--){ //encaixar lugar da nova mat
				y->mats[j] = y->mats[j-1];
			}

			if(!y->folha){
				y->mats[0] = a->mats[f-1]; //pega a mat que está no meio dos dois nós
				a->mats[f-1] = z->mats[z->nmats-1];
				y->nmats++;

				for(j=y->nmats; j>0; j--){ //encaixar lugar dos filhos da nova chave
					y->filhos[j] = y->filhos[j-1];
				}

				y->filhos[0] = z->filhos[z->nmats]; //ajusta o ponteiro do filho do novo elemento de y
				z->filhos[z->nmats] = NULL;

			}
			else{
				y->mats[0] = z->mats[z->nmats-1];
				a->mats[f-1] = z->mats[z->nmats-1];
				y->nmats++;

				for(j=y->nmats-1; j>0; j--){ //rearranjar as informações dos alunos
					y->alunos[j] = y->alunos[j-1];
				}

				y->alunos[0] = z->alunos[z->nmats-1]; //ajusta a informações de aluno do novo elemento de y
				z->alunos[z->nmats-1] = NULL;
			}
			z->nmats--;

			a->filhos[f] = remover(a->filhos[f],mat,t);
			return a;
		}

		if(f < a->nmats) {
			printf("CASO 3B i menor que nmats...\n");
			if(!y->folha){
				printf("no\n");
				z = a->filhos[f + 1];
				int j;
				y->mats[y->nmats] = a->mats[i];
				y->nmats++;

				for (j = 0; j < z->nmats; j++) { //copia os elementos de z
					y->mats[y->nmats + j] = z->mats[j];
					y->filhos[y->nmats + j] = z->filhos[j];
				}

				y->nmats += z->nmats;
				y->filhos[y->nmats] = z->filhos[z->nmats];

				for(j=0; j<=z->nmats; j++){ //desvincula z de seus filhos
					z->filhos[j] = NULL;
				}
				libera(z);                  //libera z

			}
			else{
				printf("folha\n");
				z = a->filhos[f + 1];
				int j;
				for (j = 0; j < z->nmats; j++) { //copia os elementos de z
					y->mats[y->nmats + j] = z->mats[j];
					y->alunos[y->nmats + j] = z->alunos[j];
				}

				y->nmats += z->nmats;
				for(j=0; j<y->nmats+1; j++) y->filhos[j] = NULL; //opcional (p/ ter certeza q os filhos de folha sao NULL)
				y->prox = z->prox;
				if (y->prox) y->prox->ant = y;

				for(j=0; j<z->nmats; j++) z->alunos[j] = NULL;
				libera(z);
			}

			if(a->nmats == 1){ //so se for a raiz da arvore original
				a->filhos[0] = NULL;
				a->filhos[1] = NULL;
				libera(a);
				a = y;
			}
			else{
                int j;
				for(j = f; j< a->nmats-1; j++){
					a->mats[j] = a->mats[j+1];
					a->filhos[j+1] = a->filhos[j+2];
				}
				a->filhos[j] = NULL; // num de filhos = num de mats + 1
				a->nmats--;
			}

			//a->filhos[f] = remover(a->filhos[f], mat, t);
			a = remover(a, mat, t);
		}
		else { // CASO 3B se y for o último filho
			printf("CASO 3B\n");
			if (!y->folha) {
				printf("no\n");
				z = a->filhos[f - 1];
				int j;
				z->mats[z->nmats] = a->mats[f - 1];
				z->nmats++;

				for (j = 0; j < z->nmats; j++) { //copia os elementos de y
					z->mats[z->nmats + j] = y->mats[j];
					z->filhos[z->nmats + j] = y->filhos[j];
				}

				z->nmats += y->nmats;
				z->filhos[z->nmats] = y->filhos[y->nmats]; //num filhos = nmats + 1

				for(j=0; j<y->nmats+1; j++) y->filhos[j] = NULL;
				libera(y);
			}
			else {
				printf("folha\n");
				z = a->filhos[f - 1];
				int j;
				for (j = 0; j < z->nmats; j++) { //copia os elementos de y
					z->mats[z->nmats + j] = y->mats[j];
					z->alunos[z->nmats + j] = y->alunos[j];
				}

				z->nmats += y->nmats;
				z->prox = y->prox;
				if (z->prox) z->prox->ant = z;

				for(j=0; j<y->nmats; j++) y->alunos[j] = NULL;
				libera(y);
			}

			if(a->nmats==1){   //so acontece se for a raiz original da arvore
                a->filhos[0] = NULL;
                a->filhos[1] = NULL;
                libera(a);
                a = z;
			}
			else{
                int j;
                for(j = f-1; j<a->nmats-1; j++){
                    a->mats[j] = a->mats[j+1];
                    a->filhos[j+1] = a->filhos[j+2];
                }
                a->filhos[a->nmats] = NULL;
                a->nmats--;
			}

            //a->filhos[f - 1] = remover(a->filhos[f - 1], mat, t);
            a = remover(a, mat, t);
		}
	}
	a->filhos[f] = remover(a->filhos[f], mat, t);
	return a;
}

TABM* retira(TABM* a, int mat, int t){
  if(!a || !busca(a, mat)) return a;
  return remover(a, mat, t);
}

TABM* retira_formandos(TABM* a, int t) {
    if(!a){
        printf("Arvore vazia!");
        return NULL;
    }
	TABM* b = a;
	while (!b->folha)
		b = b->filhos[0];
	TL* lista = NULL;
	printf("Removidos:");

	while (b) {
		int i;
		for (i = 0; i < b->nmats; i++)
			if (b->alunos[i]->chcs >= b->alunos[i]->cur->cht) {
				lista = ins_ini(lista, b->alunos[i]->mat);
				printf(" %d", b->alunos[i]->mat);
			}
		b = b->prox;
	}
	printf("\n");

	TL* l = lista;
	while (l) {
		a = retira(a, l->info, t);
		l = l->prox;
	}
	libera_lista(lista);
	return a;
}

TABM* retira_alunos_tnc(TABM* a, int t){
    if(!a){
        printf("Arvore vazia!");
        return NULL;
    }
    TABM *b = a;
    while(b->folha != 1){
        b = b->filhos[0];
    }
    int i;
    int npu, ntran, chcs, tnc, cht;
    TL *lista = NULL;
	printf("Removidos: ");
    while(1){
        for(i=0; i<b->nmats;i++){
            npu = b->alunos[i]->npu;
            ntran = b->alunos[i]->ntran;
            chcs = b->alunos[i]->chcs;
            tnc = b->alunos[i]->cur->tnc;
            cht = b->alunos[i]->cur->cht;
            if((npu-ntran)>= tnc){
               //chegou ao TNC(tempo nominal do curso)
               if(chcs < (cht/2)){
                    //nao atingiu 50% do cht
                    lista = ins_ini(lista, b->alunos[i]->mat);
                    printf("%d ", b->alunos[i]->mat);
               }
            }
        }
        if(!b->prox){
            break;
        }
        b = b->prox;
    }
    printf("\n");
	TL* l = lista;
	while (l) {
		a = retira(a, l->info, t);
		l = l->prox;
	}
    libera_lista(lista);
    return a;
}

TABM* retira_alunos_ntotper(TABM* a, int t){
    if(!a){
        printf("Arvore vazia!");
        return NULL;
    }
    TABM *b = a;
    while(b->folha != 1){
        b = b->filhos[0];
    }
    int i;
    int npu, ntran, ntotper;
    TL *lista = NULL;
	printf("Removidos: ");
    while(1){
        for(i=0; i<b->nmats;i++){
            npu = b->alunos[i]->npu;
            ntran = b->alunos[i]->ntran;
            ntotper = b->alunos[i]->cur->ntotper;
            if((npu-ntran)> ntotper ){
               //cursou mais períodos que o permitido
                lista = ins_ini(lista, b->alunos[i]->mat);
                printf("%d ", b->alunos[i]->mat);
            }
        }
        if(!b->prox){
            break;
        }
        b = b->prox;
    }
    printf("\n");
	TL* l = lista;
	while (l) {
		a = retira(a, l->info, t);
		l = l->prox;
	}
    libera_lista(lista);
    return a;
}

