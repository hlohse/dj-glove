#include "ControllerRange.h"
using namespace std;

ControllerRange::ControllerRange(int& value, const int controller_number)
:   ControllerRange(value, controller_number, {})
{
}

ControllerRange::ControllerRange(int& value, const int controller_number,
                                 const vector<partition_t>& partitions)
:   Controller(controller_number),
    m_value(&value),
    m_current_value(-1),
    m_partitions(partitions)
{
}

void ControllerRange::Partition(const partition_t& partition)
{
    m_partitions.push_back(partition);
}

bool ControllerRange::Changed()
{
    if (PartitionValue(*m_value) != PartitionValue()) {
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
    return PartitionValue(m_current_value);
}

int ControllerRange::PartitionValue(const int value) const
{
    for (auto it = m_partitions.cbegin(); it != m_partitions.cend(); ++it) {
        const partition_t& partition = *it;

        if (value >= get<0>(partition)
        &&  value <= get<1>(partition)) {
            return get<2>(partition);
        }
    }

    return value;
}

