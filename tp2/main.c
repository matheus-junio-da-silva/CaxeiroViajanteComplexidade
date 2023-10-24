#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

//#define MAX_CITIES 100 // Defina o número máximo de cidades, ajuste conforme necessário

// Função para calcular a soma dos dígitos de um número
int sumDigits(int number) {
    int sum = 0;
    while (number > 0) {
        sum += number % 10;
        number /= 10;
    }
    return sum;
}

// Função para calcular o fatorial
int factorial(int n) {
    if (n == 0 || n == 1) return 1;
    return n * factorial(n - 1);
}

// Função para calcular a distância total de uma permutação
int calculateDistance(int *permutation, int **matrix, int n) {
    int distance = 0;
    int startCity = permutation[0];
    int endCity = permutation[n - 1];
    for (int i = 0; i < n - 1; i++) {
        int from = permutation[i];
        int to = permutation[i + 1];
        distance += matrix[from][to];
    }
    distance += matrix[startCity][permutation[1]];
    distance += matrix[endCity][permutation[n - 2]];
    return distance;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Função para gerar a próxima permutação
int nextPermutation(int *array, int size) {
    int i = size - 2;
    while (i >= 0 && array[i] > array[i + 1]) {
        i--;
    }

    if (i < 0) {
        return 0; // Todas as permutações foram geradas
    }

    int j = size - 1;
    while (array[j] < array[i]) {
        j--;
    }

    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;

    for (int l = i + 1, r = size - 1; l < r; l++, r--) {
        temp = array[l];
        array[l] = array[r];
        array[r] = temp;
    }

    return 1; // Próxima permutação gerada
}

// Função para gerar todas as permutações de N-1 cidades
void generatePermutations(int *cities, int n, int **matrix, int N, int X) {
    int *permutation = (int *)malloc((n - 1) * sizeof(int));
    for (int i = 0; i < n - 1; i++) {
        permutation[i] = cities[i];
    }

    int minDistance = INT_MAX;
    int *bestPermutation = (int *)malloc((n - 1) * sizeof(int));

    do {
        int distance = 0;
        for (int i = 0; i < n - 2; i++) {
            distance += matrix[permutation[i]][permutation[i + 1]];
        }
        distance += matrix[permutation[n - 2]][X];
        distance += matrix[X][permutation[0]];

        if (distance < minDistance) {
            minDistance = distance;
            for (int i = 0; i < n - 1; i++) {
                bestPermutation[i] = permutation[i];
            }
        }
    } while (nextPermutation(permutation, n - 1));

    // Imprima a melhor permutação e a menor distância
    printf("Melhor percurso: %d", X);
    for (int i = 0; i < n - 1; i++) {
        printf(" -> %d", bestPermutation[i]);
    }
    printf(" -> %d\n", X);
    printf("Menor distância: %d\n", minDistance);

    free(permutation);
    free(bestPermutation);
}

// Função para gerar aleatoriamente as distâncias
void generateRandomDistances(int **matrix, int N) {
    srand(time(NULL));

    int minDistance = 1;  // Distância mínima desejada
    int maxDistance = 100;  // Distância máxima desejada

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) {
                matrix[i][j] = 0;
            } else {
                // Gere valores aleatórios no intervalo de minDistance a maxDistance
                matrix[i][j] = minDistance + rand() % (maxDistance - minDistance + 1);
            }
        }
    }
}

// Função para ler distâncias de um arquivo .txt
void readDistancesFromFile(int **matrix, int *N) {
    FILE *file = fopen("test.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo0.\n");
        exit(1);
    }

    if (fscanf(file, "%d", N) != 1) {
        printf("Erro ao ler o valor de N do arquivo1.\n");
        exit(1);
    }


    for (int i = 0; i < *N; i++) {
        for (int j = 0; j < *N; j++) {
                if(i == j){
                    matrix[i][j] = 0;
                } else {
                    fscanf(file, "%d", &matrix[i][j]);
                }
            /*
            int result = fscanf(file, "%d", &matrix[i][j]);
            if (result != 1) {
                printf("Erro ao ler dados do arquivo: %d\n", result);
                exit(1);
            }
            */
        }
    }

    fclose(file);
}

int main()
{

    printf("Hello world!\n");

    int N;
    scanf("%d", &N);

    int NArq;
    int X;
    int **matrix;

    //printf("iii%d", sumDigits(538053825779));

    //X = sumDigits(12345) % N;

    X = 62 % N;

    matrix = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        matrix[i] = (int *)malloc(N * sizeof(int));
    }


    // Escolha entre gerar aleatoriamente ou ler do arquivo .txt
    int option;
    printf("Escolha a opção (1 para aleatório, 2 para arquivo .txt): ");
    scanf("%d", &option);

    if (option == 1) {
        generateRandomDistances(matrix, N);
        for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]); // Adicione um espaço após cada número
        }
        printf("\n"); // Adicione uma quebra de linha após cada linha da matriz
    }
    } else if (option == 2) {
        readDistancesFromFile(matrix, &NArq);
    } else {
        printf("Opção inválida.\n");
        return 1;
    }

    int cities[N - 1];
    for (int i = 0, j = 0; i < N; i++) {
        if (i != X) {
            cities[j] = i;
            j++;
        }
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    generatePermutations(cities, N - 1, matrix, N, X);

    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("Tempo de execução: %ld segundos e %ld microssegundos\n", seconds, micros);

    // Libere a memória alocada para a matriz
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;

    /*
    int N = 5;
    int X;
    int **matrix;

    X = sumDigits(12345) % N; // Faça a soma de todos os dígitos dos números de matrícula dos integrantes do grupo.Em seguida faça o resto da divisão por N desse valor.

    matrix = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        matrix[i] = (int *)malloc(N * sizeof(int));
    }

    // Leia a matriz de distâncias do arquivo ou gere aleatoriamente
    // readDistancesFromFile(matrix, N);
    generateRandomDistances(matrix, N);

    int cities[N - 1];
    for (int i = 0, j = 0; i < N; i++) {
        if (i != X) {
            cities[j] = i;
            j++;
        }
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    generatePermutations(cities, N - 1, matrix, N, X);

    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("Tempo de execução: %ld segundos e %ld microssegundos\n", seconds, micros);

    // Libere a memória alocada para a matriz
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
    */
}
