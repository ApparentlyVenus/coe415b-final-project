#ifndef EXAM_HPP
# define EXAM_HPP

# include <string>

class Exam {
private:

    int _id;
    std::string _courseCode;
    int _durationMinutes;
    int _slot;

public:

    Exam() : _id(0), _durationMinutes(120), _slot(-1) {}
    Exam(int id, std::string courseCode, int durationMinutes = 120) 
        : _id(id), _courseCode(courseCode), _durationMinutes(durationMinutes), _slot(-1) {}
    
    int getId() const;
    const std::string& getCourseCode() const;
    int getDuration() const;
    int getSlot() const;

    void setDuration(int duration);
    void setCourseCode(const std::string& courseCode);
    void setSlot(int slot);
};

#endif