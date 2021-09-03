#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// just for fun
// 多线程循环打印A，B，C

using namespace std;


class print {
public:
    print():flag (1) {}
    void printa() {
        unique_lock<mutex> lk(mtx);
        for (int i = 0; i < 10; i++) {
            while (flag != 1) cv.wait(lk);
            cout << "a" << endl;
            flag = 2;
            cv.notify_all();
        }
        cout << "a finish" << endl;
    }
    void printb() {
        unique_lock<mutex> lk(mtx);
        for (int i = 0; i < 10; i++) {
            while (flag != 2) cv.wait(lk);
            cout << "b" << endl;
            flag = 3;
            cv.notify_all();
        }
        cout << "b finish" << endl;
    }
    void printc() {
        unique_lock<mutex> lk(mtx);
        for (int i = 0; i < 10; i++) {
            while (flag != 3) cv.wait(lk);
            cout << "c" << endl;
            flag = 1;
            cv.notify_all();
        }
        cout << "c finish" << endl;
    }
private:
    mutex mtx;
    condition_variable cv;
    int flag;
};

int main() {
    print pt;
    thread t1(&print::printa, ref(pt));
    thread t2(&print::printb, ref(pt));
    thread t3(&print::printc, ref(pt));
    t1.join();
    t2.join();
    t3.join();
    return 0;
}