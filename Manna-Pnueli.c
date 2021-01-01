#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

/*
A VARIÁVEL NUM_THREADS DEFINE O NÚMERO DE THREADS CLIENTES
*/
#define NUM_THREADS 4

/*
A VARIÁVEL NUM_REPEAT DEFINE O NÚMERO DE REPETIÇÕES QUE CADA THREAD CLIENTE EXECUTARÁ 
AO INCREMENTAR A VARIÁVEL GLOBAL.
*/
#define NUM_REPEAT  10000000

/*
INICIALIZAÇÃO DAS VARIÁVEIS GLOBAIS
*/

int varShared = 0;
int request = 0;
int respond = 0;
int finished = 0;

void* server(void *param);
void* client(void *param);

int main() {

    /*
    É CRIADO A THREAD PARA O SERVIDOR E PARA OS CLIENTES
    */
    
    pthread_t serverThd; 
    pthread_t clientThd[NUM_THREADS];
    
    pthread_create(&serverThd, NULL, server, NULL); 
    
    /*
    A FUNÇÃO PTRHEAD_CREATE É CHAMADA PARA AS THREADS CLIENTES
    */
    for(int i = 0; i < NUM_THREADS; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i + 1; 
        pthread_create(&clientThd[i], NULL, client, arg);  
    } 
    
    /*
    APÓS A FINALIZAÇÃO DAS THREADS CLIENTES É FEITO O JOIN DELAS.
    */
    for( int i = 0; i < NUM_THREADS; i++) {
        pthread_join( clientThd[i], NULL );
    } 
    
    /*
    A VARIÁVEL FINISHED É SETADA COM 1 E A EXECUÇÃO DA THREAD SERVIDOR É FINALIZADA
    */

    finished = 1;
    
    /*
    O VALOR DA VARIÁVEL COMPARTILHADA PELAS THREADS CLIENTES É EXIBIDO.
    */
    printf("VALOR DA VARIÁVEL COMPARTILHADA: %d\n", varShared); 
    
    return 0;
}

void* server(void *param) {

    /*
    ENQUANTO FINISHED NÃO FOR VERDADEIRO, O SERVIDOR AGUARDA AS SOLICITAÇÕES POR PARTE
    DAS THREADS CLIENTES.
    */

    while( !finished ) {

        /*
        SE REQUEST == 0 É PORQUE NÃO EXISTE NENHUMA SOLICITAÇÃO, OU SEJA, O SERVIDOR ESTÁ
        EM ESTADO DE ESPERA. O SERVIDOR ESTÁ ESPERANDO A CHEGADA DE REQUISIÇÕES A SEREM PROCESSADAS.
        */  
        while( request == 0 ) {
            //usleep(1);
        } 

        /*
        QUANDO UMA SOLICITAÇÃO É FEITA AO SERVIDOR A VARIÁVEL REQUEST É ATRIBUÍDA
        À VARIÁVEL RESPOND, INDICANDO QUE O SERVIDOR NO MOMENTO ESTÁ OCUPADO 
        RESPONDENDO À SOLICITAÇÃO FEITA PELO CLIENTE.
        */ 
        respond = request;  

        /*
        ENQUANTO RESPOND FOR DIFERENTE DE ZERO, O SERVIDOR CONTINUA A PROCESSAR A 
        SOLICITAÇÃO FEITO PELO CLIENTE.
        */
        while( respond != 0 ) {
            //usleep(1);
        }

        /* 
        QUANDO O SERVIDOR TERMINA DE PROCESSAR A SOLICITAÇÃO FEITA PELO CLIENTE,
        A VARIÁVEL REQUEST RECEBE NOVAMENTE O VALOR ZERO, INDICANDO QUE UMA NOVA
        SOLICITAÇÃO PODERÁ SER ATENDIDA.
        */
        request = 0;
    }
  pthread_exit(NULL);
}

void* client( void *param ) {

    /*
    O LAÇO SERÁ EXECUTADO POR CADA UMA DAS THREADS CLIENTES.
    */
    for(int count = 0; count < NUM_REPEAT; count++) {

        // A IDENTIFICAÇÃO DA THREAD CLIENTE É ARMAZENADA NA VARIÁVEL ID
        int id = *((int *)param);
        
        // PRÉ-PROTOCOLO 

        /*
        ENQUANTO A VARIÁVEL RESPOND FOR DIFERENTE DO ID DA THREAD SOLICITANTE O LAÇO
        É EXECUTADO; AGUARDANDO QUE A SEÇÃO CRÍTICA SEJA LIBERADA PARA USO.
        */
       
       /*
        while( respond  != id ) {
          request = id;
          //printf("Request Client: %d\n", request );   
          //fflush(stdout);
        }
        */
        
        
        /*
        AQUI A VARIÁVEL RESPOND SE TORNOU IGUAL AO ID DA THREAD SOLICITANTE E A MESMA PODE ACESSAR
        A SESSÃO CRÍTICA FAZENDO O INCREMENTO NA VARIÁVEL GLOBAL.
        */ 

        // SEÇÃO CRÍTICA
        varShared = varShared + 1;
        

        /*
        NA SAÍDA DA SESSÃO CRÍTICA, NO PÓS-PROTOCOLO, A VARIÁVEL RESPOND É SETADA
        NOVAMENTE COM O VALOR ZERO.
        */ 

        // PÓS-PROTOCOLO
        respond = 0;
    }
    pthread_exit(NULL);
}


