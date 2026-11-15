#ifndef COURSE_HPP
# define COURSE_HPP

# include <string>
# include <vector>

class Course {
private:

    std::string _code;
    std::string _name;
    std::vector<int> _students;

public:
    Course() {}
    Course(std::string code, std::string name) : _code(code), _name(name) {}

    const std::string& getCode() const;
    const std::string& getName() const;
    const std::vector<int>& getStudents() const;

    void setName(const std::string& name);

    void clearStudents();
    void addStudent(int id);
};

#endif