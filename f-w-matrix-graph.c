#include <stdio.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_STRING_SIZE	256
#define INF INT_MAX

/* --- MATRIX GRAPH IMPLEMENTATION START --- */

typedef struct
{
    int** matrix; /* Matricea de adiacenta a grafului */
    int nodes;    /* Numarul de noduri din graf. */
} matrix_graph_t;

/**
 * Initializeaza graful cu numarul de noduri primit ca parametru si aloca
 * memorie pentru matricea de adiacenta a grafului.
 */
 
/* Graful este ORIENTAT */
 
/* Nodurile sunt indexate de la 0.*/
 
matrix_graph_t*
mg_create(int nodes)
{
    matrix_graph_t * graph = malloc(sizeof(matrix_graph_t));
	if (!graph)
		exit(-1);

	graph->nodes = nodes;

	graph->matrix = malloc(nodes * sizeof(int *));
	if (!graph->matrix)
		exit(-1);

	for (int i = 0; i < nodes; ++i) {
		graph->matrix[i] = malloc(nodes * sizeof(int));
		if (!graph->matrix[i])
			exit(-1);

		for (int j = 0; j < nodes; ++j)
			if (i == j)
				graph->matrix[i][j] = 0;
			else
				graph->matrix[i][j] = INF;
	}

	return graph;
}

/* Adauga o muchie intre nodurile sursa si destinatie (salvand costul) */
void
mg_add_edge(matrix_graph_t* graph, int src, int dest, int cost)
{
    graph->matrix[src][dest] = cost;
}

/* Returneaza 1 daca exista muchie intre cele doua noduri, 0 in caz contrar */
int
mg_has_edge(matrix_graph_t* graph, int src, int dest)
{
    if (graph->matrix[src][dest] != INF && graph->matrix[src][dest])
		return 1;
	return 0;
}

/* Elimina muchia dintre nodurile sursa si destinatie */
void
mg_remove_edge(matrix_graph_t* graph, int src, int dest)
{
	if (src != dest)
    	graph->matrix[src][dest] = INF;
}

/* Elibereaza memoria folosita de matricea de adiacenta a grafului */
void
mg_free(matrix_graph_t* graph)
{
	for (int i = 0; i < graph->nodes; ++i)
		free(graph->matrix[i]);

	free(graph->matrix);

	free(graph);
}

/* Printeaza matricea ce memoreaza costul (graful in sine) */
void
print_matrix_graph(matrix_graph_t* mg)
{
    for (int i = 0; i < mg->nodes; ++i) {
		printf("\t");
		for (int j = 0; j < mg->nodes; ++j)
			if (mg->matrix[i][j] == INF)
				printf("INF\t");
			else
				printf("%d\t", mg->matrix[i][j]);
		printf("\n");
	}
}

/* Printeaza matricea ce memoreaza distantele */
void
print_dist_matrix(matrix_graph_t *mg, int **d)
{
	printf("The distance matrix is:\n");
	for (int i = 0; i < mg->nodes; ++i) {
		printf("\t");
		for (int j = 0; j < mg->nodes; ++j)
			if (d[i][j] == INF)
				printf("INF\t");
			else
				printf("%d\t", d[i][j]);
		printf("\n");
	}
}

/* Printeaza caile cele mai scurte */
void
print_paths(matrix_graph_t *mg, int **paths)
{
	for (int i = 0; i < mg->nodes; ++i) {
		printf("Paths from node %d:\n", i);
		for (int j = 0; j < mg->nodes; ++j)
			if (i != j) {
				int u = i;
				int v = j;

				if (paths[u][v] == -1) {
					printf("\tThere is no path to node %d\n", v);
				} else {
					printf("\tThe best path to node %d is: %d ", v, u);

					while (u != v) {
						u = paths[u][v];
						printf("%d ", u);
					}
					printf("\n");
				}
			}
	}
}

