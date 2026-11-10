#include <iostream>
#include<clocale>

struct Run {
    int start;
    int length;
};

int minRunLength(int n) {
    int remainder = 0;
    while (n >= 64) {
        remainder |= (n & 1);
        n >>= 1;
    }
    return n + remainder;
}

void reverseRange(int* array, int left, int right) {
    while (left < right) {
        int value = array[left];
        array[left] = array[right];
        array[right] = value;
        ++left;
        --right;
    }
}

void binaryInsertionSort(int* array, int left, int right, int sortedEnd) {
    for (int i = sortedEnd; i < right; ++i) {
        int value = array[i];
        int low = left;
        int high = i;
        while (low < high) {
            int middle = low + (high - low) / 2;
            if (value < array[middle]) high = middle;
            else low = middle + 1;
        }
        for (int j = i; j > low; --j) array[j] = array[j - 1];
        array[low] = value;
    }
}

int countRunAndMakeAscending(int* array, int start, int n) {
    int end = start + 1;
    if (end == n) return 1;

    // делаем run строго убывающим
    if (array[end] < array[start]) {
        while (end < n && array[end] < array[end - 1]) ++end;
        reverseRange(array, start, end - 1);
    } else {
        while (end < n && array[end] >= array[end - 1]) ++end;
    }
    return end - start;
}

int upperBound(const int* array, int length, int key) {
    int low = 0, high = length;
    while (low < high) {
        int middle = low + (high - low) / 2;
        if (key < array[middle]) high = middle;
        else low = middle + 1;
    }
    return low;
}

int lowerBound(const int* array, int length, int key) {
    int low = 0, high = length;
    while (low < high) {
        int middle = low + (high - low) / 2;
        if (array[middle] < key) low = middle + 1;
        else high = middle;
    }
    return low;
}

void mergeRuns(int* array, const Run& leftRun, const Run& rightRun,
               int* leftTemp, int* rightTemp) {
    const int leftLength = leftRun.length;
    const int rightLength = rightRun.length;
    for (int i = 0; i < leftLength; ++i) leftTemp[i] = array[leftRun.start + i];
    for (int i = 0; i < rightLength; ++i) rightTemp[i] = array[rightRun.start + i];

    int i = 0, j = 0, out = leftRun.start;
    const int minGallop = 7;
    while (i < leftLength && j < rightLength) {
        int leftWins = 0, rightWins = 0;
        while (i < leftLength && j < rightLength &&
               leftWins < minGallop && rightWins < minGallop) {
            if (leftTemp[i] <= rightTemp[j]) {
                array[out++] = leftTemp[i++];
                ++leftWins;
                rightWins = 0;
            } else {
                array[out++] = rightTemp[j++];
                ++rightWins;
                leftWins = 0;
            }
        }

        // галоп
        while (i < leftLength && j < rightLength) {
            int leftBlock = upperBound(leftTemp + i, leftLength - i, rightTemp[j]);
            for (int k = 0; k < leftBlock; ++k) array[out++] = leftTemp[i++];
            if (i == leftLength) break;

            int rightBlock = lowerBound(rightTemp + j, rightLength - j, leftTemp[i]);
            for (int k = 0; k < rightBlock; ++k) array[out++] = rightTemp[j++];
            if (j == rightLength) break;

            // если не было достаточно длинной серии, прекращаем галоп, потом - продолжаем
            leftWins = 0;
            rightWins = 0;
            while (i < leftLength && j < rightLength &&
                   leftWins < minGallop && rightWins < minGallop) {
                if (leftTemp[i] <= rightTemp[j]) {
                    array[out++] = leftTemp[i++];
                    ++leftWins;
                    rightWins = 0;
                } else {
                    array[out++] = rightTemp[j++];
                    ++rightWins;
                    leftWins = 0;
                }
            }
            if (leftWins < minGallop && rightWins < minGallop) break;
        }
    }
    while (i < leftLength) array[out++] = leftTemp[i++];
    while (j < rightLength) array[out++] = rightTemp[j++];
}

void timSort(int* array, int n) {
    if (n < 2) return;

    const int minRun = minRunLength(n);
    Run* runs = new Run[n];
    int* leftTemp = new int[n];
    int* rightTemp = new int[n];
    int runCount = 0;

    // с соседних run, когда нарушены инварианты стека timsort (XYZ)
    auto mergeAt = [&](int index) {
        Run left = runs[index];
        Run right = runs[index + 1];
        mergeRuns(array, left, right, leftTemp, rightTemp);
        runs[index].length += runs[index + 1].length;
        for (int k = index + 1; k < runCount - 1; ++k) runs[k] = runs[k + 1];
        --runCount;
    };

    int position = 0;
    while (position < n) {
        int runLength = countRunAndMakeAscending(array, position, n);
        int forcedLength = n - position < minRun ? n - position : minRun;
        if (runLength < forcedLength) {
            binaryInsertionSort(array, position, position + forcedLength,
                                position + runLength);
            runLength = forcedLength;
        }
        runs[runCount].start = position;
        runs[runCount].length = runLength;
        ++runCount;
        position += runLength;

        while (runCount > 1) {
            int nRuns = runCount;
            int mergeIndex = -1;
            if (nRuns >= 3 && runs[nRuns - 3].length <=
                                  runs[nRuns - 2].length + runs[nRuns - 1].length) {
                mergeIndex = runs[nRuns - 3].length < runs[nRuns - 1].length
                                 ? nRuns - 3 : nRuns - 2;
            } else if (runs[nRuns - 2].length <= runs[nRuns - 1].length) {
                mergeIndex = nRuns - 2;
            }
            if (mergeIndex < 0) break;
            mergeAt(mergeIndex);
        }
    }

    while (runCount > 1) {
        int nRuns = runCount;
        int mergeIndex = nRuns - 2;
        if (nRuns >= 3 && runs[nRuns - 3].length < runs[nRuns - 1].length)
            mergeIndex = nRuns - 3;
        mergeAt(mergeIndex);
    }

    delete[] rightTemp;
    delete[] leftTemp;
    delete[] runs;
}

int main() {
    setlocale(LC_ALL, ".UTF8");
    const int sizes[] = {10, 128, 500};
    const int scenarioCount = 3;

    // Для больших массивов печатаем первые и последние 10 значений.
    auto printArrayPreview = [](const int* array, int n) {
        const int previewSize = 10;
        if (n <= previewSize * 2) {
            for (int i = 0; i < n; ++i) {
                if (i > 0) std::cout << ' ';
                std::cout << array[i];
            }
        } else {
            for (int i = 0; i < previewSize; ++i) {
                if (i > 0) std::cout << ' ';
                std::cout << array[i];
            }
            std::cout << " ... (пропущено " << n - previewSize * 2 << ") ... ";
            for (int i = n - previewSize; i < n; ++i) {
                if (i > n - previewSize) std::cout << ' ';
                std::cout << array[i];
            }
        }
        std::cout << '\n';
    };

    for (int scenario = 0; scenario < scenarioCount; ++scenario) {
        int n = sizes[scenario];
        int* array = new int[n];

        // Создаём воспроизводимый, перемешанный пример без генератора случайных чисел.
        for (int i = 0; i < n; ++i) array[i] = (i * 37) % n;

        std::cout << "Сценарий " << scenario + 1 << ": массив из " << n << " элементов\n";
        std::cout << "minrun = " << minRunLength(n) << '\n';
        std::cout << "До сортировки:    ";
        printArrayPreview(array, n);

        timSort(array, n);

        std::cout << "После сортировки: ";
        printArrayPreview(array, n);
        std::cout << '\n';
        delete[] array;
    }
    return 0;
}
