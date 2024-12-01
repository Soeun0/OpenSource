#include <iostream>
#include <vector>

using namespace std;

int main() {
    int rows, cols;

    cin >> rows >> cols;

    vector<vector<int>> matrix1(rows, vector<int>(cols));
    vector<vector<int>> matrix2(rows, vector<int>(cols));
    vector<vector<int>> result(rows, vector<int>(cols));

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            cin >> matrix1[i][j];

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            cin >> matrix2[i][j];

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result[i][j] = matrix1[i][j] + matrix2[i][j];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            cout << result[i][j] << " ";
        cout << endl;
    }

    return 0;
}

