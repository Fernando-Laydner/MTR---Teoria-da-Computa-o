#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


// Opção de debug
#define DEBUG 1

// Cria a struc da working_tape quintupla
typedef struct transicao_t{
    int estado_input;
    char input;
    int estado_output;
    char output, op;
}transicao_t;

// Cria a struct da working_tape quadrupla
typedef struct transicao_q{
    int estado_input;
    char input;
    int estado_output;
    char op;
}transicao_q;

// Cria a struct da MT
typedef struct mt_t{
    int estado_atual, pos;
    char working_tape[5000];
}mt_t;

// Cria a struct da MT Revisível
typedef struct mt_r{
    int estado_atual, pos, hist;
    char working_tape[5000];
    int history[5000];
    char output_tape[5000];
}mt_r;

// Define todas situações da MT
enum resultado{EXECUTANDO, ACEITO, ERRO, REJEITOU, VOLTOU};

// Busca a transição correta nas transição quintuplas
int transicao5_compara(const void* a, const void* b){
    transicao_t const *_a = a;
    transicao_t const *_b = b;

    if(_a->estado_input == _b->estado_input) return _a->input - _b->input; 
    else return _a->estado_input - _b->estado_input;
}

// Busca a transição correta nas transição quadruplas
int transicao4_compara(const void* a, const void* b){
    transicao_q const *_a = a;
    transicao_q const *_b = b;

    if(_a->estado_input != _b->estado_input){return _a->estado_input - _b->estado_input;} 
    else if(_a->input != '/' && _b->input != '/' && _b->input != _a->input){return _a->input - _b->input;}
    return 0;
}

// Busca a transição correta nas transiçoes reversíveis
int transicaoR_compara(const void* a, const void* b){
    transicao_q const *_a = a;
    transicao_q const *_b = b;

    return _a->estado_output - _b->estado_output;
}

int string_compara(const void *a, const void *b){
    return strcmp(*(char const **)a, *(char const **)b);
}

void escreve_situacao_mt(enum resultado result, char *tape)
{
    printf("\nEstado final da working_tape: \033[0;32m%s\033[0m\nSituacao da MT: ", tape);
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
        case VOLTOU:
            printf("\033[0;32mVoltou\033[0m");
            break;
        default:
            assert(!"\033[0;35Resultado inesperado\033[0m");
            break;
    }
}


char **estados = NULL;
char estados_buffer[5000] = {0};
int n_estado = -1;

// Imprime as fitas quintuplas
void transicao5_imprime(transicao_t *transicao){
    #if(DEBUG == 1)
    printf("\tTransicao: (");
    if (transicao->estado_input < n_estado) printf("%s", estados[transicao->estado_input]); else printf("%d", transicao->estado_input);
    printf(",%c)=(", transicao->input);
    if (transicao->estado_output < n_estado) printf("%s", estados[transicao->estado_output]); else printf("%d", transicao->estado_output);
    printf(",%c,%c)\n", transicao->output, transicao->op);
    #endif
}

// Imprime as fitas quadruplas
void transicao4_imprime(transicao_q *transicao){
    #if(DEBUG == 1)
    printf("\tTransicao: (");
    if (transicao->estado_input < n_estado) printf("%s", estados[transicao->estado_input]); else printf("%d", transicao->estado_input);
    printf(",%c)=(", transicao->input);
    if (transicao->estado_output < n_estado) printf("%s", estados[transicao->estado_output]); else printf("%d", transicao->estado_output);
    printf(",%c)\n", transicao->op);
    #endif
}


// Verifica a fita por char inválidos
void verifica_fita(char fita[], char alfabeto[], int tamanho_alfabeto){
    for (int i = 0; i < 5000; i++){
        if (fita[i] == '\0') break;
        for (int j = 0; j < tamanho_alfabeto; j++){
            if (fita[i] == alfabeto[j]) break;
            else if(j == tamanho_alfabeto - 1){
                printf("Fita contem caracteres invalidos");
                exit(1);
            }
        }
    }
}


