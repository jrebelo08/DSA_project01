#ifndef LESSON_H
#define LESSON_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;

/**
 * @brief This class represents a lesson in a schedule.
 * The Lesson class includes details such as class code, uc code, week day, type of class,
 * starting hour and ending hour of the lesson.
 */


class Lesson {
private:
    /** @brief The code of the class. */
    string ClassCode;
    /** @brief The uc code. */
    string UcCode;
    /** @brief The day of the week the lesson is on. */
    string WeekDay;
    /** @brief The type of the lesson (example -> T;TP;PL). */
    string Type;
    /** @brief The starting hour of the lesson. */
    float startHour;
    /** @brief The ending hour of the lesson. */
    float endHour;
public:
    Lesson(string ClassCode,string UcCode,string WeekDay,float startHour,float endHour, string Type);

    string getUcCode() const;
    string getClassCode() const;
    string getWeekDay() const;
    string getType() const;
    float getStartHour() const;
    float getEndHour() const;

    void printSchedule() const;
    static string UcName(const string &uccode);
    static const map<string, int> weekNum;

    static void printTime(float time);

    static vector<Lesson> findByUcClass(const string &targetUcCode, const string &targetClassCode, const vector<Lesson> &Lessons) ;

    bool operator==(const Lesson& other) const;
    bool operator<(const Lesson& other) const;

    bool conflicts(const Lesson& other) const;

    void printScheduleUC() const;

};

#endif // LESSON_H