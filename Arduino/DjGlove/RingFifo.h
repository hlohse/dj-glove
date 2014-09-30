#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_RING_FIFO_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_RING_FIFO_H_

// Note: Ensure elements > 0
template <typename T, int elements>
class RingFifo {
public:
  RingFifo()
  : m_index(0),
    m_is_full(false)
  {
  }
  
  // Overwrites oldest element with new one if full
  void insert(const T& element)
  {
    m_elements[m_index] = element;
    m_index++;
    if (m_index >= elements) {
      m_index = 0;
      m_is_full = true;
    }
  }
  
  // 0 for oldest inserted, elements-1/count()-1 for most recently inserted etc.
  T getOrdered(const int index) const
  {
    const int index_oldest = isFull() ? (m_index + 1) % elements : 0;
    return m_elements[index_oldest + index];
  }
  
  // 0 for first in buffer, 1 for second etc.
  T getDirectly(const int index) const
  {
    return m_elements[index];
  }
  
  int count() const
  {
    if (isFull()) {
      return elements;
    }
    return m_index;
  }
  
  int size() const
  {
    return elements;
  }
  
  void clear()
  {
    m_index = 0;
    m_is_full = false;
  }
  
  bool isEmpty() const
  {
    return !isFull() && m_index == 0;
  }
  
  bool isFull() const
  {
    return m_is_full;
  }

private:
  T    m_elements[elements];
  int  m_index;
  bool m_is_full;
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_RING_FIFO_H_

