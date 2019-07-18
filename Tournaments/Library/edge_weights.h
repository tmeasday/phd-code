typedef struct
{
	float** weights;
	int size;
} edge_weights;

edge_weights create_edge_weights (int size);
float get_edge_weight (edge_weights ew, int i, int j);
void inc_edge_weight (edge_weights ew, int i, int j, float inc);
void set_edge_weight (edge_weights ew, int i, int j, float weight);