#include <fstream>
#include "RequestManager.h"

/**
 * @brief Sets the students in the RequestManager
 * This method replaces the current set of students in the RequestManager with a new set.
 * @param students The new set of students.
 */
void RequestManager::setStudentSet(StudentSet& students) {
    Students = students;
}

/**
 * @brief Gets the students in the RequestManager
 * This method retrieves the current set of students in the RequestManager.
 * @return The current set of students in the RequestManager.
 */
StudentSet& RequestManager::getStudentSet(){
    return Students;
}

/**
 * @brief Adds a request to the RequestManager
 * This method adds a new request to the RequestManager and categorizes it by its type.
 * The request is added to allRequests queue and a specific queue based on its type.
 * @param request The request to be added.
 */
void RequestManager::addRequest(const Request& request) {

    allRequests.push(request);

    if(request.getType() == "change") {
        changingRequests.push(request);
    } else if(request.getType() == "enroll") {
        enrollmentRequests.push(request);
    } else if(request.getType() == "removal") {
        removalRequests.push(request);
    }
}

/**
 * @brief Checks if a request follows the university rules
 * @param request The request to be checked.
 * @param Students The set of students.
 * @return A boolean indicating whether the request follows the UC rules or not.
 */
bool RequestManager::checkUCRules(const Request& request, StudentSet Students){
    Student estudante = Students.find_by_upcode(request.getStudent()->getCode());
    Student* student = &estudante;
    Class* newClass = request.getNewClass();

    set<string> uniqueUCCodes;
    for (const auto& lesson : student->getLessons()) {
        uniqueUCCodes.insert(lesson.getUcCode());
    }

    if(request.getType() != "change"){
        if (uniqueUCCodes.find(newClass->getUcCode()) != uniqueUCCodes.end()) {
            cout << "The student " << student->getName() << " is already enrolled in the UC " << newClass->getUcCode() << "." << endl;
            return false;
        }
    }


    if (uniqueUCCodes.size() >= 7) {
        cout << "Request denied: the student has reached the maximum limit of UC enrollments." << endl;
        return false;
    }

    if (newClass->isClassFull()) {
        cout << "Request denied: The chosen class for enrollment is already full." << endl;
        return false;
    }

    if (newClass->hasScheduleConflict(student->getLessons())) {
        cout << "Request denied: There is a schedule conflict with the student's existing classes." << endl;
        return false;
    }

    return true;
}


/**
 * @brief Checks if a request would disturb the balance of student distribution across classes in the same uc
 * The balance is considered disturbed if the class the student wants to move to would have 4 more students than the least populated class in the same UC,
 * or if the class the student wants to move from would have 4 less students than the class the student wants to move to.
 * @param request The request to be checked.
 * @return A boolean indicating whether the request would disturb the balance of student distribution or not. If the balance is disturbed, it returns true, otherwise false.
 */
bool RequestManager::balanceDisturbed(const Request& request) {
    Class* newClass = request.getNewClass();
    Class* formerClass = request.getFormerClass();

    list<Class> Turmas = Storage::readClasses();

    size_t leastStudents = UINT_MAX;
    for (const auto& Turma : Turmas) {
        if (Turma.getUcCode() == formerClass->getUcCode()) {
            leastStudents = min(leastStudents, Turma.getNumberStudents());
            if (Turma.getClassCode() == newClass->getClassCode()){
                leastStudents++;
            }
        }
    }

    size_t newClassStudents = newClass->getNumberStudents() + 1;
    size_t formerClassStudents = formerClass->getNumberStudents() - 1;

    if (leastStudents + 4 < newClassStudents) {
        cout << "Request denied: Your Class change would disturb this UC's class size balance.\n"
                " Please consider applying to a less populated class." << endl;
        return true;
    }

    if ((formerClassStudents + 4) < newClassStudents){
        cout << "Request denied: Your Class change would disturb this UC's class size balance.\n"
                " Please consider applying to a less populated class." << endl;
        return true;
    }

    return false;
}


/**
 * @brief Processes a single request
 * This method processes a single request of type "enroll", "change", or "removal".
 * The different types of requests have different rules to be followed in order to be accepted.
 * @param request The request to be processed.
 */
