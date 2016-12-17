#include <stdio.h>
#include <stdlib.h>
#include "arvore.c"

int main()
{
    int opcao = 0;
    printf("1 para inserir, 2 para imprimir, -1 para sair\n");
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
                    arvore = insere(arvore, aluno, 2);
                    //printf("%d %f %s\n", aluno->mat, aluno->cr, aluno->nome);
                    scanf("%d", &mat);
                }
                //printf("%d %f %d %d %d %d %s\n", mat,cr,ntran,chcs,npu,cur,nome);
                printf("\n1 para inserir, 2 para imprimir, -1 para sair\n");
                scanf("%d",&opcao);
                break;
            case 2:
                imprime(arvore, 0);
                printf("\n1 para inserir, 2 para imprimir, -1 para sair\n");
                scanf("%d",&opcao);
                break;
            default:
                printf("Opcao invalida!\n");
                printf("\n1 para inserir, 2 para imprimir, -1 para sair\n");
                scanf("%d",&opcao);

        }
    }

    free(curs[0]);
    free(curs[1]);
    free(curs[2]);

    return 0;
}
