#include <stdio.h>
#include <stdlib.h>

#define MAX 3
#define MIN 2

typedef struct no{
    int chave[MAX + 1];
    int cont;
    struct no *no_filho[MAX + 1];
} No;

No *raiz;

// Criação do Nó
No* cria_no(int chave, No *filho){
    No *novo_no;
    novo_no = (No*) malloc(sizeof(No));

    novo_no->chave[1] = chave;
    novo_no->cont = 1;

    novo_no->no_filho[0] = raiz;
    novo_no->no_filho[1] = filho;

    return novo_no;
}

// Inserção de Chave
void inserir_chave(int chave, int posicao, No *no, No *filho){
    int j = no->cont;

    while(j > posicao){
        no->chave[j + 1] = no->chave[j];
        no->no_filho[j + 1] = no->no_filho[j];
        j--;
    }

    no->chave[j + 1] = chave;
    no->no_filho[j + 1] = filho;
    no->cont++;
}

// Split do nó
void split_no(int chave, int *p, int posicao, No *no, No *filho, No **novo_no){
    int media, j;

    if(posicao > MIN){
        media = MIN + 1;
    }
    else{
        media = MIN;
    }

    *novo_no = (No*) malloc(sizeof(No));
    j = media + 1;

    while(j <= MAX){
        (*novo_no)->chave[j - media] = no->chave[j];
        (*novo_no)->no_filho[j - media] = no->no_filho[j];
        j++;
    }

    no->cont = media;
    (*novo_no)->cont = MAX - media;

    if(posicao <= MIN){
        inserir_chave(chave, posicao, no, filho);
    }
    else{
        inserir_chave(chave, posicao - media, *novo_no, filho);
    }

    *p = no->chave[no->cont];
    (*novo_no)->no_filho[0] = no->no_filho[no->cont];
    no->cont--;
}

// Setar o valor do nó
int setar_valor_no(int chave, int *p, No *no, No **filho){
    int posicao;
    
    if(!no){
        *p = chave;
        *filho = NULL;
        return 1;
    }

    if(chave < no->chave[1]){
        posicao = 0;
    }
    else{
        for(posicao = no->cont; chave < no->chave[posicao] && posicao > 1; posicao--);
        if(chave == no->chave[posicao]){
            printf("Chave Duplicada!, nao eh possivel inserir.\n");
            return 0;
        }
    }

    if( setar_valor_no(chave, p, no->no_filho[posicao], filho) ){
        if(no->cont < MAX) {
            inserir_chave(*p, posicao, no, *filho);
        }
        else{
            split_no(*p, p, posicao, no, *filho, filho);
            return 1;
        }
    }

    return 0;
}

// Inserir Chave
void inserir(int chave){
    int flag, i;
    No *filho;

    flag = setar_valor_no(chave, &i, raiz, &filho);
    if(flag){
        raiz = cria_no(i, filho);
    }
}

// Copiar o sucessor
void copiar_sucessor(No *no, int posicao){
    No *aux;
    aux = no->no_filho[posicao];

    for(; aux->no_filho[0] != NULL;){
        aux = aux->no_filho[0];
    }

    no->chave[posicao] = aux->chave[posicao];
}

// Shift à direita
void shift_direita(No *no, int posicao){
    No *aux = no->no_filho[posicao];
    int j = aux->cont;

    while(j > 0){
        aux->chave[j + 1] = aux->chave[j];
        aux->no_filho[j + 1] = aux->no_filho[j];
    }

    aux->chave[1] = no->chave[posicao];
    aux->no_filho[1] = aux->no_filho[0];
    aux->cont++;

    aux = no->no_filho[posicao - 1];
    no->chave[posicao] = aux->chave[aux->cont];
    no->no_filho[posicao] = aux->no_filho[aux->cont];
    aux->cont--;

    return;
}

