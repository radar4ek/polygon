#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <stack>
#include <queue>
#include <chrono>
#include <cassert>
using namespace std;

struct Node {
    double lon, lat;
    std::vector<std::pair<Node*, double>> edges;
};

struct Graph {
public:
    Node* FindClosestNode(double lat, double lon){
        double min_distance = 999999;
        Node* node_founded = nullptr;

        for (auto node : nodes){
            double distance = std::sqrt(std::pow(node -> lat - lat, 2) + std::pow(node->lon - lon,2));
            if (distance < min_distance){
                node_founded = node;
                min_distance = distance;
            }
        }
        return node_founded;

    }
    void ReadFromFile(std::ifstream& file){
        std::string line;
        
        while (file >> line)
        {
            ParsedNode parsed_nodes= Parsing(line);
            Node* node1;
            Node* node2;
            Node* node3;
            if (cords_node.count({parsed_nodes.lon1,parsed_nodes.lat1}) == 0){
                node1 = new Node();
                node1->lon = parsed_nodes.lon1;
                node1->lat = parsed_nodes.lat1;
                cords_node[{parsed_nodes.lon1,parsed_nodes.lat1}] = node1;
                nodes.push_back(node1);
            }else{
                node1 = cords_node[{parsed_nodes.lon1,parsed_nodes.lat1}];
            }
            if (cords_node.count({parsed_nodes.lon2,parsed_nodes.lat2}) == 0){
                node2 = new Node();
                node2->lon = parsed_nodes.lon2;
                node2->lat = parsed_nodes.lat2;
                cords_node[{parsed_nodes.lon2,parsed_nodes.lat2}] = node2;
                nodes.push_back(node2);
            }else{
                node2 = cords_node[{parsed_nodes.lon2,parsed_nodes.lat2}];
            }
            if (cords_node.count({parsed_nodes.lon3,parsed_nodes.lat3}) == 0){
                node3 = new Node();
                node3->lon = parsed_nodes.lon3;
                node3->lat = parsed_nodes.lat3;
                cords_node[{parsed_nodes.lon3,parsed_nodes.lat3}] = node3;
                nodes.push_back(node3);
            }else{
                node3 = cords_node[{parsed_nodes.lon3,parsed_nodes.lat3}];
            }
            node1->edges.push_back({node2,parsed_nodes.weight2});
            node1->edges.push_back({node3,parsed_nodes.weight3});
            node2->edges.push_back({node1,parsed_nodes.weight2});
            node3->edges.push_back({node1,parsed_nodes.weight3});            
        }

        
        
    }
    std::pair<std::vector<Node*>,double> DFS(Node* start, Node* end) {
        if (!start || !end) return {{},-1}; 
        std::stack<Node*> stack; // O(V) памяти в худшем случае
        std::map<Node*, Node*> previous; // чтобы востановить путь, O(V) памяти
        map<Node*,double> distances; // отслеживаем дистанцию от старта до каждой точки, O(V) памяти
        distances[start] = 0; //инициализация стартовой точки 
        previous[start] = nullptr;
        stack.push(start);
        while (!stack.empty()) { // O(V + E) сложность в худшем случае чтобы дойти до нужной вершины(перебор всех верешин и всех соседей)
            Node* current_node = stack.top();
            stack.pop();
            if (current_node == end){ // нашли путь
                break;
            }
            for (auto& edge : current_node->edges) { // перебираем соседей, O(E) в худшем случае 
                Node* neighbor = edge.first;
                double weight = edge.second;
                if (previous.count(neighbor) == 0) { // если не посещали вершину
                    distances[neighbor] = distances[current_node] + weight;
                    previous[neighbor] = current_node;
                    stack.push(neighbor);
                }
            }   

        }
        std::vector<Node*> path; // востанавливаем путь
        Node* node = end;
        while (node != nullptr){ // O(v) сложность
            path.push_back(node);
            node = previous[node];
        }
        reverse(path.begin(),path.end());
        if (path.size() == 1){
            return {{},-1};
        }
        return {path,distances[end]};

    }
    // O(V+E) ассимптотика 
    // O(V) памяти

