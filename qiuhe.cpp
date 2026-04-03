#include <iostream>
#include <chrono>
using namespace std;

int main()
{
    const int daxiao = 1000000;
    static int sz[daxiao];

    for (int i = 0; i < daxiao; i++)
    {
        sz[i] = 1;
    }

    // 平凡算法
    auto start1 = chrono::high_resolution_clock::now();
    long long zong = 0;
    for (int i = 0; i < daxiao; i++)
    {
        zong += sz[i];
    }
    auto end1 = chrono::high_resolution_clock::now();
    double shi1 = chrono::duration<double>(end1 - start1).count();

    // 优化算法
    auto start2 = chrono::high_resolution_clock::now();
    long long he1 = 0, he2 = 0;
    for (int i = 0; i < daxiao; i += 2)
    {
        he1 += sz[i];
        he2 += sz[i + 1];
    }
    long long zong2 = he1 + he2;
    auto end2 = chrono::high_resolution_clock::now();
    double shi2 = chrono::duration<double>(end2 - start2).count();
    cout << "平凡算法时间：" << shi1 << endl;
    cout << "优化算法时间：" << shi2 << endl;
    cout << "加速：" << shi1 / shi2 << endl;

    return 0;
}