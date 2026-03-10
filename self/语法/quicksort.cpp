#include <iostream>
#include <vector>

using namespace std;

void quickSort(vector<int> &array, int l, int r)
{
    int i = l, j = r;
    int tmp;
    if (l >= r)
        return;
    int pivot = array[(l + r) / 2];
    while (i <= j)
    {
        while (array[i] < pivot)
            i++;
        while (array[j] > pivot)
            j--;
        if (i <= j)
        {
            tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
            i++;
            j--;
        }
    }
    if (l < j)
        quickSort(array, l, j);
    if (i < r)
        quickSort(array, i, r);
}