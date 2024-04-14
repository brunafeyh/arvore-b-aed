#include <stdio.h>
#include <stdlib.h>

#define ORDEM 3

typedef struct no {
    int numChaves;
    int chave[ORDEM];
    struct no* filho[ORDEM+1];
} arvoreB;

int vazia(arvoreB* arv) {
    if (arv == NULL || arv->numChaves == 0)
        return 1; // Árvore está vazia
    else
        return 0; // Árvore não está vazia
}

int overflow(arvoreB* arv) {
    if (arv->numChaves >= ORDEM)
        return 1; // Árvore está em overflow
    else
        return 0; // Árvore não está em overflow
}

void adicionaDireita(arvoreB* r, int pos, int k, arvoreB* p){
    int i;
    for(i=r->numChaves; i>pos; i--){
        r->chave[i] = r->chave[i-1];
        r->filho[i+1] = r->filho[i];
    }
    r->chave[pos] = k;
    r->filho[pos+1] = p;
    r->numChaves++;
}

arvoreB* split(arvoreB* r, int * m) {
    arvoreB* y = (arvoreB*) malloc(sizeof(arvoreB));
    int q = r->numChaves/2;
    y->numChaves = r->numChaves - q - 1;
    r->numChaves = q;
    *m = r->chave[q];
    int i = 0;
    y->filho[0] = r->filho[q+1];
    for(i = 0; i < y->numChaves; i++){
        y->chave[i] = r->chave[q+i+1];
        y->filho[i+1] = r->filho[q+i+2];
    }
    return y;
}

int buscaPos(arvoreB* r, int info, int * pos) {
    *pos = 0; // Inicializa pos
    for((*pos)=0; (*pos) < r->numChaves; (*pos)++){
        if(info == r->chave[(*pos)])
            return 1; 
        else if(info < r->chave[(*pos)]) break; 
    }
    return 0; 
}

arvoreB* busca(arvoreB* r, int info, int * pos){
    if(r == NULL) return NULL;
    int i = 0;
    while(i < r->numChaves && r->chave[i] < info) i++;
    if(i == r->numChaves || r->chave[i] > info) return busca(r->filho[i], info, pos);
    *pos = i;
    return r;
}

int eh_folha(arvoreB* r) {
    return (r->filho[0] == NULL);
}
void insere_aux(arvoreB* r, int info){
    int pos;
        if(!buscaPos(r,info, &pos)){ // chave n~ao est´a no n´o r
        if(eh_folha(r)) {
        adicionaDireita(r,pos,info,NULL);
        }
            else {
            insere_aux(r->filho[pos],info);
            if(overflow(r->filho[pos])){
                int m; // valor da chave mediana
                arvoreB* aux = split(r->filho[pos],&m);
                adicionaDireita(r,pos,m,aux);
            }
        }
    }
}


arvoreB* insere(arvoreB* r, int info){
    if(vazia(r)) {
        r = malloc(sizeof(arvoreB));
        r->chave[0] = info;
        for(int i = 0; i < ORDEM; i++)
        r->filho[i] = NULL;
        r->numChaves = 1;
    }
    else {
        insere_aux(r,info);
        if(overflow(r)){ // verifica se precisa split
            int m;
            arvoreB* x = split(r,&m);
            arvoreB* novaRaiz = malloc(sizeof(arvoreB));
            novaRaiz->chave[0] = m;
            novaRaiz->filho[0] = r;
            novaRaiz->filho[1] = x;
            for(int i = (((int)ORDEM/2)+1); i < ORDEM; i++)
            r->filho[i] = NULL;
            novaRaiz->numChaves = 1;
            return novaRaiz;
        }
    }
}

int contanosminchaves(arvoreB *r) {
    if (r == NULL) return 0;
    
    int c = 0;
    
    // Verifica se o nó atual tem o número mínimo de chaves
    if (r->numChaves < ORDEM/2 + ORDEM%2 -1) c++;
    
    
    for (int i = 0; i <= r->numChaves; i++) {
        c += contanosminchaves(r->filho[i]);
    }
    
    return c;
}

// Função para percorrer a árvore B em ordem
void inOrdem(arvoreB *raiz) {
    if (raiz != NULL) {
        for (int i = 0; i < raiz->numChaves; i++) {
            inOrdem(raiz->filho[i]);
            printf("%d ", raiz->chave[i]);
        }
        inOrdem(raiz->filho[raiz->numChaves]);
    }
}

// Função para encontrar o valor máximo na árvore B
int maximo(arvoreB *raiz) {
    while (raiz->filho[raiz->numChaves] != NULL) {
        raiz = raiz->filho[raiz->numChaves];
    }
    return raiz->chave[raiz->numChaves - 1];
}

