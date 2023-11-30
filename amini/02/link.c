int average(int a, int b) {
    return (a + b) / 2;
}

int sum(int* numbers, int count) {
    int sum = 0;
    for (int i = 0; i < count; i++) {
        sum += numbers[i];
    }
    return sum;
}