#include <stdio.h>
#include <stdlib.h>
#include "arvore.c"

TABM* insere_arquivo(char* narq, TABM* a, int t, TC** curs);

int main()
{
    int opcao = 0, t = 0;
    printf("Digite o valor de t: ");
    scanf("%d", &t);
    while (t < 2) {
	    printf("Valor invalido (t deve ser maior que 1).\nDigite o valor de t: ");
	    scanf("%d", &t);
	}
	
	char* instrucoes = " 0: Inserir alunos manualmente\n 1: Inserir alunos de arquivo\n 2: Buscar aluno\n 3: Remover aluno\n 4: Alterar CR\n 5: Alterar carga horaria\n 6: Alterar número de períodos na universidade\n 7: Alterar número de trancamentos\n 8: Imprimir arvore\n 9: Remover formandos\n10: Remover alunos que violem a regra dos 50%\n11: Remover alunos que violem a regra de tempo maximo\n-1: Sair\n";

    printf("%s", instrucoes);
    scanf("%d",&opcao);
    int mat,chcs,npu,ntran,cur,r;
    float cr;
    char nome[51];
    TA* aluno;
    TABM* arvore = inicializa();

    TC* curs[3];
    curs[0] = cria_curriculo(1, 2955, 16, 8);
    curs[1] = cria_curriculo(2, 3524, 12, 8);
    curs[2] = cria_curriculo(3, 3200, 12, 8);

    while(opcao != -1){
        switch(opcao){

            case 0:
                printf("Insira a matricula, cr, numero de trancamentos, carga horaria cursada com sucesso, numero de periodos na universidade, numero do curriculo e nome\n");
                printf("Digite -1 para parar de inserir\n");
                scanf("%d", &mat);
                while (mat != -1) {
                    scanf("%f %d %d %d %d %[^\n]%*c", &cr,&ntran,&chcs,&npu,&cur,&nome);
                    if ((cur > 3)||(cur < 1)) printf("Currículo %d inválido\n", cur);
                    else if (busca_aluno(arvore, mat)) printf("Matrícula %d já cadastrada\n", mat);
                    else {
	                    aluno = cria_aluno(mat, chcs, npu, ntran, nome, cr, curs[cur - 1]);
    	                arvore = insere(arvore, aluno, t);
                    }
                    //printf("%d %f %s\n", aluno->mat, aluno->cr, aluno->nome);
                    scanf("%d", &mat);
                }
                //printf("%d %f %d %d %d %d %s\n", mat,cr,ntran,chcs,npu,cur,nome);

			    printf("\n%s", instrucoes);
                scanf("%d",&opcao);
                break;
			case 1:
				printf("Digite o nome do arquivo: ");
				char narq[51];
				scanf("%s", narq);
				arvore = insere_arquivo(narq, arvore, t, curs);
			    printf("\n%s", instrucoes);
                scanf("%d",&opcao);
				break;
			case 2:
				printf("Digite a matrícula: ");
				scanf("%d", &mat);
				aluno = busca_aluno(arvore, mat);
				if (!aluno) printf("Aluno nao encontrado\n");
				else printf("Nome: %s\nMatricula: %d\nCR:%f\nPeriodos na universidade: %d\nNumero de trancamentos: %d\nCarga horaria cursada com sucesso: %d\nNumero do curriculo: %d\n",
					aluno->nome, aluno->mat, aluno->cr, aluno->npu, aluno->ntran, aluno->chcs, aluno->cur->num);
				printf("\n%s", instrucoes);
                scanf("%d",&opcao);
				break;
            case 3:
            	printf("Digite a matrícula do aluno a ser removido: ");
            	scanf("%d", &mat);
				arvore = retira(arvore, mat, t);
			    printf("\n%s", instrucoes);
                scanf("%d",&opcao);
                break;
            case 4:
				printf("Digite a matricula: ");
				scanf("%d", &mat);
				printf("Digite o novo CR: ");
				scanf("%f", &cr);
				r = altera_cr(arvore, mat, cr);
				if (r) printf("Aluno nao encontrado\n");
				else printf("Alterado com sucesso\n");
				printf("\n%s", instrucoes);
                scanf("%d",&opcao);
				break;
            case 5:
				printf("Digite a matricula: ");
				scanf("%d", &mat);
				printf("Digite a nova carga horaria: ");
				scanf("%d", &chcs);
				r = altera_chcs(arvore, mat, chcs);
				if (r) printf("Aluno nao encontrado\n");
				else printf("Alterado com sucesso\n");
				printf("\n%s", instrucoes);
                scanf("%d",&opcao);
				break;
			case 6:
				printf("Digite a matricula: ");
				scanf("%d", &mat);
				printf("Digite o numero de periodos na universidade: ");
				scanf("%d", &npu);
				r = altera_npu(arvore, mat, npu);
				if (r) printf("Aluno nao encontrado\n");
				else printf("Alterado com sucesso\n");
				printf("\n%s", instrucoes);
                scanf("%d",&opcao);
				break;
			case 7:
				printf("Digite a matricula: ");
				scanf("%d", &mat);
				printf("Digite o numero de trancamentos: ");
				scanf("%d", &ntran);
				r = altera_ntran(arvore, mat, ntran);
				if (r) printf("Aluno nao encontrado\n");
				else printf("Alterado com sucesso\n");
				printf("\n%s", instrucoes);
                scanf("%d",&opcao);
				break;
            case 8:
                imprime(arvore, 0);
			    printf("\n%s", instrucoes);
                scanf("%d",&opcao);
                break;
            case 9:
                arvore = retira_formandos(arvore, t);
			    printf("\n%s", instrucoes);
                scanf("%d",&opcao);
                break;
            case 10:
                arvore = retira_alunos_tnc(arvore, t);
			    printf("\n%s", instrucoes);
                scanf("%d",&opcao);
                break;
            case 11:
                arvore = retira_alunos_ntotper(arvore, t);
			    printf("\n%s", instrucoes);
                scanf("%d",&opcao);
                break;
            default:
                printf("Opcao invalida!\n");
			    printf("\n%s", instrucoes);
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

	while (r == 7) {
        if ((cur > 3)||(cur < 1)) printf("Currículo %d inválido\n", cur);
        else if (busca_aluno(a, mat)) printf("Matrícula %d já cadastrada\n", mat);
        else {
            aluno = cria_aluno(mat, chcs, npu, ntran, nome, cr, curs[cur - 1]);
            a = insere(a, aluno, t);
        }
		r = fscanf(fp, "%d %f %d %d %d %d %[^\n]%*c", &mat, &cr, &ntran, &chcs, &npu, &cur, &nome);
	}

	printf("Arquivo lido com sucesso.\n");
	return a;
}
