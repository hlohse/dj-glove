#ifndef DJ_GLOVE_PC_CONTROLLER_RANGE_H_
#define DJ_GLOVE_PC_CONTROLLER_RANGE_H_

#include "Controller.h"
#include <vector>
#include <tuple>

class EXPORT ControllerRange : public Controller {
public:
    // min (inclusive), max (inclusive), value
    using partition_t = std::tuple<int, int, int>;

	ControllerRange(int& value, const int controller_number);
	ControllerRange(int& value, const int controller_number,
                    const std::vector<partition_t>& partitions);

    // Use partition value for range, current_value otherwise
    void Partition(const partition_t& partition);

    // Inherited from Controller
	virtual bool Changed();
	virtual MidiSignal Signal(const int channel);

private:
	int* m_value;
	int  m_current_value;
    std::vector<partition_t> m_partitions;

    int PartitionValue() const;
};

#endif // DJ_GLOVE_PC_CONTROLLER_RANGE_H_