int main(int argc, char *argv[]){
// Leitura da MT
    // Primeira linha com número de estados, tamanho do alfabeto da working_tape, quantidade de estados, número de transições.
    int n_simb, n_trans, n_simb_input;
    scanf("%d %d %d %d ", &n_estado, &n_simb_input, &n_simb, &n_trans);
    printf("Numero de estados: %d\nTamanho do alfabeto da working_tape: %d\nTamanho do alfabeto da MT: %d\nNumero de transicoes: %d\n", n_estado, n_simb_input, n_simb, n_trans);
    
    // Define os estados da MT
    estados = malloc(sizeof(char *)*n_estado);
    fgets(estados_buffer, 5000, stdin);
    printf("Estados na entrada: [");
    estados[0] = strtok(estados_buffer, " ");
    printf("%s", estados[0]);
    for (int i = 1; i < n_estado; i++){
        estados[i] = strtok(NULL, " ");
        if (i == n_estado - 1) estados[n_estado - 1][strlen(estados[n_estado - 1]) - 1] = '\0';
        printf(", %s", estados[i]);
    }
    printf("]\n");
    qsort(estados, n_estado, sizeof (char *), string_compara);

    // Define o alfabeto da Fita
    char *simb_input = malloc(n_simb_input);
    printf("Alfabeto da working_tape: [");
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
        char estado_input[100] = {0};
        char estado_output[100] = {0};
        scanf(" (%99[^,],%c)=(%99[^,],%c,%c)\n", estado_input, &trasicoes[i].input, estado_output, &trasicoes[i].output, &trasicoes[i].op);

        char **estado_input_encontrado = bsearch(&(char *){estado_input}, estados, n_estado, sizeof (char *), string_compara);
        if (NULL == estado_input_encontrado) {
            fprintf(stderr, "Estado %s invalido na transicao", estado_input); 
            exit(EXIT_FAILURE);
        }

        char **estado_output_encontrado = bsearch(&(char *){estado_output}, estados, n_estado, sizeof (char *), string_compara);
        if (NULL == estado_output_encontrado) {
            fprintf(stderr, "Estado %s invalido na transicao", estado_output); 
            exit(EXIT_FAILURE);
        }
        trasicoes[i].estado_input  = estado_input_encontrado  - estados;
        trasicoes[i].estado_output = estado_output_encontrado - estados;

	    assert(trasicoes[i].estado_input  < n_estado);
	    assert(trasicoes[i].estado_output < n_estado);

        transicao5_imprime(trasicoes+i);
    }
// Leitura da MT

    // Define a situação da MT
    enum resultado result = EXECUTANDO;

// Fitas Quintuplas
    if (2 == argc && '5' == argv[1][0]){

        // Lê a Fita
        mt_t mt = {.estado_atual=0};
        scanf(" %4999s", mt.working_tape);
        printf("Fita na entrada: %s\n\n", mt.working_tape);

        // Verifica se exite algum char que não faz parte do alfabeto da fita
        verifica_fita(mt.working_tape, simb_input, n_simb_input);

        // Organiza as transições para poder realizar o bsearch
        qsort(trasicoes, n_trans, sizeof(transicao_t), transicao5_compara);
        #if(DEBUG == 1)
            printf("\nTransicoes quintuplas organizadas:\n");
            for (int i = 0; i < n_trans; i++){
                transicao5_imprime(trasicoes+i);
            }
        #endif
        
        // Realiza as operações.
        printf("\nInicio das operacoes:\n");
        for(int i = 0; i < 10000; i++){
            // Lê o char da fita na posição atual
            char simb_atual = mt.working_tape[mt.pos];
            printf("Estado da Fita: \033[0;36m%s\033[0m\n", mt.working_tape);

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

            // Escreve na working_tape de acordo com a transição atual e se for diferente de 'B'
            if (transicao_atual->output != 'B'){
                mt.working_tape[mt.pos] = transicao_atual->output;
            }

            // Movimenta o leitor da MT 
            switch (transicao_atual->op){
            case 'R':
                if(4999 == mt.pos){result = ERRO; printf("Acabou a fita!\n"); exit(EXIT_FAILURE);}
                mt.pos ++; 
                break;
            case 'L': 
                if (0 == mt.pos){result = ERRO; printf("Nao pode antes do inicio da fita!\n"); exit(EXIT_FAILURE);}
                mt.pos --; 
                break;
            default: assert(!"Movimento impossivel"); break;
            }

            // Define o novo estado
            mt.estado_atual = transicao_atual->estado_output;

            // Termina o programa se for o estado de aceitação (definido como numero de estados da MT original, que é o 6)
            if(mt.estado_atual == n_estado - 1){
                result = ACEITO;
                break;
            }
        }

    // Escreve a situação final da fita e libera a memória.  
    escreve_situacao_mt(result, mt.working_tape);
    free(trasicoes);
    free(simb);
    free(simb_input);
    free(estados);
    }
