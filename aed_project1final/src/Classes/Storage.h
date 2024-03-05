#ifndef Storage_H
#define Storage_H


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <list>
#include "Lesson.h"
#include "Student.h"
#include "Class.h"
#include "StudentsSet.h"

/**
 * @brief The Storage class provides static methods for reading lessons, students, and classes data from the provided .csv files.
 */
class Storage {
public:
    static vector<Lesson> readLessons(); /*Lê toda a informação presente no ficheiro "classes.csv"*/
    static set<Student> readStudents(); /*Lê a informação sobre Estudantes (Código e Nome) e associa-os a aulas*/
    static list<Class> readClasses(); /*Lê a informação sobre turmas e associa-lhes estudantes*/



};

static void newStudentsFile(StudentSet Students) {
    ofstream outfile("../cmake-build-debug/students_classes.csv");

    if (!outfile) {
        cerr << "Failed to open students_classes.csv for writing." << endl;
        return;
    }

    outfile << "StudentCode,StudentName,UcCode,ClassCode" << endl;

    for (const Student& student : Students.getSet()) {
        set<pair<string, string>> UcClassSet;
        for (const Lesson& lesson : student.getLessons()){
            UcClassSet.insert({lesson.getUcCode(), lesson.getClassCode()});
        }
        for (const auto& par : UcClassSet){
            outfile << student.getCode() << ","
                    << student.getName() << ","
                    << par.first << ","
                    << par.second << endl;
        }
    }


    outfile.close();
}

#endif // Storage_H