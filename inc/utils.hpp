#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>

# include "University.hpp"
# include "Scheduler.hpp"
# include "Storage.hpp"

std::string slotLabel(int s);
int         readInt(const std::string& prompt);
std::string readLine(const std::string& prompt);

void printTimetable(University& u, const t_schedule_result& r);
void conflictReport(University& u, Scheduler& s);
void compareOrders(Scheduler& s);
void compareWithActual(University& u, Scheduler& s, Storage& st);
void listAll(University& uni);

#endif