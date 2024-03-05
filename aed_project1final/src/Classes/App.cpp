#include <limits>
#include "App.h"


using namespace std;

void displaySearchMenu(StudentSet& Students, list<Class> Turmas);
void displayRequestsMenu(StudentSet& Students, list<Class>& Turmas, RequestManager& RequestManager);

int Menu() {
    string choice;
    bool exitMenu = false;
    cout << "Loading...";
    StudentSet Stored = StudentSet(Storage::readStudents());
    int count = 0;
    list<Class> Turmas = Storage::readClasses();

    RequestManager RequestManager;
    RequestManager.setStudentSet(Stored);



    while (!exitMenu) {
        cout << "\n-----------------------------\n";
        cout << "     Welcome to Main Menu       \n";
        cout << "-----------------------------\n";
        cout << "Enter the number of the option that suits your needs:\n";
        cout << "1. Search Data\n";
        cout << "2. Make or Manage Requests\n";
        cout << "3. Exit\n";
        cout << "-----------------------------\n";
        cout << "Your choice: ";
        cin >> choice;

        if (choice.length() != 1) {
            choice = "0";
        }

        switch (choice[0]) {
            case '1':
                displaySearchMenu(RequestManager.getStudentSet(), Turmas);
                break;
            case '2':
                displayRequestsMenu(RequestManager.getStudentSet(), Turmas, RequestManager);
                break;
            case '3':
                cout << "Exiting menu system...\n";
                exitMenu = true;
                break;
            default:
                cout << "Invalid input. Please choose a valid option.\n";
        }
    }

    return 0;
}


void printStudentScheduleByName(StudentSet students){
    string name;

    while (true) {
        cout << "Enter student's name: (or insert 'b' to go back) ";
        getline(cin,name);


        if (name == "b") {
            return;
        }

        Student student = students.find_by_name(name);

        if (student.getCode() != 0) {
            cout << "Searching by name: " << name << endl;
            vector<Lesson> Aulas = student.getLessons();

            sort(Aulas.begin(),Aulas.end());
            for (const auto& Aula : Aulas) {
                Aula.printSchedule();
            }
        } else {
            cout << "Student with name '" << name << "' not found. Please try again.\n";
        }
    }
}

void printStudentScheduleByUpcode(StudentSet students) {
    string input;
    bool studentFound = false;

    while (!studentFound) {
        cout << "Enter student's upcode: (or insert b to go back) ";
        cin >> input;

        if (input == "b") {
            return;
        }

        bool isNumeric = true;
        for (char c : input) {
            if (!isdigit(c)) {
                isNumeric = false;
                break;
            }
        }

        if (isNumeric) {
            stringstream ss(input);
            int upcode;
            ss >> upcode;

            Student student = students.find_by_upcode(upcode);

            if (student.getCode() != 0) {
                cout << "Searching by upcode: " << upcode << endl;
                vector<Lesson> lessons = student.getLessons();

                for (const auto& lesson : lessons) {
                    lesson.printSchedule();
                }

                studentFound = true;
            } else {
                cout << "Student with upcode " << upcode << " not found. Please try again.\n";
            }
        } else {
            cout << "Invalid input. Please enter a numeric upcode.\n";
        }
    }
}



