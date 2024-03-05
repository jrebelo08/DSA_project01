#include "Lesson.h"

/**
 * @brief Constructor for the Lesson class.
 * @param ClassCode The class code.
 * @param UcCode The UC code.
 * @param WeekDay The weekday.
 * @param startHour The start hour of the lesson.
 * @param endHour The end hour of the lesson.
 * @param Type The type of the lesson.
 */
Lesson::Lesson(string ClassCode, string UcCode, string WeekDay, float startHour, float endHour, string Type)
        : ClassCode(ClassCode), UcCode(UcCode), WeekDay(WeekDay), startHour(startHour), endHour(endHour), Type(Type) { }

/**
* @return The UC code of the lesson.
*/
string Lesson::getUcCode() const { return UcCode; }

/**
 * @return The class code of the lesson.
 */
string Lesson::getClassCode() const { return ClassCode; }

/**
 * @return The day of the week of the lesson.
 */
string Lesson::getWeekDay() const { return WeekDay; }

/**
 * @return The start hour of the lesson.
 */
float Lesson::getStartHour() const { return startHour; }

/**
 * @return The end hour of the lesson.
 */
float Lesson::getEndHour() const { return endHour; }

/**
 * @return The type of the lesson.
 */
string Lesson::getType() const { return Type; }

/**
 * @brief Finds a lesson by UC code and class code.
 * @param targetUcCode The target UC code.
 * @param targetClassCode The target class code.
 * @param Lessons A vector of lessons.
 * @return The lesson with the matching UC code and class code.
 * @throws runtime_error If no lesson is found with the provided UC code and class code.
 * @details Time complexity: O(n), where n is the size of the vector 'Lessons'.
 */
vector<Lesson> Lesson::findByUcClass(const string& targetUcCode, const string& targetClassCode, const vector<Lesson>& Lessons) {
    vector<Lesson> matchingLessons;

    for (const auto& l : Lessons) {
        if (l.getUcCode() == targetUcCode && l.getClassCode() == targetClassCode) {
            matchingLessons.push_back(l);
        }
    }

    if (matchingLessons.empty()) {
        throw std::runtime_error("No lesson found with the provided ucCode and classCode.");
    }

    return matchingLessons;
}

/**
 * @brief Prints formatted time
 * @param time The time to print.
 */
void Lesson::printTime(float time) {
    int hours = static_cast<int>(time);
    int minutes = static_cast<int>((time - hours) * 60);

    cout << setfill('0') << setw(2) << hours << ":"
         << setfill('0') << setw(2) << minutes << endl;
}

/**
 * @brief Returns the name of a UC based on its code.
 * @param uccode The UC code.
 * @return The name of the UC.
 */
string Lesson::UcName(const string& uccode){
    map<string, string> UcCode_Name = {{"L.EIC001", "ALGA"},{"L.EIC002", "AMI"},{"L.EIC003", "FP"},{"L.EIC004", "FSC"},{"L.EIC005", "MD"},{"L.EIC006", "AC"},{"L.EIC007", "AMII"},{"L.EIC008", "FI"},{"L.EIC009", "P"},{"L.EIC010", "TC"},{"L.EIC011", "AED"},{"L.EIC012", "BD"},{"L.EIC013", "FII"},{"L.EIC014", "LDTS"},{"L.EIC015", "SO"},{"L.EIC016", "DA"},{"L.EIC017", "ES"},{"L.EIC018", "LC"},{"L.EIC019", "LTW"},{"L.EIC020", "ME"},{"L.EIC021", "FSI"},{"L.EIC022", "IPC"},{"L.EIC023", "LBAW"},{"L.EIC024", "PFL"},{"L.EIC025", "RC"},{"L.EIC026", "C"},{"L.EIC027", "CG"},{"L.EIC028", "CPD"},{"L.EIC029", "IA"},{"L.EIC030", "PI"},{"UP001", "PUP"}};
    return UcCode_Name[uccode];
}


/**
 * @brief Prints the schedule of the lesson.
 */
void Lesson::printSchedule() const {
    cout << endl;
    cout << "UC Code: " << getUcCode() << " (" << UcName(getUcCode()) << ")" << endl
         << "Day of Week: " << getWeekDay() << endl
         << "Start Time: ";
    printTime(getStartHour());
    cout << "End Time: ";
    printTime(getEndHour());
    cout << "Type: " << getType() << endl;
}

/**
 * @brief Prints the schedule of the UC.
 */
void Lesson::printScheduleUC() const {
    cout << endl;
    cout << "Class Code: " << getClassCode() << endl
         << "UC Code: " << getUcCode() << " (" << UcName(getUcCode()) << ")" << endl
         << "Day of Week: " << getWeekDay() << endl
         << "Start Time: ";
    printTime(getStartHour());
    cout << "End Time: ";
    printTime(getEndHour());
    cout << "Type: " << getType() << endl;
}

/**
 * @brief Checks if the lesson conflicts with another lesson.
 * @param other The other lesson to compare.
 * @return True if the lessons conflict, false otherwise.
 */
bool Lesson::conflicts(const Lesson& other) const  {
    if (this->getType() == "T" || other.getType() == "T") {
        return false;
    }

    if (this->getWeekDay() == other.getWeekDay()) {
        if ((this->getStartHour() >= other.getStartHour() && this->getStartHour() < other.getEndHour()) ||
            (other.getStartHour() >= this->getStartHour() && other.getStartHour() < this->getEndHour())) {
            return true;
        }
    }
    return false;
}


/**
 * @brief Equality operator for the Lesson class.
 * @param other The other lesson to compare with.
 * @return True if the lessons are equal, false otherwise.
 */
bool Lesson::operator==(const Lesson& other) const {
    return WeekDay == other.getWeekDay() &&
           startHour == other.getStartHour() &&
           endHour == other.getEndHour() &&
           Type == other.getType();
}

/**
 * @brief Less than operator for the Lesson class.
 * @param other The other lesson to compare with.
 * @return True if this lesson is before the other lesson, false otherwise.
 */
bool Lesson::operator<(const Lesson& other) const {
    if (WeekDay != other.getWeekDay()) {
        return WeekDay < other.getWeekDay();
    }
    if (startHour != other.getStartHour()) {
        return startHour < other.getStartHour();
    }
    if (endHour != other.getEndHour()) {
        return endHour < other.getEndHour();
    }
    return Type < other.getType();
}