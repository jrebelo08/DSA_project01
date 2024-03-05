#include <algorithm>
#include "Storage.h"


/**
 * @brief Reads lessons from "classes.csv".
 * @return A vector of Lesson objects parsed from the file.
 */
vector<Lesson> Storage::readLessons() {
    vector<Lesson> Aulas;
    ifstream docAulas("../cmake-build-debug/classes.csv");
    string linhaAula;



    getline(docAulas, linhaAula); // Dá skip ao Header com os nomes

    while (getline(docAulas, linhaAula)) {
        istringstream iss(linhaAula);
        string classCode, ucCode, weekday, type;
        float startHour, duration;

        getline(iss, classCode, ',');
        getline(iss, ucCode, ',');
        getline(iss, weekday, ',');
        string startHourStr, durationStr;
        getline(iss, startHourStr, ',');
        getline(iss, durationStr, ',');
        getline(iss, type);

        // Converte strings para floats
        startHour = stof(startHourStr);
        duration = stof(durationStr);

        // Calcula o horário de término
        float endHour = startHour + duration;

        // Cria e adiciona aula diretamente ao vetor
        Aulas.emplace_back(classCode, ucCode, weekday, startHour, endHour, type);
    }

    return Aulas;
}


/**
 * @brief Reads students from "students_classes.csv" file and associates them with lessons.
 * @return A set of Student objects parsed from the file.
 */
set<Student> Storage::readStudents() {
    set<Student> studentsSet;

    ifstream docStudents("../cmake-build-debug/students_classes.csv");
    string line;

    // Skip Header
    getline(docStudents, line);

    int currentCode = 0;
    string currentName;
    vector<Lesson> aulasAluno;

    while (getline(docStudents, line)) {
        istringstream iss(line);
        string studentCodeStr, studentName, UcCode, ClassCode;

        vector<Lesson> Aulas = Storage::readLessons();

        getline(iss, studentCodeStr, ',');
        getline(iss, studentName, ',');
        getline(iss, UcCode, ',');
        getline(iss, ClassCode, ',');

        int studentCode = stoi(studentCodeStr);

        if (currentCode != studentCode) {
            if (currentCode != 0) {
                studentsSet.insert(Student(currentCode, currentName, aulasAluno));
            }

            currentCode = studentCode;
            currentName = studentName;
            aulasAluno.clear();
        }

        // Obter todas as aulas correspondentes ao par UC - Class
        vector<Lesson> matchingLessons = Lesson::findByUcClass(UcCode, ClassCode, Aulas);

        // Adicionar todas as aulas correspondentes ao vetor aulasAluno
        aulasAluno.insert(aulasAluno.end(), matchingLessons.begin(), matchingLessons.end());
    }

    if (currentCode != 0) {
        studentsSet.insert(Student(currentCode, currentName, aulasAluno));
    }

    return studentsSet;
}




/**
 * @brief Reads classes from "classes_per_uc.csv" file and associates them with students.
 * @return A list of Class objects parsed from the file.
 */

list<Class> Storage::readClasses() {
    list<Class> Classes;

    ifstream docTurmas("../cmake-build-debug/classes_per_uc.csv");
    string line;

    getline(docTurmas, line);

    set<pair<string, string>> UcClassCodes;

    while (getline(docTurmas, line)) {
        istringstream iss(line);
        string UcCode, ClassCode;
        getline(iss, UcCode, ',');
        getline(iss, ClassCode, ',');
        UcClassCodes.insert({ClassCode, UcCode});
    }

    vector<Lesson> allLessons = Storage::readLessons();
    set<Student> allStudents = Storage::readStudents();
    vector<Lesson> AulasEstudante;

    for (const auto& codePair : UcClassCodes) {
        Class currentClass(codePair.second, codePair.first, set<Student>());

        for (const auto& lesson : allLessons) {
            if (lesson.getClassCode() == codePair.first && lesson.getUcCode() == codePair.second) {
                currentClass.addSchedule(lesson);
            }
        }
        for (const auto& Student : allStudents){
            AulasEstudante = Student.getLessons();
            for (const auto& Aula : AulasEstudante){
                if ((Aula.getClassCode() == currentClass.getClassCode()) && (Aula.getUcCode() == currentClass.getUcCode())){
                    currentClass.addStudent(Student);
                }
            }
        }

        Classes.push_back(currentClass);
    }

    return Classes;
}