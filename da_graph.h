// Нахождение кратчайших путей от одной вершины до всех остальных.
// Алгоритм нахождения кратчайших путей в ориентированном ациклическом графе
// (на основе топологической сортировки).
#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <algorithm>
#include <cstdio>

#include <limits.h>
#define INF INT_MAX

using std::string;
using std::list;
using std::stack;


class ListNode{
    int nodeVertex;
    int weight;
public:
    ListNode(int v,int w);
    int getNodeVertex();
    int getWeight();
};

// На основе списков смежности
class Graph{
    int vertex;// кол-во вершин
    string row;// названия вершин
    list<ListNode> *adj;// список смежности
    /*
     * topologicalSort()
     * Представляет собой такое линейное
     * урорядочивание всех вершин графа,
     * что если граф содержит ребро (a,b),
     * то a при таком упорядочивание
     * будет распологаться до b
     * Вершины хранятся в Stack
     */
    void topologicalSort(int vert,bool visited[],stack<int> &Stack);
    int* dist;        // Расстояния до вершин
    string *strRow;   // Ребра до этих вершин

    // mode = 0 всегда кроме
    // Если раньше граф был пустым, чтобы не обрабатывать
    // его снова
    // mode=1 нужно только для поиска путей
    int mode;
public:
    /*
     * explicit используется для создания явных конструкторов
     * Graph *adj_name(str);
     * Принимаем строку в виде структуры графа "str"
     */
    explicit Graph(string str);
    ~Graph();

    void shortestPath(char symbol);
    /*
     * line_number помогает соотнести
     * вершуну "vert" с номером в списке.
     * Сравнивается со списком вершин "row"
     */
    int line_number(char vert);         // преобразовывает вершину в число
    int getVertex();                    // возвращает кол-во вершин
    list<ListNode>* getAllNode();       // возвращает список
    list<ListNode> getNode(char symbol);// возвращает узел
    int* getDist();                     // возвращает расстояния до вершин
    string* getStrRow();                // возвращает пути до вершин
};
