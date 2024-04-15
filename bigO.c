#include <stdio.h>

int main() {
    printf("Hello World!");
}

int addTen(int num) { return num + 10; }


// void addArrays(int arr1[], int size1, int arr2[], int size2) {
//   int maxSize;
//   if (size1 > size2) {
//     maxSize = size1;
//   } else {
//     maxSize = size2;
//   }
//   int i;
//   int sumArr[maxSize];

//   for (i = 0; i < maxSize; i++) {
//     sumArr[i] = arr1[i] + arr2[i];
//     printf("%d", sumArr[i]);
//   }

// }

int addArrays(int arr1[], int size1, int arr2[], int size2) {
  int i;
  int j;
  int sum;
  for (i = 0; i < size1; i++) {
    for (j = 0; j < size2; j++) {
      sum += arr1[i] + arr2[j];
    }
  }
  return sum;
};

int recurs(int x) {
  if (x == 0) {
    return 1;
  }

  for (int i = 0; i < x; i++) {
    recurs(x--);
  }
}