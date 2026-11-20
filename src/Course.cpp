#include "../inc/Course.hpp"

const std::string& Course::getCode() const { return _code; }
const std::string& Course::getName() const { return _name; }
const std::vector<int>& Course::getStudents() const { return _students; }

void Course::setName(const std::string& name) { _name = name; }

void Course::clearStudents() { _students.clear(); }
void Course::addStudent(int id) { _students.push_back(id); }