#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define DEBUG 1

typedef struct transicao_t{
    char input, output, pos;
    int estado_input, estado_output;
}transicao_t;

typedef struct transicao_q{
    int estado_input;
    char input;
    int estado_output;
    char op;
}transicao_q;

typedef struct mt_t{
    int estado_atual, pos;
    char fita[5000];
}mt_t;

typedef struct transicao_mf{
    int estado_input;
    char input[3];
    int estado_output;
    char op[3];
}transicao_mf;


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

int transcao4_compara(const void* a, const void* b){
    transicao_q *_a = a;
    transicao_q *_b = b;

    if(_a->estado_input == _b->estado_input){
        return _a->input - _b->input;
    } 
    else{
        return _a->estado_input - _b->estado_input;
    }
}

int transcao_compara_mf(const void* a, const void* b){
    transicao_mf *_a = a;
    transicao_mf *_b = b;

    if(_a->estado_input == _b->estado_input){
        return strncmp(_a->input, _b->input, 3);
    }
    else{
        return _a->estado_input - _b->estado_input;
    }
}

void transicao_imprime(transicao_t *transicao){
    #if(DEBUG == 1)
    printf("(%d,%c)=(%d,%c,%c)\n", transicao->estado_input, transicao->input, transicao->estado_output, transicao->output, transicao->pos);
    #endif
}

void transicao4_imprime(transicao_q *transicao){
    #if(DEBUG == 1)
    printf("(%d,%c)=(%d,%c)\n", transicao->estado_input, transicao->input, transicao->estado_output, transicao->op);
    #endif
}

enum resultado{EXECUTANDO, ACEITO, ERRO, REJEITOU};

//type  entrada-quintupla.txt | ./mtr
int main(void){
    int n_estado, n_simb, n_trans, n_simb_input;
    scanf("%d %d %d %d", &n_estado, &n_simb_input, &n_simb, &n_trans);
    printf("%d %d %d %d\n", n_estado, n_simb_input, n_simb, n_trans);
    
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
    enum resultado result = EXECUTANDO;

    #if 0
    qsort(trasicoes, n_trans, sizeof(transicao_t), transcao_compara);
    
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
    #elif 1
    int indica_utimo = n_estado+1;
    transicao_q *trasicoes_q = malloc(sizeof(transicao_q)*n_trans*2);

    for(int i = 0; i < n_trans; i++){
        trasicoes_q[i*2] = (transicao_q){trasicoes[i].estado_input, trasicoes[i].input, indica_utimo, trasicoes[i].output};
        trasicoes_q[i*2+1] = (transicao_q){indica_utimo, '/', trasicoes[i].estado_output , trasicoes[i].pos};
        transicao4_imprime(trasicoes_q+i*2);
        transicao4_imprime(trasicoes_q+i*2+1);
        indica_utimo++;
    }

    qsort(trasicoes_q, n_trans*2, sizeof(transicao_q), transcao4_compara);
    
    for(int i = 0; i < 10000; i++){
        char simb_atual = mt.fita[mt.pos];
        printf("%s", mt.fita);
        if('\0' == simb_atual){simb_atual = simb[n_simb-1];}

        transicao_q chave;
        if (i%2 == 0){
            chave = (transicao_q){mt.estado_atual, simb_atual, 0 , 0};
        }
        else{
            chave = (transicao_q){mt.estado_atual, '/', 0 , 0};
        }

        transicao_q *transicao_atual = bsearch(&chave, trasicoes_q, n_trans*2, sizeof(transicao_q), transcao4_compara);

        if(NULL == transicao_atual){
            printf("\nTransicao inexistente\n");
            transicao4_imprime(&chave);
            result = REJEITOU; 
            break;
        }
        else{
            transicao4_imprime(transicao_atual);
        }

        if (transicao_atual->input == '/')
            switch (transicao_atual->op){
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
        else{
            if (transicao_atual->op != 'B'){
                mt.fita[mt.pos] = transicao_atual->op;
            }
        }
        mt.estado_atual = transicao_atual->estado_output;

        if(mt.estado_atual == n_estado){
            result = ACEITO;
            break;
        }

    }
    #else
    int indica_utimo = n_estado+1;
    transicao_mf *trasicoes_mf = malloc(sizeof(transicao_mf)*n_trans*2);

    for(int i = 0; i < n_trans; i++){
        trasicoes_mf[i*2] = (transicao_mf){trasicoes[i].estado_input, trasicoes[i].input, indica_utimo, trasicoes[i].output};
        trasicoes_mf[i*2+1] = (transicao_mf){indica_utimo, '/', trasicoes[i].estado_output , trasicoes[i].pos};
        transicao4_imprime(trasicoes_mf+i*2);
        transicao4_imprime(trasicoes_mf+i*2+1);
        indica_utimo++;
    }

    qsort(trasicoes_mf, n_trans*2, sizeof(transicao_mf), transcao_compara_mf);
    
    for(int i = 0; i < 10000; i++){
        char simb_atual = mt.fita[mt.pos];
        printf("%s", mt.fita);
        if('\0' == simb_atual){simb_atual = simb[n_simb-1];}

        transicao_mf chave;
        if (i%2 == 0){
            chave = (transicao_mf){mt.estado_atual, simb_atual, 0 , 0};
        }
        else{
            chave = (transicao_mf){mt.estado_atual, '/', 0 , 0};
        }

        transicao_q *transicao_atual = bsearch(&chave, trasicoes_mf, n_trans*2, sizeof(transicao_mf), transcao_compara_mf);

        if(NULL == transicao_atual){
            printf("\nTransicao inexistente\n");
            transicao4_imprime(&chave);
            result = REJEITOU; 
            break;
        }
        else{
            transicao4_imprime(transicao_atual);
        }

        if (transicao_atual->input == '/')
            switch (transicao_atual->op){
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
        else{
            if (transicao_atual->op != 'B'){
                mt.fita[mt.pos] = transicao_atual->op;
            }
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

