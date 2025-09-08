#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <chrono>

// =====================
// Contadores globais
// =====================
long long atribuicoes = 0;
long long comparacoes = 0;

void resetCounters()
{
    atribuicoes = 0;
    comparacoes = 0;
}

void printCounters(const std::string &name)
{
    std::cout << name << " -> Comparações: " << comparacoes
              << ", Atribuições: " << atribuicoes
              << ", Total: " << (comparacoes + atribuicoes) << "\n";
}

// =====================
// Função para gerar vetor do melhor caso para QuickSort
// =====================
std::vector<int> generateBestCaseVector(int size)
{
    std::vector<int> arr(size);
    // Preenche com valores de 1 a size
    for (int i = 0; i < size; i++)
    {
        arr[i] = i + 1;
    }
    // Embaralha levemente
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(arr.begin(), arr.end(), g);
    // Ajusta para garantir pivô central próximo da mediana
    std::sort(arr.begin(), arr.end(), [size](int a, int b)
              {
        int median = size / 2;
        return abs(a - median) < abs(b - median); });
    return arr;
}

// =====================
// Insertion Sort
// =====================
void insertionSort(std::vector<int> &arr)
{
    int n = arr.size();
    for (int i = 1; i < n; i++)
    {
        int temp = arr[i];
        atribuicoes++;
        int j = i - 1;
        while (j >= 0)
        {
            comparacoes++;
            if (arr[j] > temp)
            {
                arr[j + 1] = arr[j];
                atribuicoes++;
                j--;
            }
            else
                break;
        }
        arr[j + 1] = temp;
        atribuicoes++;
    }
}

// =====================
// Shell Sort
// =====================
void shellSort(std::vector<int> &arr)
{
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            int temp = arr[i];
            atribuicoes++;
            int j = i;
            while (j >= gap)
            {
                comparacoes++;
                if (arr[j - gap] > temp)
                {
                    arr[j] = arr[j - gap];
                    atribuicoes++;
                    j -= gap;
                }
                else
                    break;
            }
            arr[j] = temp;
            atribuicoes++;
        }
    }
}

// =====================
// Quick Sort
// =====================
int partition(std::vector<int> &arr, int low, int high)
{
    int pivot = arr[high];
    atribuicoes++;
    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        comparacoes++;
        if (arr[j] <= pivot)
        {
            i++;
            std::swap(arr[i], arr[j]);
            atribuicoes += 3;
        }
    }
    std::swap(arr[i + 1], arr[high]);
    atribuicoes += 3;
    return i + 1;
}

void quickSort(std::vector<int> &arr, int low, int high)
{
    comparacoes++;
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// =====================
// Merge Sort
// =====================
void merge(std::vector<int> &arr, int left, int mid, int right)
{
    int n1 = mid - left + 1, n2 = right - mid;
    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[left + i];
        atribuicoes++;
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[mid + 1 + j];
        atribuicoes++;
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        comparacoes++;
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            atribuicoes++;
            i++;
        }
        else
        {
            arr[k] = R[j];
            atribuicoes++;
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        atribuicoes++;
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = R[j];
        atribuicoes++;
        j++;
        k++;
    }
}

