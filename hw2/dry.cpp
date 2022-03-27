
#include <vector>
#include <iostream>
#include <memory>
#include <set>
#include <math.h>
/*class BadInput
{};

//question 5.1
template <class T>
std::vector<T> slice(std::vector<T> vec, int start, int step, int stop)
{
    //Bad input cases
    if((start < 0) || (start >= vec.size()))
    {
        throw BadInput();
    }
    if((stop < 0) || (stop > vec.size()))
    {
        throw BadInput();
    }
    if(step <= 0)
    {
        throw BadInput();
    }
    if(start >= stop)
    {
        std::vector<T> new_vector;
        return new_vector; //returns an empty new vector
    }

    //The function
    std::vector<T> new_vector;
    for(int index = start; index < stop; index += step )
    {
        new_vector.push_back(vec[index]);
    }
    return new_vector;
}
*/


using namespace std;


template<class T>
class TwoWayStack{
    private:
        class Node {
            private:
                T data;
                Node* next;
                Node* prev;
            public:
                Node(const T& data);
                Node(const Node& other);
                virtual ~Node();
                friend class TwoWayStack<T>;
                Node& operator=(const Node& other);
        };
    Node* top;
    Node* bottom;
    public:
       TwoWayStack(const T& item);   
       ~TwoWayStack();
       TwoWayStack(const TwoWayStack& tws);
       TwoWayStack& operator=(const TwoWayStack& other);
       void pushTop(const T& value);
       void pushBottom(const T& value);
       friend ostream& operator<<(ostream& os, const TwoWayStack& tws);
       void popTop();
       void popBottom();
       T& bottom() const;
       T& top() const;
       friend bool operator<(const TwoWayStack& tws1, const TwoWayStack& tws2);
       friend bool operator>=(const TwoWayStack& tws1, const TwoWayStack& tws2);
       int countVertices() const;
};

template<class T>
bool operator<(const TwoWayStack<T>& tws1, const TwoWayStack<T>& tws2);

template<class T>
bool operator>=(const TwoWayStack<T>& tws1, const TwoWayStack<T>& tws2);

template<class T>
ostream& operator<<(ostream& os, const TwoWayStack<T>& tws);

template<class T>
int TwoWayStack<T>::countVertices() const{
    int counter = 0;
    Node* temp = top;
    while(temp != nullptr){
        temp = top->next;
        counter++
    }
    return counter;
}

template<class T>
void TwoWayStack<T>::popBottom(){
    class NullNode: public std::exception {};
    if(bottom == nullptr)
    {
        throw NullNode();
    }
    bottom->next = nullptr;
    bottom->prev->next = nullptr;
    Node* temp = bottom;
    bottom = bottom->prev;
    ~temp();
}

class Fruit {

    public:
        Fruit* clone();
        bool CheckQuality();
};


class Factory
{
    private:
        TwoWayStack<shared_ptr<Fruit>> track;
    public:

        void AddFruit(Fruit* pFruit){
            track.pushTop(shared_ptr<pFruit->clone()>);
        }
        void CheckFruit(){
            try{
                shared_ptr<Fruit>* f = track.bottom();
                track.popBottom();
            }catch(std::exception& e){
                return;
            }
        }
};