void RequestManager::processRequest(Request& request) {

    request.setStatus("Processing");

    if (request.getType() == "enroll") {
        Student estudante = Students.find_by_upcode(request.getStudent()->getCode());
        Student *student = &estudante;
        Class *newClass = request.getNewClass();


        if (!checkUCRules(request, Students)) {

            rejectedRequests.emplace_back(request, "UC Rules Check Failed");
            request.setStatus("Denied");
            allRequests.pop();
            enrollmentRequests.pop();
            Students.insert(*student);
            delete request.getStudent();
            delete request.getNewClass();

            return;
        }

        Students.removeStudent(*student);
        set<Lesson> newLessons = newClass->getSchedules();
        for (const auto &lesson: newLessons) {
            student->addLesson(lesson);
        }
        newClass->addStudent(*student);
        Students.insert(*student);
        request.setStatus("Processed");
        Actions.push(request);
        fileRequest(request);

        allRequests.pop();
        enrollmentRequests.pop();
    } else if (request.getType() == "change") {
        Student studentCopy = Students.find_by_upcode(request.getStudent()->getCode());
        Students.removeStudent(studentCopy);

        if (!checkUCRules(request, Students) || balanceDisturbed(request)) {
            rejectedRequests.emplace_back(request, "UC Rules Check Failed");
            request.setStatus("Denied");
            Students.insert(studentCopy);
            allRequests.pop();
            changingRequests.pop();
            return;
        }

        Class* formerClass = request.getFormerClass();
        set<Lesson> oldLessons = formerClass->getSchedules();

        for (const auto &lesson: oldLessons) {
            studentCopy.removeLesson(lesson);
        }

        Class* newClass = request.getNewClass();
        set<Lesson> newLessons = newClass->getSchedules();

        for (const auto &lesson: newLessons) {
            studentCopy.addLesson(lesson);
        }

        newClass->addStudent(studentCopy);
        Students.insert(studentCopy);

        request.setStatus("Processed");
        Actions.push(request);
        fileRequest(request);
        allRequests.pop();
        changingRequests.pop();
    }


    else if (request.getType() == "removal") {
        Student estudante = Students.find_by_upcode(request.getStudent()->getCode());
        Student* student = &estudante;

        Class* formerClass = request.getNewClass();

        Students.removeStudent(*student);
        formerClass->removeStudent(*student);

        set<Lesson> oldLessons = formerClass->getSchedules();
        for (const auto& lesson : oldLessons) {
            student->removeLesson(lesson);
        }
        Students.insert(*student);
        request.setStatus("Processed");

        Actions.push(request);

        fileRequest(request);
        allRequests.pop();
        removalRequests.pop();
    }
}

/**
 * @brief Processes all pending requests
 * This method processes all pending requests in the RequestManager.
 * It iterates over each request in the 'allRequests' queue and uses the 'processRequest' method to process each request.
 */
void RequestManager::processAllRequests() {

    while (!allRequests.empty()) {
        Request request = allRequests.front();
        newStudentsFile(Students);
        processRequest(request);
    }
}

/**
 * @brief Undoes the last accepted request
 * This method reverses the last accepted request of type "enroll", "change", or "removal".
 * It uses the 'Actions' stack which stores requests in the order they were processed.
 */
