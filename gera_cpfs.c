#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void gerarCPF(char *cpf) {
    int nums[11];
    int soma = 0;

    // Gera os 9 primeiros dígitos
    for (int i = 0; i < 9; i++) {
        nums[i] = rand() % 10;
    }

    // Primeiro dígito verificador
    for (int i = 0; i < 9; i++) {
        soma += nums[i] * (10 - i);
    }

    int resto = soma % 11;
    nums[9] = (resto < 2) ? 0 : 11 - resto;

    // Segundo dígito verificador
    soma = 0;
    for (int i = 0; i < 10; i++) {
        soma += nums[i] * (11 - i);
    }

    resto = soma % 11;
    nums[10] = (resto < 2) ? 0 : 11 - resto;

    // Converte para string
    sprintf(
        cpf,
        "%d%d%d%d%d%d%d%d%d%d%d\n",
        nums[0], nums[1], nums[2],
        nums[3], nums[4], nums[5],
        nums[6], nums[7], nums[8],
        nums[9], nums[10]
    );
}

int main() {
    srand(time(NULL));

    char cpf[16];

    FILE* arq = fopen("lista_cpfs.txt", "w");

    for (int i = 0; i < 4096; i++) {
        gerarCPF(cpf);
        fprintf(arq, cpf);
    }

    return 0;
}
