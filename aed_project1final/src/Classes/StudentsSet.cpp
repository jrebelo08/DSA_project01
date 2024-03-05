#include <functional>
#include <utility>
#include "StudentsSet.h"



/**
 * @brief Constructor for a new StudentSet object
 * @param Students set of students
 */
StudentSet::StudentSet(const set<Student>& Students) : studentsSet(Students) {}


/**
 * @brief Insert a student into the set
 * @param data student to be inserted
 * @details This operation has a time complexity of O(log n).
 */
void StudentSet::insert(const Student& data) {
    studentsSet.insert(data);
}


/**
 * @brief Find a student by their code
 * @param upcode unique code of the student
 * @return The student with the specified unique code
 * @details This operation has a time complexity of O(n).
 */
Student StudentSet::find_by_upcode(int upcode) {
    auto it = find_if(studentsSet.begin(), studentsSet.end(),
                      [upcode](const Student& student) {
                          return student.getCode() == upcode;
                      });

    if (it != studentsSet.end()) {
        return *it;
    } else {
        return Student();
    }
}


/**
 * @brief Find a student by their name
 * @param name name of the student
 * @return The student with the specified name
 * @details This operation has a time complexity of O(n).
 */
Student StudentSet::find_by_name(const string& name) {
    auto it = find_if(studentsSet.begin(), studentsSet.end(),
                      [name](const Student& student) {
                          return student.getName() == name;
                      });

    if (it != studentsSet.end()) {
        return *it;
    } else {
        return Student();
    }
}


/**
 * @brief Finds students enrolled in a specific class.
 * @param classCode The class code of the class.
 * @return A vector of students enrolled in the specified class.
 * @details This operation has a time complexity of O(n * m).
 */
vector<Student> StudentSet::findStudentsByClass(const string& classCode) {
    vector<Student> students;
    for (const auto& student : studentsSet) {
        vector<Lesson> lessons = student.getLessons();
        for (const auto& lesson : lessons) {
            if (lesson.getClassCode() == classCode) {
                students.push_back(student);
                break;
            }
        }
    }
    return students;
}



/**
 * @brief Finds students who have the maximum number of lessons in a specific academic year.
 * @param year The academic year.
 * @return A vector of students who have the maximum number of lessons in the specified year.
 * @details This operation has a time complexity of O(n * m).
 */
vector<Student> StudentSet::findStudentsByYear(int year) {
    vector<Student> students;

    unordered_map<string, unordered_map<int, int>> studentYearLessons;

    for (const auto& student : studentsSet) {
        vector<Lesson> lessons = student.getLessons();
        unordered_map<int, int> yearLessons;

        for (const auto& lesson : lessons) {
            string classCode = lesson.getClassCode();
            if (!classCode.empty()) {
                int lessonYear = classCode[0] - '0';
                yearLessons[lessonYear]++;
            }
        }

        int maxYear = year;
        int maxLessons = 0;

        for (const auto& [lessonYear, lessonCount] : yearLessons) {
            if (lessonCount > maxLessons) {
                maxYear = lessonYear;
                maxLessons = lessonCount;
            }
        }

        if (maxYear == year) {
            students.push_back(student);
        }
    }

    return students;
}


/**
 * @brief Finds the occupation of each UC.
 * @return A map where the key is the UC code and the value is the number of students enrolled in the UC.
 */
map<string, int> StudentSet::UCoccupation() {
    map<string, int> ucOccupation;


    set<string> UniqueUcCodes;
    for (const auto& student : studentsSet) {
        vector<Lesson> lessons = student.getLessons();
        UniqueUcCodes = {};
        for (const auto& lesson : lessons) {
            UniqueUcCodes.insert(lesson.getUcCode());
        }
        for (const auto& UcCode : UniqueUcCodes){
            ucOccupation[UcCode]++;
        }
    }

    return ucOccupation;
}


/**
 * @brief Finds the occupation of each class.
 * @return A map where the key is a pair of class code and UC code and the value is the number of students enrolled in the class.
 * @details This operation has a time complexity of O(n * m).
 */
map<pair<string, string>, int> StudentSet::classOccupation() {
    map<pair<string, string>, int> classOccupationMap;

    for (const auto& student : studentsSet) {
        set<pair<string,string>> UniqueUcClass;
        vector<Lesson> lessons = student.getLessons();
        for (const auto &lesson: lessons) {
            pair<string, string> classKey = make_pair(lesson.getClassCode(),lesson.getUcCode());
            UniqueUcClass.insert(classKey);
        }
        for(const auto& UcClass : UniqueUcClass)
        classOccupationMap[UcClass]++;
    }

    return classOccupationMap;
}



/**
 * @brief Prints the students sorted in a specific order.
 * @param sortType The type of order to sort the students in.
 * @details The time complexity of this operation is O(n log n).
 */
void StudentSet::printStudents(const string &sortType) {
    map<string, function<bool(const Student&, const Student&)>> comparators = {
            {"alphabetical", [](const Student &first, const Student &second) { return first.getName() < second.getName(); }},
            {"reverse alphabetical", [](const Student &first, const Student &second) { return first.getName() > second.getName(); }},
            {"numerical", [](const Student &first, const Student &second) { return first.getCode() < second.getCode(); }},
            {"reverse numerical", [](const Student &first, const Student &second) { return first.getCode() > second.getCode(); }}
    };

    auto comparator = comparators.find(sortType);
    if (comparator == comparators.end()) {
        cout << "Invalid sort Type" << endl;
        return;
    }

    vector<Student> studentsVector(studentsSet.begin(), studentsSet.end());

    sort(studentsVector.begin(), studentsVector.end(), comparator->second);

    for (const auto& student : studentsVector) {
        cout << "Student Code: " << student.getCode() << " Name: " << student.getName() << endl;
    }
}


