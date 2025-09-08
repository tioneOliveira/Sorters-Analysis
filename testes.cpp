#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>

std::string vectorString(const std::vector<int> &vector)
{
    std::string vectorItens;
    vectorItens.reserve(vector.size() * 5);
    for (size_t i = 0; i < vector.size(); i++)
    {
        vectorItens += std::to_string(vector[i]);
        if (i != vector.size() - 1)
            vectorItens += ", ";
    }
    return vectorItens;
}

std::vector<int> createUnorganizedVector(int length, int minimum, int maximum)
{
    std::vector<int> unorganizedVector;
    unorganizedVector.reserve(length);
    int range = maximum - minimum + 1;

    for (int i = 0; i < length; i++)
    {
        unorganizedVector.push_back(minimum + (rand() % range));
    }
    return unorganizedVector;
}

void insertionSort(std::vector<int> &unorganizedVector)
{
    int vectorSize = unorganizedVector.size();

    for (int i = 1; i < vectorSize; i++)
    {
        int temp = unorganizedVector[i];
        int j = i - 1;

        while (j >= 0 && unorganizedVector[j] > temp)
        {
            unorganizedVector[j + 1] = unorganizedVector[j];
            j--;
        }
        unorganizedVector[j + 1] = temp;
    }
}

void shellSort(std::vector<int> &unorganizedVector)
{
    int n = unorganizedVector.size();
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            int temp = unorganizedVector[i];
            int j;
            for (j = i; j >= gap && unorganizedVector[j - gap] > temp; j -= gap)
                unorganizedVector[j] = unorganizedVector[j - gap];
            unorganizedVector[j] = temp;
        }
    }
}

int partition(std::vector<int> &vec, int low, int high)
{
    int mid = low + (high - low) / 2;
    if (vec[mid] < vec[low])
        std::swap(vec[low], vec[mid]);
    if (vec[high] < vec[low])
        std::swap(vec[low], vec[high]);
    if (vec[mid] < vec[high])
        std::swap(vec[mid], vec[high]);

    int pivot = vec[high];
    int i = (low - 1);

    for (int j = low; j < high; j++)
    {
        if (vec[j] <= pivot)
        {
            i++;
            std::swap(vec[i], vec[j]);
        }
    }
    std::swap(vec[i + 1], vec[high]);
    return i + 1;
}

void quickSort(std::vector<int> &vec, int low, int high)
{
    if (low < high)
    {
        int pi = partition(vec, low, high);
        quickSort(vec, low, pi - 1);
        quickSort(vec, pi + 1, high);
    }
}

void merge(std::vector<int> &arr, int left,
           int mid, int right)
{

    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<int> &arr, int left, int right)
{

    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

long measureSortTimeOn2(void (*sortFunc)(std::vector<int> &), std::vector<int> &unorganizedVector)
{
    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(unorganizedVector);
    auto stop = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
}

long measureSortTimeRecursion(void (*sortFuncRec)(std::vector<int> &, int, int), std::vector<int> &unorganizedVector)
{
    int vecQuickSortSize = unorganizedVector.size();
    auto start = std::chrono::high_resolution_clock::now();
    sortFuncRec(unorganizedVector, 0, vecQuickSortSize - 1);
    auto stop = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
}

std::string tests(int vectorSize, int timesToRun)
{
    long insertionSortClock = 0;
    long insertionSortClockOrg = 0;

    long shellSortClock = 0;
    long shellSortClockOrg = 0;

    long quickSortClock = 0;
    long quickSortClockOrg = 0;

    long mergeSortClock = 0;
    long mergeSortClockOrg = 0;

    for (int i = 0; i < timesToRun; i++)
    {
        std::vector<int> vecTest = createUnorganizedVector(vectorSize, 0, 1500);
        insertionSortClock += measureSortTimeOn2(insertionSort, vecTest);
        insertionSortClockOrg += measureSortTimeOn2(insertionSort, vecTest);

        vecTest = createUnorganizedVector(vectorSize, 0, 1500);
        shellSortClock += measureSortTimeOn2(shellSort, vecTest);
        shellSortClockOrg += measureSortTimeOn2(shellSort, vecTest);

        vecTest = createUnorganizedVector(vectorSize, 0, 1500);
        quickSortClock += measureSortTimeRecursion(quickSort, vecTest);
        quickSortClockOrg += measureSortTimeRecursion(quickSort, vecTest);

        vecTest = createUnorganizedVector(vectorSize, 0, 1500);
        mergeSortClock += measureSortTimeRecursion(mergeSort, vecTest);
        mergeSortClockOrg += measureSortTimeRecursion(mergeSort, vecTest);
    }

    std::stringstream result;
    result << "Results for Vector Size (microseconds/ms): " << vectorSize << "\n";
    result << "Insertion Sort: " << (insertionSortClock / timesToRun) << " us / "
           << std::fixed << std::setprecision(4) << ((shellSortClock / timesToRun) / 1000.0) << " ms\n";
    result << "Insertion Sort ja organizado: " << (insertionSortClockOrg / timesToRun) << " us / "
           << std::fixed << std::setprecision(4) << ((shellSortClockOrg / timesToRun) / 1000.0) << " ms\n";
    result << "Shell Sort: " << (shellSortClock / timesToRun) << " us / "
           << std::fixed << std::setprecision(4) << ((shellSortClock / timesToRun) / 1000.0) << " ms\n";
    result << "Shell Sort ja organizado: " << (shellSortClockOrg / timesToRun) << " us / "
           << std::fixed << std::setprecision(4) << ((shellSortClockOrg / timesToRun) / 1000.0) << " ms\n";
    result << "Quick Sort: " << (quickSortClock / timesToRun) << " us / "
           << std::fixed << std::setprecision(4) << ((quickSortClock / timesToRun) / 1000.0) << " ms\n";
    result << "Quick Sort ja organizado: " << (quickSortClockOrg / timesToRun) << " us / "
           << std::fixed << std::setprecision(4) << ((quickSortClockOrg / timesToRun) / 1000.0) << " ms\n";
    result << "Merge Sort: " << (mergeSortClock / timesToRun) << " us / "
           << std::fixed << std::setprecision(4) << ((mergeSortClock / timesToRun) / 1000.0) << " ms\n";
    result << "Merge Sort ja organizado: " << (mergeSortClockOrg / timesToRun) << " us / "
           << std::fixed << std::setprecision(4) << ((mergeSortClockOrg / timesToRun) / 1000.0) << " ms\n";
    result << "----------------------------------\n";

    return result.str();
}
int main()
{
    srand((unsigned)time(NULL));

    std::string textForFile = "";
    std::ofstream testResultFile("aula4Teste.txt");

    int timesToRun = 1;

    textForFile += tests(1000, timesToRun);
    std::cout << textForFile << std::endl;

    textForFile += tests(5000, timesToRun);
    std::cout << textForFile << std::endl;

    textForFile += tests(10000, timesToRun);
    std::cout << textForFile << std::endl;

    textForFile += tests(50000, timesToRun);
    std::cout << textForFile << std::endl;

    textForFile += tests(100000, timesToRun);
    std::cout << textForFile << std::endl;

    textForFile += tests(250000, timesToRun);
    std::cout << textForFile << std::endl;

    textForFile += tests(500000, timesToRun);
    std::cout << textForFile << std::endl;

    testResultFile << textForFile;
    testResultFile.close();
    return 0;
}