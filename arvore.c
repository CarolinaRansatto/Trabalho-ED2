TABM* inicializa() {
    return NULL;
}

TABM* cria_no() {
    TABM* a = (TABM*) malloc(sizeof(Arvore));
    a->alunos = NULL;
    a->ant = NULL;
    a->filhos = NULL;
    a->folha = 1;
    a->mats = NULL;
    a->nmats = 0;
    a->prox = NULL;
    return a;
}

TABM* insere(TABM* a, TA* aln);
TABM* retira(TABM* a, int mat);

TA* busca(TABM* a, int mat) {
    if (!a) return NULL;
    int i = 0;
    while ((i < a->nmats) && (mat > a->mats[i])) i++;
    if ((i < a->nmats) && (a->folha) && (mat == a->alunos[i]->mat))
        return a->alunos[i];
    if (a->folha) return NULL;
    if (a->mat[i] == mat) i++;
    return busca(a->filhos[i], mat);
}

void libera(TABM* a);
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
