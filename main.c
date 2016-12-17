#include <stdio.h>
#include <stdlib.h>
#include "arvore.c"

int main()
{
    int opcao = 0;
    printf("1 para inserir, -1 para sair\n");
    scanf("%d",&opcao);
    int mat,chcs,npu,ntran,cur;
    float cr;
    char nome[51];
    TA* aluno = (TA*) malloc(sizeof(TA));
    while(opcao != -1){
        switch(opcao){

            case 1:
                printf("Insira a matricula, cargas horarias cursadas com sucesso, numero de periodos cursados, trancamentos, cr, nome e numero do curriculo\n");
                scanf("%d %f %d %d %d %d %[^\n]%*c", &mat,&cr,&ntran,&chcs,&npu,&cur,&nome);
                aluno = cria_aluno(mat, chcs, npu, ntran, nome, cr, cur);
                //printf("%d %f %d %d %d %d %s\n", mat,cr,ntran,chcs,npu,cur,nome);
                printf("%d %f %s", aluno->mat, aluno->cr, aluno->nome);
                printf("1 para inserir, -1 para sair\n");
                scanf("%d",&opcao);
                break;
            default:
                printf("Opcao invalida!\n");
                printf("1 para inserir, -1 para sair\n");
                scanf("%d",&opcao);

        }
    }
    return 0;
}