/**
 * @brief Prints the students enrolled in a specific UC.
 * @param uccode The UC code.
 * @details The time complexity of this operation is O(n * m).
 */
void StudentSet::print_by_uc(const string& uccode) {
    for (const auto& student : studentsSet) {
        for (const auto& lesson : student.getLessons()) {
            if (lesson.getUcCode() == uccode) {
                cout << "Name: " << student.getName() << endl;
                cout << "Student Code: " << student.getCode() << endl;
                cout << endl;
                break;
            }
        }
    }
}



/**
 * @brief Prints the number of students who are enrolled in more than a certain number of UCs.
 * @param n The number of UCs.
 * @details The time complexity of this operation is O(n * m).
 */
void StudentSet::moreUcsThan(int n) {
    int count = 0;
    for (const auto& student : studentsSet) {
        set<string> uniqueUcCodes;
        for (const auto& lesson : student.getLessons()) {
            uniqueUcCodes.insert(lesson.getUcCode());
        }
        if (uniqueUcCodes.size() > n) {
            count++;
        }
    }
    cout << count;
}


/**
 * @brief Prints the students who have the maximum number of lessons in a specific academic year.
 * @param year The academic year.
 * @details The time complexity of this operation is O(n * m).
 */
void StudentSet::printStudentsByYear(int year) {
    vector<Student> studentsInYear = findStudentsByYear(year);

    for (const auto& student : studentsInYear) {
        cout << "Name: " << student.getName() << endl;
        cout << "Student Code: " << student.getCode() << endl;
        cout << endl;
    }
}


/**
 * @brief Counts the number of students who have the maximum number of lessons in a specific academic year.
 * @param year The academic year.
 * @return The number of students who have the maximum number of lessons in the specified year.
 * @details The time complexity of this operation is O(n * m).
 */
int StudentSet::countStudentsByYear(int year) {
    return findStudentsByYear(year).size();
}


/**
 * @brief Prints the occupation of each UC in either ascending or descending order.
 * @param ascending A boolean that determines whether to print in ascending or descending order.
 * @details The time complexity of this operation is O(n * m).
 */
void StudentSet::printUCoccupation(bool ascending) {
    map<string, int> ucOccupation = UCoccupation();

    vector<pair<string, int>> ucOccupationVec(ucOccupation.begin(), ucOccupation.end());

    sort(ucOccupationVec.begin(), ucOccupationVec.end(),
         [ascending](const pair<string, int>& uc1, const pair<string, int>& uc2) {
             return ascending ? uc1.second < uc2.second : uc1.second > uc2.second;
         });

    for (const auto& uc : ucOccupationVec) {
        cout << "UC Code: " << uc.first << ", Occupation: " << uc.second << endl;
    }
}


/**
 * @brief Prints the occupation of each class in either ascending or descending order.
 * @param ascending A boolean that determines whether to print in ascending or descending order.
 * @details The time complexity of this operation is O(n * m).
 */
void StudentSet::printClassOccupation(bool ascending) {
    map<pair<string, string>, int> classOccupationData = classOccupation();

    vector<pair<pair<string, string>, int>> classOccupationVec(classOccupationData.begin(), classOccupationData.end());

    sort(classOccupationVec.begin(), classOccupationVec.end(),
         [ascending](const pair<pair<string, string>, int>& class1, const pair<pair<string, string>, int>& class2) {
             return ascending ? class1.second < class2.second : class1.second > class2.second;
         });

    for (const auto& classOccupation : classOccupationVec) {
        cout << "Class Code: " << classOccupation.first.first << ", UC Code: " << classOccupation.first.second << ", Occupation: " << classOccupation.second << endl;
    }
}


/**
 * @brief Prints the students enrolled in a specific class.
 * @param ClassCode The class code.
 * @details This operation has a time complexity of O(n * m).
 */
void StudentSet::print_by_class(string ClassCode) {
    vector<Student> students = findStudentsByClass(ClassCode);
    for (const auto& student : students) {
        cout << "Name: " << student.getName() << endl;
        cout << "Student Code: " << student.getCode() << endl;
        cout << endl;
    }
}


/**
 * @brief Prints the top UCs with the most number of students.
 * @param maxUcs The maximum number of UCs to print.
 * @details This operation has a time complexity of O(n * m).
 */
void StudentSet::printTopUcs(int maxUcs) {
    map<string, int> ucOccupation = UCoccupation();

    vector<pair<string, int>> ucOccupationVec(ucOccupation.begin(), ucOccupation.end());

    sort(ucOccupationVec.begin(), ucOccupationVec.end(),
         [](const pair<string, int>& uc1, const pair<string, int>& uc2) {
             return uc1.second > uc2.second;
         });

    int count = 0;
    for (const auto& uc : ucOccupationVec) {
        if (uc.second > 0 && count < maxUcs) {
            cout << "Position " << count + 1 << " - UC Code: " << uc.first << ", Occupation: " << uc.second << endl;
            count++;
        }
    }
}

/**
 * @brief Removes a lesson from a student's schedule.
 * @param upcode The UP code of the student.
 * @param lesson The lesson to remove.
 * @details This operation has a time complexity of O(n).
 */
void StudentSet::removeStudent(const Student& student){
    studentsSet.erase(find_by_upcode(student.getCode()));
}

/**
 * @brief Returns the updated studentsSet
 */
set<Student> StudentSet::getSet() {
    return studentsSet;
}