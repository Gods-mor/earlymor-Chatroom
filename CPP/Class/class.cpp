/* simple class construction */

/* 要点如下：
 * 1.默认构造函数
 * 2.含参构造函数
 * 3.析构函数
 * 4.深拷贝
 * 5.浅拷贝
 */
#include <iostream>

using namespace std;
class Person {
   public:
    Person() { cout << "默认构造函数" << endl; }
    Person(int age) {
        cout << "含参构造函数" << endl;
        m_age = age;
    }
    Person(int age, int height) {
        m_age = age;
        m_height = new int(height);
    }
    ~Person() {
        if (m_height != NULL) {
            delete m_height;
            m_height = NULL;
        }
        cout << "析构函数" << endl;
    }
    Person(Person& p) {
        cout << "深拷贝构造函数" << endl;
        m_age = p.m_age;
        m_height = new int(*p.m_height);  // 分配空间
        cout << "m_age:" << m_age << endl;
        cout << "*m_height:" << *m_height << endl;
    }
#if 0
    Person(Person& p) {
        cout << "浅拷贝构造函数" << endl;
        m_age = p.m_age;
        m_height = p.m_height; // 直接逐字节拷贝
        cout << "m_age:" << m_age << endl;
        cout << "*m_height:" << *m_height << endl;
    }
#endif
    int m_age;
    int* m_height;
    static int m_name;
};
int Person::m_name = 100;
int main() {
    Person p1(18, 160);
    // p1.m_height = new int(160);
    Person p2(p1);
    Person p3 = p1;
    cout << "";
    return 0;
}