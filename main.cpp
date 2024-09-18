#include <iostream>
using namespace std;


int main(){
    cout<<"Привіт"<< endl;

    cout<<"Скажіть скільки вам років, для перевірки на повонолітність"<< endl;
    int a;
    cin>>a;
    if (a>17){
        cout<<"Успішно, ви повнолітній!";
    }
    else {
        cout<<"Успішно, ви не повнолітній ";
    }
    

}