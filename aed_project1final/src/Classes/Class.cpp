#include "Class.h"
#include "Storage.h"


/**
 * @brief Default constructor, initializes an empty class.
 */
Class::Class() : Students(set<Student>{}), UcCode(""), ClassCode(""), Lessons(set<Lesson>{}) { }


/**
 * @brief Constructor that initializes a class with a given UC code and class code.
 * @param UcCode The UC code to initialize the class.
 * @param ClassCode The class code to initialize the class.
 */
Class::Class(string UcCode, string ClassCode) : UcCode(UcCode), ClassCode(ClassCode) {

    list<Class> Turmas = Storage::readClasses();
    set<Lesson> Aulas;
    for (const auto& Turma : Turmas){
        Aulas = Turma.getSchedules();
        for (const auto& Aula : Aulas){
            if (Aula.getClassCode() == getClassCode() && (Aula.getUcCode() == getUcCode())){
                Lessons.insert(Aula);
            }
        }
    }

    for (const auto& Student : Storage::readStudents()){
        for(const auto& Lesson : Student.getLessons()){
            if ((Lesson.getClassCode() == getClassCode()) && (Lesson.getUcCode() == getUcCode())){
                Students.insert(Student);
            }
        }
    }
}


/**
 * @brief Constructor that initializes a class with a given UC code, class code, and set of students enrolled in it.
 * @param UcCode The UC code to initialize the class.
 * @param ClassCode The class code to initialize the class.
 * @param Students The set of students to initialize the class.
 */
Class::Class(string UcCode, string ClassCode, set<Student> Students) : UcCode(UcCode), ClassCode(ClassCode), Students(Students) { }


/**
 * @return The number of students in the class.
 */
size_t Class::getNumberStudents() const { return Students.size(); }


/** The maximum number of students a class can have. */
int Class::maxStudents = 30;


/**
 * @return The class code.
 */
string Class::getClassCode() const { return ClassCode; }


/**
 * @return The UC code.
 */
string Class::getUcCode() const { return UcCode; }


/**
 * @brief Adds a lesson to the class schedule.
 * @param schedule The lesson to add to the schedule.
 */
void Class::addSchedule(const Lesson& schedule) { Lessons.insert(schedule); }


/**
 * @return The Set of lessons for the class.
 */
set<Lesson> Class::getSchedules() const { return Lessons; }


/**
 * @brief Prints the schedule for the class.
 * @details This method reads all the classes from storage, finds the current class, and prints all its lessons.
 */
void Class::printClassSchedule() const {

    list<Class> Turmas = Storage::readClasses();

    for (const auto &Turma: Turmas) {
        if (Turma.getClassCode() == getClassCode()) {
            set<Lesson> Aulas = Turma.getSchedules();
            for (const auto &lesson: Aulas) {
                lesson.printSchedule();
            }
        }
    }
}


/**
 * @brief Checks if the class has a schedule conflict with another set of lessons.
 * @details This method checks every lesson in the class and compares it to every lesson in the other set of lessons looking for a conflict between the schedules.
 * @param otherLessons The other set of lessons to check for conflicts.
 * @return True if there is a conflict, false otherwise.
 */
bool Class::hasScheduleConflict(vector<Lesson> otherLessons) const {
    for (const auto& classLesson : Lessons) {
        for (const auto& otherLesson : otherLessons) {
            if (classLesson.conflicts(otherLesson)) {
                return true;
            }
        }
    }
    return false;
}


/**
 * @brief Checks if the class is full.
 * @details This method compares the number of students in the class to the maximum number of students.
 * @return True if the class is full, false otherwise.
 */
bool Class::isClassFull() const  {
    return getNumberStudents() >= Class::maxStudents;
}


/**
 * @brief Removes a student from the class.
 * @param s The student to remove from the class.
 * @details Time complexity is O(log n), where n is the number of students in the class.
 */
void Class::removeStudent(const Student &s) { Students.erase(s);}



/**
 * @brief Adds a student to the class.
 * @param s The student to add to the class.
 * @details Time complexity is O(log n), where n is the number of students in the class.
 */
void Class::addStudent(const Student& s) { Students.insert(s); }



