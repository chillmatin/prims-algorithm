#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;

/**
 * A vertex is a structure that contains an integer id and a boolean is_shootable.
 * @property {int} id - The id of the vertex.
 * @property {bool} is_shootable - If the vertex is shootable, then it is a vertex that can be shot at.
 */
struct Vertex
{
    int id;
    bool is_shootable;
};

/**
 * A vertex is a pair of a string and a vector of edges.
 * @property {Vertex} from - The vertex that the edge is coming from.
 * @property {int} to - the index of the vertex that the edge is pointing to
 * @property {double} weight - the weight of the edge
 */
struct Edge
{
    Vertex from;
    int to;
    double weight;
};

/**
 * It takes a string (the path of a csv file) and returns a vector of strings (each line of the csv
 * file)
 *
 * @param csv_file the name of the csv file you want to read
 *
 * @return A vector of strings
 */
vector<string> csv_to_vector(string csv_file)
{
    vector<string> result;
    string line;
    ifstream myfile(csv_file);
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            result.push_back(line);
        }
        myfile.close();
    }
    else
        cout << "Unable to open file";

    return result;
}

/**
 * It takes a csv file and returns a vector of vectors of doubles
 *
 * @param csv_file the name of the csv file to be read
 *
 * @return A vector of vectors of doubles.
 */
vector<vector<double>> csv_to_double_vector(string csv_file)
{
    vector<vector<double>> result;
    vector<string> lines = csv_to_vector(csv_file);
    for (int i = 0; i < lines.size(); i++)
    {
        vector<double> line;
        stringstream ss(lines[i]);
        string item;
        while (getline(ss, item, ','))
        {
            line.push_back(stod(item));
        }
        result.push_back(line);
    }
    return result;
}

/**
 * It takes a vector of strings as an argument and prints each string in the vector on a new line
 *
 * @param v the vector to print
 */
void print_vector(vector<string> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i] << endl;
    }
}

/**
 * `print_csv` takes a string (the path of a csv file) and prints the contents of the csv file to the
 * screen
 *
 * @param csv_file the name of the csv file you want to read
 */
void print_csv(string csv_file)
{

    vector<string> v = csv_to_vector(csv_file);
    print_vector(v);
}

/**
 * It takes a csv file and returns a map of vertices to edges
 *
 * @param csv_file the name of the csv file that contains the graph
 *
 * @return A map of vectors of edges.
 */
unordered_map<int, vector<Edge>> csv_to_graph_map(string csv_file)
{
    unordered_map<int, vector<Edge>> result;
    vector<vector<double>> lines = csv_to_double_vector(csv_file);
    int id;

    for (int i = 0; i < lines.size(); i++)
    {
        Vertex from;
        from.id = i;
        if (lines[i][i] != 0)
        {
            from.is_shootable = true;
        }
        else
        {
            from.is_shootable = false;
        }
        vector<Edge> edges;
        for (int j = 0; j < lines.size(); j++)
        {
            Edge edge;

            edge.from = from;
            edge.to = j;
            edge.weight = lines[i][j];
            edges.push_back(edge);
        }

        result.insert({from.id, edges});
    }
    return result;
}

/**
 * Returns true if x is in v, false otherwise.
 *
 * @param v the vector to search
 * @param x the value we're looking for
 *
 * @return A boolean value.
 */
bool contains(vector<int> v, int x)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] == x)
        {
            return true;
        }
    }
    return false;
}

/**
 * It finds the minimum edge that is not in the visited list
 *
 * @param visited a vector of ints that represent the nodes that have already been visited
 * @param edges a vector of edges
 *
 * @return The minimum edge that is not in the visited list.
 */
Edge find_min_edge(vector<int> visited, vector<Edge> edges)
{
    double min_weight = 999999.0;
    Edge min_edge;
    for (int i = 0; i < edges.size(); i++)
    {
        if (edges[i].from.is_shootable)
        {
            return edges[i];
        }
        if (edges[i].weight < min_weight && edges[i].weight != 0 && !contains(visited, edges[i].to))
        {
            min_weight = edges[i].weight;
            min_edge = edges[i];
        }
    }
    return min_edge;
}

/**
 * We start at the start node, and then we find the shortest edge to a node that we haven't visited
 * yet. We keep doing this until we find a node that is shootable
 *
 * @param graph a graph represented as an adjacency list
 * @param start the starting node
 *
 * @return A vector of ints
 */
vector<int> shortest_path(unordered_map<int, vector<Edge>> graph, int start)
{
    vector<int> visited;
    vector<Edge> edges;
    int to = start;
    Edge min_edge;
    for (int i = 0; i < graph.size(); i++)
    {
        visited.push_back(to);
        edges = graph[to];
        min_edge = find_min_edge(visited, edges);
        to = min_edge.to;
        if (min_edge.from.is_shootable)
        {
            return visited;
        }
    }

    return visited;
}

/**
 * It takes a filepath to a csv file and a starting node, and prints the shortest path from the
 * starting node to every other node in the graph
 *
 * @param filepath the path to the csv file
 * @param start the starting node
 */
void print_path(string filepath, int start)
{
    vector<int> path = shortest_path(csv_to_graph_map(filepath), start);
    for (int i = 0; i < path.size(); i++)
    {
        cout << path[i] << " ";
    }
    cout << "\n";
}

int main(int argc, char **argv)
{
    string file = "../data/times.csv";
    int player = atoi(argv[1]);

    print_csv(file);
    print_path(file, player);
}
