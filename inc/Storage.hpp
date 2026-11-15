#ifndef STORAGE_HPP
# define STORAGE_HPP

# include "University.hpp"
# include "Scheduler.hpp"

# include <map>
# include <string>

class Storage {
private:

    std::string _dir;

public:

    explicit Storage(const std::string& dir = "data") : _dir(dir) {}

    void save(University& u) const;
    void load(University& u) const;
    void saveTimetable(const University& u, const t_schedule_result& r, const std::string& strategy) const;
    std::map<std::string, int> loadActualSchedule() const;
};

#endif