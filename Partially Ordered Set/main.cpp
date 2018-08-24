#include <iostream>
#include <fstream>

class Element;
class Following;

class Following {
public:
    Following(Element *n4 = NULL, Following *n2 = NULL) : next4(n4), next2(n2) {}
    Element *next4;
    Following *next2;
};

class Element: public Following {
public:
//    Element(int v = 0, int c = 0, Element *n4 = NULL, Following *n2 = NULL) : value(v), count(c), Following(n4, n2) {}
    Element(int v = 0, int c = 0, Element *n4 = NULL, Following *n2 = NULL) : value(v), count(c) { next4 = n4; next2 = n2; }
    int value, count;
};

class PartOrdSet {
public:
    PartOrdSet() { head = NULL; }
    ~PartOrdSet() { makeNull(); }
    void initSet(); // инициализирует множество значениями из файла
    PartOrdSet &buildLinearOrder(); // вычисляет линейную последовательность из множества
    void print();
private:
    Element *head;
    void makeNull();
    void connect(int previous, int current);
    Element *searchElement(int val);
    Element *searchZero();
    void removeFollowings(Element *curr);
    void disconnectElement(Element *curr);
    void clearSet(Element *elem);
    Element *searchLast(Element *elem);
};

void PartOrdSet::makeNull() {
    Element *temp = head;
    Following *del;
    
    while (temp) {
        head = head->next4;
        del = temp->next2;
        while (del) {
            temp->next2 = del->next2;
            delete del;
            del = temp->next2;
        }
        delete temp;
        temp = head;
    }
    
    head = NULL;
}

void PartOrdSet::initSet() {
    std::ifstream info("info.txt");
    int prev, curr;
    
    if (info.is_open()) {
        while (!info.eof()) {
            info >> prev;
            info >> curr;
            connect(prev, curr);
        }
        info.close();
    }
}

Element *PartOrdSet::searchElement(int val) {
    Element *temp = head;
    
    while (temp) {
        if (temp->value == val)
            return temp;
        temp = temp->next4;
    }
    
    return NULL;
}

void PartOrdSet::connect(int previous, int current) {
    if (!head) {
        Element *curr = new Element(current, 1);
        Following *between = new Following(curr);
        head = new Element(previous, 0, curr, between);
        return;
    }
    
    Element *curr = searchElement(current), *prev = searchElement(previous);
    
    if (curr && prev) {
        Following *between = new Following(curr, prev->next2);
        prev->next2 = between;
        curr->count++;
        return;
    }
    
    if (curr) {
        Following *between = new Following(curr);
        prev = new Element(previous, 0, head, between);
        curr->count++;
        head = prev;
        return;
    }
    
    if (prev) {
        curr = new Element(current, 1, head);
        Following *between = new Following(curr, prev->next2);
        prev->next2 = between;
        head = curr;
        return;
    }
    
    curr = new Element(current, 1, head);
    Following *between = new Following(curr);
    head = new Element(previous, 0, curr, between);
}

Element *PartOrdSet::searchZero() {
    Element *temp = head;
    
    while (temp) {
        if (temp->count == 0)
            return temp;
        temp = temp->next4;
    }
    
    return NULL;
}

void PartOrdSet::removeFollowings(Element *curr) {
    Following *del = curr->next2;
    
    while (del) {
        del->next4->count--;
        curr->next2 = del->next2;
        delete del;
        del = curr->next2;
    }
}

void PartOrdSet::disconnectElement(Element *curr) { // ???
    Element *temp = head->next4, *prev = head;
    
    if (curr == head) {
        head = head->next4;
        curr->next4 = NULL;
        return;
    }
    
    while (temp != curr) {
        prev = temp;
        temp = temp->next4;
    }
    prev->next4 = temp->next4;
    curr->next4 = NULL; // ??? bulidLinearSet(); (zero->next4 = NULL;) ???
}

Element *PartOrdSet::searchLast(Element *elem) {
    Element *temp = elem, *prev = elem;
    
    while (temp) {
        prev = temp;
        temp = temp->next4;
    }
    
    return prev;
}

void PartOrdSet::clearSet(Element *elem) {
    Element *temp = elem;
    
    while (temp) {
        elem = elem->next4;
        delete temp;
        temp = elem;
    }
}

PartOrdSet &PartOrdSet::buildLinearOrder() {
    if (!head)
        return *this;
    
    Element *zero = searchZero();
    if (!zero) {
        makeNull();
        return *this;
    }
    
    removeFollowings(zero);
    disconnectElement(zero);
    std::cout << head->value << std::endl;
    //zero->next4 = NULL;
    
    Element *newHead = zero;
    while (head) {
        zero = searchZero();
        
        if (!zero) {
            makeNull();
            clearSet(zero);
            return *this;
        }
        removeFollowings(zero);
        disconnectElement(zero);
        //zero->next4 = NULL;
        searchLast(newHead)->next4 = zero;
    }
    
    head = newHead;
    
    return *this;
}

void PartOrdSet::print() {
    Element *temp = head;
    
    if (!head) {
        std::cout << "List is empty!" << std::endl;
        return;
    }
    
    while (temp) {
        std::cout << temp->value << " ";
        temp = temp->next4;
    }
    
    std::cout << std::endl;
}

int main() {
    PartOrdSet P;
    
    P.initSet();
    P.print();
    P.buildLinearOrder();
    P.print();
}
