#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


// Opção de debug
#define DEBUG 1

// Define qual fita será executada ('5' -> quintupla, '4' -> quadrupla, '69' -> reversível).
#define FITA 5

// Cria a struc da fita quintupla
typedef struct transicao_t{
    int estado_input;
    char input;
    int estado_output;
    char output, op;
}transicao_t;

// Cria a struct da fita quadrupla
typedef struct transicao_q{
    int estado_input;
    char input;
    int estado_output;
    char op;
}transicao_q;

// Cria a struct da MT
typedef struct mt_t{
    int estado_atual, pos;
    char fita[5000];
}mt_t;

// Cria a struct das fitas reversíveis, possivelmente não seja necessário
typedef struct transicao_mf{
    int estado_input;
    char input[3];
    int estado_output;
    char op[3];
}transicao_mf;

// Define todas situações da MT
enum resultado{EXECUTANDO, ACEITO, ERRO, REJEITOU};

// Busca o estado correto das fitas quintuplas
int transicao5_compara(const void* a, const void* b){
    transicao_t *_a = a;
    transicao_t *_b = b;

    if(_a->estado_input == _b->estado_input){
        return _a->input - _b->input;
    }
    else{
        return _a->estado_input - _b->estado_input;
    }
}

// Busca o estado correto das fitas quadruplas
int transicao4_compara(const void* a, const void* b){
    transicao_q *_a = a;
    transicao_q *_b = b;

    if(_a->estado_input == _b->estado_input){
        return _a->input - _b->input;
    } 
    else{
        return _a->estado_input - _b->estado_input;
    }
}

// Talvez não seja necessário, paramos aqui!
int transcao_mf_compara(const void* a, const void* b){
    transicao_mf *_a = a;
    transicao_mf *_b = b;

    if(_a->estado_input == _b->estado_input){
        return strncmp(_a->input, _b->input, sizeof(char)*3);
    }
    else{
        return _a->estado_input - _b->estado_input;
    }
}

// Imprime as fitas quintuplas
void transicao5_imprime(transicao_t *transicao){
    #if(DEBUG == 1)
    printf("\tTransicao: (%d,%c)=(%d,%c,%c)\n", transicao->estado_input, transicao->input, transicao->estado_output, transicao->output, transicao->op);
    #endif
}

// Imprime as fitas quadruplas
void transicao4_imprime(transicao_q *transicao){
    #if(DEBUG == 1)
    printf("\tTransicao: (%d,%c)=(%d,%c)\n", transicao->estado_input, transicao->input, transicao->estado_output, transicao->op);
    #endif
}

