
// 5. Нахождение кратчайших путей от одной вершины до всех остальных.
// Алгоритм нахождения кратчайших путей в ориентированном ациклическом графе
// (на основе топологической сортировки).

#define CATCH_CONFIG_MAIN
#include "../da_graph.h"
#include "../bin/catch.hpp"

TEST_CASE("Проверка работы конструктора","fun"){
    string str="a,b,c,d,e,ab5,ad6,bc7,be8"; // simpleGraph.png
    Graph obj(str);
    REQUIRE(obj.getVertex()==5);

    list<ListNode> lNode_1=obj.getNode('a');
    list<ListNode> lNode_2=obj.getNode('b');
    REQUIRE(lNode_1.size()==2);
    REQUIRE(lNode_2.size()==2);

    list<ListNode>::iterator i;
    int weg=5;// Для проверки весов
    for(i=lNode_1.begin();i!=lNode_1.end();++i){
        REQUIRE(i->getWeight()==weg);
        /*
         * Из вершины 'a' ребра с весами:
         * 5 - в вершину 'b'
         * 6 - в вершину 'd'
         */

        if(weg==5) REQUIRE(i->getNodeVertex()==obj.line_number('b'));
        if(weg==6) REQUIRE(i->getNodeVertex()==obj.line_number('d'));
        ++weg;
    }
    for(i=lNode_2.begin();i!=lNode_2.end();++i){
        REQUIRE(i->getWeight()==weg);
        /*
         * Из вершины 'b' ребра с весами:
         * 7 - в вершину 'с'
         * 8 - в вершину 'e'
         */

        if(weg==7) REQUIRE(i->getNodeVertex()==obj.line_number('c'));
        if(weg==8) REQUIRE(i->getNodeVertex()==obj.line_number('e'));
        ++weg;
    }
}

TEST_CASE("Проверка конструктора крайние данные","fun"){
    // без ребер
    string str="a,b,c,d,e";
    Graph obj(str);
    REQUIRE(obj.getVertex()==5);
    REQUIRE(obj.getAllNode()== nullptr);

    // одна вершина
    string str_1="a";
    Graph obj_1(str_1);
    REQUIRE(obj_1.getVertex()==1);
    REQUIRE(obj_1.getAllNode()== nullptr);

    // отрицательные веса ребер
    string str_2="a,b,c,d,ab-10,ac-20,ad-40";
    Graph obj_2(str_2);
    REQUIRE(obj_2.getVertex()==4);

    list<ListNode> lNode_1=obj_2.getNode('a');
    list<ListNode>::iterator i;
    int weg=-10;// Для проверки весов
    for(i=lNode_1.begin();i!=lNode_1.end();++i){
        REQUIRE(i->getWeight()==weg);
         /*
         * Из вершины 'a' ребра с весами:
         * -10 - в вершину 'b'
         * -20 - в вершину 'c'
         * -40 - в вершину 'd'
         */
        if(weg==-10) REQUIRE(i->getNodeVertex()==obj.line_number('b'));
        if(weg==-20) REQUIRE(i->getNodeVertex()==obj.line_number('c'));
        if(weg==-40) REQUIRE(i->getNodeVertex()==obj.line_number('d'));
        weg=weg*2;
    }

}

TEST_CASE("Проверка конструктора неверные данные","fun"){
    string str_0="aa";
    REQUIRE_THROWS((Graph)(str_0));

    string str_1="!a,b,c,ab10";
    REQUIRE_THROWS((Graph)(str_1));

    string str_2="a,,";
    REQUIRE_THROWS((Graph)(str_2));

    // Строка без символов
    string str_3=" ";
    REQUIRE_THROWS((Graph)(str_3));

    // Пустая строка
    string str_3_1;
    REQUIRE_THROWS((Graph)(str_3_1));

    // Ребро в несуществующую вершину "P"
    string str_4="a,b,c,ab10,bP20";
    REQUIRE_THROWS((Graph)(str_4));

    // Если строка обычная то без исключений
    string str_5="a,b,c,ab10,bc-10";
    REQUIRE_NOTHROW((Graph)(str_5));

    // Если пропушен вес ребра
    string str_6="a,b,c,ab,bc-10";
    REQUIRE_THROWS((Graph)(str_6));
}

