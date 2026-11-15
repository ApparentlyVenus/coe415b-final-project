#ifndef UNIVERSITY_HPP
# define UNIVERSITY_HPP

# include "Student.hpp"
# include "Course.hpp"
# include "Exam.hpp"

# include <map>
# include <string>

class University {
private:
    
    std::map<int, Student> _students;
    std::map<std::string, Course> _courses;
    std::map<int, Exam> _exams;
    int _nextExamId;

public:

    University() : _nextExamId(1) {}

    std::map<int, Student>& getStudents();
    std::map<std::string, Course>& getCourses();
    std::map<int, Exam>& getExams();
    const std::map<int, Student>& getStudents() const;
    const std::map<std::string, Course>& getCourses() const;
    const std::map<int, Exam>& getExams() const;

    int getNextExamId() const;
    void setNextExamId(int id);

    bool addStudent(int id, const std::string& name);
    bool removeStudent(int id);
    Student* findStudent(int id);

    bool addCourse(const std::string& code, const std::string& name);
    bool removeCourse(const std::string& code);
    Course* findCourse(const std::string& code);

    int addExam(const std::string& code, int duration = 120);
    bool removeExam(int id);
    Exam* findExam(int id);

    bool enroll(int studentId, const std::string& code);
    bool drop(int studentId, const std::string& code);

    void rebuildEnrollment();
};

#endif