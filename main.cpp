#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;

vector<vector<double>> read_matrix(const string& filename, int& rows, int& cols) {
    ifstream fin(filename);
    if (!fin) {
        cerr << "Failed to open file: " << filename << endl;
        exit(1);
    }
    fin >> rows >> cols;
    vector<vector<double>> matrix(rows, vector<double>(cols));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            fin >> matrix[i][j];
    return matrix;
}

void write_matrix(const string& filename, const vector<vector<double>>& matrix) {
    ofstream fout(filename);
    int rows = matrix.size();
    int cols = matrix[0].size();
    fout << rows << " " << cols << endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j)
            fout << matrix[i][j] << " ";
        fout << endl;
    }
}

vector<vector<double>> multiply(const vector<vector<double>>& A, const vector<vector<double>>& B) {
    int n = A.size(), m = A[0].size(), k = B[0].size();
    vector<vector<double>> C(n, vector<double>(k, 0.0));
    #pragma omp parallel for
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < k; ++j)
            for (int l = 0; l < m; ++l)
                C[i][j] += A[i][l] * B[l][j];
    return C;
}

int main() {
    string fileA = "matrixA.txt";
    string fileB = "matrixB.txt";
    string fileC = "result.txt";

    int n1, m1, n2, m2;
    auto A = read_matrix(fileA, n1, m1);
    auto B = read_matrix(fileB, n2, m2);

    if (m1 != n2) {
        cerr << "Matrix multiplication is not possible: dimensions do not match." << endl;
        return 1;
    }

    auto start = chrono::high_resolution_clock::now();
    auto C = multiply(A, B);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    write_matrix(fileC, C);

    ofstream fout("info.txt");
    fout << "Matrix A size: " << n1 << "x" << m1 << endl;
    fout << "Matrix B size: " << n2 << "x" << m2 << endl;
    fout << "Result size: " << n1 << "x" << m2 << endl;
    fout << "Execution time (sec): " << elapsed.count() << endl;
    fout.close();

    cout << "Multiplication completed. Result in 'result.txt', info in 'info.txt'." << endl;
    return 0;
} 