void mergeSort(std::vector<int> &arr, int left, int right)
{
    comparacoes++;
    if (left >= right)
        return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

int main()
{
    // Vetores para Insertion Sort e Shell Sort
    std::vector<int> insertion_best_10 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> insertion_best_100 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                                           21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                                           41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
                                           61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
                                           81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100};
    std::vector<int> insertion_best_1000;
    for (int i = 1; i <= 1000; i++)
        insertion_best_1000.push_back(i);

    std::vector<int> insertion_worst_10 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<int> insertion_worst_100;
    for (int i = 100; i >= 1; i--)
        insertion_worst_100.push_back(i);
    std::vector<int> insertion_worst_1000;
    for (int i = 1000; i >= 1; i--)
        insertion_worst_1000.push_back(i);

    // Shell Sort: Melhor e pior caso (reutiliza os de Insertion Sort)
    std::vector<int> shell_best_10 = insertion_best_10;
    std::vector<int> shell_best_100 = insertion_best_100;
    std::vector<int> shell_best_1000 = insertion_best_1000;
    std::vector<int> shell_worst_10 = insertion_worst_10;
    std::vector<int> shell_worst_100 = insertion_worst_100;
    std::vector<int> shell_worst_1000 = insertion_worst_1000;

    // QuickSort: Melhor caso (vetores gerados para divisões equilibradas)
    std::vector<int> quick_best_10 = generateBestCaseVector(10);
    std::vector<int> quick_best_100 = generateBestCaseVector(100);
    std::vector<int> quick_best_1000 = generateBestCaseVector(1000);

    // QuickSort: Pior caso (vetor ordenado, pois pivô é o último elemento)
    std::vector<int> quick_worst_10 = insertion_best_10; // Já ordenado
    std::vector<int> quick_worst_100 = insertion_best_100;
    std::vector<int> quick_worst_1000 = insertion_best_1000;

    std::srand(std::time(nullptr));

    // Insertion Sort

    std::cout << "======================================================" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    insertionSort(insertion_best_10);
    auto stop = std::chrono::high_resolution_clock::now();
    printCounters("Insertion Sort Melhor Caso 10");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    insertionSort(insertion_best_100);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Insertion Sort Melhor Caso 100");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    insertionSort(insertion_best_1000);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Insertion Sort Melhor Caso 1000");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    std::cout << "------------------------------------------------------" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    insertionSort(insertion_worst_10);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Insertion Sort Pior Caso 10");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    insertionSort(insertion_worst_100);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Insertion Sort Pior Caso 100");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    insertionSort(insertion_worst_1000);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Insertion Sort Pior Caso 1000");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;

    // Shell Sort

    std::cout << "======================================================" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    shellSort(shell_best_10);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Shell Sort Melhor Caso 10");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    shellSort(shell_best_100);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Shell Sort Melhor Caso 100");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    shellSort(shell_best_1000);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Shell Sort Melhor Caso 1000");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    std::cout << "------------------------------------------------------" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    shellSort(shell_worst_10);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Shell Sort Pior Caso 10");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    shellSort(shell_worst_100);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Shell Sort Pior Caso 100");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    shellSort(shell_worst_1000);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Shell Sort Pior Caso 1000");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;

    // Quick Sort

    std::cout << "======================================================" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    quickSort(quick_best_10, 0, quick_best_10.size() - 1);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Quick Sort Melhor Caso 10");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    quickSort(quick_best_100, 0, quick_best_100.size() - 1);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Quick Sort Melhor Caso 100");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    quickSort(quick_best_1000, 0, quick_best_1000.size() - 1);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Quick Sort Melhor Caso 1000");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    std::cout << "------------------------------------------------------" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    quickSort(quick_worst_10, 0, quick_worst_10.size() - 1);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Quick Sort Pior Caso 10");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    quickSort(quick_worst_100, 0, quick_worst_100.size() - 1);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Quick Sort Pior Caso 100");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    quickSort(quick_worst_1000, 0, quick_worst_1000.size() - 1);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Quick Sort Pior Caso 1000");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;

    // Merge Sort

    std::cout << "======================================================" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    mergeSort(insertion_best_10, 0, insertion_best_10.size() - 1);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Merge Sort 10");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    mergeSort(insertion_best_100, 0, insertion_best_100.size() - 1);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Merge Sort 100");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();
    start = std::chrono::high_resolution_clock::now();
    mergeSort(insertion_best_1000, 0, insertion_best_1000.size() - 1);
    stop = std::chrono::high_resolution_clock::now();
    printCounters("Merge Sort 1000");
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us" << std::endl;
    resetCounters();

    return 0;
}