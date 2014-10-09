#include "ControllerRange.h"
using namespace std;

ControllerRange::ControllerRange(int& value, const int controller_number)
:   Controller(controller_number),
    m_value(&value),
    m_current_value(-1)
{
}

void ControllerRange::Partition(const partition_t& partition)
{
    m_partitions.push_back(partition);
}

bool ControllerRange::Changed()
{
	if (*m_value != m_current_value) {
		m_current_value = *m_value;
		return true;
	}
	return false;
}

MidiSignal ControllerRange::Signal(const int channel)
{
    return {Midi::Status::ControllerChange, channel, m_controller, PartitionValue()};
}

int ControllerRange::PartitionValue() const
{
    for (auto it = m_partitions.cbegin(); it != m_partitions.cend(); ++it) {
        const partition_t& partition = *it;

        if (m_current_value >= get<0>(partition)
        &&  m_current_value <= get<1>(partition)) {
            return get<2>(partition);
        }
    }

    return m_current_value;
}

