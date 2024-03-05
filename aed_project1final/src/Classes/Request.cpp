#include "Request.h"



/**
 * @brief Constructor for the Request class.
 * @param student The student making the request.
 * @param newClass The class desired by the student.
 * @param type The type of request.
 */
Request::Request(Student* student, Class* newClass, string type) : student(student), newClass(newClass), type(type) {}


/**
 * @brief Getter for the student.
 * @return A pointer to the student making the request.
 */
Student* Request::getStudent() const { return student; }


/**
 * @brief Getter for the new class.
 * @return A pointer to the new class the student desires to join.
 */
Class* Request::getNewClass() const { return newClass; }


/**
 * @brief Getter for the former class.
 * @return A pointer to the former class of the student.
 */
Class* Request::getFormerClass() const {
    string ucCode = newClass->getUcCode();

    vector<Lesson> studentLessons = student->getLessons();

    for (const Lesson& lesson : studentLessons) {
        if (lesson.getUcCode() == ucCode) {
            return new Class(ucCode,lesson.getClassCode());
        }
    }
}


/**
 * @return The type of the request.
 */
string Request::getType() const { return type; }


/**
 * @brief Setter for the status of the request.
 * @param requestStatus The new status of the request.
 */
void Request::setStatus(string requestStatus) { status = requestStatus; }
