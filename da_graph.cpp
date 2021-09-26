// Нахождение кратчайших путей от одной вершины до всех остальных.
// Алгоритм нахождения кратчайших путей в ориентированном ациклическом графе
// (на основе топологической сортировки).

#include "da_graph.h"

ListNode::ListNode(int v, int w) {
    nodeVertex=v;
    weight=w;
}

int ListNode::getNodeVertex() {
    return nodeVertex;
}

int ListNode::getWeight() {
    return weight;
}
int Graph::line_number(char vert) {
    for(int i=0;i<row.size();++i){
        if(row[i]==vert) return i;
    }
    // Если такой вершины не существует
    throw "Error 1";
}
Graph::Graph(string str) {
    vertex=0;                                // Вначале кол-вовершин равно 0
    if(str.empty()) throw "Error 1";         // Если пустая строка
    if(!isalpha(str[0])) throw "Error 2.1";  // Не корректное начало
    mode=0;// используется в поиске путей
    /*
      Переключатель чтения строки
      0 - чтение вершин
      1 - создание списка
      2 - чтение ребер
   */
    int choice=0;                             // сначала читаем вершины
    // цикл до конца строки, символ "\0"
    for(int k=0;str[k]!='\0';++k){
        // Если более 1-ой буквы подряд,
        // значит уже не вершина
        // choice==2 значит case 2: пройден и список создан
        if( isalpha(str[k])&&isalpha(str[k+1])&&(choice!=2))  choice=1;
        if(isalpha(str[k])) {
            switch (choice) {
                default:;
                case 0:
                    row += str[k];
                    ++vertex;
                    /*
                     * При считывание вершин после каждой
                     * должна быть запятая
                     * Или конец строки
                     */
                    if (!ispunct(str[k + 1])) {
                        if (str[k + 1] != '\0') throw "Error 3.0";
                    }
                    /*
                     * После запятой обязательно буква
                     */
                    if (ispunct(str[k + 1])) {
                        if (!isalpha(str[k + 2])) throw "Error 3.1";
                    }
                    /*
                     * Если нет ребер
                     */
                    if (str[k + 1] == '\0'){
                        adj=nullptr;
                        dist= nullptr;
                        strRow= nullptr;
                    }
                    break;
                case 1:

                    sort(row.begin(), row.end()); // сортируем вершины в лексикографическом порядке
                    adj=new list<ListNode>[vertex];

                    // Создадим массив расстояний
                    // Понадобится в shortestPath()
                    dist=new int[vertex];
                    strRow=new string[vertex];
                    choice=2;

                case 2:
                    if (!isalpha(str[k]) || !isalpha(str[k + 1])) throw "Error 4.0";
                    /*
                     * Связи идут по 2 символа после них цифра
                     * или знак минус
                     */
                    int plus = 2;                      // для учёта отрицательных ребер сторока 119
                    if (str[k + plus] == '-') ++plus;
                    if (!isdigit(str[k + plus])) throw "Error 4.1";

                    int start = line_number(str[k]);   // откуда
                    int end = line_number(str[k + 1]); // куда
                    k = k + plus;                      // начинаем считывать вес
                    string strNum;
                    while (isdigit(str[k])) {
                        strNum += str[k];
                        /*
                         * После веса идёт запятая
                         * или конец строки
                         */
                        if (!isdigit(str[k + 1])) {
                            if (!ispunct(str[k + 1]))
                                if(str[k+1]!='\0') throw "Error 4.2";
                        }
                        ++k;
                    }
                    --k;
                    /*
                     * создадим char* для преобразования в число
                     * функцией strtol()
                     */
                    char *charNum = new char[strNum.size() + 1];
                    for (int i = 0; i < strNum.size(); ++i) {
                        charNum[i] = strNum[i];
                    }
                    charNum[strNum.size() + 1] = '\0';
                    int weight = (int)std::strtol(charNum,nullptr,0);
                    if(plus==3) {
                        weight=weight*(-1);
                    }

                    ListNode node(end, weight);
                    adj[start].push_back(node);
                    break;
            }// switch (choice)
        } // if(isalpha(str[k]))
    }// for(int k=0;str[k]!='\0';++k)
}
void Graph::topologicalSort(int vert,bool visited[],stack<int> &Stack) {
    // Почемаем что посетили данную вершину
    visited[vert]= true;
    list<ListNode>:: iterator i;
    for(i=adj[vert].begin();i!=adj[vert].end();++i){
        ListNode node =*i;

        if(!visited[node.getNodeVertex()]){
            topologicalSort(node.getNodeVertex(),visited,Stack);
        }

        Stack.push(vert);
    }
}

