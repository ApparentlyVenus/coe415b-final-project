#include "../inc/Student.hpp"
#include <algorithm>

int Student::getId() const { return _id; }
const std::string& Student::getName() const { return _name; }
const std::vector<std::string>& Student::getCourseCodes() const { return _courseCodes; }

void Student::setName(const std::string& name) { _name = name; }

bool Student::isEnrolled(const std::string& code) const {
    return std::find(_courseCodes.begin(), _courseCodes.end(), code) != _courseCodes.end();
}

void Student::enroll(const std::string& code) {
    if (!isEnrolled(code))
        _courseCodes.push_back(code);
}

void Student::drop(const std::string& code) {
    _courseCodes.erase(std::remove(_courseCodes.begin(), _courseCodes.end(), code), _courseCodes.end());
}