    std::pair<std::vector<Node*>,double> BFS(Node* start, Node* end) {
        if (!start || !end) return {{},-1};
        std::queue<Node*> queue; // O(V) памяти в худшем случае
        std::map<Node*, Node*> previous; // чтобы востановить путь,O(V) памяти
        map<Node*,double> distances; // отслеживаем дистанцию от старта до каждой точки,O(V) памяти
        queue.push(start); //инициализация стартовой точки 
        distances[start] = 0;
        previous[start] = nullptr;
        while (!queue.empty()) {// O(V + E) сложность в худшем случае чтобы дойти до нужной вершины(перебор всех верешин и всех соседей)
            auto current_node = queue.front();
            queue.pop();
            if (current_node == end){// нашли путь
                break;
            }
            for (auto& edge : current_node->edges) {// перебираем соседей, O(E) в худшем случае 
                Node* neighbor = edge.first;
                double weight = edge.second;
                if (previous.count(neighbor) == 0) { // если не посещали вершину
                    distances[neighbor] = distances[current_node] + weight;
                    previous[neighbor] = current_node;
                    queue.push(neighbor);
                }
                    
            }            
        }
        std::vector<Node*> path;
        Node* node = end;
        while (node != nullptr){ // O(V)
            path.push_back(node);
            node = previous[node];
        }
        reverse(path.begin(),path.end());
        if (path.size() == 1){
            return {{},-1};
        }
        return {path,distances[end]};
    }
    // O(V+E) ассимптотика 
    // O(V) памяти


    std::pair<std::vector<Node*>,double> Dijkstra(Node* start, Node* end) {
        // приоретеная очередь
        std::priority_queue<std::pair<double, Node*>, std::vector<std::pair<double, Node*>>, std::greater<>> pqueue;//O(V) памяти
        std::map<Node*, double> distances;//O(V) памяти
        std::map<Node*, Node*> previous;//O(V) памяти
        for (auto* node : nodes) {//O(V) 
            distances[node] = std::numeric_limits<double>::max();
        }
        distances[start] = 0;
        pqueue.push({0, start});
        previous[start] = nullptr;
        double ans;
        while (!pqueue.empty()) {//O(V)
            auto [current_distance, current_node] = pqueue.top();
            pqueue.pop();//O(log(V))
            if (current_node == end) { // нашли путь
                ans = current_distance;
                break;
            }
            if (current_distance > distances[current_node]) continue; // путь до текущей точки не минимальный 
            for (auto& edge : current_node->edges) {// O(E)
                Node* neighbor = edge.first;
                double weight = edge.second;
                double new_distance = current_distance + weight;
                if (new_distance < distances[neighbor]) {// если нашли путь короче чем было тогда обновляем distances и previous
                    distances[neighbor] = new_distance;  // и снова добавляем в очередь
                    previous[neighbor] = current_node;
                    pqueue.push({new_distance, neighbor});//O(log(V))
                }
            }
        }
        std::vector<Node*> path; // востанавливаем путь
        Node* node = end;
        while (node != nullptr){//O(V)
            path.push_back(node);
            node = previous[node];
        }
        reverse(path.begin(),path.end());
        if (path.size() == 1){
            return {{},-1};
        }
        return {path,distances[end]};
    }
    // ассмптотика O(VlogV + Elog(V)) = O(log(V)(E+V))
    // память O(V)

    std::pair<std::vector<Node*>,double> AStar(Node* start, Node* end) {
        //почти тоже самое что и Dijkstra
        std::priority_queue<std::pair<double, Node*>, std::vector<std::pair<double, Node*>>, std::greater<>> pqueue;// O(V) память
        std::map<Node*, double> distances; // O(V) память
        std::map<Node*, double> heuristic_distances; // O(V) память
        std::map<Node*, Node*> previous;// O(V) память
        for (auto* node : nodes) {// O(V) 
            distances[node] = std::numeric_limits<double>::max();
            heuristic_distances[node] = std::numeric_limits<double>::max();
        }
        distances[start] = 0;
        heuristic_distances[start] = Heuristic(start, end);
        previous[start] = nullptr;
        double ans;
        pqueue.push({heuristic_distances[start], start});

        while (!pqueue.empty()) {//O(V)
            auto [current_f, current_node] = pqueue.top();
            pqueue.pop();//O(logV)

            if (current_node == end) {// нашли путь
                ans = distances[end];
                break;
            }

            for (auto& edge : current_node->edges) { // перебор ребр O(E)
                Node* neighbor = edge.first;
                double weight = edge.second;
                double new_distance = distances[current_node] + weight;
                if (new_distance < distances[neighbor]) {
                    distances[neighbor] = new_distance;
                    previous[neighbor] = current_node;
                    heuristic_distances[neighbor] = new_distance + Heuristic(neighbor, end);
                    pqueue.push({heuristic_distances[neighbor], neighbor}); //O(logV)
                }
            }
        }

        std::vector<Node*> path; // востановление пути
        Node* node = end;
        while (node != nullptr){ // O(V)
            path.push_back(node);
            node = previous[node];
        }
        reverse(path.begin(),path.end());
        if (path.size() == 1){
            return {{},-1};
        }
        return {path,distances[end]};
    }
    // ассмптотика O(VlogV + Elog(V)) = O(log(V)(E+V))
    // память O(V)
private:

    struct ParsedNode
    {
        double lon1,lat1,lon2,lat2,weight2,lon3,lat3,weight3;
    };
    ParsedNode Parsing(std::string& line){
        ParsedNode result;
        size_t separator = line.find(':');
        std::string first_parth = line.substr(0,separator);
        std::string second_parth = line.substr(separator + 1,line.size() - separator - 2);        
        separator = first_parth.find(',');
        result.lon1 = std::stod(first_parth.substr(0,separator));
        result.lat1 = std::stod(first_parth.substr(separator + 1,first_parth.size() - separator));
        separator = second_parth.find(';');
        std::string node2_text = second_parth.substr(0,separator);
        std::string node3_text = second_parth.substr(separator + 1,second_parth.size() - separator);
        std::vector<double> node2_v;
        size_t start = 0;
        size_t end = node2_text.find(',');
        while (true)
        {
            node2_v.push_back(std::stod(node2_text.substr(start,end - start)));
            start = end + 1;
            end = node2_text.find(',',start);
            if (end == std::string::npos){
                node2_v.push_back(std::stod(node2_text.substr(start,end - start)));
                break;
            }
                
        }
        result.lon2 = node2_v[0];
        result.lat2 = node2_v[1];
        result.weight2 = node2_v[2];
        std::vector<double> node3_v;
        start = 0;
        end = node3_text.find(',');
        while (true)
        {
            node3_v.push_back(std::stod(node3_text.substr(start,end - start)));
            start = end + 1;
            end = node3_text.find(',',start);
            if (end == std::string::npos){
                node3_v.push_back(std::stod(node3_text.substr(start,end - start)));
                break;
            }
        }
        result.lon3 = node3_v[0];
        result.lat3 = node3_v[1];
        result.weight3 = node3_v[2];
        return result;
    }

    double Heuristic(Node* a, Node* b) {// расстоение между точек
        return std::sqrt(std::pow(a->lon - b->lon, 2) + std::pow(a->lat - b->lat, 2));
    }
public:// чтобы тесты делать
    std::map<std::pair<double, double>, Node*> cords_node; 
    std::vector<Node*> nodes; 
};