/* Initializeaza F-W */
void
init_f_w(matrix_graph_t *mg,int ***d, int ***paths)
{
	*d = (int **)malloc(mg->nodes * sizeof(int *));

	if (!(*d))
		exit(-1);

	for (int i = 0; i < mg->nodes; ++i) {
		(*d)[i] = (int *)malloc(mg->nodes * sizeof(int));

		if (!(*d)[i])
			exit(-1);

		for (int j = 0; j < mg->nodes; ++j)
			(*d)[i][j] = mg->matrix[i][j];
	}

	*paths = (int **)malloc(mg->nodes * sizeof(int *));

	if (!(*d))
		exit(-1);

	for (int i = 0; i < mg->nodes; ++i) {
		(*paths)[i] = (int *)malloc(mg->nodes * sizeof(int));

		if (!(*paths)[i])
			exit(-1);

		for (int j = 0; j < mg->nodes; ++j)
			if (mg->matrix[i][j] == INF)
				(*paths)[i][j] = -1;
			else
				(*paths)[i][j] = j;
	}
}

/* Elibereaza memoria alocata dinamic pentru F-W */
void
free_f_w(matrix_graph_t *mg, int **d, int **paths)
{
	for (int i = 0; i < mg->nodes; ++i)
		free(d[i]);
	free(d);

	for (int i = 0; i < mg->nodes; ++i)
		free(paths[i]);
	free(paths);
}

/* Implementeaza algoritmul F-W */
void
floyd_warshall_matrix(matrix_graph_t *mg)
{
	int **d;

	int **paths;

	init_f_w(mg, &d, &paths);
	
	for (int k = 0; k < mg->nodes; ++k) {
		for (int i = 0; i < mg->nodes; ++i) {
			for (int j = 0; j < mg->nodes; ++j) {
				if ((d[i][k] != INF) && (d[k][j] != INF) && (d[i][k] + d[k][j] < d[i][j])) {
					d[i][j] = d[i][k] + d[k][j];
					paths[i][j] = paths[i][k];
				}
			}
		}
	}

	print_dist_matrix(mg, d);

	print_paths(mg, paths);

	free_f_w(mg, d, paths);
}

int main(void)
{

    matrix_graph_t *mg = NULL;

    while (1) {
        char command[MAX_STRING_SIZE];
        int nr1, nr2, cost;
        int nr_nodes;

        scanf("%s", command);

        if (strncmp(command, "create_mg", 9) == 0) {
            scanf("%d", &nr_nodes);
            mg = mg_create(nr_nodes);
        }

        if (strncmp(command, "add_edge", 8) == 0) {
            if (mg != NULL) {
                scanf("%d %d %d", &nr1, &nr2, &cost);

                mg_add_edge(mg, nr1, nr2, cost);
            } else {
                printf("Create a graph first!\n");
                exit(0);
            }
        }

        if (strncmp(command, "remove_edge", 11) == 0) {
            if (mg != NULL) {
                scanf("%d %d", &nr1, &nr2);
                mg_remove_edge(mg, nr1, nr2);
            } else {
                printf("Create a graph first!\n");
                exit(0);
            }
        }

        if (strncmp(command, "print_graph", 11) == 0) {
            if (mg != NULL) {
                print_matrix_graph(mg);
            } else {
                printf("Create a graph first!\n");
                exit(0);
            }
        }

        if (strncmp(command, "has_edge", 8) == 0) {
            if (mg != NULL) {
                int flag;
                scanf("%d %d", &nr1, &nr2);
                flag = mg_has_edge(mg, nr1, nr2);
                if (flag == 1) {
                    printf("Has edge\n");
                }
                else if (flag == 0) {
                    printf("No edge\n");
                }
            } else {
                printf("Create a graph first!\n");
                exit(0);
            }
        }

		if (strncmp(command, "f-w", 3) == 0) {
			if (mg != NULL) {
				floyd_warshall_matrix(mg);
			} else {
				printf("Create a graph first!\n");
                exit(0);
			}
		}

        if (strncmp(command, "free", 4) == 0) {
            if (mg != NULL) {
                mg_free(mg);
            } else {
                printf("Create a graph first!\n");
                exit(0);
            }
            break;
        }
    }

    return 0;
}
