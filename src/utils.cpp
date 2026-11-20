#include "../inc/utils.hpp"

#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <vector>

std::string slotLabel(int s) {
    return "Day " + std::to_string(s / 2 + 1) + (s % 2 == 0 ? " AM" : " PM");
}

int readInt(const std::string& prompt) {
    int x;
    std::cout << prompt;
    while (!(std::cin >> x)) {
        if (std::cin.eof())
            std::exit(0);
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "number: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return x;
}

std::string readLine(const std::string& prompt) {
    std::string s;
    std::cout << prompt;
    std::getline(std::cin, s);
    return s;
}

void printTimetable(University& u, const t_schedule_result& r) {
    std::map<int, std::vector<std::string> > bySlot;
    for (std::map<int, int>::const_iterator it = r.slotOfExam.begin(); it != r.slotOfExam.end(); ++it)
        bySlot[it->second].push_back(u.getExams().at(it->first).getCourseCode());
    for (std::map<int, std::vector<std::string> >::iterator it = bySlot.begin(); it != bySlot.end(); ++it) {
        std::cout << std::setw(10) << std::left << slotLabel(it->first) << ": ";
        for (size_t i = 0; i < it->second.size(); ++i)
            std::cout << it->second[i] << "  ";
        std::cout << "\n";
    }
    std::cout << "Total time slots required: " << r.slotsUsed << "\n";
}

void conflictReport(University& u, Scheduler& s) {
    s.buildConflictGraph();
    const std::vector<t_conflict_info>& cs = s.conflicts();
    if (cs.empty()) {
        std::cout << "No conflicts.\n";
        return;
    }
    for (size_t i = 0; i < cs.size(); ++i) {
        std::cout << u.getExams().at(cs[i].examA).getCourseCode() << " <-> "
                  << u.getExams().at(cs[i].examB).getCourseCode() << "  shared by: ";
        for (size_t j = 0; j < cs[i].sharedStudents.size(); ++j)
            std::cout << u.getStudents().at(cs[i].sharedStudents[j]).getName() << " ";
        std::cout << "\n";
    }
}

void compareOrders(Scheduler& s) {
    s.buildConflictGraph();
    std::cout << std::setw(24) << std::left << "As entered" << s.colorGreedy(Order::AS_ENTERED).slotsUsed << " slots\n";
    std::cout << std::setw(24) << std::left << "Largest degree first" << s.colorGreedy(Order::LARGEST_DEGREE_FIRST).slotsUsed << " slots\n";
    std::cout << std::setw(24) << std::left << "Smallest degree first" << s.colorGreedy(Order::SMALLEST_DEGREE_FIRST).slotsUsed << " slots\n";
    std::cout << std::setw(24) << std::left << "Random (seed 1)" << s.colorGreedy(Order::RANDOM, 1).slotsUsed << " slots\n";
    int best = std::numeric_limits<int>::max();
    for (unsigned seed = 1; seed <= 200; ++seed)
        best = std::min(best, s.colorGreedy(Order::RANDOM, seed).slotsUsed);
    std::cout << std::setw(24) << std::left << "Best of 200 random" << best << " slots\n";
}

void compareWithActual(University& u, Scheduler& s, Storage& st) {
    std::map<std::string, int> actual = st.loadActualSchedule();
    if (actual.empty()) {
        std::cout << "No data/actual_schedule.json found.\n";
        return;
    }
    s.buildConflictGraph();
    t_schedule_result ours = s.colorGreedy(Order::LARGEST_DEGREE_FIRST);

    std::set<int> actualSlots;
    std::map<int, Exam>& exams = u.getExams();
    for (std::map<int, Exam>::iterator it = exams.begin(); it != exams.end(); ++it)
        if (actual.count(it->second.getCourseCode()))
            actualSlots.insert(actual[it->second.getCourseCode()]);

    std::cout << "Our timetable uses " << ours.slotsUsed << " slots, the university's uses "
              << actualSlots.size() << " distinct slots for these exams.\n";

    int clashes = 0;   // does the real schedule clash for our group?
    const std::vector<t_conflict_info>& cs = s.conflicts();
    for (size_t i = 0; i < cs.size(); ++i) {
        std::string a = exams.at(cs[i].examA).getCourseCode();
        std::string b = exams.at(cs[i].examB).getCourseCode();
        if (actual.count(a) && actual.count(b) && actual[a] == actual[b]) {
            std::cout << "Real schedule clash: " << a << " and " << b << "\n";
            ++clashes;
        }
    }
    std::cout << clashes << " clash(es) in the real schedule.\n";
}

void listAll(University& uni) {
    for (std::map<int, Student>::iterator it = uni.getStudents().begin(); it != uni.getStudents().end(); ++it) {
        std::cout << it->first << " " << it->second.getName() << ": ";
        for (size_t i = 0; i < it->second.getCourseCodes().size(); ++i)
            std::cout << it->second.getCourseCodes()[i] << " ";
        std::cout << "\n";
    }
    for (std::map<std::string, Course>::iterator it = uni.getCourses().begin(); it != uni.getCourses().end(); ++it)
        std::cout << it->first << " " << it->second.getName() << " (" << it->second.getStudents().size() << " students)\n";
    for (std::map<int, Exam>::iterator it = uni.getExams().begin(); it != uni.getExams().end(); ++it)
        std::cout << "exam " << it->first << " " << it->second.getCourseCode() << " "
                  << it->second.getDuration() << "min slot " << it->second.getSlot() << "\n";
}