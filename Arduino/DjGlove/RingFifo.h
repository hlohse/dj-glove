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
  
  // Overwrites oldest value if full
  void insert(const T& element)
  {
    m_elements[m_index] = element;
    m_index++;
    if (m_index >= elements) {
      m_index = 0;
      m_is_full = false;
    }
  }
  
  // [0] starts at oldest inserted value (not front of buffer)
  T operator[](const int index) const
  {
    const int index_oldest = m_index == elements - 1 ? 0 : m_index + 1;
    return m_elements[index_oldest + index];
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

