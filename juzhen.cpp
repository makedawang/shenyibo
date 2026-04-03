#include <iostream>
#include <chrono>
using namespace std;
const int changdu = 512;
int ju[512][512] = { 0 };
int xiang[512] = { 0 };
int jie[512] = { 0 };

int main()
{
    for (int i = 0; i < changdu; i++)
    {
        xiang[i] = 2;
        for (int j = 0; j < changdu; j++)
        {
            ju[i][j] = 1;
        }
    }

    // 平凡算法
    auto start1 = chrono::high_resolution_clock::now();
    for (int lie = 0; lie < changdu; lie++)
    {
        jie[lie] = 0;
        for (int hang = 0; hang < changdu; hang++)
        {
            jie[lie] += ju[hang][lie] * xiang[lie];
        }
    }
    auto end1 = chrono::high_resolution_clock::now();
    double shi1 = chrono::duration<double>(end1 - start1).count();

    // 优化算法
    auto start2 = chrono::high_resolution_clock::now();
    for (int hang = 0; hang < changdu; hang++)
    {
        for (int lie = 0; lie < changdu; lie++)
        {
            jie[lie] += ju[hang][lie] * xiang[lie];
        }
    }
    auto end2 = chrono::high_resolution_clock::now();
    double shi2 = chrono::duration<double>(end2 - start2).count();

    cout << "平凡算法时间：" << shi1 << endl;
    cout << "优化算法时间：" << shi2 << endl;
    cout << "加速：" << shi1 / shi2 << endl;

    return 0;
}