#include "../inc/Storage.hpp"
#include "../third_party/json.hpp"

#include <fstream>

nlohmann::json readFile(const std::string& path) {
    std::ifstream in(path.c_str());
    if (!in)
        return nlohmann::json();
    try {
        return nlohmann::json::parse(in);
    } catch (...) {
        return nlohmann::json();
    }
}

void writeFile(const std::string& path, const nlohmann::json& j) {
    std::ofstream out(path.c_str());
    out << j.dump(2);
}

void Storage::save(University& u) const {
    nlohmann::json students = nlohmann::json::array();
    nlohmann::json courses = nlohmann::json::array();
    nlohmann::json exams = nlohmann::json::array();

    const std::map<int, Student>& ss = u.getStudents();
    for (std::map<int, Student>::const_iterator it = ss.begin(); it != ss.end(); ++it) {
        nlohmann::json s;
        s["id"] = it->first;
        s["name"] = it->second.getName();
        s["courses"] = it->second.getCourseCodes();
        students.push_back(s);
    }

    const std::map<std::string, Course>& cs = u.getCourses();
    for (std::map<std::string, Course>::const_iterator it = cs.begin(); it != cs.end(); ++it) {
        nlohmann::json c;
        c["code"] = it->second.getCode();
        c["name"] = it->second.getName();
        courses.push_back(c);
    }

    const std::map<int, Exam>& es = u.getExams();
    for (std::map<int, Exam>::const_iterator it = es.begin(); it != es.end(); ++it) {
        nlohmann::json e;
        e["id"] = it->first;
        e["course"] = it->second.getCourseCode();
        e["duration"] = it->second.getDuration();
        e["slot"] = it->second.getSlot();
        exams.push_back(e);
    }

    nlohmann::json examFile;
    examFile["nextId"] = u.getNextExamId();
    examFile["exams"] = exams;

    writeFile(_dir + "/students.json", students);
    writeFile(_dir + "/courses.json", courses);
    writeFile(_dir + "/exams.json", examFile);
}

void Storage::load(University& u) const {
    nlohmann::json courses = readFile(_dir + "/courses.json");
    for (nlohmann::json::iterator it = courses.begin(); it != courses.end(); ++it)
        u.addCourse(it->value("code", ""), it->value("name", ""));

    nlohmann::json students = readFile(_dir + "/students.json");
    for (nlohmann::json::iterator it = students.begin(); it != students.end(); ++it) {
        int id = (*it)["id"];
        u.addStudent(id, it->value("name", ""));
        Student* s = u.findStudent(id);
        for (nlohmann::json::iterator c = (*it)["courses"].begin(); c != (*it)["courses"].end(); ++c)
            s->enroll(*c);
    }

    nlohmann::json examFile = readFile(_dir + "/exams.json");
    if (examFile.is_object()) {
        for (nlohmann::json::iterator it = examFile["exams"].begin(); it != examFile["exams"].end(); ++it) {
            int id = (*it)["id"];
            Exam e(id, (*it)["course"], it->value("duration", 120));
            e.setSlot(it->value("slot", -1));
            u.getExams()[id] = e;
        }
        u.setNextExamId(examFile.value("nextId", 1));
    }
    u.rebuildEnrollment();
}

void Storage::saveTimetable(const University& u, const t_schedule_result& r, const std::string& strategy) const {
    nlohmann::json t;
    t["strategy"] = strategy;
    t["slotsUsed"] = r.slotsUsed;
    t["assignments"] = nlohmann::json::array();
    for (std::map<int, int>::const_iterator it = r.slotOfExam.begin(); it != r.slotOfExam.end(); ++it) {
        nlohmann::json a;
        a["exam"] = it->first;
        a["course"] = u.getExams().at(it->first).getCourseCode();
        a["slot"] = it->second;
        t["assignments"].push_back(a);
    }
    writeFile(_dir + "/timetable.json", t);
}

std::map<std::string, int> Storage::loadActualSchedule() const {
    std::map<std::string, int> m;
    nlohmann::json j = readFile(_dir + "/actual_schedule.json");
    if (j.is_object())
        for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it)
            m[it.key()] = it.value();
    return m;
}