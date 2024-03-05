#include "Student.h"
#include "Storage.h"
#include "Lesson.h"


/**
 * @brief Default constructor for the Student class.
 */
Student::Student() : Code(0), Name(""), Lessons({})  { }


/**
 * @brief Constructor for the Student class that takes Code, Name, and Lessons.
 * @param Code Unique identifier for the student.
 * @param Name The name of the student.
 * @param Lessons The lessons that the student is enrolled in.
 */
Student::Student(int Code, string Name, vector<Lesson> Lessons): Code(Code), Name(Name), Lessons(Lessons) {}


/**
 * @return The code that identifies the student.
 */
int Student::getCode() const { return Code; }


/**
 * @return The name of the student.
 */
string Student::getName() const { return Name; }

/**
 * @return The lessons that the student is enrolled in.
 */
vector<Lesson> Student::getLessons() const { return Lessons; }


/**
 * @brief Less than operator used for ordering students. Students are ordered by their code.
 * @param other The other student to compare with.
 * @return True if this student's code is less than the other's, false otherwise.
 */
bool Student::operator<(const Student &other) const {return (this->getCode()<other.getCode());}

/**
 * @brief Adds a lesson to the student's schedule.
 * @param lesson The lesson to add.
 */
void Student::addLesson(const Lesson& lesson) {
    Lessons.push_back(lesson);
}

/**
 * @brief Removes a lesson from the student's schedule.
 * @param lesson The lesson to remove.
 * @return True if the lesson was found and removed, false otherwise.
 * @details Time complexity: O(n), where n is the number of lessons the student has.
 */
void Student::removeLesson(const Lesson& lesson) {
    auto it = Lessons.begin();
    while (it != Lessons.end()) {
        if (*it == lesson) {
            it = Lessons.erase(it);
        } else {
            ++it;
        }
    }
}


/**
 * @brief Equality operator for the Student class. Two student objects are considered equal if they have the same code and name.
 * @param other The other student to compare with.
 * @return True if the students are equal, false otherwise.
 */
bool Student::operator==(const Student& other) const {
    return (this->Code == other.getCode()) && (this->Name == other.getName());
}