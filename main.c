#include <stdio.h>
#include <stdlib.h>
#include "arvore.c"

#define T 2

TABM* insere_arquivo(char* narq, TABM* a, int t, TC** curs);

int main()
{
    int opcao = 0;
    printf("1 para inserir manualmente, 2 para inserir de arquivo, 3 para remover, 4 para imprimir, 5 para remover formandos, 6 para remover os que violem a regra do 50%, 7 para remover os que violem a regra de tempo maximo, -1 para sair\n");
    scanf("%d",&opcao);
    int mat,chcs,npu,ntran,cur;
    float cr;
    char nome[51];
    TA* aluno;
    TABM* arvore = inicializa();

    TC* curs[3];
    curs[0] = cria_curriculo(2955, 16, 8);
    curs[1] = cria_curriculo(3524, 12, 8);
    curs[2] = cria_curriculo(3200, 12, 8);

    while(opcao != -1){
        switch(opcao){

            case 1:
                printf("Insira a matricula, cr, numero de trancamentos, cargas horarias cursadas com sucesso, numero de periodos na universidade, numero do curriculo e nome\n");
                printf("Digite -1 para parar de inserir\n");
                scanf("%d", &mat);
                while (mat != -1) {
                    scanf("%f %d %d %d %d %[^\n]%*c", &cr,&ntran,&chcs,&npu,&cur,&nome);
                    aluno = cria_aluno(mat, chcs, npu, ntran, nome, cr, curs[cur - 1]);
                    arvore = insere(arvore, aluno, T);
                    //printf("%d %f %s\n", aluno->mat, aluno->cr, aluno->nome);
                    scanf("%d", &mat);
                }
                //printf("%d %f %d %d %d %d %s\n", mat,cr,ntran,chcs,npu,cur,nome);

			    printf("\n1 para inserir manualmente, 2 para inserir de arquivo, 3 para remover, 4 para imprimir, 5 para remover formandos, 6 para remover os que violem a regra do 50%, 7 para remover os que violem a regra de tempo maximo, -1 para sair\n");
                scanf("%d",&opcao);
                break;
			case 2:
				printf("Digite o nome do arquivo: ");
				char narq[51];
				scanf("%s", narq);
				arvore = insere_arquivo(narq, arvore, T, curs);
			    printf("\n1 para inserir manualmente, 2 para inserir de arquivo, 3 para remover, 4 para imprimir, 5 para remover formandos, 6 para remover os que violem a regra do 50%, 7 para remover os que violem a regra de tempo maximo, -1 para sair\n");
                scanf("%d",&opcao);
				break;
            case 3:
				//remover
			    printf("\n1 para inserir manualmente, 2 para inserir de arquivo, 3 para remover, 4 para imprimir, 5 para remover formandos, 6 para remover os que violem a regra do 50%, 7 para remover os que violem a regra de tempo maximo, -1 para sair\n");
                scanf("%d",&opcao);
                break;
            case 4:
                imprime(arvore, 0);
			    printf("\n1 para inserir manualmente, 2 para inserir de arquivo, 3 para remover, 4 para imprimir, 5 para remover formandos, 6 para remover os que violem a regra do 50%, 7 para remover os que violem a regra de tempo maximo, -1 para sair\n");
                scanf("%d",&opcao);
                break;
            case 5:
                arvore = retira_formandos(arvore, 2);
			    printf("\n1 para inserir manualmente, 2 para inserir de arquivo, 3 para remover, 4 para imprimir, 5 para remover formandos, 6 para remover os que violem a regra do 50%, 7 para remover os que violem a regra de tempo maximo, -1 para sair\n");
                scanf("%d",&opcao);
                break;
            case 6:
                arvore = retira_alunos_tnc(arvore, 2);
			    printf("\n1 para inserir manualmente, 2 para inserir de arquivo, 3 para remover, 4 para imprimir, 5 para remover formandos, 6 para remover os que violem a regra do 50%, 7 para remover os que violem a regra de tempo maximo, -1 para sair\n");
                scanf("%d",&opcao);
                break;
            case 7:
                arvore = retira_alunos_ntotper(arvore, 2);
			    printf("\n1 para inserir manualmente, 2 para inserir de arquivo, 3 para remover, 4 para imprimir, 5 para remover formandos, 6 para remover os que violem a regra do 50%, 7 para remover os que violem a regra de tempo maximo, -1 para sair\n");
                scanf("%d",&opcao);
                break;
            default:
                printf("Opcao invalida!\n");
			    printf("\n1 para inserir manualmente, 2 para inserir de arquivo, 3 para remover, 4 para imprimir, 5 para remover formandos, 6 para remover os que violem a regra do 50%, 7 para remover os que violem a regra de tempo maximo, -1 para sair\n");
                scanf("%d",&opcao);
                break;
        }
    }

    free(curs[0]);
    free(curs[1]);
    free(curs[2]);

    return 0;
}

TABM* insere_arquivo(char* narq, TABM* a, int t, TC** curs) {
	FILE* fp = fopen(narq, "rt");
	if (!fp) {
		printf("O arquivo não existe.\n");
		return a;
	}
	
	int mat, chcs, npu, ntran, cur;
    float cr;
    char nome[51];
    TA* aluno;
	int r = fscanf(fp, "%d %f %d %d %d %d %[^\n]%*c", &mat, &cr, &ntran, &chcs, &npu, &cur, &nome);
	printf("%d", r);

	while (r == 7) {
		aluno = cria_aluno(mat, chcs, npu, ntran, nome, cr, curs[cur - 1]);
		a = insere(a, aluno, t);
		r = fscanf(fp, "%d %f %d %d %d %d %[^\n]%*c", &mat, &cr, &ntran, &chcs, &npu, &cur, &nome);
	}
	
	printf("Arquivo lido com sucesso.\n");
	return a;
}
