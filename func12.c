/*
Notas para o futuro: estamos a declarar o erro no main mas já n é necessário, se tirar tira da função de ler o ficheiro
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int cidade1;
    int cidade2;
    char meio[16]; // "autocarro", "comboio", "barco", "aviao"
    int tempo;     // duração da viagem
    int custo;     // custo da viagem
    int ti;        // hora da primeira partida
    int tf;        // hora da última partida
    int p;         // periodicidade
} Ligacao;

typedef struct{
    int task;
    int cidadeA;
    int cidadeB;
    int hora_do_dia;
} Task;


//Função que lê ficheiro
Ligacao *ler_ficheiro_map(const char *mapa, int *N, int *L, int *erro);
//Funcão que lê o ficheiro da quest
Task *ler_quest(const char *quest);
//Função que mostra a mensagem de erro
void mostrar_erro_de_ligacao(const char *mensagem, int valor, int numero_ligacao);
//funcção que verifica se a ligação tem os parametros bem definidos
int validar_ligacao( Ligacao lig, int numero_ligacao);
//função que dá print ao mapa
void imprimir_ligacoes(Ligacao* ligacoes, int num_ligacoes, int num_cidades);
//Função para a task1
int task1_contar_meios_diretos(Ligacao* ligacoes, int num_ligacoes, int cidadeA, int cidadeB);
//Função para a task2
int task2_menor_tempo_para_cada_meio(Ligacao* ligacoes, int num_ligacoes, int cidadeA, int cidadeB);
int main(int argc, char*argv[]) {
	if( argc !=3){
		printf("Uso: %s <nome1>.map <nome2>.quests \n", argv[0]);
		return 1;
	}

    int N, L, erro =0;
    const char *mapa = argv[1];
    const char *quest = argv[2];
    printf("A começar\n");
    Ligacao *ligacoes = ler_ficheiro_map(mapa, &N, &L, &erro);
    printf("leu mapa\n");
    Task *task = ler_quest(quest);
    printf("leu task\n");
    //Verifica se a função ler_ficheiro teve sucesso. Quando volta null 
    if (ligacoes ==NULL){
        printf("Erro na leitura do mapa");
        return -1;
    }
     if (task ==NULL){
        printf("Erro na leitura da quest");
        return -1;
    }
    imprimir_ligacoes(ligacoes,N,L); 
    /*int cidadeA = 0;
    int cidadeB= 0;
    printf("Quais as ligações que queres?\n");
    scanf("%d %d", &cidadeA, &cidadeB);
    */
    printf("\n valores quest: Task%d, %d, %d\n",task->task, task->cidadeA, task->cidadeB );
    int task1 = task1_contar_meios_diretos(ligacoes, L,  task->cidadeA, task->cidadeB);
   
    printf("O resultado é: %d\n", task1);

	free(ligacoes);
    free(task);
    return 0;
    
}


//Função que lê o ficheiro
Ligacao *ler_ficheiro_map(const char *mapa, int *N, int *L, int *erro){
    FILE* ficheiro = fopen(mapa, "r");

    if (ficheiro == NULL) {
        printf("Erro ao abrir ficheiro\n");
        return NULL;
    }

    if(fscanf(ficheiro, "%d %d", N, L)!=2){ // número de cidades e ligações e caso não exista um dos valores dá erro
	printf("Erro ao ler cabeçalho do ficheiro\n");
	fclose(ficheiro);
	return NULL;
	}
    Ligacao *ligacoes = malloc(*L * sizeof(Ligacao));
    if (ligacoes == NULL) { //Confirma a alocação de memória
        printf("Erro ao alocar memória\n");
        fclose(ficheiro);
        return NULL;
    }
	//ler todas as linhas do ficheiro até concluir as L(numero de ligações) linhas e guardar a informação na estrutura  ligações
    for (int i = 0; i < *L; i++) {
        if(fscanf(ficheiro, "%d %d %15s %d %d %d %d %d",
               &ligacoes[i].cidade1,
               &ligacoes[i].cidade2,
               ligacoes[i].meio,
               &ligacoes[i].tempo,
               &ligacoes[i].custo,
               &ligacoes[i].ti,
               &ligacoes[i].tf,
               &ligacoes[i].p)!=8){

		printf("Erro ao ler ligação %d \n", i+1);
		free(ligacoes);
		fclose(ficheiro);
		return NULL;}
	//verificar as ligações
	validar_ligacao(ligacoes[i], i+1);
}
    fclose(ficheiro);
    *erro = 0;
    return ligacoes;
}
//Função que lê o ficheiro quest
Task *ler_quest(const char *quest){
     FILE* ficheiro = fopen(quest, "r");

    if (ficheiro == NULL) {
        printf("Erro ao abrir ficheiro\n");
        return NULL;
    }
    Task *task = malloc(sizeof(Task));
     if(fscanf(ficheiro, "Task%d %d %d",&task->task, &task->cidadeA, &task->cidadeB)!=3){
        free(task);
        fclose (ficheiro);
        return NULL;
    };
    if (task->task == 4) {
        if (fscanf(ficheiro, "%d", &task->hora_do_dia) != 1) {
            printf("Erro: Task4 requer parâmetro hora_do_dia\n");
            free(task);
            fclose(ficheiro);
            return NULL;
        }
    } else {
        task->hora_do_dia = -1; // Valor inválido para tasks não-4
    }
    fclose (ficheiro);
    return(task);
}