// Shift à esquerda
void shift_esquerda(No *no, int posicao){
    int j = 1;
    No *aux = no->no_filho[posicao - 1];

    aux->cont++;
    aux->chave[aux->cont] = no->chave[posicao];
    aux->no_filho[aux->cont] = no->no_filho[posicao]->no_filho[0];

    aux = no->no_filho[posicao];
    no->chave[posicao] = aux->chave[1];
    aux->no_filho[0] = aux->no_filho[1];
    aux->cont--;

    while(j <= aux->cont){
        aux->chave[j] = aux->chave[j + 1];
        aux->no_filho[j] = aux->no_filho[j + 1];
        j++;
    }

    return;
}

// Mergear nós
void merge_nos(No *no, int posicao){
    int j = 1;
    No *a = no->no_filho[posicao];
    No *b = no->no_filho[posicao - 1];

    b->cont++;
    b->chave[b->cont] = no->chave[posicao];
    b->no_filho[b->cont] = no->no_filho[0];

    while(j <= a->cont){
        b->cont++;
        b->chave[b->cont] = a->chave[j];
        b->no_filho[b->cont] = a->no_filho[j];
        j++;
    }

    j = posicao;
    while(j < no->cont){
        no->chave[j] = no->chave[j + 1];
        no->no_filho[j] = no->no_filho[j + 1];
        j++;
    }

    no->cont--;
    free(a);
}

// Ajustar nó
void ajusta_no(No *no, int posicao){
    if(!posicao){
        if(no->no_filho[1]->cont > MIN){
            shift_esquerda(no, 1);
        }
        else{
            merge_nos(no, 1);
        }
    }
    else{
        if(no->cont != posicao){
            if( no->no_filho[posicao - 1]->cont > MIN ){
                shift_direita(no, posicao);
            }
            else{
                if( no->no_filho[posicao + 1]->cont > MIN ){
                    shift_esquerda(no, posicao + 1);
                }
                else{
                    merge_nos(no, posicao);
                }
            }
        }
        else{
            if(no->no_filho[posicao - 1]->cont > MIN){
                shift_direita(no, posicao);
            }
            else{
                merge_nos(no, posicao);
            }
        }
    }
}

// Imprime in-ordem
void imprime_in_ordem(No *no){
    int i;

    if(no){
        for(i = 0; i < raiz->cont; i++){
            imprime_in_ordem(no->no_filho[i]);
            if(i + 1 <= no->cont) printf("%d ", no->chave[i + 1]);
        }
        imprime_in_ordem(no->no_filho[i]);
    }
}

// Destroi a arvore
void destruir_arvore(No *no){
    int i;
    
    if(no){
        for(i = 0; i < raiz->cont; i++){
            free(no->no_filho[i]);
        }
        free(no->no_filho[i]);
    }
}

// Verifica se a Árvore está vazia
void vazia(No *no){
    if(no == NULL || no->cont == 0) printf("A arvore estah vazia.\n");
    else printf("A arvore nao estah vazia.\n");
}

int main(){
    int opcao, valor_chave;

    do{
        printf("1 - Vazia?\n");
        printf("2 - Inserir Chave\n");
        printf("3 - Exibir in-ordem\n");
        printf("4 - Exibir pre-ordem\n");
        printf("5 - Exibir pos-ordem\n");
        printf("6 - Destruir a Arvore e Sair\n");
        printf("Opcao? ");
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
                vazia(raiz);
                break;

            case 2:
                printf("Digite a chave que deseja inserir: ");
                scanf("%d", &valor_chave);
                inserir(valor_chave);
                break;

            case 3:
                imprime_in_ordem(raiz);
                printf("\n");
                break;

            case 4:
                printf("Em Desenvolvimento.\n");
                break;

            case 5:
                printf("Em Desenvolvimento.\n");
                break;

            case 6:
                printf("Saindo...\n");
                destruir_arvore(raiz);
                break;

            default:
                printf("Opcao invalida! Favor, tentar novamente.\n");
                break;
        }

    } while(opcao != 6);

    return 0;
}