// Fitas Quadruplas
    else if (2 == argc && '4' == argv[1][0]){
        
        // Lê a Fita
        mt_t mt = {.estado_atual=0};
        scanf(" %4999s", mt.working_tape);
        printf("Fita na entrada: %s\n\n", mt.working_tape);

        // Verifica se exite algum char que não faz parte do alfabeto da fita
        verifica_fita(mt.working_tape, simb_input, n_simb_input);

        // Indica o ultimo estado para adicionar os estados novos e aloca as a quantidade de transições
        int indica_utimo = n_estado;
        transicao_q *trasicoes_q = malloc(sizeof(transicao_q)*n_trans*2);

        // Converte as transições quintupla em quadruplas
        #if(DEBUG == 1)
            printf("Novas transicoes:\n");
        #endif
            for(int i = 0; i < n_trans; i++){
                trasicoes_q[i*2] = (transicao_q){trasicoes[i].estado_input, trasicoes[i].input, indica_utimo, trasicoes[i].output};
                trasicoes_q[i*2+1] = (transicao_q){indica_utimo, '/', trasicoes[i].estado_output , trasicoes[i].op};
                transicao4_imprime(trasicoes_q+i*2);
                transicao4_imprime(trasicoes_q+i*2+1);
                indica_utimo++;
            }

        // Organiza as transições para poder realizar o bsearch
        qsort(trasicoes_q, n_trans*2, sizeof(transicao_q), transicao4_compara);
        #if(DEBUG == 1)
            printf("\nTransicoes quadruplas organizadas:\n");
            for(int i = 0; i < n_trans*2; i++){
                transicao4_imprime(trasicoes_q+i);
            }
        #endif
        
        // Realiza as operações.
        printf("\nInicio das operacoes:\n");
        for(int i = 0; i < 10000; i++){
            // Lê o char da working_tape na posição atual
            char simb_atual = mt.working_tape[mt.pos];
            printf("Estado da Fita: \033[0;36m%s\033[0m\n", mt.working_tape);

            // Coloca Branco caso seja um char nulo
            if('\0' == simb_atual){simb_atual = simb[n_simb-1];}

            // Define a chave e realiza a busca na lista de transições
            transicao_q chave;
            chave = (transicao_q){mt.estado_atual, simb_atual, 0 , 0};
            transicao_q *transicao_atual = bsearch(&chave, trasicoes_q, n_trans*2, sizeof(transicao_q), transicao4_compara);

            // Se não for encontrado uma transição termina o programa
            if(NULL == transicao_atual){
                printf("\nTransicao inexistente!\n");
                // Se DEBUG estiver ativado ela imprime a chave que foi utilizada para causar o erro
                transicao4_imprime(&chave);
                result = REJEITOU; 
                break;
            }
            else{
                // Se DEBUG estiver ativado ela imprime a transição que estiver sendo realizada.
                transicao4_imprime(transicao_atual);
            }

            // Movimenta o leitor da working_tape
            if (transicao_atual->input == '/')
                switch (transicao_atual->op){
                    case 'R':
                        if(4999 == mt.pos){result = ERRO; printf("Acabou a fita!\n"); exit(EXIT_FAILURE);}
                        mt.pos ++; 
                        break;
                    case 'L': 
                        if (0 == mt.pos){result = ERRO; printf("Nao pode antes do inicio da fita!\n"); exit(EXIT_FAILURE);}
                        mt.pos --; 
                        break;
                    default: assert(!"Movimento impossivel"); break;
                }
            // Escreve na working_tape de acordo com a transição atual
            else{
                mt.working_tape[mt.pos] = transicao_atual->op;
            }

            // Define o novo estado
            mt.estado_atual = transicao_atual->estado_output;

            // Termina o programa se for o estado de aceitação (definido como numero de estados da MT original, que é o 6)
            if(mt.estado_atual == n_estado - 1){
                result = ACEITO;
                break;
            }
        }

    // Escreve a situação final da fita e libera a memória. 
    escreve_situacao_mt(result, mt.working_tape);
    free(trasicoes_q);
    free(trasicoes);
    free(simb);
    free(simb_input);
    free(estados);
    }