void RequestManager::undoLastAction() {
    if (Actions.empty()) {
        cout << "No actions to undo." << endl;
        return;
    }

    Request lastRequest = Actions.top();
    Actions.pop();

    if (lastRequest.getType() == "change") {
        Student estudante = Students.find_by_upcode(lastRequest.getStudent()->getCode());
        Student *student = &estudante;
        Students.removeStudent(*student);

        Class* newClass = lastRequest.getNewClass();
        Class* formerClass = lastRequest.getFormerClass();

        if (formerClass->isClassFull() || formerClass->hasScheduleConflict(student->getLessons())) {
            cout << "Cannot undo action because the old class is full or the schedule conflicts." << endl;
            return;
        }

        newClass->removeStudent(*student);
        formerClass->addStudent(*student);

        vector<Lesson> aulas = student->getLessons();
        for (const auto& lesson : aulas){
            if (lesson.getUcCode() == newClass->getUcCode()){
                student->removeLesson(lesson);
            }
        }

        set<Lesson> oldLessons = formerClass->getSchedules();
        for (const auto& lesson : oldLessons) {
            student->addLesson(lesson);
        }
        Students.insert(*student);

    } else if (lastRequest.getType() == "enroll") {
        Student estudante = Students.find_by_upcode(lastRequest.getStudent()->getCode());
        Student *student = &estudante;
        Students.removeStudent(*student);
        Class* newClass = lastRequest.getNewClass();

        vector<Lesson> aulas = student->getLessons();
        for (const auto& lesson : aulas){
            if (lesson.getUcCode() == newClass->getUcCode()){
                student->removeLesson(lesson);
            }
        }

        newClass->removeStudent(*student);
        Students.insert(*student);

    } else if (lastRequest.getType() == "removal") {
        Student estudante = Students.find_by_upcode(lastRequest.getStudent()->getCode());
        Student *student = &estudante;
        Class* formerClass = lastRequest.getFormerClass();
        Students.removeStudent(*student);

        if (formerClass->hasScheduleConflict(student->getLessons())) {
            cout << "Cannot undo action because the old class is full or the schedule conflicts." << endl;
            return;
        }

        formerClass->addStudent(*student);

        set<Lesson> oldLessons = formerClass->getSchedules();
        for (const auto& lesson : oldLessons) {
            student->addLesson(lesson);
        }
        Students.insert(*student);
    }
}

/**
 * @brief Prints all pending requests
 * This method prints the names of all students who have pending requests.
 * It differentiates between "change", "remove", and "enroll" requests.
 * If there are no pending requests of a certain type or at all, it outputs a message indicating this.
 */
void RequestManager::printPending() const {
    if (!allRequests.empty()) {
        cout << "Pending Requests:" << endl;
        queue<Request> temp = allRequests;
        while (!temp.empty()) {
            cout << "- Student Name: " << temp.front().getStudent()->getName() << endl;
            temp.pop();
        }
    } else {
        cout << "No pending requests." << endl;
    }

    if (!changingRequests.empty()) {
        cout << "Pending Change Requests:" << endl;
        queue<Request> temp = changingRequests;
        while (!temp.empty()) {
            cout << "- Student Name: " << temp.front().getStudent()->getName() << endl;
            temp.pop();
        }
    }

    if (!removalRequests.empty()) {
        cout << "Pending Removal Requests:" << endl;
        queue<Request> temp = removalRequests;
        while (!temp.empty()) {
            cout << "- Student Name: " << temp.front().getStudent()->getName() << endl;
            temp.pop();
        }
    }

    if (!enrollmentRequests.empty()) {
        cout << "Pending Enrollment Requests:" << endl;
        queue<Request> temp = enrollmentRequests;
        while (!temp.empty()) {
            cout << "- Student Name: " << temp.front().getStudent()->getName() << endl;
            temp.pop();
        }
    }
}


/**
 * @brief Writes a request to a file
 * This method writes an accepted request to a .csv file named "requestsLog.csv".
 * @param request The request to be written to the file.
 */
void RequestManager::fileRequest(const Request& request) {
    ofstream outFile("requestsLog.csv", ios::app);

    if (!outFile) {
        cerr << "O arquivo não pode ser aberto para escrita!" << endl;
        return;
    }

    if (request.getType() == "enroll") {
        outFile << request.getType() << ","
                << request.getStudent()->getCode() << ","
                << request.getNewClass()->getUcCode() << ","
                << request.getNewClass()->getClassCode();
    } else if (request.getType() == "removal") {
        outFile << request.getType() << ","
                << request.getStudent()->getCode() << ","
                << request.getNewClass()->getUcCode() << ","
                << request.getNewClass()->getClassCode();
    } else if (request.getType() == "change") {
        outFile << request.getType() << ","
                << request.getStudent()->getCode() << ","
                << request.getFormerClass()->getUcCode() << ","
                << request.getNewClass()->getClassCode() << ","
                << request.getFormerClass()->getClassCode();
    } else {
        cerr << "Tipo de solicitação inválido!" << endl;
        outFile.close();
        return;
    }

    outFile << endl;

    if (!outFile) {
        cerr << "Falha ao escrever no arquivo!" << endl;
    }

    outFile.close();
}

/**
 * @brief Returns all pending requests
 * This method returns a queue containing all pending requests.
 * @return A queue containing all pending requests.
 */
queue<Request> RequestManager::getAllRequests() const {
    return allRequests;
}