TEST_CASE("Проверка поиска кратчайших путей","fun"){
    string str="a,b,c,d,e,ab5,ad6,bc7,be8"; // simpleGraph.png
    Graph obj(str);
    REQUIRE(obj.getVertex()==5);
    obj.shortestPath('a');
    int* dist=obj.getDist();

    string* strRow=obj.getStrRow();
    REQUIRE(obj.getVertex()==5);// Значение не должно меняться
    REQUIRE(dist[obj.line_number('a')]==0);
    REQUIRE(dist[obj.line_number('b')]==5);
    REQUIRE(dist[obj.line_number('c')]==12);
    REQUIRE(dist[obj.line_number('d')]==6);
    REQUIRE(dist[obj.line_number('e')]==13);
    REQUIRE(strRow[0]=="Null");
    REQUIRE(strRow[1]=="ab");
    REQUIRE(strRow[2]=="ab,bc");
    REQUIRE(strRow[3]=="ad");
    REQUIRE(strRow[4]=="ab,be");

    obj.shortestPath('b');
    strRow=obj.getStrRow();
    REQUIRE(obj.getVertex()==5);// Значение не должно меняться
    REQUIRE(dist[obj.line_number('a')]==INF);
    REQUIRE(dist[obj.line_number('b')]==0);
    REQUIRE(dist[obj.line_number('c')]==7);
    REQUIRE(dist[obj.line_number('d')]==INF);
    REQUIRE(dist[obj.line_number('e')]==8);
    REQUIRE(strRow[0]=="INF");
    REQUIRE(strRow[1]=="Null");
    REQUIRE(strRow[2]=="bc");
    REQUIRE(strRow[3]=="INF");
    REQUIRE(strRow[4]=="be");

    obj.shortestPath('c');
    strRow=obj.getStrRow();
    REQUIRE(obj.getVertex()==5);// Значение не должно меняться
    REQUIRE(dist[obj.line_number('a')]==INF);
    REQUIRE(dist[obj.line_number('b')]==INF);
    REQUIRE(dist[obj.line_number('c')]==0);
    REQUIRE(dist[obj.line_number('d')]==INF);
    REQUIRE(dist[obj.line_number('e')]==INF);
    REQUIRE(strRow[0]=="INF");
    REQUIRE(strRow[1]=="INF");
    REQUIRE(strRow[2]=="Null");
    REQUIRE(strRow[3]=="INF");
    REQUIRE(strRow[4]=="INF");
}

TEST_CASE("Поиск кратчайших путей крайние значения","fun"){
    // Без ребер
    string str="a,b,c,d,e";
    Graph obj(str);
    obj.shortestPath('a');
    string* strRow=obj.getStrRow();
    int* dist=obj.getDist();

    REQUIRE(obj.getVertex()==5);
    for(int i=0;i<obj.getVertex();++i){
        REQUIRE(strRow[i]=="INF");
        REQUIRE(dist[i]==INF);
    }

    // Проверим сразу для другой вершины
    obj.shortestPath('e');
    for(int i=0;i<obj.getVertex();++i){
        REQUIRE(strRow[i]=="INF");
        REQUIRE(dist[i]==INF);
    }

    // 1 вершина
    string str_1="a";
    Graph obj_1(str_1);
    obj_1.shortestPath('a');
    string* strRow_1=obj_1.getStrRow();
    int* dist_1=obj_1.getDist();

    REQUIRE(obj_1.getVertex()==1);
    for(int i=0;i<obj_1.getVertex();++i){
        REQUIRE(strRow_1[i]=="INF");
        REQUIRE(dist_1[i]==INF);
    }

    // 1 ребро
    string str_2="a,b,ab10";
    Graph obj_2(str_2);
    obj_2.shortestPath('a');
    string* strRow_2=obj_2.getStrRow();
    int* dist_2=obj_2.getDist();

    REQUIRE(obj_2.getVertex()==2);
    REQUIRE(strRow_2[0]=="Null");
    REQUIRE(strRow_2[1]=="ab");

    REQUIRE(dist_2[0]==0);
    REQUIRE(dist_2[1]==10);

    // 1 отрицательное ребро
    string str_3="a,b,ab-10";
    Graph obj_3(str_3);
    obj_3.shortestPath('a');
    string* strRow_3=obj_3.getStrRow();
    int* dist_3=obj_3.getDist();

    REQUIRE(obj_3.getVertex()==2);
    REQUIRE(strRow_3[0]=="Null");
    REQUIRE(strRow_3[1]=="ab");

    REQUIRE(dist_3[0]==0);
    REQUIRE(dist_3[1]==-10);
}
/*
 * Тесты на неверный ввод в кратчайший поиск
 * не востребованы, т.к сразу вначале идёт проверка
 * принадлежал ли входные данные вершинам графа.
 * Если нет,то будут исключения которые ранее обрабатывались
 */
