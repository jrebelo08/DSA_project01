#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include "Lesson.h"

using namespace std;


/**
 * @brief Class to store the information about a student.
 */


class Student {
private:
    /** @brief identifier for the student. */
    int Code;
    /** @brief name of the student. */
    string Name;
    /** @brief vector of lessons that the student is enrolled in. */
    vector<Lesson> Lessons;

public:
    Student();

    Student(int Code, string Name, vector<Lesson> Lessons);

    int getCode() const;
    string getName() const;
    vector<Lesson> getLessons() const;

    bool operator<(const Student&) const;

    bool operator==(const Student& other) const;

    void addLesson(const Lesson &lesson);

    void removeLesson(const Lesson &lesson);
};

#endif // STUDENT_H