void TestDFS(){
    Graph graph;
    Node* n1 = new Node{30.0, 60.0, {}};
    Node* n2 = new Node{31.0, 61.0, {}};
    Node* n3 = new Node{32.0, 62.0, {}};
    n1->edges.push_back({n2, 10.0});
    n2->edges.push_back({n3, 15.0});
    graph.nodes = {n1, n2, n3};

    // Тест с путём
    auto result1 = graph.DFS(n1, n3);
    assert(result1.second == 25.0);
    std::vector<Node*> res = {n1, n2, n3};
    assert(result1.first == res);
    // Тест без пути
    Node* isolated = new Node{33.0, 63.0, {}};
    auto result2 = graph.DFS(n1, isolated);
    assert(result2.second == -1);
    assert(result2.first.empty());
    std::cout << "Test DFS is OK\n";
}
void TestBFS(){
    Graph graph;
    Node* n1 = new Node{30.0, 60.0, {}};
    Node* n2 = new Node{31.0, 61.0, {}};
    Node* n3 = new Node{32.0, 62.0, {}};
    Node* n4 = new Node{33.0, 63.0, {}};
    n1->edges.push_back({n2, 10.0});
    n2->edges.push_back({n3, 15.0});
    n1->edges.push_back({n4, 5.0});
    n4->edges.push_back({n3, 5.0});
    n1->edges.push_back({n3,400});
    graph.nodes = {n1, n2, n3, n4};
    // Тест с путём
    
    auto result1 = graph.BFS(n1, n3);
    assert(result1.second == 400);
    std::vector<Node*> res = {n1, n3};
    assert(result1.first == res);

    // Тест без пути
    Node* isolated = new Node{34.0, 64.0, {}};
    auto result2 = graph.BFS(n1, isolated);
    assert(result2.second == -1);
    assert(result2.first.empty());
    std::cout << "Test BFS is OK\n";
}
void TestDijlstra(){
    Graph graph;
    Node* n1 = new Node{30.0, 60.0, {}};
    Node* n2 = new Node{31.0, 61.0, {}};
    Node* n3 = new Node{32.0, 62.0, {}};
    Node* n4 = new Node{33.0, 63.0, {}};
    n1->edges.push_back({n2, 10.0});
    n2->edges.push_back({n3, 20.0});
    n1->edges.push_back({n4, 5.0});
    n4->edges.push_back({n3, 5.0});

    graph.nodes = {n1, n2, n3, n4};
    auto result = graph.Dijkstra(n1, n3);

    assert(result.second == 10.0); 
    std::vector<Node*> res = {n1, n4, n3};
    assert(result.first == res);
    std::cout << "Test Dijlstra is OK\n";
}
void TestAStar(){
    Graph graph;
    Node* n1 = new Node{30.0, 60.0, {}};
    Node* n2 = new Node{31.0, 61.0, {}};
    Node* n3 = new Node{32.0, 62.0, {}};
    Node* n4 = new Node{33.0, 63.0, {}};
    n1->edges.push_back({n2, 10.0});
    n2->edges.push_back({n3, 20.0});
    n1->edges.push_back({n4, 5.0});
    n4->edges.push_back({n3, 5.0});

    graph.nodes = {n1, n2, n3, n4};
    auto result = graph.AStar(n1, n3);

    assert(result.second == 10.0); 
    std::vector<Node*> res = {n1, n4, n3};
    assert(result.first == res);

    std::cout << "Test A* is OK\n";
}


int main(){
    TestDFS();
    TestBFS();
    TestDijlstra();
    TestAStar();
    Graph graph;
    //std::ifstream file("/Users/mishakobgunov/Desktop/rotube/lab8/spb_graph.txt"); //у меня просто название почему то не работает
    std::ifstream file("spb_graph.txt");
    
    double lon1 = 30.431707;
    double lat1 = 59.881115;
    double lon2 = 30.310184;
    double lat2 = 59.956436;
    graph.ReadFromFile(file);
    Node* home = graph.FindClosestNode(lat1,lon1);
    Node* itmo = graph.FindClosestNode(lat2,lon2);

    auto start_time = chrono::high_resolution_clock::now();
    auto dfs = graph.DFS(home,itmo);
    auto end_time = chrono::high_resolution_clock::now();
    auto time = chrono::duration_cast<chrono::duration<double>>(end_time - start_time);
    cout << "Time of DFS: " << time.count() << " seconds\n";
    start_time = chrono::high_resolution_clock::now();
    auto bfs = graph.BFS(home,itmo);
    end_time = chrono::high_resolution_clock::now();
    time = chrono::duration_cast<chrono::duration<double>>(end_time - start_time);
    cout << "Time of BFS: " << time.count() << " seconds\n";
    start_time = chrono::high_resolution_clock::now();
    auto dijlstra = graph.Dijkstra(home,itmo);
    end_time = chrono::high_resolution_clock::now();
    time = chrono::duration_cast<chrono::duration<double>>(end_time - start_time);
    cout << "Time of Dijkstra: " << time.count() << " seconds\n";
    start_time = chrono::high_resolution_clock::now();
    auto a_star = graph.AStar(home,itmo);
    end_time = chrono::high_resolution_clock::now();
    time = chrono::duration_cast<chrono::duration<double>>(end_time - start_time);
    cout << "Time of A*: " << time.count() << " seconds\n\n";
    std::cout << "DFS distance: " << dfs.second << "\n" << "BFS distance: "<< bfs.second << "\n" << "Dijlstra distance: "<< dijlstra.second << "\n"<< "A* distance: " << a_star.second << "\n\n";
    std::cout << "DFS edges: " << dfs.first.size() << "\n" << "BFS edges: "<< bfs.first.size() << "\n" << "Dijlstra edges: "<< dijlstra.first.size() << "\n"<< "A* edges: " << a_star.first.size() << "\n";
    return 0;
}