// Fitas Reversíveis
    else if (1 == argc || (2 == argc && 'R' == argv[1][0])){

        // Lê a Fita
        mt_r mt = {.estado_atual=0};
        scanf(" %4999s", mt.working_tape);
        printf("Fita na entrada: %s\n\n", mt.working_tape);

        // Verifica se exite algum char que não faz parte do alfabeto da fita
        verifica_fita(mt.working_tape, simb_input, n_simb_input);

        // Indica o ultimo estado para adicionar os estados novos e aloca as a quantidade de transições
        int indica_utimo = n_estado;
        transicao_q *trasicoes_q = malloc(sizeof(transicao_q)*n_trans*2);

        // Converte as transições quintupla em quadruplas
        #if(DEBUG == 1) 
            printf("Novas transicoes:\n");
        #endif
        for(int i = 0; i < n_trans; i++){
            trasicoes_q[i*2] = (transicao_q){trasicoes[i].estado_input, trasicoes[i].input, indica_utimo, trasicoes[i].output};
            trasicoes_q[i*2+1] = (transicao_q){indica_utimo, '/', trasicoes[i].estado_output , trasicoes[i].op};
            transicao4_imprime(trasicoes_q+i*2);
            transicao4_imprime(trasicoes_q+i*2+1);
            indica_utimo++;
        }

        // Organiza as transicoes para poder realizar o bsearch
        qsort(trasicoes_q, n_trans*2, sizeof(transicao_q), transicao4_compara);
        #if(DEBUG == 1)
            printf("\nTransicoes quadruplas organizadas:\n");
            for(int i = 0; i < n_trans*2; i++){
                transicao4_imprime(trasicoes_q+i);
            }
        #endif
        
        // Realiza as operações.
        printf("\nInicio das operacoes:\n");
        for(int i = 0; i < 10000; i++){
            // Lê o char da fita na posição atual
            char simb_atual = mt.working_tape[mt.pos];
            printf("Estado da Fita: \033[0;36m%s\033[0m\n", mt.working_tape);

            // Coloca Branco caso seja um char nulo
            if('\0' == simb_atual){simb_atual = simb[n_simb-1];}

            // Define a chave e realiza a busca na lista de transições
            transicao_q chave;
            chave = (transicao_q){mt.estado_atual, simb_atual, 0 , 0};
            transicao_q *transicao_atual = bsearch(&chave, trasicoes_q, n_trans*2, sizeof(transicao_q), transicao4_compara);

            // Se não for encontrado uma transição termina o programa
            if(NULL == transicao_atual){
                printf("\nTransicao inexistente!\n");
                // Se DEBUG estiver ativado ela imprime a chave que foi utilizada para causar o erro
                transicao4_imprime(&chave);
                result = REJEITOU; 
                break;
            }
            else{
                // Se DEBUG estiver ativado ela imprime a transição que estiver sendo realizada.
                transicao4_imprime(transicao_atual);
            }

            // Movimenta o leitor da MT
            if (transicao_atual->input == '/'){
                switch (transicao_atual->op){
                    case 'R':
                        if(4999 == mt.pos){result = ERRO; printf("Acabou a fita!\n"); exit(EXIT_FAILURE);}
                        mt.pos ++; 
                        break;
                    case 'L': 
                        if (0 == mt.pos){result = ERRO; printf("Nao pode antes do inicio da fita!\n"); exit(EXIT_FAILURE);}
                        mt.pos --; 
                        break;
                    default: assert(!"Movimento impossivel"); break;
                }
                mt.history[mt.hist++] = transicao_atual->estado_input;
            }
            // Escreve na working_tape de acordo com a transição atual
            else{
                mt.working_tape[mt.pos] = transicao_atual->op;
            }

            // Define o novo estado
            mt.estado_atual = transicao_atual->estado_output;

            // Termina o programa se for o estado de aceitação (definido como numero de estados da MT original, que é o 6)
            if(mt.estado_atual == n_estado - 1){
                result = ACEITO;
                break;
            }
        }

        // Copia a fita da working_tape pra output_tape
        memcpy(mt.output_tape, mt.working_tape, 5000);

        // Escreve o histórico
        printf("\nHistorico: \033[1;35m[");
        for (int i = 0; i < mt.hist; i++){
            printf("%d", mt.history[i]);
            if (i < mt.hist-1){printf(", ");}
        }
        printf("]\033[0m\n");
    // Volta
        // Cria as transições reversíveis
        transicao_q *trasicoes_r = malloc(sizeof(transicao_q)*n_trans*2);
        memcpy(trasicoes_r, trasicoes_q, sizeof(transicao_q)*n_trans*2);

        // Organiza as transiçoes pra volta e para poder realizar o bsearch
        qsort(trasicoes_r, n_trans*2, sizeof(transicao_q), transicaoR_compara);
        #if(DEBUG == 1)
            printf("\nTransicoes reversiveis organizadas:\n");
            for(int i = 0; i < n_trans*2; i++){
                transicao4_imprime(trasicoes_r+i);
            }
        #endif

        // Realiza as operações de volta
        printf("\nInicio de volta:\n");
        for(int i = 0; i < 10000; i++){
            // Lê os estados do histórico de trás pra frente
            char estado_atual;
            if (mt.hist > 0){
                estado_atual = mt.history[--mt.hist];
            }
            else{
                result = VOLTOU;
                break;
            }

            // Imprime o estado da fita na volta  
            printf("Estado da Fita: \033[0;36m%s\033[0m\n", mt.working_tape);

            // Define a chave e realiza a busca na lista de transições
            transicao_q chave;
            chave = (transicao_q){estado_atual, 0, 0 , 0};
            transicao_q *transicao_atual = bsearch(&chave, trasicoes_q, n_trans*2, sizeof(transicao_q), transicao4_compara);

            // Se não for encontrado uma transição termina o programa
            if(NULL == transicao_atual){
                printf("\nTransicao inexistente!\n");
                // Se DEBUG estiver ativado ela imprime a chave que foi utilizada para causar o erro
                transicao4_imprime(&chave);
                result = REJEITOU; 
                break;
            }
            else{
                // Se DEBUG estiver ativado ela imprime a transição que estiver sendo realizada.
                transicao4_imprime(transicao_atual);
            }

            // Movimenta o leitor da MT
            switch (transicao_atual->op){
                case 'R':
                    if(4999 == mt.pos){result = ERRO; printf("Acabou a fita!\n"); exit(EXIT_FAILURE);}
                    mt.pos --; 
                    break;
                case 'L': 
                    if (-1 == mt.pos){result = ERRO; printf("Nao pode antes do inicio da fita!\n"); exit(EXIT_FAILURE);}
                    mt.pos ++; 
                    break;
                default: assert(!"Movimento impossivel"); break;
            }

            // Apaga o histórico
            mt.history[mt.hist] = 0;

            // Define a chave reversível e realiza a busca na lista de transições reversíveis
            transicao_q chaveR;
            chaveR = (transicao_q){0, 0, estado_atual , 0};
            transicao_q *transicao_volta = bsearch(&chaveR, trasicoes_r, n_trans*2, sizeof(transicao_q), transicaoR_compara);
            transicao4_imprime(transicao_volta);

            // Pega o valor que deve ter sido lido e escreve na fita (se for pra escrever B ele apaga).
            if (transicao_volta->input != 'B')
                mt.working_tape[mt.pos] = transicao_volta->input;
            else
                mt.working_tape[mt.pos] = '\0';
        }

    // Escreve a situação final da fita e libera a memória. 
    escreve_situacao_mt(result, mt.working_tape);
    free(trasicoes_r);
    free(trasicoes_q);
    free(trasicoes);
    free(simb);
    free(simb_input);
    free(estados);
    }
    
// Argumentos inválidos
    else if (2 == argc)fprintf(stderr, "Argumento invalido %s, utilize um dos argumentos a seguir: 4, 5 ou R.\n", argv[1]);
    else fprintf(stderr, "Quantidade de argumentos passados nao e valida.\n");
}