void Graph::shortestPath(char symbol) {
    /*
     * Если нет ребер то рассояние
     * до вершин бесконечно большое
     */
    if(strRow == nullptr){
        mode=1;
        strRow=new string[vertex];
        dist=new int[vertex];
        for(int i=0;i<vertex;++i){
            strRow[i]+="INF";
            dist[i]=INF;
        }
        return;
    }
    if(mode==1)
        return;
    int s=line_number(symbol);
    /*
     * Вначале не были ни в одной вершине
     * пометим все не посещёнными
     */
    bool *visited=new bool[vertex];
    for(int i=0;i<vertex;++i){
        visited[i]= false;
    }

    stack<int> Stack;// Для топологической сортировки
                     // Упорядочим вершины
    for(int i=0;i<vertex;++i){
        if(!visited[i]) topologicalSort(i,visited,Stack);
    }

    /*
     * Обозначим все расстояния до вершин
     * бесконесностью
     */
    for(int i=0;i<vertex;++i){
        dist[i]=INF;
    }
    dist[s]=0;// Путь вершины самой в себя ноль

    /*
     * Если раньше использовали очистим
     * для нового заполнения
     */
    if(!strRow[0].empty()){
        for(int i=0;i<vertex;++i){
            strRow[i]="";
        }
    }

    strRow[s]+="Null";
    while(!Stack.empty()){
        int u=Stack.top();// Получаем 1-ый
        Stack.pop();      // удаляем его

        list<ListNode>:: iterator i;
        /*
         * Начинаем искать кратчайшие пути
         * Сразу откидывает пути с бесконечностью
         */
        if(dist[u]!=INF){
            /*
             * Идём по рёбрам из нашей вершины
             */
            for(i=adj[u].begin();i!=adj[u].end();++i){
                /*
                 * Если путь из данной вершины в доступную
                 * ей, может быть меньше, Изначального пути тогда
                 * переопределяем кратчайший путь
                 */
                if(dist[i->getNodeVertex()] > dist[u]+i->getWeight()){
                    dist[i->getNodeVertex()]=dist[u]+i->getWeight();
                    /*
                     * Теперь оформим крайний путь в виде строки.
                     * Для удобства обозначим
                     * 'c'  -row[i->getNodeVertex()]
                     * 'b'  -row[u]
                     * 'ab' -strRow[u]
                     * Мы получаем что есть путь из 'b' в 'c'
                     * перед тем как записать его, мы проверяем
                     * был ли кратчайшить путь из искомой вершины
                     * в 'b' !strRow[u].empty(), если он был
                     * то вначале вписываем его, потом наш путь 'bс'
                     * получим на выходе путь из искомой вершины 'a' в 'с'
                     * равный 'ab,bс'
                     */                                                                   // Если ранее создавалось, но сейчас алгоритм
                    if(!strRow[i->getNodeVertex()].empty()) strRow[i->getNodeVertex()]="";// опять у этой вершины, значит найден более
                                                                                          // кратчайшить путь, старый удаляем
                    if(!strRow[u].empty()&& u!=s){// добавлени всех предшествующих ребер
                        strRow[i->getNodeVertex()]+=strRow[u];
                        strRow[i->getNodeVertex()]+=',';
                    }
                    // Добавление крайнего ребра
                    strRow[i->getNodeVertex()]+=row[u];
                    strRow[i->getNodeVertex()]+=row[i->getNodeVertex()];

                }// if(dist[i->getNodeVertex()] > dist[u]+i->getWeight())
            }// for(i=adj[u].begin();i!=adj[u].end();++i)
        }// if(dist[u]!=INF)
    }// while(!Stack.empty())
    for(int i=0;i<vertex;++i){
        if(strRow[i].empty()&&i!=s) strRow[i]+="INF";
    }
    delete[] visited;
}

string* Graph::getStrRow() {
    return strRow;
}

Graph::~Graph(){
    if(adj!= nullptr){
        for(int i=0;i<vertex;++i) {
            adj[i].erase(adj[i].begin(),adj[i].end());
        }
    }
    delete[] adj;
    delete[] dist;
}

int Graph::getVertex() {
    return vertex;
}

list<ListNode>* Graph::getAllNode() {
    return adj;
}

list<ListNode> Graph::getNode(char symbol) {
    int num=line_number(symbol);
    for(int i=0;i<vertex;++i){
        if(num==i) return adj[i];
    }
}

int* Graph::getDist() {
    return dist;
}