// Função para encontrar o valor mínimo na árvore B
int minimo(arvoreB *raiz) {
    while (raiz->filho[0] != NULL) {
        raiz = raiz->filho[0];
    }
    return raiz->chave[0];
}

// Função para calcular a altura da árvore B
int altura(arvoreB *raiz) {
    if (raiz == NULL) {
        return -1;
    }
    int maxAltura = -1;
    for (int i = 0; i <= raiz->numChaves; i++) {
        int alturaFilho = altura(raiz->filho[i]);
        if (alturaFilho > maxAltura) {
            maxAltura = alturaFilho;
        }
    }
    return maxAltura + 1;
}

// Função para calcular a soma de todas as chaves na árvore B
int soma(arvoreB *raiz) {
    if (raiz == NULL) {
        return 0;
    }
    int somaChaves = 0;
    for (int i = 0; i < raiz->numChaves; i++) {
        somaChaves += raiz->chave[i];
        somaChaves += soma(raiz->filho[i]);
    }
    somaChaves += soma(raiz->filho[raiz->numChaves]);
    return somaChaves;
}

// Função auxiliar para contar o número de nós na árvore B
int numNos(arvoreB *r) {
    if (r == NULL) return 0;
    
    int contador = 1;
    for (int i = 0; i <= r->numChaves; i++) {
        contador += numNos(r->filho[i]);
    }
    return contador;
}

// Função para contar o número total de chaves em uma árvore B
int contarChaves(arvoreB* raiz) {
    if (raiz == NULL)
        return 0;

    int totalChaves = raiz->numChaves; // Contabiliza as chaves no nó atual

    // Percorre os filhos do nó atual
    for (int i = 0; i <= raiz->numChaves; i++) {
        totalChaves += contarChaves(raiz->filho[i]);
    }

    return totalChaves;
}

void imprimir_chaves_no(arvoreB* r, int a, int b) {
    if (r == NULL) return;
    
    if (r->chave[0] >= b) {
        imprimir_chaves_no(r->filho[0], a, b);
    } else if (r->chave[r->numChaves - 1] <= a) {
        imprimir_chaves_no(r->filho[r->numChaves], a, b);
    } else {
        for (int i = 0; i < r->numChaves; i++) {
            imprimir_chaves_no(r->filho[i], a, b);
            if (r->chave[i] >= a && r->chave[i] <= b) {
                printf("%d ", r->chave[i]);
            }
        }
        imprimir_chaves_no(r->filho[r->numChaves], a, b);
    }
}

// Função auxiliar para contar a quantidade de folhas na árvore B
int qdeFolhas(arvoreB *raiz) {
    if (raiz == NULL) {
        return 0;
    }
    if (raiz->filho[0] == NULL) {
        return 1;
    }
    int contador = 0;
    for (int i = 0; i <= raiz->numChaves; i++) {
        contador += qdeFolhas(raiz->filho[i]);
    }
    return contador;
}


// Função auxiliar para encontrar a chave seguinte na árvore B
int proximaChave(arvoreB* r, int val) {
    if (r != NULL) {
        int i;
        if (r->filho[0] == NULL) {
            for (i = 0; i < r->numChaves; i++) {
                if (r->chave[i] > val) return r->chave[i];
            }
            return -1;
        }

        for (i = 0; i < r->numChaves; i++) {
            if (r->chave[i] > val){
                if(r->filho[i] == NULL) return r->chave[i];
                return proximaChave(r->filho[i], val);
            }
        }

        return proximaChave(r->filho[i], val);
    }

    return -1;
}


// Função auxiliar para encontrar a chave anterior na árvore B
int chaveAnterior(arvoreB *raiz, int chave) {
    if (raiz == NULL) {
        return -1; // Valor sentinela para indicar que a chave não foi encontrada
    }
    int i = 0;
    while (i < raiz->numChaves && chave > raiz->chave[i]) {
        i++;
    }
    if (i < raiz->numChaves && chave == raiz->chave[i]) {
        if (raiz->filho[i] != NULL) {
            // Se a chave está presente no nó, retorna a última chave no nó filho à esquerda
            arvoreB *no = raiz->filho[i];
            while (no->filho[no->numChaves] != NULL) {
                no = no->filho[no->numChaves];
            }
            return no->chave[no->numChaves - 1];
        } else {
            // Se a chave está presente no nó, mas não há filho à esquerda, retornamos -1
            return -1;
        }
    } else {
        // Se a chave não está presente no nó, chamamos a função recursivamente para o filho adequado
        return chaveAnterior(raiz->filho[i], chave);
    }
}

