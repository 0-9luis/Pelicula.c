#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nodo {
    char nombre[100];
    int anio;
    char genero[50];
    float recaudacion;
    struct Nodo* izquierda;
    struct Nodo* derecha;
} Nodo;

// Función para crear un nuevo nodo
Nodo* crearNodo(char nombre[], int anio, char genero[], float recaudacion) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    strcpy(nuevo->nombre, nombre);
    nuevo->anio = anio;
    strcpy(nuevo->genero, genero);
    nuevo->recaudacion = recaudacion;
    nuevo->izquierda = nuevo->derecha = NULL;
    return nuevo;
}

// Insertar en el árbol
Nodo* insertar(Nodo* raiz, char nombre[], int anio, char genero[], float recaudacion) {
    if (raiz == NULL) {
        return crearNodo(nombre, anio, genero, recaudacion);
    }

    if (anio < raiz->anio || (anio == raiz->anio)) {
        raiz->izquierda = insertar(raiz->izquierda, nombre, anio, genero, recaudacion);
    } else {
        raiz->derecha = insertar(raiz->derecha, nombre, anio, genero, recaudacion);
    }
    return raiz;
}

// Recorridos
void inorden(Nodo* raiz) {
    if (raiz != NULL) {
        inorden(raiz->izquierda);
        printf("%s (%d) - %s - $%.1fM\n", raiz->nombre, raiz->anio, raiz->genero, raiz->recaudacion);
        inorden(raiz->derecha);
    }
}

void preorden(Nodo* raiz) {
    if (raiz != NULL) {
        printf("%s (%d) - %s - $%.1fM\n", raiz->nombre, raiz->anio, raiz->genero, raiz->recaudacion);
        preorden(raiz->izquierda);
        preorden(raiz->derecha);
    }
}

void posorden(Nodo* raiz) {
    if (raiz != NULL) {
        posorden(raiz->izquierda);
        posorden(raiz->derecha);
        printf("%s (%d) - %s - $%.1fM\n", raiz->nombre, raiz->anio, raiz->genero, raiz->recaudacion);
    }
}

// Buscar película por nombre
void buscarPorNombre(Nodo* raiz, char nombre[]) {
    if (raiz == NULL) return;
    
    if (strcmp(raiz->nombre, nombre) == 0) {
        printf("Película encontrada:\n");
        printf("%s (%d) - %s - $%.1fM\n", raiz->nombre, raiz->anio, raiz->genero, raiz->recaudacion);
    }
    
    buscarPorNombre(raiz->izquierda, nombre);
    buscarPorNombre(raiz->derecha, nombre);
}

// Mostrar todas las películas de un género
void mostrarGenero(Nodo* raiz, char genero[]) {
    if (raiz == NULL) return;

    mostrarGenero(raiz->izquierda, genero);
    if (strcmp(raiz->genero, genero) == 0) {
        printf("%s (%d) - %s - $%.1fM\n", raiz->nombre, raiz->anio, raiz->genero, raiz->recaudacion);
    }
    mostrarGenero(raiz->derecha, genero);
}

// Encontrar los 3 fracasos taquilleros
void encontrarFracasos(Nodo* raiz, Nodo** peores, int* contador) {
    if (raiz == NULL) return;

    encontrarFracasos(raiz->izquierda, peores, contador);

    if (*contador < 3) {
        peores[*contador] = raiz;
        (*contador)++;
    } else {
        int maxIndex = 0;
        for (int i = 1; i < 3; i++) {
            if (peores[i]->recaudacion > peores[maxIndex]->recaudacion) {
                maxIndex = i;
            }
        }
        if (raiz->recaudacion < peores[maxIndex]->recaudacion) {
            peores[maxIndex] = raiz;
        }
    }

    encontrarFracasos(raiz->derecha, peores, contador);
}

int main() {
    Nodo* raiz = NULL;

    // Películas de prueba
    raiz = insertar(raiz, "Avatar", 2009, "Ciencia Ficción", 2.8);
    raiz = insertar(raiz, "Titanic", 1997, "Drama", 2.2);
    raiz = insertar(raiz, "Harry Potter", 2001, "Fantasía", 1.0);
    raiz = insertar(raiz, "Dragon Ball", 1986, "Anime", 0.3);

    int opcion;
    char nombre[100];
    char genero[50];
    int anio;
    float recaudacion;

    do {
        printf("\nMenu:\n");
        printf("1. Insertar película\n");
        printf("2. Mostrar Inorden\n");
        printf("3. Mostrar Preorden\n");
        printf("4. Mostrar Posorden\n");
        printf("5. Buscar película por nombre\n");
        printf("6. Mostrar películas por género\n");
        printf("7. Mostrar 3 fracasos taquilleros\n");
        printf("8. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        getchar(); // Limpiar el buffer

        switch (opcion) {
            case 1:
                printf("Nombre de la película: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = '\0'; // Eliminar salto de línea
                printf("Año de realización: ");
                scanf("%d", &anio);
                getchar();
                printf("Género: ");
                fgets(genero, sizeof(genero), stdin);
                genero[strcspn(genero, "\n")] = '\0';
                printf("Dinero recaudado (en millones): ");
                scanf("%f", &recaudacion);
                raiz = insertar(raiz, nombre, anio, genero, recaudacion);
                break;
            case 2:
                printf("\nRecorrido Inorden:\n");
                inorden(raiz);
                break;
            case 3:
                printf("\nRecorrido Preorden:\n");
                preorden(raiz);
                break;
            case 4:
                printf("\nRecorrido Posorden:\n");
                posorden(raiz);
                break;
            case 5:
                printf("Nombre de la película a buscar: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = '\0';
                buscarPorNombre(raiz, nombre);
                break;
            case 6:
                printf("Género a mostrar: ");
                fgets(genero, sizeof(genero), stdin);
                genero[strcspn(genero, "\n")] = '\0';
                printf("\nPelículas de género %s:\n", genero);
                mostrarGenero(raiz, genero);
                break;
            case 7: {
                Nodo* peores[3] = {NULL, NULL, NULL};
                int contador = 0;
                encontrarFracasos(raiz, peores, &contador);
                printf("\n3 Fracasos Taquilleros:\n");
                for (int i = 0; i < 3; i++) {
                    if (peores[i] != NULL) {
                        printf("%s (%d) - %s - $%.1fM\n", peores[i]->nombre, peores[i]->anio, peores[i]->genero, peores[i]->recaudacion);
                    }
                }
                break;
            }
            case 8:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción inválida.\n");
        }

    } while (opcion != 8);

    return 0;
}

