#ifndef STUDENT_HPP
# define STUDENT_HPP

# include <string>
# include <vector>

class Student {
private:

    int _id;
    std::string _name;
    std::vector<std::string> _courseCodes;

public:

    Student() : _id(0) {}
    Student(int id, std::string name) : _id(id), _name(name) {}

    int getId() const;
    const std::string& getName() const;
    const std::vector<std::string>& getCourseCodes() const;

    void setName(const std::string& name);

    bool isEnrolled(const std::string& code) const;
    void enroll(const std::string& code);
    void drop(const std::string& code);
};

#endif