#include <iostream>
#include <vector>
using namespace std;
//merge函数 
void merge(vector<int> &a, vector<int> &temp, int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        if (a[i] <= a[j]) temp[k++] = a[i++];
        else temp[k++] = a[j++];
    }
    while (i <= mid) temp[k++] = a[i++];
    while (j <= right) temp[k++] = a[j++];
    for (int p = left; p <= right; ++p) a[p] = temp[p];
}

void mergeSortIterative(vector<int> &a) {
    int n = a.size();
    vector<int> temp(n);

    // 子数组长度从 1、2、4、8 ... 逐步扩大 步长
    for (int step = 1; step < n; step *= 2) {
        //遍历 通过step
        for (int left = 0; left + step < n; left += 2 * step) {
            int mid = left + step - 1;
            //判断边界和n 总边界那个大 
            int right = min(left + 2 * step - 1, n - 1);
            merge(a, temp, left, mid, right);
        }
    }
}

int main() {
    vector<int> a = {5, 2, 4, 7, 1, 3, 2, 6};
    mergeSortIterative(a);
    for (int x : a) cout << x << " ";
}
