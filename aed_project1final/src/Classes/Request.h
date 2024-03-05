#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include "Student.h"
#include "Class.h"
#include "StudentsSet.h"

using namespace std;

/**
 * @brief A class representing a student's request that stores its type, its status, the student information and the class he desires to join.
 */


class Request {
private:
    /** @brief The type of the request (example -> change;enroll;remove). */
    string type;
    /** @brief The current status of the request. */
    string status;
    /** @brief A pointer to the Student who made the request. */
    Student* student;
    /** @brief A pointer to the Class that the student wants to join or leave. */
    Class* newClass;
public:
    Request(Student *student, Class *newClass, string type);

    string getType() const;
    string getStatus() const;
    Student* getStudent() const;
    Class* getFormerClass() const; // tira a classe do estudante para a uc que ele quer mudar
    Class* getNewClass() const;

    void setStatus(string requestStatus);



};

#endif //REQUEST_H