#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define N_NODES 6


typedef struct Node {
	size_t dest, weight;
	struct Node *next;
} Node;

typedef struct {
	Node *nodes[N_NODES];
	bool directed;
} Graph;

typedef struct {
	size_t start, end, weight;
} Edge;


Node* create_node(size_t dest, size_t weight, Node *next)
{
	Node *node = malloc(sizeof(Node));
	node->dest = dest;
	node->weight = weight;
	node->next = next;
	return node;
}


Graph* create_graph_from_edges(Edge *edges, size_t n_edges, bool directed)
{
	Graph *graph = malloc(sizeof(Graph));
	graph->directed = directed;
	for (size_t i = 0; i < N_NODES; ++i)
		graph->nodes[i] = NULL;

	for (size_t i = 0; i < n_edges; ++i) {
		Edge current = edges[i];
		Node *new_node = create_node(current.end, 
									 current.weight,
			                         graph->nodes[current.start]);
		graph->nodes[current.start] = new_node;

		if (!directed) {
			Node *new_node = create_node(current.start,
										 current.weight,
									     graph->nodes[current.end]);
			graph->nodes[current.end] = new_node; 
		}
	}
	return graph;
}


void print_graph(Graph *graph)
{
	if (graph->directed)
		printf("\nAdjacency lists for directed graph:\n");
	else
		printf("\nAdjacency lists for undirected graph:\n");

	for (size_t i = 0; i < N_NODES; ++i) {
		Node *node = graph->nodes[i];
		printf("%lu: ", i);
		while (node != NULL) {
			printf("(%lu -> %lu (%lu))  ", i, node->dest, node->weight);
			node = node->next;
		} 
		printf("\n");
	}
}


void delete_graph(Graph *graph)
{
	for (size_t i = 0; i < N_NODES; ++i) {
		Node *current = graph->nodes[i];
		while (current != NULL) {
			Node *temp = current->next;
			free(current);
			current = temp;
		}
	}
	free(graph);
}


bool all_visited(bool *visited)
{
	for (size_t i = 0; i < N_NODES; ++i){
		if (!visited[i])
			return false;
	}
	return true;
}


/*
Dijkstra implelementation written from scratch, 
based on explanation section found here:
https://iq.opengenus.org/dijkstras-algorithm-finding-shortest-path-between-all-nodes/
*/
void dijkstra_shortest_path(Graph *graph, size_t starting_node)
{
	assert(starting_node < N_NODES);

	size_t current_node = starting_node;

	bool visited[N_NODES] = {false};
	size_t distances[N_NODES];
	for (size_t i = 0; i < N_NODES; ++i)
		distances[i] = SIZE_MAX;

	distances[starting_node] = 0;

	while(!all_visited(visited)) {
		visited[current_node] = true;
		Node *adj_list = graph->nodes[current_node];
		while (adj_list != NULL) {
			if (distances[adj_list->dest] > adj_list->weight + distances[current_node])
				distances[adj_list->dest] = adj_list->weight + distances[current_node];
			adj_list = adj_list->next;
		}

		Node *min_node;
		size_t min_dist = SIZE_MAX;
		for (size_t i = 0; i < N_NODES; ++i) {
			if (!visited[i])
				continue;
			Node *cur = graph->nodes[i];
			while(cur != NULL) {
				if (!visited[cur->dest] && distances[cur->dest] < min_dist) {
					min_node = cur;
					min_dist = distances[cur->dest];
				}
				cur = cur->next;
			}
		}
		current_node = min_node->dest;
	}

	printf("\nShortest distance from node %lu:\n", starting_node);
	for(size_t i = 0; i < N_NODES; ++i) {
		printf("%lu: %lu\n", i, distances[i]);
	}
}


int main(void)
{

	Edge edges[] = {
		{0, 2, 5}, {0, 3, 1}, {1, 2, 2}, 
		{2, 4, 2}, {3, 1, 7}, {4, 0, 5}, 
		{5, 4, 1}, {5, 3, 4}, {1, 5, 1}
	};

	size_t blocks = sizeof(edges) / sizeof(Edge);

	Graph *graph = create_graph_from_edges(edges, blocks, true);
	print_graph(graph);
	dijkstra_shortest_path(graph, 2);
	delete_graph(graph);

	graph = create_graph_from_edges(edges, blocks, false);
	print_graph(graph);
	dijkstra_shortest_path(graph, 2);
	delete_graph(graph);

	sizeof graph;

	return 0;
}
