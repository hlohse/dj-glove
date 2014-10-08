class ButtonSwitcher{
private:
	bool* m_button;
	bool m_oldState;
	const int m_lowValue;
	const int m_highValue;
	bool m_activated;
	const int m_controller;
public:
	ButtonSwitcher(bool* button, int controller, int lowValue = 0, int highValue = 127) :
		m_lowValue(lowVal),
		m_highValue(highVal),
		m_button(&button),
		m_oldState(false),
		m_controller controller
	{}

	MidiSignal checkAndGetSwitchSignal(){
		if (*m_button && !m_oldState){
			MidiSignal switchSignal;
			switchSignal.Status(Midi::Status::ControllerChange);
			switchSignal.Channel(m_channel);
			switchSignal.Controller(m_controller);
			if (!m_activated){
				loopStartStop.ControllerValue(m_highValue);
			}
			else{
				loopStartStop.ControllerValue(m_lowValue);
			}
			m_activated = !m_activated;
		}
		m_oldState = *m_button;
	}


};