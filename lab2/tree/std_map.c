#include <iostream>
#include <map>
#include <string>

using namespace std;

struct Student {
    int midtermScore;
    int finalScore;
};

int main() {
    int studentCount;
    cout << "학생 수를 입력하세요: ";
    cin >> studentCount;

    map<string, Student> studentMap;

    for (int i = 0; i < studentCount; i++) {
        string name;
        Student student;

        cout << "학생 " << (i + 1) << "의 이름: ";
        cin >> name;
        cout << "학생 " << (i + 1) << "의 중간 점수: ";
        cin >> student.midtermScore;
        cout << "학생 " << (i + 1) << "의 기말 점수: ";
        cin >> student.finalScore;

        studentMap[name] = student;
    }

    cout << "\n학생 정보 (이름 기준 정렬):\n";
    for (const auto& entry : studentMap) {
        cout << "이름: " << entry.first 
             << ", 중간 점수: " << entry.second.midtermScore 
             << ", 기말 점수: " << entry.second.finalScore << endl;
    }

    string nameToSearch;
    cout << "\n탐색할 학생의 이름을 입력하세요: ";
    cin >> nameToSearch;

    auto it = studentMap.find(nameToSearch);
    if (it != studentMap.end()) {
        cout << "학생 찾기 성공!\n";
        cout << "이름: " << it->first 
             << ", 중간 점수: " << it->second.midtermScore 
             << ", 기말 점수: " << it->second.finalScore << endl;
    } else {
        cout << "학생을 찾을 수 없습니다." << endl;
    }

    return 0;
}

