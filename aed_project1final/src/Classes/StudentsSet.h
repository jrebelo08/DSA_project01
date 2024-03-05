#ifndef STUDENTSSET_H
#define STUDENTSSET_H

#include <set>
#include <vector>
#include <iostream>
#include <algorithm>
#include "Student.h"

using namespace std;

/**
 * @brief Represents a set of students.
 * Provides methods for manipulating searching and printing information about students.
 * Capabilities include insertion, removal, and various search operations using the c++ Standard Template Library (STL).
 */

class StudentSet {
private:

    /**
 * @brief A set holding the Student objects.
 * The Student objects are stored in a set to prevent duplicates and allow efficient operations.
 */
    set<Student> studentsSet;

public:

    StudentSet()= default;

    explicit StudentSet(const std::set<Student>& Students);

    void insert(const Student& student);

    Student find_by_upcode(int upcode);
    vector<Student> findStudentsByYear(int year);
    void printStudents(const string &sortType);
    void print_by_uc(const string& uccode);
    void printStudentsByYear(int year);

    map<string, int> UCoccupation();
    void printUCoccupation(bool ascending);


    Student find_by_name(const string &name);

    vector<Student> findStudentsByClass(const string &classCode);

    void print_by_class(string ClassCode);

    void moreUcsThan(int n);

    map<pair<string, string>, int> classOccupation();

    void printClassOccupation(bool ascending);

    int countStudentsByYear(int year);

    void printTopUcs(int maxUcs);

    void removeStudent(const Student& student);

    set<Student> getSet();

};

#endif // STUDENTSSET_H
