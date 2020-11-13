#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;
/*
class A {
public:
    vector< vector<string> > vv;
};

class B {
public:
    vector<A> av;
    vector<int> iv;
};

int main() {
    
    vector<string> v1, v2;
    v1.push_back("hello");
    v1.push_back("world");
    v2.push_back("123");
    v2.push_back("456");
    A a1;
    a1.vv.push_back(v1);
    a1.vv.push_back(v2);

    vector<string> v3 = v1;
    a1.vv.push_back(v3);

    A a2;
    a2 = a1;
    B b1;
    b1.av.push_back(a1);
    b1.av.push_back(a2);
    b1.iv.push_back(1);
    b1.iv.push_back(2);

    B b2;
    b2 = b1;
    
    b2.av[1].vv[2][0] = "abcdef";

    B b3 = b1;
    cout << b3.av[1].vv[2][0] << endl;
    cout << b3.av[1].vv[2][1] << endl;
    cout << b3.av[1].vv[1][0] << endl;
    cout << b3.av[1].vv[1][1] << endl;
    cout << b3.iv[0] << endl;
    cout << b3.iv[1] << endl;
    b3 = b2;
    cout << b3.av[1].vv[2][0] << endl;
    cout << b3.av[1].vv[2][1] << endl;
    cout << b3.av[1].vv[1][0] << endl;
    cout << b3.av[1].vv[1][1] << endl;
    cout << b3.iv[0] << endl;
    cout << b3.iv[1] << endl;
    return 0;    
}
*/
/*
class A {
public:
    A() {
        cout << "constructor " << this << endl;
    }
    A(const A& a) {
        cout << "constructor1" << this << endl;
    }
    const A& operator=(const A& a) {
        cout << "constructor2" << this << endl;
        return *this;
    }
    const A& operator=(A& a) {
        cout << "constructor3" << this << endl;
        return *this;
    }
    ~A() {
        cout << "distructor" << this << endl;
    }
};
int main() {
    vector<A> s1;
    vector<A> s2;
    A a1, a2, a3;
    s1.push_back(a1);
    s1.push_back(a2);
    s2.push_back(a3);
    cout << "before s3 = s1" << endl;
    vector<A> s3;
    s3 = s1;
    cout << s3.size() << endl;
    cout << "after s3 = s1" << endl;
    cout << "before s3 = s2" << endl;
    s3 = s2;
    cout << s3.size() << endl;
    cout << "after s3 = s2" << endl;
    return 0;
}
*/

/*
int main() {
    ifstream ifs("data/hello.txt");
    ostringstream oss;
    oss << ifs.rdbuf();
    string raw_data = oss.str();
    ifs.close();
    if (raw_data.empty() == true) {
        cout << "empty content" << endl;
    }
    return 0;
}
*/

/*
class A {
private:
    int i;
    static A a;

    A() {
        i = 5;
    }
    const string mToString() {
        return to_string(i);
    }
public:
    static const string toString() {
        return a.mToString();
    }
};
A A::a;

int main() {
    cout << A::toString() << endl;
    return 0;
}
*/

int main() {
    map<string, int> m;
    m["abc"] = 1;
    cout << "size is " << m.size() << endl;
    cout << "value is " << m["abc"] << endl;
    m["abc"] = 2;
    cout << "size is " << m.size() << endl;
    cout << "value is " << m["abc"] << endl;
    return 0;
}