#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <string>
#include <queue>
#include <stack>
#include <vector>
#include <climits>
#include "Request.h"
#include "Lesson.h"
#include "Storage.h"
#include "StudentsSet.h"





/**
 * @brief Manages and processes student requests
 * This class is responsible for managing and processing different types of requests made by students.
 * It maintains queues of different types of requests, a record of rejected requests, a stack of processed requests for undo actions, and a set of all students.
 */
class RequestManager {
private:

    /**
    * @brief Queue of all requests
    * This queue stores all types of requests that are pending.
    */
    queue<Request> allRequests;

    /**
    * @brief Queue of "change" requests
    * This queue stores only "change" type requests that are pending.
    */
    queue<Request> changingRequests;

    /**
     * @brief Queue of "removal" requests
     * This queue stores only "removal" type requests that are pending.
     */
    queue<Request> removalRequests;

    /**
     * @brief Queue of "enrollment" requests
     * This queue stores only "enrollment" type requests that are pending.
     */
    queue<Request> enrollmentRequests;

    /**
     * @brief Vector of rejected requests and the reasons for rejection
     * This vector stores pairs where the first element is a rejected request and the second element is the reason why the request was rejected.
     */
    vector<pair<Request, string>> rejectedRequests;

    /**
     * @brief Stack of processed requests
     * This stack stores requests that have been accepted, in the order they were accepted. It is used for undo operations.
     */
    stack<Request> Actions;

    /**
     * @brief Set of all students
     * This set stores all the students that are managed by the RequestManager.
     */
    StudentSet Students;

public:

    void setStudentSet(StudentSet& students);

    StudentSet& getStudentSet();

    queue<Request> getAllRequests() const;

    void addRequest(const Request& request);

    void processRequest(Request& request);
    void processAllRequests();

    static bool checkUCRules(const Request& request,StudentSet Students);
    static bool balanceDisturbed(const Request& request);

    void printPending() const;

    void undoLastAction();

    static void fileRequest(const Request& request);

};


#endif //REQUESTMANAGER_H