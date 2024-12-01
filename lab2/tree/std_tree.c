#include <iostream>
#include <string>

using namespace std;

struct Student {
    string name;
    int midtermScore;
    int finalScore;
};

struct TreeNode {
    Student student;
    TreeNode* left;
    TreeNode* right;
};

TreeNode* insert(TreeNode* root, const Student& student) {
    if (root == nullptr) {
        TreeNode* newNode = new TreeNode{student, nullptr, nullptr};
        return newNode;
    }
    if (student.name < root->student.name) {
        root->left = insert(root->left, student);
    } else {
        root->right = insert(root->right, student);
    }
    return root;
}

TreeNode* search(TreeNode* root, const string& name) {
    if (root == nullptr || root->student.name == name) {
        return root;
    }
    if (name < root->student.name) {
        return search(root->left, name);
    } else {
        return search(root->right, name);
    }
}

void inorder(TreeNode* root) {
    if (root != nullptr) {
        inorder(root->left);
        cout << "이름: " << root->student.name 
             << ", 중간 점수: " << root->student.midtermScore 
             << ", 기말 점수: " << root->student.finalScore << endl;
        inorder(root->right);
    }
}

void freeTree(TreeNode* root) {
    if (root != nullptr) {
        freeTree(root->left);
        freeTree(root->right);
        delete root;
    }
}

int main() {
    int studentCount;
    cout << "학생 수를 입력하세요: ";
    cin >> studentCount;

    Student* students = new Student[studentCount];

    for (int i = 0; i < studentCount; i++) {
        cout << "학생 " << (i + 1) << "의 이름: ";
        cin >> students[i].name;
        cout << "학생 " << (i + 1) << "의 중간 점수: ";
        cin >> students[i].midtermScore;
        cout << "학생 " << (i + 1) << "의 기말 점수: ";
        cin >> students[i].finalScore;
    }

    TreeNode* root = nullptr;
    for (int i = 0; i < studentCount; i++) {
        root = insert(root, students[i]);
    }

    cout << "\n학생 정보 (이름 기준 정렬):\n";
    inorder(root);

    string nameToSearch;
    cout << "\n탐색할 학생의 이름을 입력하세요: ";
    cin >> nameToSearch;
    TreeNode* found = search(root, nameToSearch);
    if (found) {
        cout << "학생 찾기 성공!\n";
        cout << "이름: " << found->student.name 
             << ", 중간 점수: " << found->student.midtermScore 
             << ", 기말 점수: " << found->student.finalScore << endl;
    } else {
        cout << "학생을 찾을 수 없습니다." << endl;
    }

    delete[] students;
    freeTree(root);

    return 0;
}