int busca_k_esima(arvoreB* r, int k) {
    if (r == NULL || k <= 0 || k > contarChaves(r)) return -1;

    int tam_arvore_esq = (r->filho[0] != NULL) ? contarChaves(r->filho[0]) : 0;
    int i;

    for (i = 0; i < r->numChaves; i++) {
        if (k <= tam_arvore_esq) { // a chave está no filho
            return busca_k_esima(r->filho[i], k);
        } else if (k == tam_arvore_esq + 1) { // Chave está aqui
            return r->chave[i];
        } else {
            k -= tam_arvore_esq + 1;
            tam_arvore_esq = (r->filho[i + 1] != NULL) ? contarChaves(r->filho[i + 1]) : 0;
        }
    }
    return busca_k_esima(r->filho[i], k);
}

void redistribuirChaves(arvoreB* pai, int pos_filho_esq) {
    arvoreB* filho_esq = pai->filho[pos_filho_esq];
    arvoreB* filho_dir = pai->filho[pos_filho_esq + 1];

    if (filho_esq->numChaves < (ORDEM/2)) {
        filho_esq->chave[filho_esq->numChaves] = pai->chave[pos_filho_esq];
        filho_esq->numChaves++;

        pai->chave[pos_filho_esq] = filho_dir->chave[0];

        filho_esq->filho[filho_esq->numChaves] = filho_dir->filho[0];

        for (int i = 0; i < filho_dir->numChaves - 1; i++) {
            filho_dir->chave[i] = filho_dir->chave[i + 1];
            filho_dir->filho[i] = filho_dir->filho[i + 1];
        }
        
        filho_dir->filho[filho_dir->numChaves - 1] = filho_dir->filho[filho_dir->numChaves];

        filho_dir->numChaves--;
    }
    
    else {
       
        for (int i = filho_dir->numChaves; i > 0; i--) {
            filho_dir->chave[i] = filho_dir->chave[i - 1];
            filho_dir->filho[i + 1] = filho_dir->filho[i];
        }
        
        filho_dir->filho[1] = filho_dir->filho[0];
        filho_dir->filho[0] = filho_esq->filho[filho_esq->numChaves];

        
        filho_dir->chave[0] = pai->chave[pos_filho_esq];
        filho_dir->numChaves++;

      
        pai->chave[pos_filho_esq] = filho_esq->chave[filho_esq->numChaves - 1];

        filho_esq->numChaves--;
    }
}

int main() {
    arvoreB* raiz = NULL;


    raiz = insere(raiz, 1);
    raiz = insere(raiz, 0);
    
    printf("Imprimindo in ordem : \n");
    
    inOrdem(raiz);

    printf("Altura da arvore: %d\n", altura(raiz));
    printf("Numero de chaves na arvore: %d\n", contarChaves(raiz));
    printf("Numero de nos na arvore: %d\n", numNos(raiz));
    printf("Soma das chaves na arvore: %d\n", soma(raiz));
    printf("Quantidade de folhas na arvore: %d\n", qdeFolhas(raiz));

    // Impressão em ordem
    printf("Chaves da arvore em ordem: ");
    inOrdem(raiz);
    printf("\n");

    // Exemplo de busca por uma chave
    int chave = 10;
    int pos;
    if (busca(raiz, chave, &pos) != NULL) {
        printf("Chave %d encontrada na posicao %d.\n", chave, pos);
    } else {
        printf("Chave %d nao encontrada na arvore.\n", chave);
    }

    // Exemplo de busca pela k-ésima chave
    int k = 5;
    int k_esima = busca_k_esima(raiz, k);
    if (k_esima != -1) {
        printf("A %d-esima chave na arvore e: %d\n", k, k_esima);
    } else {
        printf("Nao ha %d chaves na arvore.\n", k);
    }

    // Exemplo de busca pelo valor mínimo e máximo
    printf("Valor minimo na arvore: %d\n", minimo(raiz));
    printf("Valor maximo na arvore: %d\n", maximo(raiz));

    // Exemplo de busca pela chave anterior e seguinte
    int chave_anterior = 8;
    printf("Chave anterior a %d: %d\n", chave_anterior, chaveAnterior(raiz, chave_anterior));
    int chave_seguinte = 12;
    printf("Chave seguinte a %d: %d\n", chave_seguinte, proximaChave(raiz, chave_seguinte));

    // Exemplo de impressão das chaves no intervalo [a, b]
    int a = 5, b = 15;
    printf("Chaves no intervalo [%d, %d]: ", a, b);
    imprimir_chaves_no(raiz, a, b);
    printf("\n");

    return 0;
}