int main(void){

// Leitura do arquivo original

    // Primeira linha com número de estados, tamanho do alfabeto da fita, quantidade de estados, número de transições.
    int n_estado, n_simb, n_trans, n_simb_input;

    scanf("%d %d %d %d", &n_estado, &n_simb_input, &n_simb, &n_trans);
    printf("Numero de estados: %d\nTamanho do alfabeto da fita: %d\nTamanho do alfabeto da MT: %d\nNumero de transicoes: %d\n", n_estado, n_simb_input, n_simb, n_trans);
    
    // Define os estados da MT
    int *estados = malloc(sizeof(int)*n_estado);
    printf("Estados na entrada: [");
    for(int i = 0; i < n_estado; i++){
        scanf("%d", estados+i);
        printf("%d", estados[i]);
        if (i < n_estado-1){printf(", ");}
    }
    printf("]\n");

    // Define o alfabeto da Fita
    char *simb_input = malloc(n_simb_input);
    printf("Alfabeto da fita: [");
    for(int i = 0; i < n_simb_input; i++){
        scanf(" %c", simb_input+i);
        printf("%c", simb_input[i]);
        if (i < n_simb_input-1){printf(", ");}
    }
    printf("]\n");

    // Define o alfabeto da MT
    char *simb = malloc(n_simb);
    printf("Alfabeto da MT: [");
    for(int i = 0; i < n_simb; i++){
        scanf(" %c", simb+i);
        printf("%c", simb[i]);
        if (i < n_simb-1){printf(", ");}
    }
    printf("]\n");

    // Carrega todas transições do arquivo
    transicao_t *trasicoes = malloc(sizeof(transicao_t)*n_trans);
    printf("Transicoes:\n");
    for(int i = 0; i < n_trans; i++){
        scanf(" (%d,%c)=(%d,%c,%c)\n", &trasicoes[i].estado_input, &trasicoes[i].input, &trasicoes[i].estado_output, &trasicoes[i].output, &trasicoes[i].op);
        transicao5_imprime(trasicoes+i);
    }

    // Lê a Fita
    mt_t mt = {.estado_atual=1};
    scanf(" %4999s", mt.fita);
    printf("Fita na entrada: %s\n\n", mt.fita);

// Leitura do arquivo original

    // Define a situação da MT
    enum resultado result = EXECUTANDO;

// Fitas Quintuplas
    #if(FITA == 5)
    printf("Inicio das operacoes:\n");
    // Organiza a fita para poder realizar o bsearch
    qsort(trasicoes, n_trans, sizeof(transicao_t), transicao5_compara);
    
    // Realiza as operações.
    for(int i = 0; i < 10000; i++){
        // Lê o char da fita na posição atual
        char simb_atual = mt.fita[mt.pos];
        printf("Estado da Fita: \033[0;36m%s\033[0m\n", mt.fita);

        // Coloca Branco caso seja um char nulo
        if('\0' == simb_atual){simb_atual = simb[n_simb-1];}

        // Define a chave e realiza a busca na lista de transições
        transicao_t chave = {mt.estado_atual, simb_atual, 0 , 0, 0};
        transicao_t *transicao_atual = bsearch(&chave, trasicoes, n_trans, sizeof(transicao_t), transicao5_compara);

        // Se não for encontrado uma transição termina o programa
        if(NULL == transicao_atual){
            printf("\nTransicao inexistente!\n");
            // Se DEBUG estiver ativado ela imprime a chave que foi utilizada para causar o erro
            transicao5_imprime(&chave);
            result = REJEITOU; 
            break;
        }
        else{
            // Se DEBUG estiver ativado ela imprime a transição que estiver sendo realizada.
            transicao5_imprime(transicao_atual);
        }

        // Escreve na fita de acordo com a transição atual e se for diferente de 'B'
        if (transicao_atual->output != 'B'){
            mt.fita[mt.pos] = transicao_atual->output;
        }

        // Movimenta o leitor da fita 
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

        // Define o novo estado
        mt.estado_atual = transicao_atual->estado_output;

        // Termina o programa se o estado de aceitação (definido como numero de estados da MT original, que é o 6)
        // Talvez definir o estado de aceitação antes seja uma boa ideia, caso não seja o ultimo.
        if(mt.estado_atual == n_estado){
            result = ACEITO;
            break;
        }
    }

// Fitas Quadruplas
    #elif(FITA == 4)
    // Indica o ultimo estado para adicionar os estados novos e aloca as a quantidade de transições
    int indica_utimo = n_estado+1;
    transicao_q *trasicoes_q = malloc(sizeof(transicao_q)*n_trans*2);

    printf("Novas transicoes:\n");
    // Converte as transições quintupla em quadruplas 
    for(int i = 0; i < n_trans; i++){
        trasicoes_q[i*2] = (transicao_q){trasicoes[i].estado_input, trasicoes[i].input, indica_utimo, trasicoes[i].output};
        trasicoes_q[i*2+1] = (transicao_q){indica_utimo, '/', trasicoes[i].estado_output , trasicoes[i].op};
        transicao4_imprime(trasicoes_q+i*2);
        transicao4_imprime(trasicoes_q+i*2+1);
        indica_utimo++;
    }
    printf("\n\nInicio das operacoes:\n");

    // Organiza a fita para poder realizar o bsearch
    qsort(trasicoes_q, n_trans*2, sizeof(transicao_q), transicao4_compara);
    
    // Realiza as operações.
    for(int i = 0; i < 10000; i++){
        // Lê o char da fita na posição atual
        char simb_atual = mt.fita[mt.pos];
        printf("Estado da Fita: \033[0;36m%s\033[0m\n", mt.fita);

        // Coloca Branco caso seja um char nulo
        if('\0' == simb_atual){simb_atual = simb[n_simb-1];}

        // Define a chave e realiza a busca na lista de transições
        transicao_q chave;
        if (i%2 == 0){
            // Leitura
            chave = (transicao_q){mt.estado_atual, simb_atual, 0 , 0};
        }
        else{
            // Escrita
            chave = (transicao_q){mt.estado_atual, '/', 0 , 0};
        }
        transicao_q *transicao_atual = bsearch(&chave, trasicoes_q, n_trans*2, sizeof(transicao_q), transicao4_compara);

        // Se não for encontrado uma transição termina o programa
        if(NULL == transicao_atual){
            printf("\nTransição inexistente!\n");
            // Se DEBUG estiver ativado ela imprime a chave que foi utilizada para causar o erro
            transicao4_imprime(&chave);
            result = REJEITOU; 
            break;
        }
        else{
            // Se DEBUG estiver ativado ela imprime a transição que estiver sendo realizada.
            transicao4_imprime(transicao_atual);
        }

        // Movimenta o leitor da fita
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
        // Escreve na fita de acordo com a transição atual e se for diferente de 'B'
        else{
            if (transicao_atual->op != 'B'){
                mt.fita[mt.pos] = transicao_atual->op;
            }
        }

        // Define o novo estado
        mt.estado_atual = transicao_atual->estado_output;

        // Termina o programa se o estado de aceitação (definido como numero de estados da MT original, que é o 6)
        // Talvez definir o estado de aceitação antes seja uma boa ideia, caso não seja o ultimo.
        if(mt.estado_atual == n_estado){
            result = ACEITO;
            break;
        }

    }
// Fitas Reversíveis WIP
    #elif(FITA == 69)
    int indica_utimo = n_estado+1;
    transicao_mf *trasicoes_mf = malloc(sizeof(transicao_mf)*n_trans*2);

    for(int i = 0; i < n_trans; i++){
        trasicoes_mf[i*2] = (transicao_mf){trasicoes[i].estado_input, trasicoes[i].input, indica_utimo, trasicoes[i].output};
        trasicoes_mf[i*2+1] = (transicao_mf){indica_utimo, '/', trasicoes[i].estado_output , trasicoes[i].op};
        transicao4_imprime(trasicoes_mf+i*2);
        transicao4_imprime(trasicoes_mf+i*2+1);
        indica_utimo++;
    }

    qsort(trasicoes_mf, n_trans*2, sizeof(transicao_mf), transcao_mf_compara);
    
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

        transicao_q *transicao_atual = bsearch(&chave, trasicoes_mf, n_trans*2, sizeof(transicao_mf), transcao_mf_compara);

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

    // Escreve a situação da MT
    printf("\nEstado final da fita: \033[0;32m%s\033[0m\nSituacao da MT: ", mt.fita);
    switch (result){
        case ACEITO:
            printf("\033[0;32mAceitou\033[0m");
            break;
        case REJEITOU:
            printf("\033[0;33mRejeitou\033[0m");
            break;
        case ERRO:
            printf("\033[0;31mErro\033[0m");
            break;
        case EXECUTANDO:
            printf("\033[0;34mExecutando\033[0m");
            break;
        default:
            assert(!"\033[0;35Resultado inesperado\033[0m");
            break;
    }
}

