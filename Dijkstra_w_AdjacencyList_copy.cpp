#include<iostream>
#include<limits.h>

using namespace std;

//CTDL ĐỒ THỊ ĐƯỢC BIỂU DIỄN BẰNG DANH SÁCH KỀ

//Kiểu có cấu trúc dùng để biểu diễn một node danh sách kề 
//(danh sách các đỉnh kề của một đỉnh)
struct AdjListNode {

    //chỉ số đỉnh kề
    int dest;

    //trọng số khoảng cách đến đỉnh kề 
    int weight;
    AdjListNode *next;
};

//Kiểu có cấu trúc đại diện cho danh sách kề
struct AdjList {
    AdjListNode *head;
};

//Kiểu có cấu trúc dùng để biểu diễn đồ thị
//Đồ thị là một mảng các danh sách kề
//Kích thước của mảng sẽ là V (số lượng các đỉnh)
struct Graph {
    //Số lượng các đỉnh của đồ thị
    int V;
    AdjList *array;
};

//Hàm dùng để tạo danh sách kề mới
AdjListNode *newAdjListNode(int dest, int weight) {
    AdjListNode *newNode = new AdjListNode;
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

//Hàm dùng để tạo một đồ thị gồm có V đỉnh
Graph *createGraph(int V) {
    Graph *graph = new Graph;
    graph->V = V;

    //Tạo một mảng các danh sách kề
    //Kích thước của các danh sách kề sẽ là V
    graph->array = new AdjList[V];

    //Khởi tạo giá trị cho từng danh sách kề
    //với giá trị head là NULL
    for(int i = 0; i < V; i++) {
        graph->array[i].head = NULL;
    }
    return graph;
}

//Thêm một cạnh vào đồ thị vô hướng
void addEdge(Graph *graph, int src, int dest, int weight) {
    //Thêm một cạnh từ đỉnh nguồn tới đỉnh đích.
    //Một node sẽ được thêm vào danh sách kề
    AdjListNode *check = NULL;
    AdjListNode *newNode = newAdjListNode(dest, weight);

    //Kiểm tra xem danh sách kề có trống hay không
    if(graph->array[src].head == NULL) {
        newNode->next = graph->array[src].head;
        graph->array[src].head = newNode;
    }

    //Nếu danh sách kề không trống
    else {
		check = graph->array[src].head;
		while (check->next != NULL) {
			check = check->next;
		}
		check->next = newNode;
	}

    //Bời vì đây là đồ thị vô hướng nên là giá trị của cạnh
    //từ đỉnh đích tới đỉnh nguồn cũng được thêm vào
    newNode = newAdjListNode(src, weight);
    if (graph->array[dest].head == NULL) {
		newNode->next = graph->array[dest].head;
		graph->array[dest].head = newNode;
	}
	else {
		check = graph->array[dest].head;
		while (check->next != NULL) {
			check = check->next;
		}
		check->next = newNode;
	}
}

//Hàm hiển thị kết quả
void printAns(int dist[], Graph *graph) {
    cout << "Vertex \t Distance from source" << endl;
    for(int i = 0; i < graph->V; i++) {
        cout << i << " \t\t" << dist[i] << endl;
    }
}

//Hàm dùng để tìm đỉnh có khoảng cách ngắn nhất
//từ đỉnh nguồn, đỉnh này nằm trong các đỉnh chưa được xử lý xong
int minDist(int dist[], bool check[], Graph *graph) {
    //Khởi tạo giá trị nhỏ nhất
    //Khai báo biến lưu giá trị hệ số trong mảng dist[] và check[] của đỉnh được tìm thấy
    int min = INT_MAX, minIndex;
    for(int v = 0; v < graph->V; v++) {
        if (check[v] == false && dist[v] <= min) {
            min = dist[v];
            minIndex = v;
        }
    }
    return minIndex;
}

//Hàm thực hiện thuật toán Dijkstra tìm đường đi ngắn nhất
void dijkstra(Graph *graph, int src) {
    //Mảng lưu trữ kết quả đầu ra
    //Giá trị của các phần tử trong mảng
    //sẽ là khoảng cách ngắn nhất từ gốc đến đỉnh i
    int dist[graph->V]; 
    
    //Mảng dùng để kiểm tra xem giá trị khoảng cách
    //ngắn nhất của đỉnh i đã được tìm thấy hay chưa
    bool check[graph->V];
    
    //Khởi tạo giá trị ban đầu của các phần tử trong mảng kết
    //quả dist[] là VÔ CÙNG và các giá trị trong mảng check[] là flase 
    for(int i = 0; i < graph->V; i++) {
        dist[i] = INT_MAX;
        check[i] = false;
    }

    //Khoảng các từ đỉnh gốc đến chính nó là 0
    dist[src] = 0;

    //Tìm khoảng các ngắn nhất từ đỉnh gốc tới các đỉnh
    for(int i = 0; i < graph->V; i++) {
        //Chọn đỉnh có khoảng cách ngắn nhất
        //từ đỉnh nguồn, đỉnh này nằm trong các đỉnh chưa được xử lý xong
        int u = minDist(dist, check, graph);

        //Đánh dấu khoảng cách ngắn nhất
        //từ nguồn tới đỉnh u đã được tìm thấy, đã được xử lý xong
        //u sẽ luôn bằng src (đỉnh gốc) trong lần thực hiện thuật toán đầu tiên
        check[u] = true;

        //Cập nhật các giá trị của dist[v] khi:
        //1.Có một cạnh từ u đến v
        //2.Tổng của "Khoảng cách từ gốc đến đỉnh v qua đỉnh u" nhỏ hơn giá trị hiện tại trong dist[v]
        AdjListNode *tmp = graph->array[u].head;
        while (tmp != NULL) {
            int v = tmp->dest;
            if(dist[u] != INT_MAX && dist[u] + tmp->weight < dist[v]) {
                dist[v] = dist[u] + tmp->weight;
            }
            tmp = tmp->next;
        }
        
    }

    //In kết quả ra màn hình
    printAns(dist, graph);
}

int main() {
    //Tạo một đồ thị có 5 đỉnh
    //và thêm khoảng các giữa các đỉnh
	int V = 5;
	struct Graph* graph = createGraph(V);
	addEdge(graph, 0, 1, 1);
	addEdge(graph, 0, 4, 3);
	addEdge(graph, 1, 2, 9);
	addEdge(graph, 1, 3, 4);
	addEdge(graph, 1, 4, 2);
	addEdge(graph, 2, 3, 7);
	addEdge(graph, 3, 4, 5);

    //Nhập đỉnh gốc mà ta muốn tìm khoảng cách ngắn nhất
    //tới các đỉnh khác
    int src;
    cout << "Enter the source vertex: ";
    cin >> src;

    //Gọi hàm thực hiện thuật toán Dijkstra
    dijkstra(graph, src);

	return 0;
}