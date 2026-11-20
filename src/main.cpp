#include "../inc/University.hpp"
#include "../inc/Scheduler.hpp"
#include "../inc/Storage.hpp"
#include "../inc/utils.hpp"

#include <iostream>
#include <string>

int main() {
    University uni;
    Storage st("data");
    Scheduler sched(uni);
    st.load(uni);

    for (;;) {
        std::cout << "\n1 Add student   2 Add course   3 Enroll   4 Add exam   5 Delete exam\n"
                     "6 List all      7 Generate timetable   8 Conflict report\n"
                     "9 Compare orderings   10 Compare with real schedule   11 Delete student   0 Save & exit\n";
        int choice = readInt("> ");
        if (choice == 0) {
            st.save(uni);
            break;
        }
        switch (choice) {
        case 1: {
            int id = readInt("id: ");
            std::cout << (uni.addStudent(id, readLine("name: ")) ? "added\n" : "id exists\n");
            break;
        }
        case 2: {
            std::string code = readLine("code: ");
            std::cout << (uni.addCourse(code, readLine("name: ")) ? "added\n" : "exists\n");
            break;
        }
        case 3: {
            int id = readInt("student id: ");
            std::cout << (uni.enroll(id, readLine("course code: ")) ? "ok\n" : "not found\n");
            break;
        }
        case 4: {
            std::string code = readLine("course code: ");
            int duration = readInt("duration (min): ");
            int id = uni.addExam(code, duration);
            if (id < 0)
                std::cout << "failed (no such course, or it already has an exam)\n";
            else
                std::cout << "exam id " << id << "\n";
            break;
        }
        case 5:
            std::cout << (uni.removeExam(readInt("exam id: ")) ? "deleted\n" : "not found\n");
            break;
        case 6:
            listAll(uni);
            break;
        case 7: {
            sched.buildConflictGraph();
            t_schedule_result r = sched.colorGreedy(Order::LARGEST_DEGREE_FIRST);
            sched.apply(r);
            st.saveTimetable(uni, r, "LARGEST_DEGREE_FIRST");
            printTimetable(uni, r);
            std::cout << (sched.isValid(r) ? "valid\n" : "INVALID\n");
            break;
        }
        case 8:
            conflictReport(uni, sched);
            break;
        case 9:
            compareOrders(sched);
            break;
        case 10:
            compareWithActual(uni, sched, st);
            break;
        case 11:
            std::cout << (uni.removeStudent(readInt("student id: ")) ? "deleted\n" : "not found\n");
            break;
        }
    }
    return 0;
}