#include <iostream>
using namespace std;
class Person1 {};
class Person {
   public:
    int m_age;            // 非静态成员变量 属于类上对象
    static int m_height;  // 静态成员变量 不属于类上对象
    void getAge() { cout << "age: " << m_age << endl; }
};
int Person::m_height = 100;
void test01() {
    // 类成员变量和成员函数分开存储
    Person p;
    cout << "sizeof p: " << sizeof(p) << endl;  //  输出 sizeof p: 4
    Person1 p1;  // 空对象分配独一无二的地址空间，防止冲突
    cout << "sizeof p1: " << sizeof(p1) << endl;  //  输出 sizeof p1: 1
}
int main() {
    test01();
    return 0;
}