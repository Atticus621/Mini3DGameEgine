#include "application.h"

void engine::Application::SetNeedToBeClosed(bool flag)
{
	m_needToBeClosed = flag;
}

bool engine::Application::GetNeedToBeClosed() const
{
	return m_needToBeClosed;
}