//Função que escreve o erro de ligação
void mostrar_erro_de_ligacao(const char *mensagem, int valor, int numero_ligacao){
	printf("Erro %s (%d) na ligação %d", mensagem, valor, numero_ligacao);
}
//Função que verifica se a ligação tem os parametros correctos
int validar_ligacao( Ligacao lig, int numero_ligacao){
	int erros = 0;



	if (lig.ti <0 || lig.ti > 1440){
	mostrar_erro_de_ligacao("ti inválido", lig.ti, numero_ligacao);
	erros++;
	}
	if (lig.tf <0 || lig.tf > 1440){
	mostrar_erro_de_ligacao("tf inválido", lig.tf, numero_ligacao);
	erros++;
	}
	if (lig.ti>lig.tf){
	mostrar_erro_de_ligacao("tf inválido", lig.tf, numero_ligacao);
	erros++;
	}
	if (lig.p <=0){
	mostrar_erro_de_ligacao("Priodocidade inválida", lig.p, numero_ligacao);
	erros++;
	}
	if ( lig.tempo <=0){
	mostrar_erro_de_ligacao("tempo de viagem inválido", lig.tempo, numero_ligacao);
	erros ++;
	}
	if (lig.custo <0){
	mostrar_erro_de_ligacao("Custo de viagem inválido", lig.custo, numero_ligacao);
	erros++;}

	return erros;
}


//função que dá print do mapa
void imprimir_ligacoes(Ligacao* ligacoes, int num_ligacoes, int num_cidades) {
    printf("Mapa com %d cidades e %d ligações:\n", num_cidades, num_ligacoes);
    
    for (int i = 0; i < num_ligacoes; i++) {
        printf("L%d: %d <-> %d por %s | tempo: %d | custo: %d | [%d, %d] cada %d\n",
               i + 1,
               ligacoes[i].cidade1,
               ligacoes[i].cidade2,
               ligacoes[i].meio,
               ligacoes[i].tempo,
               ligacoes[i].custo,
               ligacoes[i].ti,
               ligacoes[i].tf,
               ligacoes[i].p);
    }
}



int task1_contar_meios_diretos(Ligacao* ligacoes, int num_ligacoes, int cidadeA, int cidadeB) {
    int encontrados[4] = {0}; // 0=autocarro, 1=comboio, 2=barco, 3=aviao
    
    for (int i = 0; i < num_ligacoes; i++) {
        // Verificar se a ligação é entre cidadeA e cidadeB (em qualquer ordem)
        if ((ligacoes[i].cidade1 == cidadeA && ligacoes[i].cidade2 == cidadeB) ||
            (ligacoes[i].cidade1 == cidadeB && ligacoes[i].cidade2 == cidadeA)) {
            
            // Identificar o tipo de transporte e marcar como encontrado
            if (strcmp(ligacoes[i].meio, "autocarro") == 0)
                encontrados[0] = 1;
            else if (strcmp(ligacoes[i].meio, "comboio") == 0)
                encontrados[1] = 1;
            else if (strcmp(ligacoes[i].meio, "barco") == 0)
                encontrados[2] = 1;
            else if (strcmp(ligacoes[i].meio, "aviao") == 0)
                encontrados[3] = 1;
        }
    }
  
    // Contar quantos tipos diferentes foram encontrados
    int count = 0;
    for (int j = 0; j < 4; j++) {
        count += encontrados[j];
    }
    
    return count;
} 
int task2_menor_tempo_para_cada_meio(Ligacao* ligacoes, int num_ligacoes, int cidadeA, int cidadeB){
    //int encontrados[4] = {0}; // 0=autocarro, 1=comboio, 2=barco, 3=aviao
    int menor_tempo[4];
    for(int j = 0; j<4; j++){
        menor_tempo[j] = -1;
    }
     for (int i = 0; i < num_ligacoes; i++) {
        // Verificar se a ligação é entre cidadeA e cidadeB (em qualquer ordem)
        if ((ligacoes[i].cidade1 == cidadeA && ligacoes[i].cidade2 == cidadeB) ||
            (ligacoes[i].cidade1 == cidadeB && ligacoes[i].cidade2 == cidadeA)) {
            
            // Identificar o tipo de transporte e marcar como encontrado
            if (strcmp(ligacoes[i].meio, "autocarro") == 0){
                //encontrados[0] = 1;
                if (menor_tempo[0]== -1 || ligacoes[i].tempo < menor_tempo[0]){
                    menor_tempo[0] = ligacoes[i].tempo ;
                }
            }
            else if (strcmp(ligacoes[i].meio, "comboio") == 0){
                if (menor_tempo[1]== -1 || ligacoes[i].tempo < menor_tempo[1]){
                    menor_tempo[1] = ligacoes[i].tempo;
                }}
                //encontrados[1] = 1;
            else if (strcmp(ligacoes[i].meio, "barco") == 0){
                if (menor_tempo[2]== -1 || ligacoes[i].tempo < menor_tempo[2]){
                    menor_tempo[2] = ligacoes[i].tempo;
                }}
                //encontrados[2] = 1;
            else if (strcmp(ligacoes[i].meio, "aviao") == 0){
                if (menor_tempo[3]== -1 || ligacoes[i].tempo < menor_tempo[3]){
                    menor_tempo[3] = ligacoes[i].tempo;
                }
            }
                
        }
    }
    return *menor_tempo;
}

