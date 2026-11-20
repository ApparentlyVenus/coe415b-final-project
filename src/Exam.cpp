#include "../inc/Exam.hpp"

int Exam::getId() const { return _id; }
const std::string& Exam::getCourseCode() const { return _courseCode; }
int Exam::getDuration() const { return _durationMinutes; }
int Exam::getSlot() const { return _slot; }

void Exam::setDuration(int duration) { _durationMinutes = duration; }
void Exam::setCourseCode(const std::string& courseCode) { _courseCode = courseCode; }
void Exam::setSlot(int slot) { _slot = slot; }