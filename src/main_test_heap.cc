// range heap example
#include <iostream>     // std::cout
#include <algorithm>    // std::make_heap, std::pop_heap, std::push_heap, std::sort_heap
#include <vector>       // std::vector
#include <queue>

using namespace std;


void pushNew(int newValue, std::priority_queue<int, std::vector<int>>&queue) {
    queue.push(newValue);
    if(queue.size()>5)
        queue.pop();
}

int main () {
    std::priority_queue<int, std::vector<int>> minimumValues;

    pushNew(1, minimumValues);
    pushNew(2, minimumValues);
    pushNew(3, minimumValues);
    pushNew(4, minimumValues);
    pushNew(1, minimumValues);
    pushNew(2, minimumValues);
    pushNew(3, minimumValues);
    pushNew(4, minimumValues);
    pushNew(1, minimumValues);
    pushNew(2, minimumValues);
    pushNew(3, minimumValues);
    pushNew(4, minimumValues);

    // One by one extract items from max heap
    while (minimumValues.empty() == false)
    {
        cout<< minimumValues.top() << " ";
        minimumValues.pop();
    }



    return 0;
}