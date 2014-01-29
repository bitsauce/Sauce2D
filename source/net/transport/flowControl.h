#ifndef NET_FLOW_CONTROL_H
#define NET_FLOW_CONTROL_H

#include "platform/platform.h"

// Simple binary flow control
// - Two modes of operation: good and bad mode
// - If RTT exceeds 250ms drop to bad mode immediately
// - If RTT is under 250ms for a period of time, return to good mode
const float RTT_Threshold = 250.0f;

class FlowControl
{
public:

	FlowControl() :
		m_mode(Bad),
		m_penaltyTime(4.0f),
		m_goodAccum(0.0f),
		m_reductionAccum(0.0f)
	{
		print("** Net flow control initialized **");
	}

	void reset()
	{
		m_mode = Bad;
		m_penaltyTime = 4.0f;
		m_goodAccum = 0.0f;
		m_reductionAccum = 0.0f;
	}

	void update(float deltaTime, float rtt)
	{
		// The good mode
		if(m_mode == Good)
		{
			if(rtt > RTT_Threshold)
			{
				// Swich to bad mode
				print("** Dropping to bad mode **");
				m_mode = Bad;
				if(m_goodAccum < 10.0f && m_penaltyTime < 60.0f)
				{
					// Apply penalty time
					m_penaltyTime *= 2.0f;
					if(m_penaltyTime > 60.0f)
						m_penaltyTime = 60.0f;
					print("** Penalty time increased to %.1f **", m_penaltyTime);
				}

				// Reset accums
				m_goodAccum = 0.0f;
				m_reductionAccum = 0.0f;
				return;
			}

			// Let the good times roll
			m_goodAccum += deltaTime;
			m_reductionAccum += deltaTime;

			if(m_reductionAccum > 10.0f && m_penaltyTime > 1.0f)
			{
				// Remove penalty time
				m_penaltyTime /= 2.0f;
				if(m_penaltyTime < 1.0f)
					m_penaltyTime = 1.0f;
				print("** Penalty time reduced to %.1f **", m_penaltyTime);
				m_reductionAccum = 0.0f;
			}
		}

		// The bad mode
		if(m_mode == Bad)
		{
			// Check for good times
			if(rtt <= RTT_Threshold)
				m_goodAccum += deltaTime;
			else
				m_goodAccum = 0.0f;

			if(m_goodAccum > m_penaltyTime)
			{
				print("** Swiching to good mode **");
				m_goodAccum = 0.0f;
				m_reductionAccum = 0.0f;
				m_mode = Good;
				return;
			}
		}
	}

	float sendRate()
	{
		return m_mode == Good ? 30.0f : 10.0f;
	}

private:

	enum Mode
	{
		Good,
		Bad
	};

	Mode m_mode;
	float m_penaltyTime;
	float m_goodAccum;
	float m_reductionAccum;
};

#endif // NET_FLOW_CONTROL_H