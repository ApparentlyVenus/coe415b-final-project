#include "../inc/University.hpp"

std::map<int, Student>& University::getStudents() { return _students; }
std::map<std::string, Course>& University::getCourses() { return _courses; }
std::map<int, Exam>& University::getExams() { return _exams; }
const std::map<int, Student>& University::getStudents() const { return _students; }
const std::map<std::string, Course>& University::getCourses() const { return _courses; }
const std::map<int, Exam>& University::getExams() const { return _exams; }

int University::getNextExamId() const { return _nextExamId; }
void University::setNextExamId(int id) { _nextExamId = id; }

bool University::addStudent(int id, const std::string& name) {
    if (_students.count(id))
        return false;
    _students[id] = Student(id, name);
    return true;
}

bool University::removeStudent(int id) {
    bool erased = _students.erase(id) > 0;
    rebuildEnrollment();
    return erased;
}

Student* University::findStudent(int id) {
    std::map<int, Student>::iterator it = _students.find(id);
    return it == _students.end() ? NULL : &it->second;
}

// courses

bool University::addCourse(const std::string& code, const std::string& name) {
    if (_courses.count(code))
        return false;
    _courses[code] = Course(code, name);
    return true;
}

bool University::removeCourse(const std::string& code) {
    if (_courses.erase(code) == 0)
        return false;
    for (std::map<int, Student>::iterator it = _students.begin(); it != _students.end(); ++it)
        it->second.drop(code);
    for (std::map<int, Exam>::iterator it = _exams.begin(); it != _exams.end();) {
        if (it->second.getCourseCode() == code)
            _exams.erase(it++);
        else
            ++it;
    }
    rebuildEnrollment();
    return true;
}

Course* University::findCourse(const std::string& code) {
    std::map<std::string, Course>::iterator it = _courses.find(code);
    return it == _courses.end() ? NULL : &it->second;
}

int University::addExam(const std::string& code, int duration) {
    if (!findCourse(code))
        return -1;
    for (std::map<int, Exam>::iterator it = _exams.begin(); it != _exams.end(); ++it)
        if (it->second.getCourseCode() == code)
            return -1;
    int id = _nextExamId++;
    _exams[id] = Exam(id, code, duration);
    return id;
}

bool University::removeExam(int id) {
    return _exams.erase(id) > 0;
}

Exam* University::findExam(int id) {
    std::map<int, Exam>::iterator it = _exams.find(id);
    return it == _exams.end() ? NULL : &it->second;
}

// registration

bool University::enroll(int studentId, const std::string& code) {
    Student* s = findStudent(studentId);
    if (!s || !findCourse(code))
        return false;
    s->enroll(code);
    rebuildEnrollment();
    return true;
}

bool University::drop(int studentId, const std::string& code) {
    Student* s = findStudent(studentId);
    if (!s)
        return false;
    s->drop(code);
    rebuildEnrollment();
    return true;
}

void University::rebuildEnrollment() {
    for (std::map<std::string, Course>::iterator it = _courses.begin(); it != _courses.end(); ++it)
        it->second.clearStudents();
    for (std::map<int, Student>::iterator s = _students.begin(); s != _students.end(); ++s) {
        const std::vector<std::string>& codes = s->second.getCourseCodes();
        for (size_t i = 0; i < codes.size(); ++i) {
            Course* c = findCourse(codes[i]);
            if (c)
                c->addStudent(s->first);
        }
    }
}