TEST_CASE("Поиск кратчайших путей на сложном графе","fun"){
    // graph.png
    // стоят пометки для нахождения путей из вершины 'a'
    string str="a,b,c,d,e,f,g,h,i,j,ab5,ac7,ad1,bi10,be5,ce2,cf3,df10,dg14,ij4,ei7,eh3,fh1,fg6,gj5";
    Graph obj(str);
    REQUIRE(obj.getVertex()==10);

    obj.shortestPath('a');
    string* strRow=obj.getStrRow();
    int* dist=obj.getDist();
    REQUIRE(strRow[obj.line_number('a')]=="Null");
    REQUIRE(dist[obj.line_number('a')]==0);

    REQUIRE(strRow[obj.line_number('b')]=="ab");
    REQUIRE(dist[obj.line_number('b')]==5);

    REQUIRE(strRow[obj.line_number('c')]=="ac");
    REQUIRE(dist[obj.line_number('c')]==7);

    REQUIRE(strRow[obj.line_number('d')]=="ad");
    REQUIRE(dist[obj.line_number('d')]==1);

    REQUIRE(strRow[obj.line_number('e')]=="ac,ce");
    REQUIRE(dist[obj.line_number('e')]==9);

    REQUIRE(strRow[obj.line_number('f')]=="ac,cf");
    REQUIRE(dist[obj.line_number('f')]==10);

    REQUIRE(strRow[obj.line_number('g')]=="ad,dg");
    REQUIRE(dist[obj.line_number('g')]==15);

    REQUIRE(strRow[obj.line_number('h')]=="ac,cf,fh");
    REQUIRE(dist[obj.line_number('h')]==11);

    REQUIRE(strRow[obj.line_number('i')]=="ab,bi");
    REQUIRE(dist[obj.line_number('i')]==15);

    REQUIRE(strRow[obj.line_number('j')]=="ab,bi,ij");
    REQUIRE(dist[obj.line_number('j')]==19);

    REQUIRE(obj.getVertex()==10);// С вершинами ничего не случилось

    // graph.png
    // Поменяем вершину
    obj.shortestPath('e');
    dist=obj.getDist();

    REQUIRE(strRow[obj.line_number('a')]=="INF");
    REQUIRE(dist[obj.line_number('a')]==INF);

    REQUIRE(strRow[obj.line_number('b')]=="INF");
    REQUIRE(dist[obj.line_number('b')]==INF);

    REQUIRE(strRow[obj.line_number('c')]=="INF");
    REQUIRE(dist[obj.line_number('c')]==INF);

    REQUIRE(strRow[obj.line_number('d')]=="INF");
    REQUIRE(dist[obj.line_number('d')]==INF);

    REQUIRE(strRow[obj.line_number('e')]=="Null");
    REQUIRE(dist[obj.line_number('e')]==0);

    REQUIRE(strRow[obj.line_number('f')]=="INF");
    REQUIRE(dist[obj.line_number('f')]==INF);

    REQUIRE(strRow[obj.line_number('g')]=="INF");
    REQUIRE(dist[obj.line_number('g')]==INF);

    REQUIRE(strRow[obj.line_number('h')]=="eh");
    REQUIRE(dist[obj.line_number('h')]==3);

    REQUIRE(strRow[obj.line_number('i')]=="ei");
    REQUIRE(dist[obj.line_number('i')]==7);

    REQUIRE(strRow[obj.line_number('j')]=="ei,ij");
    REQUIRE(dist[obj.line_number('j')]==11);

    /*
     * Создадим тот же самый граф но с отрицательныйм ребром eh
     * поменяется путь в 'h' вместо 'ac,cf,fh' будет 'ac,ce,eh'
     * и вес пути будет равен 7+2-3=6
     */
    string str_1="a,b,c,d,e,f,g,h,i,j,ab5,ac7,ad1,bi10,be5,ce2,cf3,df10,dg14,ij4,ei7,eh-3,fh1,fg6,gj5";
    Graph obj_1(str_1);                                                            // тут
    REQUIRE(obj_1.getVertex()==10);
    obj_1.shortestPath('a');
    strRow=obj_1.getStrRow();
    dist=obj_1.getDist();

    REQUIRE(strRow[obj_1.line_number('a')]=="Null");
    REQUIRE(dist[obj_1.line_number('a')]==0);

    REQUIRE(strRow[obj_1.line_number('b')]=="ab");
    REQUIRE(dist[obj_1.line_number('b')]==5);

    REQUIRE(strRow[obj_1.line_number('c')]=="ac");
    REQUIRE(dist[obj_1.line_number('c')]==7);

    REQUIRE(strRow[obj_1.line_number('d')]=="ad");
    REQUIRE(dist[obj_1.line_number('d')]==1);

    REQUIRE(strRow[obj_1.line_number('e')]=="ac,ce");
    REQUIRE(dist[obj_1.line_number('e')]==9);

    REQUIRE(strRow[obj_1.line_number('f')]=="ac,cf");
    REQUIRE(dist[obj_1.line_number('f')]==10);

    REQUIRE(strRow[obj_1.line_number('g')]=="ad,dg");
    REQUIRE(dist[obj_1.line_number('g')]==15);

    REQUIRE(strRow[obj_1.line_number('h')]=="ac,ce,eh"); // Совпало
    REQUIRE(dist[obj_1.line_number('h')]==6);

    REQUIRE(strRow[obj_1.line_number('i')]=="ab,bi");
    REQUIRE(dist[obj_1.line_number('i')]==15);

    REQUIRE(strRow[obj_1.line_number('j')]=="ab,bi,ij");
    REQUIRE(dist[obj_1.line_number('j')]==19);
}
