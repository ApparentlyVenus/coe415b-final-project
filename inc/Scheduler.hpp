#ifndef SCHEDULER_HPP
# define SCHEDULER_HPP

# include "Graph.hpp"
# include "University.hpp"

# include <map>
# include <vector>

enum class Order {
    AS_ENTERED,
    LARGEST_DEGREE_FIRST,
    SMALLEST_DEGREE_FIRST,
    RANDOM
};

typedef struct s_conflict_info {
    int examA;
    int examB;
    std::vector<int> sharedStudents;
} t_conflict_info;

typedef struct s_schedule_result {
    std::map<int, int> slotOfExam;
    int slotsUsed = 0;
} t_schedule_result;

class Scheduler {
private:

    University& _uni;
    Graph _graph;
    std::vector<int> _examIds;
    std::map<int, int> _indexOf;
    std::vector<t_conflict_info> _conflicts;

public:

    explicit Scheduler(University& u) : _uni(u) {}

    void buildConflictGraph();
    t_schedule_result colorGreedy(Order o, unsigned seed = 1);
    t_schedule_result colorGreedy(const std::vector<int>& examOrder);
    void apply(const t_schedule_result& r);
    bool isValid(const t_schedule_result& r) const;

    const Graph& graph() const;
    const std::vector<t_conflict_info>& conflicts() const;
    const std::vector<int>& examIds() const;
};

#endif