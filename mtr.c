#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


typedef struct transicao_t{
    char input, output, pos;
    int estado_input, estado_output;
}transicao_t;

typedef struct mt_t{
    int estado_atual, pos;
    char fita[5000];
}mt_t;


int transcao_compara(const void* a, const void* b){
    transicao_t *_a = a;
    transicao_t *_b = b;

    if(_a->estado_input == _b->estado_input){
        return _a->input - _b->input;
    }
    else{
        return _a->estado_input - _b->estado_input;
    }
}

void transicao_imprime(transicao_t *transicao){
    printf("(%d,%c)=(%d,%c,%c)\n", transicao->estado_input, transicao->input, transicao->estado_output, transicao->output, transicao->pos);
}

enum resultado{EXECUTANDO, ACEITO, ERRO, REJEITOU};

//type  entrada-quintupla.txt | ./mtr
int main(void){
    int n_estado, n_simb, n_trans, n_simb_input;
    scanf("%d %d %d %d", &n_estado, &n_simb_input, &n_simb, &n_trans);
    printf("%d %d %d %d\n", n_estado, n_simb_input, n_simb, n_trans);
    
    #if 1
    int *estados = malloc(sizeof(int)*n_estado);

    for(int i = 0; i < n_estado; i++){
        scanf("%d", estados+i);
        printf("%d ", estados[i]);
    }
    putchar('\n');

    char *simb_input = malloc(n_simb_input);
    
    for(int i = 0; i < n_simb_input; i++){
        scanf(" %c", simb_input+i);
        printf("%c ", simb_input[i]);
    }
    putchar('\n');

    char *simb = malloc(n_simb);

    
    for(int i = 0; i < n_simb; i++){
        scanf(" %c", simb+i);
    }

    transicao_t *trasicoes = malloc(sizeof(transicao_t)*n_trans);

    for(int i = 0; i < n_trans; i++){
        scanf(" (%d,%c)=(%d,%c,%c)\n", &trasicoes[i].estado_input, &trasicoes[i].input, &trasicoes[i].estado_output, &trasicoes[i].output, &trasicoes[i].pos);
        transicao_imprime(trasicoes+i);
    }
    
    mt_t mt = {.estado_atual=1};
    scanf("%4999s", mt.fita);
    printf("%s\n\n", mt.fita);
    qsort(trasicoes, n_trans, sizeof(transicao_t), transcao_compara);

    enum resultado result = EXECUTANDO;

    for(int i = 0; i < 10000; i++){
        char simb_atual = mt.fita[mt.pos];
        printf("%s", mt.fita);
        if('\0' == simb_atual){simb_atual = simb[n_simb-1];}

        transicao_t chave = {simb_atual, 0 , 0, mt.estado_atual, 0};
        transicao_t *transicao_atual = bsearch(&chave, trasicoes, n_trans, sizeof(transicao_t), transcao_compara);

        if(NULL == transicao_atual){
            printf("Transicao inexistente");
            result = REJEITOU; 
            break;
        }
        else{
            transicao_imprime(transicao_atual);
        }

        mt.fita[mt.pos] = transicao_atual->output;

        switch (transicao_atual->pos){
        case 'R':
            if(4999 == mt.pos){result = ERRO; exit(EXIT_FAILURE);}
            mt.pos ++; 
            break;
        case 'L': 
            if (0 == mt.pos){result = ERRO; exit(EXIT_FAILURE);}
            mt.pos --; 
            break;
        default: assert(!"Movimento impossivel"); break;
        }

        mt.estado_atual = transicao_atual->estado_output;

        if(mt.estado_atual == n_estado){
            result = ACEITO;
            break;
        }

    }
    #endif

    switch (result){
    case ACEITO:
        printf("Aceito");
        break;
    case REJEITOU:
        printf("Rejeitou");
        break;
    case ERRO:
        printf("Erro");
        break;
    case EXECUTANDO:
        printf("Executando");
        break;
    default:
        assert(!"Resultado inesperado");
        break;
    }
}