void displayStudentSchedule(StudentSet students){
    string choice;
    bool back = false;

    while (!back) {
        cout << "\n---------------------------------------------------------\n";
        cout << "                   Student Schedule                       \n";
        cout << "---------------------------------------------------------\n";
        cout << "Choose an option:\n";
        cout << "1: Search by name\n";
        cout << "2: Search by UP code\n";
        cout << "b: Go back\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice.length() != 1) {
            choice = "3";
        }

        switch (choice[0]) {
            case '1':
                printStudentScheduleByName(students);
                break;
            case '2':
                printStudentScheduleByUpcode(students);
                break;
            case 'b':
                back = true;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
}



bool classExists(const string& classCode, const list<Class>& classes) {
    return any_of(classes.begin(), classes.end(), [&classCode](const Class& currentClass) {
        return currentClass.getClassCode() == classCode;
    });
}

void displayClassSchedule(){
    string classCode;

    while (true) {
        cout << "\n---------------------------------------------------------\n";
        cout << "                  Class Schedule                           \n";
        cout << "---------------------------------------------------------\n";
        cout << "Enter Class Code (or enter 'b' to go back): ";
        cin >> classCode;

        if (classCode == "b") {
            return;
        }

        auto classes = Storage::readClasses();
        auto foundClass = find_if(classes.begin(), classes.end(), [&classCode](const Class& currentClass) {
            return currentClass.getClassCode() == classCode;
        });

        if (foundClass != classes.end()) {
            cout << "\nClass Schedule for Class Code: " << classCode << endl;
            cout << "----------------------------------\n";
            foundClass->printClassSchedule();
        } else {
            cout << "Class with code '" << classCode << "' not found. Please try again.\n";
        }
    }
}

bool ucExists(const string& ucCode) {
    const auto& lessons = Storage::readLessons();
    return any_of(lessons.begin(), lessons.end(), [&ucCode](const Lesson& lesson) {
        return lesson.getUcCode() == ucCode;
    });
}


void displayUcSchedule() {
    string ucCode;

    while (true) {
        cout << "\n---------------------------------------------------------\n";
        cout << "                    UC Schedule                           \n";
        cout << "---------------------------------------------------------\n";
        cout << "Enter UC Code (or enter 'b' to go back): ";
        cin >> ucCode;

        if (ucCode == "b") {
            return;
        }

        if (ucExists(ucCode)) {
            vector<Lesson> matchingLessons;
            for (const auto& lesson : Storage::readLessons()) {
                if (lesson.getUcCode() == ucCode) {
                    matchingLessons.push_back(lesson);
                }
            }

            sort(matchingLessons.begin(), matchingLessons.end());

            cout << "\nSchedule for UC with code '" << ucCode << "':\n";
            for (const auto& lesson : matchingLessons) {
                lesson.printScheduleUC();
            }
        } else {
            cout << "UC with code '" << ucCode << "' not found. Please try again.\n";
        }
    }
}

void displayStudentsWithinClass(StudentSet& students, const list<Class>& Turmas) {
    string classCode;

    while (true) {
        cout << "\n---------------------------------------------------------\n";
        cout << "               Students Within a Class                    \n";
        cout << "---------------------------------------------------------\n";
        cout << "Enter the Class Code (or enter 'b' to go back): ";
        cin >> classCode;

        if (classCode == "b") {
            return;
        }

        if (classExists(classCode, Turmas)) {
            cout << "\nStudents enrolled in Class Code '" << classCode << "':" << endl;
            students.print_by_class(classCode);
        } else {
            cout << "Class with code '" << classCode << "' not found. Please try again.\n";
        }
    }
}


void displayStudentsWithinYear(StudentSet& students) {
    int year;
    bool validInput = false;

    while (!validInput) {
        string input;
        cout << "\n---------------------------------------------------------\n";
        cout << "               Students Within a Year                     \n";
        cout << "---------------------------------------------------------\n";
        cout << "Enter the Year (or enter 'b' to go back): ";
        cin >> input;

        if (input == "b") {
            return;
        }

        try {
            year = stoi(input);
            validInput = true;
        } catch (invalid_argument&) {
            cout << "Invalid input. Please enter a valid year or 'b' to go back.\n";
        }
    }

    students.printStudentsByYear(year);
}

void displayStudentsWithinUc(StudentSet& students) {
    string ucCode;

    cout << "\n---------------------------------------------------------\n";
    cout << "               Students Within a UC                        \n";
    cout << "---------------------------------------------------------\n";

    while (true) {
        cout << "Enter the UC Code (or enter 'b' to go back): ";
        cin >> ucCode;

        if (ucCode == "b") {
            return;
        }

        if (ucExists(ucCode)) {
            cout << "\nStudents enrolled in UC with code '" << ucCode << "':" << endl;
            students.print_by_uc(ucCode);
            return;
        } else {
            cout << "UC with code '" << ucCode << "' not found. Please try again.\n";
        }
    }
}


void displayStudentsAtLeastNUcs(StudentSet& students) {
    int n;
    bool validInput = false;

    while (!validInput) {
        cout << "\n---------------------------------------------------------\n";
        cout << "          Students Enrolled in N or More UCs             \n";
        cout << "---------------------------------------------------------\n";
        cout << "Enter the minimum number of UCs for enrollment (or enter 'b' to go back): ";
        string input;
        cin >> input;

        if (input == "b") {
            return;
        }

        try {
            n = stoi(input);
            if (n >= 0 && n <= 8) {
                validInput = true;
            } else {
                cout << "Invalid input. Please enter a single number from 0 to 8.\n";
            }
        } catch (invalid_argument&) {
            cout << "Invalid input. Please enter a valid number.\n";
        }
    }

    cout << "There are currently "; students.moreUcsThan(n-1);
    cout << " students enrolled in " << n << " or more UCs.\n";
}


void displayUcOccupation(StudentSet& students) {
    while (true) {
        string sortOrder;
        cout << "\n---------------------------------------------------------\n";
        cout << "                     UC Occupation                        \n";
        cout << "---------------------------------------------------------\n";
        cout << "Sort UC occupation in ascending (enter 'a') or descending (enter 'd') order, or 'b' to go back: ";
        cin >> sortOrder;

        bool ascending = (sortOrder == "a");

        if (sortOrder == "a" || sortOrder == "d") {
            cout << "\n---------------------------------------------------------\n";
            cout << "                  UC Occupation                           \n";
            cout << "---------------------------------------------------------\n";
            students.printUCoccupation(ascending);
        } else if (sortOrder == "b") {
            cout << "\nReturning to the previous menu...\n";
            break;
        } else {
            cout << "\nInvalid input. Please enter 'a' for ascending, 'd' for descending, or 'b' to go back.\n";
        }
    }
}


void displayClassOccupation(StudentSet& studentSet) {
    while (true) {
        string sortOrder;
        cout << "\n---------------------------------------------------------\n";
        cout << "                    Class Occupation                      \n";
        cout << "---------------------------------------------------------\n";
        cout << "Sort class occupation in ascending (enter 'a') or descending (enter 'd') order, or 'b' to go back: ";
        cin >> sortOrder;

        bool ascending = (sortOrder == "a");

        if (sortOrder == "a" || sortOrder == "d") {
            cout << "\n----------------------------------------------\n";
            cout << "| Class Code | UC Code | Occupation           |\n";
            cout << "----------------------------------------------\n";
            studentSet.printClassOccupation(ascending);
        } else if (sortOrder == "b") {
            cout << "\nReturning to the previous menu...\n";
            break;
        } else {
            cout << "\nInvalid input. Please enter 'a' for ascending, 'd' for descending, or 'b' to go back.\n";
        }
    }
}


void displayYearOccupation(StudentSet& studentSet) {
    while (true) {
        string sortOrder;
        cout << "\n---------------------------------------------------------\n";
        cout << "                    Yearly Occupation                     \n";
        cout << "---------------------------------------------------------\n";
        cout << "Sort years in ascending (enter 'a') or descending (enter 'd') order, or 'b' to go back: ";
        cin >> sortOrder;

        if (sortOrder == "a" || sortOrder == "d") {
            vector<int> years = {1, 2, 3, 4, 5, 6, 7};

            if (sortOrder == "d") {
                reverse(years.begin(), years.end());
            }
            cout << "\nYear  |  Occupation\n";
            cout << "-------------------\n";
            for (int year : years) {
                int studentCount = studentSet.countStudentsByYear(year);
                cout << setw(4) << year << setw(12) << studentCount << endl;
            }
            cout << "-------------------\n";
            cout << "Sorted by " << (sortOrder == "a" ? "ascending" : "descending") << " year number.\n";
        } else if (sortOrder == "b") {
            cout << "\nReturning to the previous menu...\n";
            break;
        } else {
            cout << "\nInvalid input. Please enter 'a' for ascending, 'd' for descending, or 'b' to go back.\n";
        }
    }
}

void displayTopUcs(StudentSet& studentSet) {
    while (true) {
        string input;
        cout << "Enter the maximum number of UCs to display (or enter 'b' to go back): ";
        cin >> input;

        if (input == "b") {
            cout << "\nReturning to the previous menu...\n";
            break;
        }

        try {
            int maxUcs = stoi(input);
            if (maxUcs >= 0) {
                cout << "\n---------------------------------------------------------\n";
                cout << "                 Top UCs by Number of Students           \n";
                cout << "---------------------------------------------------------\n";

                studentSet.printTopUcs(maxUcs);
            } else {
                cout << "\nInvalid input. Maximum number of UCs should be a non-negative integer.\n";
            }
        } catch (invalid_argument&) {
            cout << "\nInvalid input. Please enter a valid non-negative integer or 'b' to go back.\n";
        }
        catch (out_of_range&) {
            cout << "\nInvalid input. The number you entered is too high.\n";
        }
    }
}






void displayAllClasses(const list<Class>& Turmas) {
    string sortingOption;

    while (true) {
        cout << "Sort by Class Code (1) or UC Code (2)? Enter 'b' to go back: ";
        getline(cin, sortingOption);

        if (sortingOption == "b") {
            break;
        }

        if (sortingOption == "1") {
            cout << "List of Classes and Corresponding UCs (Sorted by Class Code):\n";
            for (const auto& Turma : Turmas) {
                cout << "Class Code: " << Turma.getClassCode() << "\tUC Code: " << Turma.getUcCode() << endl;
            }
        } else if (sortingOption == "2") {
            list<Class> sortedTurmas = Turmas;
            sortedTurmas.sort(Class::compareByUcCode);
            cout << "List of Classes and Corresponding UCs (Sorted by UC Code):\n";
            for (const auto& Turma : sortedTurmas) {
                cout << "Class Code: " << Turma.getClassCode() << "\tUC Code: " << Turma.getUcCode() << endl;
            }
        } else {
            cout << "Invalid input. Please try again.\n";
        }
    }
}

void displayAllStudents(StudentSet& students) {
    map<string, string> sortingOptions = {
            {"1", "alphabetical"},
            {"2", "reverse alphabetical"},
            {"3", "numerical"},
            {"4", "reverse numerical"}
    };

    string sortingOption;

    while (true) {
        cout << "Sort by alphabetical (1), reverse alphabetical (2), numerical (3), reverse numerical (4). Enter 'b' to go back: ";
        getline(cin, sortingOption);

        if (sortingOption == "b") {
            break;
        }

        auto it = sortingOptions.find(sortingOption);
        if (it != sortingOptions.end()) {
            students.printStudents(it->second);
        } else {
            cout << "Invalid input. Please try again.\n";
        }
    }
}


void displayAllUCs(list<Class>& Turmas) {
    string sortingOption;

    while (true) {
        cout << "Sort by UC Code (1) - Ascending, (2) - Descending. Enter 'b' to go back: ";
        getline(cin, sortingOption);

        if (sortingOption == "b") {
            break;
        }

        if (sortingOption == "1") {
            Turmas.sort(Class::compareByUcCode);
        } else if (sortingOption == "2") {
            Turmas.sort([](const Class& lhs, const Class& rhs) {
                return Class::compareByUcCode(rhs, lhs);
            });
        } else {
            cout << "Invalid input. Please try again.\n";
            continue;
        }

        cout << "List of UCs:\n";
        for (const auto& Turma : Turmas) {
            cout << "UC Code: " << Turma.getUcCode() << endl;
        }
    }
}

void displaySearchMenu(StudentSet& Students, list<Class>Turmas){
    string choice;
    bool back = false;

    while (!back) {
        cout << "\n----------------------------------\n";
        cout << "            Search Menu           \n";
        cout << "----------------------------------\n";
        cout << "Choose an option:\n";
        cout << "1: Consult the schedule of a given student, class or UC\n";
        cout << "2: Consult the students of a given class, year or UC\n";
        cout << "3: Consult the number of students registered in at least n UCs\n";
        cout << "4: Consult the class/year/UC occupation\n";
        cout << "5: Consult the UCs with the greatest number of students\n";
        cout << "l: Consult the list of all students, classes or UCs \n";
        cout << "b. Go back\n";
        cout << "----------------------------------\n";
        cout << "Your choice: ";

        cin >> choice;

        if (choice.length() != 1){
            choice = "h";
        }

        switch (choice[0]) {
            case '1':
                while (true) {
                    cout << "\n----------------------------------\n";
                    cout << "         Schedules Sub-Menu        \n";
                    cout << "----------------------------------\n";
                    cout << "Choose a sub-option:\n";
                    cout << "1. Consult student's schedule\n";
                    cout << "2. Consult class schedule\n";
                    cout << "3. Consult UC schedule\n";
                    cout << "b. Go back\n";
                    cout << "----------------------------------\n";
                    cout << "Your choice: ";
                    cin >> choice;
                    if (choice.length() != 1){
                        choice = "h";
                    }
                    if (choice == "1") {
                        displayStudentSchedule(Students);
                    } else if (choice == "2") {
                        displayClassSchedule();
                    } else if (choice == "3") {
                        displayUcSchedule();
                    } else if (choice == "b") {
                        break;
                    } else {
                        cout << "Invalid choice. Please try again.\n";
                    }
                }
                break;
            case '2':
                while (true) {
                    cout << "\n----------------------------------\n";
                    cout << "          Students Sub-Menu      \n";
                    cout << "----------------------------------\n";
                    cout << "Choose an option:\n";
                    cout << "1. Consult students within a class\n";
                    cout << "2. Consult students within a school year\n";
                    cout << "3. Consult students within a UC\n";
                    cout << "b. Go back\n";
                    cout << "----------------------------------\n";
                    cout << "Your choice: ";
                    cin >> choice;
                    if (choice.length() != 1){
                        choice = "h";
                    }
                    if (choice == "1") {
                        displayStudentsWithinClass(Students, Turmas);
                    } else if (choice == "2") {
                        displayStudentsWithinYear(Students);
                    } else if (choice == "3") {
                        displayStudentsWithinUc(Students);
                    } else if (choice == "b") {
                        break;
                    } else {
                        cout << "Invalid choice. Please try again.\n";
                    }
                }
                break;
            case '3':
                displayStudentsAtLeastNUcs(Students);
                break;
            case '4':
                while (true) {
                    cout << "\n----------------------------------\n";
                    cout << "        Occupation Sub-Menu    \n";
                    cout << "----------------------------------\n";
                    cout << "Choose an option:\n";
                    cout << "1. Consult class occupation\n";
                    cout << "2. Consult year occupation\n";
                    cout << "3. Consult UC occupation\n";
                    cout << "b. Go back\n";
                    cout << "----------------------------------\n";
                    cout << "Your choice: ";
                    cin >> choice;
                    if (choice.length() != 1){
                        choice = "h";
                    }
                    if (choice == "1") {
                        displayClassOccupation(Students);
                    } else if (choice == "2") {
                        displayYearOccupation(Students);
                    } else if (choice == "3") {
                        displayUcOccupation(Students);
                    } else if (choice == "b") {
                        break;
                    } else {
                        cout << "Invalid choice. Please try again.\n";
                    }
                }
                break;
            case '5':
                displayTopUcs(Students);
                break;
            case 'l':
                while (true) {
                    cout << "\n----------------------------------\n";
                    cout << "        List Sub-Menu          \n";
                    cout << "----------------------------------\n";
                    cout << "Choose an option:\n";
                    cout << "1. List all classes\n";
                    cout << "2. List all students\n";
                    cout << "3. List all UCs\n";
                    cout << "b. Go back\n";
                    cout << "----------------------------------\n";
                    cout << "Your choice: ";

                    cin >> choice;

                    while (cin.get() != '\n') {
                        continue;
                    }

                    if (choice.length() != 1) {
                        choice = "h";
                    }

                    if (choice == "1") {
                        displayAllClasses(Turmas);
                    } else if (choice == "2") {
                        displayAllStudents(Students);
                    } else if (choice == "3") {
                        displayAllUCs(Turmas);
                    } else if (choice == "b") {
                        break;
                    } else {
                        cout << "Invalid choice. Please try again.\n";
                    }
                }
                break;
            case 'b':
                back = true;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}


bool isNumeric(const string& str) {
    for (char ch : str) {
        if (ch < '0' || ch > '9') {
            return false;
        }
    }
    return true;
}


string getStudentCode() {
    cout << "Enter your Student Code ('b' to go back): ";
    string studentCode;
    cin >> studentCode;
    return studentCode;
}

string getUcCode() {
    string ucCode;
    cout << "Enter UC code ('b' to go back): ";
    cin >> ucCode;
    return ucCode;
}

string getCurrentClass() {
    string currentClass;
    cout << "Enter current class ('b' to go back): ";
    cin >> currentClass;
    return currentClass;
}

string getDesiredClass() {
    string desiredClass;
    cout << "Enter desired class ('b' to go back): ";
    cin >> desiredClass;
    return desiredClass;
}

string getDesiredUc() {
    string desiredUC;
    cout << "Enter desired UC ('b' to go back): ";
    cin >> desiredUC;
    return desiredUC;
}


bool isValidStudentCode(const string& StudentCode, StudentSet& Students){
    Student student;
    if (!isNumeric(StudentCode)){
        return false;
    }
    else{
        if (Students.find_by_upcode(stoi(StudentCode)) == student){
            return false;
        }
    }
    return true;
}

bool isValidUcCode(const string& UcCode, StudentSet& Students, const string& studentCode){
    vector<Lesson> Aulas = Students.find_by_upcode(stoi(studentCode)).getLessons();
    for (const auto& Aula : Aulas){
        if (Aula.getUcCode() == UcCode){
            return true;
        }
    }
    return false;
}

bool isValidCurrentClass(const string& classCode, StudentSet& Students, const string& studentCode, const string& UcCode){
    vector<Lesson> Aulas = Students.find_by_upcode(stoi(studentCode)).getLessons();
    for (const auto& Aula : Aulas){
        if ((Aula.getClassCode() == classCode) && (Aula.getUcCode() == UcCode)){
            return true;
        }
    }
    return false;
}


bool isValidDesiredClass(const string& classCode, const string& UcCode, StudentSet& Students, string StudentCode){
    vector<Lesson> AulasEstudante = Students.find_by_upcode(stoi(StudentCode)).getLessons();

    for (const auto& AulaEstudante : AulasEstudante){
        if ((AulaEstudante.getClassCode() == classCode) && (AulaEstudante.getUcCode() == UcCode)) {
            cout << "You're already in this class. \nClass Code: " << classCode << "\nUC Code: " << UcCode << endl;
            return false;
        }
    }

    bool existe = false;
    vector<Lesson> Aulas = Storage::readLessons();

    for (const auto& Aula : Aulas){
        if ((Aula.getClassCode() == classCode) && (Aula.getUcCode() == UcCode)){
            existe = true;
            break;
        }
    }

    if (!existe){
        cout << "Class Code does not exist for this UC. Class Code: " << classCode << ", UC Code: " << UcCode << endl;
        return false;
    }

    return true;
}


bool isValidDesiredUc(const string& UcCode, StudentSet& Students, const string& StudentCode){
    if (!ucExists(UcCode)){
        cout << "UC Code not found. Please try again with a valid UC Code. \n";
        return false;
    }

    vector<Lesson> AulasEstudante = Students.find_by_upcode(stoi(StudentCode)).getLessons();
    for (const auto& AulaEstudante : AulasEstudante){
        if (AulaEstudante.getUcCode() == UcCode){
            cout << "You're already enrolled in this UC. Please try again with another UC Code. \n";
            return false;
        }
    }

    return true;
}

void makeClassChangeRequest(StudentSet& Students, const list<Class>& Turmas, RequestManager& RequestManager) {
    string studentCode, ucCode, currentClass, desiredClass;



    while (true){
        studentCode = getStudentCode();
        if (studentCode == "b") {
            cout << "Going back to the previous step..." << endl;
            return;
        }

        if (!isValidStudentCode(studentCode, Students)) {
            cout << "Student code not found. Please try again with valid input." << endl;
            continue;
        }
        break;
    }

    while (true) {
        ucCode = getUcCode();
        if (ucCode == "b") {
            cout << "Going back to the Menu..." << endl;
            return;
        }

        if (!isValidUcCode(ucCode, Students, studentCode)) {
            cout << "UC code not found in the student's list of UCs. Please try again with valid input." << endl;
            continue;
        }
        break;
    }

    while (true) {
        currentClass = getCurrentClass();
        if (currentClass == "b") {
            cout << "Going back to the Menu..." << endl;
            return;
        }

        if (!isValidCurrentClass(currentClass, Students, studentCode, ucCode)) {
            cout << "You are not in this Class for the given UC. Please try again with valid input." << endl;
            continue;
        }
        break;
    }

    while (true) {
        desiredClass = getDesiredClass();
        if (desiredClass == "b") {
            cout << "Going back to the Menu..." << endl;
            return;
        }

        if (!isValidDesiredClass(desiredClass, ucCode, Students, studentCode)) {
            continue;
        }
        break;
    }

    Student* studentPtr = new Student(Students.find_by_upcode(stoi(studentCode)));
    Class* classObject = new Class(ucCode, desiredClass);
    Request req(studentPtr, classObject, "change");
    RequestManager.addRequest(req);

    cout << "Your request has been successfully made:" << endl;
    cout << "Student Code: " << studentCode << endl;
    cout << "Course Code: " << ucCode << endl;
    cout << "Current Class: " << currentClass << endl;
    cout << "Desired Class: " << desiredClass << endl;
    cout << "We've noted your request and will process it shortly." << endl;

}


bool isValidDesiredClass2(const string& classCode, const string& UcCode, StudentSet& Students, string StudentCode){
    vector<Lesson> AulasEstudante = Students.find_by_upcode(stoi(StudentCode)).getLessons();

    for (const auto& AulaEstudante : AulasEstudante){
        if ((AulaEstudante.getClassCode() == classCode) && (AulaEstudante.getUcCode() == UcCode)) {
            cout << "You're already in this class. Class Code: " << classCode << endl;
            return false;
        }
    }

    bool existe = false;
    vector<Lesson> Aulas = Storage::readLessons();

    for (const auto& Aula : Aulas){
        if ((Aula.getClassCode() == classCode) && (Aula.getUcCode() == UcCode)){
            existe = true;
            break;
        }
    }

    if (!existe){
        cout << "The given class Code does not exist for this UC. Class Code: " << classCode << ", UC Code: " << UcCode << endl;
        return false;
    }

    return true;
}

void makeUcEnrollmentRequest(StudentSet& Students, RequestManager& RequestManager) {
    string studentCode, desiredUc, desiredClass;

    cout << "Preparing your request..." << endl;

    while (true){
        studentCode = getStudentCode();
        if (studentCode == "b") {
            cout << "Going back to the Menu..." << endl;
            return;
        }

        if (!isValidStudentCode(studentCode, Students)) {
            cout << "Student code not found. Please try again with valid input." << endl;
            continue;
        }
        break;
    }

    while (true) {
        desiredUc = getDesiredUc();
        if (desiredUc == "b") {
            cout << "Going back to the Menu..." << endl;
            return;
        }

        if (!isValidDesiredUc(desiredUc, Students, studentCode)) {
            continue;
        }
        break;
    }

    while (true) {
        desiredClass = getDesiredClass();
        if (desiredUc == "b") {
            cout << "Going back to the Menu..." << endl;
            return;
        }

        if (!isValidDesiredClass2(desiredClass, desiredUc, Students, studentCode)) {
            continue;
        }
        break;
    }

    Student* studentPtr = new Student(Students.find_by_upcode(stoi(studentCode)));
    Class* classObject = new Class(desiredUc, desiredClass);
    Request req(studentPtr, classObject, "enroll");
    RequestManager.addRequest(req);


    cout << "Your enrollment request has been successfully made:" << endl;
    cout << "Student Code: " << studentCode << endl;
    cout << "Desired UC Code: " << desiredUc << endl;
    cout << "We've noted your request and will process it shortly." << endl;
}


bool isValidDesiredUc2(const string& UcCode, StudentSet& Students, const string& StudentCode){
    if (!ucExists(UcCode)){
        return false;
    }

    vector<Lesson> AulasEstudante = Students.find_by_upcode(stoi(StudentCode)).getLessons();
    for (const auto& AulaEstudante : AulasEstudante){
        if (AulaEstudante.getUcCode() == UcCode){
            return true;
        }
    }

    return false;
}

void makeUcRemovalRequest(StudentSet& Students, RequestManager& RequestManager) {
    string studentCode, ucToRemove;

    cout << "Preparing your request..." << endl;

    while (true){
        studentCode = getStudentCode();
        if (studentCode == "b") {
            cout << "Going back to the Menu..." << endl;
            return;
        }

        if (!isValidStudentCode(studentCode, Students)) {
            cout << "Student code not found. Please try again with valid input." << endl;
            continue;
        }
        break;
    }

    while (true) {
        ucToRemove = getUcCode();
        if (ucToRemove == "b") {
            cout << "Going back to the Menu..." << endl;
            return;
        }

        if (!isValidDesiredUc2(ucToRemove, Students, studentCode)) {
            cout << "UC code not found in the student's list of UCs. Please try again with valid input." << endl;
            continue;
        }
        break;
    }
    Student estudante = Students.find_by_upcode(stoi(studentCode));
    string classCode;
    for(auto Lesson : estudante.getLessons()){
        if (Lesson.getUcCode() == ucToRemove){
            classCode = Lesson.getClassCode();
        }
    }


    Student* studentPtr = new Student(Students.find_by_upcode(stoi(studentCode)));
    Class* classObject = new Class(ucToRemove, classCode);
    Request req(studentPtr, classObject, "removal");
    RequestManager.addRequest(req);

    cout << "Your UC removal request has been successfully made:" << endl;
    cout << "Student Code: " << studentCode << endl;
    cout << "UC Code to Remove: " << ucToRemove << endl;
    cout << "We've noted your request and will process it shortly." << endl;
}


void displayRequestsSubMenu1(StudentSet& Students, const list<Class>& Turmas, RequestManager& RequestManager) {
    string choice;
    bool back = false;

    while (!back) {
        cout << "\n--------------------------------\n";
        cout << "     Requests Submenu 1       \n";
        cout << "--------------------------------\n";
        cout << "Choose an option:\n";
        cout << "1. Request for a Class change\n";
        cout << "2. Request for a UC enrollment\n";
        cout << "3. Request for a UC removal\n";
        cout << "b. Go back\n";
        cout << "--------------------------------\n";
        cout << "Your choice: ";

        cin >> choice;

        if (choice.length() != 1) {
            choice = "4";
        }

        switch (choice[0]) {
            case '1':
                makeClassChangeRequest(Students, Turmas, RequestManager);
                break;

            case '2':
                makeUcEnrollmentRequest(Students, RequestManager);
                break;

            case '3':
                makeUcRemovalRequest(Students, RequestManager);
                break;

            case 'b':
                cout << "Going back to the previous menu...\n";
                back = true;
                break;

            default:
                cout << "Invalid input. Please try again.\n";
        }
    }
}





void displayRequestsSubMenu2(StudentSet& Students, RequestManager& RequestManager) {
    string choice;
    bool back = false;
    while (!back) {
        cout << "\n--------------------------------\n";
        cout << "       Processing Submenu        \n";
        cout << "--------------------------------\n";
        cout << "Choose an option:\n";
        cout << "1. Process the next request in queue\n";
        cout << "2. Undo the last action \n";
        cout << "3. Process all requests\n";
        cout << "4. Check pending requests\n";
        cout << "b. Go back\n";
        cout << "--------------------------------\n";
        cout << "Your choice: ";
        cin >> choice;

        if (choice.length() != 1) {
            choice = "6";
        }

        switch (choice[0]) {
            case '1':
                cout << "Processing the next pending request...\n";
                if (RequestManager.getAllRequests().empty()) {
                    cout << "There are no pending requests to process." << endl;
                } else {
                    RequestManager.processRequest(RequestManager.getAllRequests().front());
                    Students = RequestManager.getStudentSet();
                    newStudentsFile(Students);
                }
                if (RequestManager.getAllRequests().empty()) {
                    cout << "No more pending requests left." << endl;
                }
                return;
            case '2':
                cout << "Processing all requests in line...";
                RequestManager.undoLastAction();
                newStudentsFile(Students);
                break;
            case '3':
                RequestManager.processAllRequests();
                newStudentsFile(Students);
                break;
            case '4':
                RequestManager.printPending();
                break;
            case 'b':
                cout << "Going back to previous menu...\n";
                back = true;
                break;
            default:
                cout << "Invalid input. Please try again.\n";
        }
    }
}

void displayRequestsMenu(StudentSet& Students, list<Class>& Turmas, RequestManager& RequestManager){
    string choice;
    bool back = false;

    while (!back) {
        cout << "\n--------------------------------\n";
        cout << "        Request's Menu        \n";
        cout << "--------------------------------\n";
        cout << "Choose an option:\n";
        cout << "1. Make a request\n";
        cout << "2. Manage requests\n";
        cout << "b. Go back\n";
        cout << "--------------------------------\n";
        cout << "Your choice: ";
        cin >> choice;

        cin.ignore();

        if (choice.length() != 1) {
            choice = "3";
        }

        switch (choice[0]) {
            case '1':
                cout << "Executing Option 1 in Requests Menu...\n";
                displayRequestsSubMenu1(Students, Turmas, RequestManager);
                break;
            case '2':
                cout << "Executing Option 2 in Requests Menu...\n";
                displayRequestsSubMenu2(Students, RequestManager);
                break;
            case 'b':
                cout << "Going back to main menu...\n";
                back = true;
                break;
            default:
                cout << "Invalid input. Please try again.\n";
        }
    }
}

void App::run() {
    Menu();

}
