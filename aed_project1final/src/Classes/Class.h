#ifndef CLASS_H
#define CLASS_H

#include <string>
#include <set>
#include <list>
#include <algorithm>
#include "Student.h"

using namespace std;

/**
 * @brief Encapsulates the data associated with a class, such as its code, the students enrolled in it and the lessons.
 */
class Class {
private :
    /** @brief Code of the Class and of the UC, together they define a unique class. */
    string ClassCode,UcCode;
    /** @brief Set of Students of a class. */
    set<Student> Students;
    /** @brief Set of Lessons of a Class that define the schedule. */
    set<Lesson> Lessons;
public:
    Class();
    Class(string UcCode, string ClassCode);
    Class(string UcCode, string ClassCode, set<Student> Students);

    static int maxStudents;
    size_t getNumberStudents() const;
    string getClassCode() const;
    string getUcCode() const;


    set<Lesson> getSchedules() const;

    void printClassSchedule() const;

    void addSchedule(const Lesson& schedule);

    void addStudent(const Student& s);

    static bool compareByUcCode(const Class& lhs, const Class& rhs) {
        return lhs.getUcCode() < rhs.getUcCode();
    }


    bool hasScheduleConflict(vector<Lesson> otherLessons) const;

    bool isClassFull() const;

    void removeStudent(const Student &s);
};

#endif // CLASS_H


