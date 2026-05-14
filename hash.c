#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 4096 com folga de 20% da 4915. 4919 eh o primo mais proximo 
#define TAMANHO_TABELA 4919
#define NUM_CPFS 4096

typedef struct {
  char chave[12]; // cpf + '\n'
  int ocupado; // 1 = ocupado, 0 = livre
} item;

typedef struct {
  item tabela[TAMANHO_TABELA];
} hash_table;

int minha_hash(char *chave)
{
  int indice = 0;
  int primo = 4931; // primo posterior a 4919

  while(*chave)
  {
    indice = indice * primo + *chave;
    indice ^= indice >> 7;
    chave++;
  }
  return indice % TAMANHO_TABELA;
}

int outra_hash(char *chave)
{
  int indice = 0;
  int primo = 4909; // primo anterior a 4919
  
  while(*chave)
  {
    indice = indice * primo + *chave;
    indice ^= indice >> 5;
    indice *= 31; // diminuiu muito as colisoes 1740 -> 1068
    chave++;
  }
  return 1 + (indice % (TAMANHO_TABELA - 1));
}

void insere_hashtable(hash_table *table, char *chave, int *colisoes)
{
  int len = strlen(chave);
  int indice = minha_hash(chave);
  int indice2 = outra_hash(chave);
  
  // flag para evitar inflar o num de colisoes. Anterior: 4167
  int houve_colisao = 0;

  for(int i = 0; i < TAMANHO_TABELA; i++)
  {
    int p = (indice + (indice2 * i)) % TAMANHO_TABELA;
    if(table->tabela[p].ocupado == 0)
    {
      strcpy(table->tabela[p].chave, chave);
      table->tabela[p].ocupado = 1;
      if(houve_colisao) (*colisoes)++;
      return;
    } 
    houve_colisao = 1;
  }
}

int main(void)
{
  // inicializa espaco da memoria da hash_table com zero
  hash_table tabela;
  memset(&tabela, 0, sizeof(tabela));

  char cpf[12];

  FILE* arq = fopen("lista_cpfs.txt", "r");
  if (arq == NULL)
  {
    printf("Erro ao abrir arquivo contendo cpfs\n");
    return -1;
  }

  
  for(int n_chaves = 100; n_chaves < NUM_CPFS; n_chaves+= 100)
  { 
    // reseta tabela, numero de colisoes e volta pro inicio do arquivo cada iteracao
    rewind(arq);
    memset(&tabela, 0, sizeof(tabela));
    int colisoes = 0;
    int inseridas = 0;

    while(fgets(cpf, sizeof(cpf), arq) && inseridas < n_chaves)
    {
      // tira o "\n" no final de cada linha
      cpf[strcspn(cpf, "\n")] = 0;
      if(strlen(cpf) != 11) continue;
      insere_hashtable(&tabela, cpf, &colisoes);
      inseridas++;
    }
    printf("colisoes: %d | n_chaves: %d\n", colisoes, inseridas);
  }

  // ultima execucao pois como andamos de 100 em 100, 4100 > 4096 e nao roda as ultimas 96 chaves
  int n_chaves = 4096;
  rewind(arq);
  memset(&tabela, 0, sizeof(tabela));
  int colisoes = 0;
  int inseridas = 0;

  while(fgets(cpf, sizeof(cpf), arq) && inseridas < n_chaves)
  {
  // tira o "\n" no final de cada linha
    cpf[strcspn(cpf, "\n")] = 0;
    if(strlen(cpf) != 11) continue;
    insere_hashtable(&tabela, cpf, &colisoes);
    inseridas++;
  }
  printf("colisoes: %d | n_chaves: %d\n", colisoes, inseridas);
  fclose(arq);
  return 0;
}
