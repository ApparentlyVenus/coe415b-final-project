#include "Scheduler.hpp"

#include <algorithm>
#include <random>
#include <set>

void Scheduler::buildConflictGraph() {
    _examIds.clear();
    _indexOf.clear();
    _conflicts.clear();

    std::map<int, Exam>& exams = _uni.getExams();
    std::map<std::string, int> examOfCourse;

    for (std::map<int, Exam>::iterator it = exams.begin(); it != exams.end(); ++it) {
        _indexOf[it->first] = static_cast<int>(_examIds.size());
        _examIds.push_back(it->first);
        examOfCourse[it->second.getCourseCode()] = it->first;
    }

    _graph = Graph(static_cast<int>(_examIds.size()));
    std::map<std::pair<int, int>, std::vector<int> > shared;

    std::map<int, Student>& students = _uni.getStudents();
    for (std::map<int, Student>::iterator s = students.begin(); s != students.end(); ++s) {
        std::vector<int> mine;
        const std::vector<std::string>& codes = s->second.getCourseCodes();
        for (size_t i = 0; i < codes.size(); ++i) {
            std::map<std::string, int>::iterator f = examOfCourse.find(codes[i]);
            if (f != examOfCourse.end())
                mine.push_back(f->second);
        }
        for (size_t i = 0; i < mine.size(); ++i) {
            for (size_t j = i + 1; j < mine.size(); ++j) {
                int a = std::min(mine[i], mine[j]);
                int b = std::max(mine[i], mine[j]);
                _graph.addEdge(_indexOf[a], _indexOf[b]);
                shared[std::make_pair(a, b)].push_back(s->first);
            }
        }
    }

    for (std::map<std::pair<int, int>, std::vector<int> >::iterator it = shared.begin(); it != shared.end(); ++it) {
        t_conflict_info c;
        c.examA = it->first.first;
        c.examB = it->first.second;
        c.sharedStudents = it->second;
        _conflicts.push_back(c);
    }
}

t_schedule_result Scheduler::colorGreedy(const std::vector<int>& examOrder) {
    std::vector<int> color(_graph.size(), -1);
    int maxColor = -1;

    for (size_t i = 0; i < examOrder.size(); ++i) {
        int v = _indexOf.at(examOrder[i]);
        std::set<int> used;
        const std::set<int>& nbs = _graph.neighbors(v);
        for (std::set<int>::const_iterator it = nbs.begin(); it != nbs.end(); ++it)
            if (color[*it] != -1)
                used.insert(color[*it]);
        int c = 0;
        while (used.count(c))
            ++c;
        color[v] = c;
        maxColor = std::max(maxColor, c);
    }

    t_schedule_result r;
    for (size_t i = 0; i < color.size(); ++i)
        if (color[i] != -1)
            r.slotOfExam[_examIds[i]] = color[i];
    r.slotsUsed = maxColor + 1;
    return r;
}

t_schedule_result Scheduler::colorGreedy(Order o, unsigned seed) {
    std::vector<int> order = _examIds;   // AS_ENTERED

    if (o == Order::LARGEST_DEGREE_FIRST || o == Order::SMALLEST_DEGREE_FIRST) {
        bool largestFirst = (o == Order::LARGEST_DEGREE_FIRST);
        std::stable_sort(order.begin(), order.end(), [&](int a, int b) {
            int degA = _graph.degree(_indexOf.at(a));
            int degB = _graph.degree(_indexOf.at(b));
            return largestFirst ? degA > degB : degA < degB;
        });
    }
    else if (o == Order::RANDOM) {
        std::mt19937 rng(seed);
        std::shuffle(order.begin(), order.end(), rng);
    }
    return colorGreedy(order);
}

void Scheduler::apply(const t_schedule_result& r) {
    std::map<int, Exam>& exams = _uni.getExams();
    for (std::map<int, Exam>::iterator it = exams.begin(); it != exams.end(); ++it) {
        std::map<int, int>::const_iterator f = r.slotOfExam.find(it->first);
        it->second.setSlot(f == r.slotOfExam.end() ? -1 : f->second);
    }
}

bool Scheduler::isValid(const t_schedule_result& r) const {
    for (int u = 0; u < _graph.size(); ++u) {
        const std::set<int>& nbs = _graph.neighbors(u);
        for (std::set<int>::const_iterator it = nbs.begin(); it != nbs.end(); ++it)
            if (r.slotOfExam.at(_examIds[u]) == r.slotOfExam.at(_examIds[*it]))
                return false;
    }
    return true;
}

const Graph& Scheduler::graph() const { return _graph; }
const std::vector<t_conflict_info>& Scheduler::conflicts() const { return _conflicts; }
const std::vector<int>& Scheduler::examIds() const { return _examIds; }